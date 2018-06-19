#include "process.h"
#include "image.h"

#ifdef LOGFILE
#undef LOGFILE
#define LOGFILE(fmt, ...) WriteLog("injector.txt", fmt, ##__VA_ARGS__)
#endif

namespace injector
{
	BOOL EnableDebugPrivilege(HANDLE proc_handle, TOKEN_PRIVILEGES *prev_tp_ptr)
	{
		ZeroMemory(prev_tp_ptr, sizeof(TOKEN_PRIVILEGES));

		HANDLE token_handle = NULL;
		BOOL result = OpenProcessToken(proc_handle, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token_handle);
		LOGFILE("OpenProcessToken: result=%d proc_handle=0x%p token_handle=0x%p\n", result, proc_handle, token_handle);
		if (result == FALSE)
			return result;

		LUID luid;
		result = LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);
		LOGFILE("LookupPrivilegeValue: result=%d luid=%d+%d\n", result, luid.HighPart, luid.LowPart);
		if (result == FALSE)
		{
			CloseHandle(token_handle);
			return result;
		}

		TOKEN_PRIVILEGES new_tp;
		ZeroMemory(&new_tp, sizeof(TOKEN_PRIVILEGES));
		new_tp.PrivilegeCount = 1;
		new_tp.Privileges[0].Luid = luid;
		new_tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		DWORD ret_len;
		result = AdjustTokenPrivileges(token_handle, FALSE, &new_tp, sizeof(TOKEN_PRIVILEGES), prev_tp_ptr, &ret_len);
		LOGFILE("AdjustTokenPrivileges: result=%d prev_tp.count=%d\n", result, prev_tp_ptr->PrivilegeCount);
		for (size_t i = 0; i < prev_tp_ptr->PrivilegeCount; i++)
			LOGFILE(" prev_tp[%d] luid=%d+%d attr=%d\n", i, prev_tp_ptr->Privileges[i].Luid.HighPart, prev_tp_ptr->Privileges[i].Luid.LowPart, prev_tp_ptr->Privileges[i].Attributes);

		CloseHandle(token_handle);
		return result;
	}

	BOOL DisableDebugPrivilege(HANDLE proc_handle, TOKEN_PRIVILEGES prev_tp)
	{
		HANDLE token_handle = NULL;
		BOOL result = OpenProcessToken(proc_handle, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token_handle);
		if (result == FALSE)
			return result;

		/*LUID luid;
		result = LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);
		if (result == FALSE)
		{
		CloseHandle(token_handle);
		return result;
		}*/

		DWORD ret_len;
		result = AdjustTokenPrivileges(token_handle, FALSE, &prev_tp, sizeof(TOKEN_PRIVILEGES), NULL, &ret_len);
		CloseHandle(token_handle);
		return result;
	}

	// Allocate and write to remote process virtual memory address
	LPVOID WriteRemoteParam(HANDLE proc_handle, PBYTE param_ptr, SIZE_T param_size)
	{
		SetLastError(0);
		// Allocate virtual memory by default way
		LPVOID alloc_ptr = VirtualAllocEx(proc_handle, NULL, param_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		LOGFILE("VirtualAllocEx: use=DEFAULT alloc_ptr=0x%p size=%d err=%d\n", alloc_ptr, param_size, GetLastError());
		if (alloc_ptr == NULL)
		{
			LOGFILE("%s: virtual address allocated failed, usually caused by memory protection\n", __func__);
			return NULL;
		}

		// Conform virtual address has been commit
		MEMORY_BASIC_INFORMATION mem_info;
		ZeroMemory(&mem_info, sizeof(mem_info));
		SIZE_T info_size = VirtualQueryEx(proc_handle, alloc_ptr, &mem_info, sizeof(mem_info));
		LOGFILE("VirtualQueryEx: address=0x%p size=%I64u state=0x%x prot=0x%x type=0x%x allo_base=0x%x allo_prot=0x%x\n",
			mem_info.BaseAddress, mem_info.RegionSize, mem_info.State, mem_info.Protect, mem_info.Type,
			mem_info.AllocationBase, mem_info.AllocationProtect);
		if (info_size == 0 || mem_info.RegionSize == 0)
		{
			LOGFILE("%s: virtual address allocated failed, usually caused by memory protection\n", __func__);
			return NULL;
		}

		SetLastError(0);
		// Write function parameter(eg. dll path) to remote address
		BOOL result = WriteProcessMemory(proc_handle, alloc_ptr, param_ptr, param_size, NULL);
		LOGFILE("WriteProcessMemory: result=%d proc_handle=0x%x param=%s size=%d err=%d\n", result, proc_handle, param_ptr, param_size, GetLastError());
		if (result == FALSE)
		{
			// Free allocated memory
			LOGFILE("%s: write process memory failed, usually caused by memory protection\n", __func__);
			VirtualFreeEx(proc_handle, alloc_ptr, param_size, MEM_RELEASE);
			return NULL;
		}
		return alloc_ptr;
	}

	// Overload get remote memory function that if want to do platform target specific for memory addressing
	LPVOID WriteRemoteParamEx(HANDLE proc_handle, PBYTE param_ptr, SIZE_T *size_ptr,
		ULONG_PTR mem_begin, ULONG_PTR mem_end, ULONG_PTR mem_align)
	{
		LOGFILE("%s: begin=0x%p end=0x%p align=0x%p\n", __func__, mem_begin, mem_end, mem_align);

		// For logging virtual address map
		ULONG_PTR free_ptr = 0;
		ULONG_PTR reserve_ptr = 0;
		ULONG_PTR commit_ptr = 0;
		// Query from begin
		ULONG_PTR query_address = mem_begin;
		MEMORY_BASIC_INFORMATION mem_info;
		ZeroMemory(&mem_info, sizeof(mem_info));
		size_t info_size = 0;
		// Represent virtual address map
		while (query_address < mem_end)
		{
			info_size = VirtualQueryEx(proc_handle, (LPCVOID)query_address, &mem_info, sizeof(mem_info));
			// Awareness memory info size 48(x64) or 28(x86) depends on this library which is, instead of remote process
			if (info_size == 0)
			{
				LOGFILE("VirtualQueryEx: size=0 err=%d\n", GetLastError());
				break;
			}
			// User mode address space has none aligned size at the end of virtual memory
			if ((mem_info.RegionSize & 0xFFF) == 0xFFF)
			{
				LOGFILE("VirtualQueryEx: memory ended\n");
				break;
			}

			//printf("Address:0x%p size=%I64u state=0x%x prot=0x%x type=0x%x\n", mbi.BaseAddress, mbi.RegionSize, mbi.State, mbi.Protect, mbi.Type);
			if ((ULONG_PTR)mem_info.BaseAddress < mem_begin)
			{
				query_address += mem_info.RegionSize;
				continue;
			}
			if (free_ptr == 0 && mem_info.State == MEM_FREE &&
				mem_info.RegionSize > 0x11000 &&
				mem_info.Type == 0 &&
				mem_info.AllocationBase == 0 &&
				mem_info.AllocationProtect == 0)
			{
				// Align adress makes allocation begin from 0xXXXXXXXX0000 + 0x10000 to 0x11000 (0x10000 + 4096)
				free_ptr = (((ULONG_PTR)mem_info.BaseAddress & mem_align) + 0x10000);
				LOGFILE(" Query free_ptr=0x%p\n", free_ptr);
			}
			if (reserve_ptr == 0 && mem_info.State == MEM_RESERVE &&
				mem_info.RegionSize > 0x11000 &&
				mem_info.Type == MEM_PRIVATE &&
				mem_info.AllocationBase != 0 &&
				mem_info.AllocationProtect == PAGE_READWRITE)
			{
				// Align adress makes allocation begin from 0xXXXXXXXX0000 + 0x10000 to 0x11000 (0x10000 + 4096)
				reserve_ptr = (((ULONG_PTR)mem_info.BaseAddress & mem_align) + 0x10000);
				LOGFILE(" Query reserve_ptr=0x%p\n", reserve_ptr);
			}
			if (commit_ptr == 0 && mem_info.State == MEM_COMMIT &&
				mem_info.RegionSize >= 4096 &&
				mem_info.Type == MEM_PRIVATE &&
				mem_info.Protect == PAGE_READWRITE &&
				mem_info.AllocationBase != 0 &&
				mem_info.AllocationProtect == PAGE_READWRITE)
			{
				//LOGFILE(" commit address=0x%p\n", mem_info.BaseAddress);
				// Check commited address has enough space to overwrite data, but mostly does not
				// This behavior will cause memory leak
				char buff[4096] = "";
				SIZE_T buff_size = 0;
				if (ReadProcessMemory(proc_handle, (LPCVOID)mem_info.BaseAddress, buff, 4096, &buff_size) == TRUE)
				{
					//LOGFILE(" commit existed size=%d\n", buff_size);
					// At least can be given size of file path
					if (4096 - buff_size >= MAX_PATH)
					{
						commit_ptr = (ULONG_PTR)mem_info.BaseAddress + 4096 - MAX_PATH;
						LOGFILE(" Query commit_ptr=0x%p\n", commit_ptr);
					}
				}
			}
			query_address += mem_info.RegionSize;
		} // End of query address

		SetLastError(0);
		// Allocate virtual memory by default way
		LPVOID alloc_ptr = VirtualAllocEx(proc_handle, NULL, *size_ptr, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		LOGFILE("VirtualAllocEx: use=DEFAULT alloc_ptr=0x%p size=%d err=%d\n", alloc_ptr, *size_ptr, GetLastError());
		if (alloc_ptr == NULL && free_ptr != 0)
		{
			*size_ptr = 4096;
			alloc_ptr = VirtualAllocEx(proc_handle, (LPVOID)free_ptr, *size_ptr, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			LOGFILE("VirtualAllocEx: use=FREE alloc_ptr=0x%p size=%d err=%d\n", alloc_ptr, *size_ptr, GetLastError());
		}
		if (alloc_ptr == NULL && reserve_ptr != 0)
		{
			*size_ptr = 4096;
			alloc_ptr = VirtualAllocEx(proc_handle, (LPVOID)reserve_ptr, *size_ptr, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			LOGFILE("VirtualAllocEx: use=RESERVE alloc_ptr=0x%p size=%d err=%d\n", alloc_ptr, *size_ptr, GetLastError());
		}
		if (alloc_ptr == NULL && commit_ptr != 0)
		{
			alloc_ptr = (LPVOID)commit_ptr;
			LOGFILE("Assign use=COMMIT alloc_ptr=0x%p\n", alloc_ptr);
		}
		if (alloc_ptr == NULL)
		{
			LOGFILE("%s: virtual address allocated failed, usually caused by memory protection\n", __func__);
			return NULL;
		}

		// Conform virtual address has been commit
		ZeroMemory(&mem_info, sizeof(mem_info));
		info_size = VirtualQueryEx(proc_handle, alloc_ptr, &mem_info, sizeof(mem_info));
		LOGFILE("VirtualQueryEx: address=0x%p size=%I64u state=0x%x prot=0x%x type=0x%x allo_base=0x%x allo_prot=0x%x\n",
			mem_info.BaseAddress, mem_info.RegionSize, mem_info.State, mem_info.Protect, mem_info.Type,
			mem_info.AllocationBase, mem_info.AllocationProtect);
		if (info_size == 0 || mem_info.RegionSize == 0)
		{
			LOGFILE("%s: virtual address allocated failed, usually caused by memory protection\n", __func__);
			return NULL;
		}

		SetLastError(0);
		// Write function parameter(eg. dll path) to remote address
		BOOL result = WriteProcessMemory(proc_handle, alloc_ptr, param_ptr, *size_ptr, NULL);
		LOGFILE("WriteProcessMemory: result=%d proc_handle=0x%x param=%s size=%d err=%d\n", result, proc_handle, param_ptr, *size_ptr, GetLastError());
		if (result == FALSE)
		{
			// Free allocated memory
			LOGFILE("%s: write process memory failed, usually caused by memory protection\n", __func__);
			VirtualFreeEx(proc_handle, alloc_ptr, *size_ptr, MEM_RELEASE);
			return NULL;
		}

		return alloc_ptr;
	}

	BOOL ExecuteRemoteAddress(HANDLE proc_handle, ULONG_PTR func_ptr, LPVOID alloc_ptr, SIZE_T alloc_size, DWORD *func_result_ptr)
	{
		BOOL result = FALSE;
		SetLastError(0);
		// Create remote thread
		HANDLE thread_handle = CreateRemoteThread(proc_handle, NULL, 0, (LPTHREAD_START_ROUTINE)func_ptr, alloc_ptr, 0, NULL);
		LOGFILE("CreateRemoteThread: thread_handle=0x%x err=%d\n", thread_handle, GetLastError());
		*func_result_ptr = 0;
		if (thread_handle != NULL)
		{
			WaitForSingleObject(thread_handle, INFINITE);
			// Sadly function result return as DWORD type,
			// for loadlibrary will return address of loaded module
			GetExitCodeThread(thread_handle, func_result_ptr);
			CloseHandle(thread_handle);
			result = TRUE;
		}
		LOGFILE("%s: func_result=0x%x\n", __func__, thread_handle, *func_result_ptr);
		VirtualFreeEx(proc_handle, alloc_ptr, alloc_size, MEM_RELEASE);
		return result;
	}

#if _WIN64
	BOOL InvokeRemoteThread64(HANDLE proc_handle, ULONG_PTR func_ptr, PBYTE param_ptr, SIZE_T param_size)
	{
		// Limit address range for x64 virtual memory allocated
		// ULONG_PTR in x64 is unsigned int64
		ULONG_PTR begin = 0x010000000000ui64;
		ULONG_PTR end = 0x7FF000000000ui64;
		ULONG_PTR align = 0x7FFFFFFF0000ui64;

		SIZE_T alloc_size = param_size;
		LPVOID alloc_ptr = WriteRemoteParamEx(proc_handle, param_ptr, &alloc_size, begin, end, align);
		if (alloc_ptr == NULL)
			return FALSE;
		
		DWORD func_result = 0;
		BOOL result = ExecuteRemoteAddress(proc_handle, func_ptr, alloc_ptr, alloc_size, &func_result);
		return result;
	}
#endif

	BOOL InvokeRemoteThread32(HANDLE proc_handle, ULONG_PTR func_ptr, PBYTE param_ptr, SIZE_T param_size)
	{
		// Limit address range for x86 virtual memory allocated
		// ULONG_PTR in x86 is unsigned int32
		ULONG_PTR begin = 0x01000000;
		ULONG_PTR end = 0x7FF00000;
		ULONG_PTR align = 0x7FFF0000;

		SIZE_T alloc_size = param_size;
		LPVOID alloc_ptr = WriteRemoteParamEx(proc_handle, param_ptr, &alloc_size, begin, end, align);
		if (alloc_ptr == NULL)
			return FALSE;

		DWORD func_result = 0;
		BOOL result = ExecuteRemoteAddress(proc_handle, func_ptr, alloc_ptr, alloc_size, &func_result);
		return result;
	}

	BOOL InjectProcessHandle(HANDLE proc_handle)
	{
		// Get file path of process execution image
		char proc_path[1024] = "";
		DWORD path_len = 1024;
		BOOL result = QueryFullProcessImageNameA(proc_handle, NULL, proc_path, &path_len);
		LOGFILE("QueryFullProcessImageName: result=%d path=%s len=%d\n", result, proc_path, path_len);
		if (result == FALSE)
			return result;

		WORD machine_type = IMAGE_FILE_MACHINE_UNKNOWN;
		result = GetFileMachineType(proc_path, &machine_type);
		LOGFILE("GetFileMachineType: result=%d type=0x%x(0x014C=x86,0x8664=x64)\n", result, machine_type);

		char dll_path64[MAX_PATH];
		char dll_path32[MAX_PATH];
		get_this_module_path(dll_path64, dll_path32);
		LOGFILE("InjectFilePath: x64=%s\n", dll_path64);
		LOGFILE("InjectFilePath: x86=%s\n", dll_path32);

		// If caller process platform target is different with remote process,
		// It can not simply use GetProcAddress to get LoadLibrary method address
		ULONG_PTR func_ptr = NULL;
		PBYTE param_ptr = NULL;
		SIZE_T param_size = 0;
		result = FALSE;
		if (machine_type == IMAGE_FILE_MACHINE_I386)
		{
#if _WIN64
			// x64 platform inject x86 process
			func_ptr = GetImageFuncAddress(proc_handle, "c:\\windows\\syswow64\\kernel32.dll", "LoadLibraryA");
#else
			// x86 platform only can inject x86 process
			func_ptr = GetLoadedFuncAddress("kernel32.dll", "LoadLibraryA");
#endif
			param_ptr = (PBYTE)dll_path32;
			param_size = strlen(dll_path32);
			result = InvokeRemoteThread32(proc_handle, func_ptr, param_ptr, param_size);
		}
#if _WIN64
		// x64 platform inject x64 process
		if (machine_type == IMAGE_FILE_MACHINE_AMD64)
		{
			func_ptr = GetLoadedFuncAddress("kernel32.dll", "LoadLibraryA");
			param_ptr = (PBYTE)dll_path64;
			param_size = strlen(dll_path64);
			result = InvokeRemoteThread64(proc_handle, func_ptr, param_ptr, param_size);
		}
#endif

		LOGFILE("%s: result=%d\n", __func__, result);
		return result;
	}

	BOOL InjectProcess(DWORD proc_id, HANDLE proc_handle)
	{
		if (proc_handle == NULL)
		{
			if (proc_id == 0)
			{
				LOGFILE("%s: At least one of arguments must be valid\n", __func__);
				return FALSE;
			}
			SetLastError(0);
			proc_handle = OpenProcess(
				PROCESS_CREATE_THREAD |
				PROCESS_QUERY_INFORMATION |
				PROCESS_VM_OPERATION |
				PROCESS_VM_WRITE |
				PROCESS_VM_READ,
				FALSE, proc_id);
			LOGFILE("OpenProcess: id=%d proc_handle=0x%x err=%d\n", proc_id, proc_handle, GetLastError());
			if (proc_handle == NULL)
				return FALSE;
		}

		// Enable process debug token privilege
		TOKEN_PRIVILEGES prev_tp;
		ZeroMemory(&prev_tp, sizeof(TOKEN_PRIVILEGES));
		BOOL token_result = EnableDebugPrivilege(proc_handle, &prev_tp);
		LOGFILE("EnableDebugPrivilege: result=%d\n", token_result);

		BOOL result = InjectProcessHandle(proc_handle);
		LOGFILE("InjectProcessHandle: result=%d\n", result);

		if (token_result == TRUE)
			DisableDebugPrivilege(proc_handle, prev_tp);

		CloseHandle(proc_handle);
		return result;
	}

	// Inject process handle and return remote thread instead of waiting completed
	HANDLE AttachProcessHandle(HANDLE proc_handle)
	{
		// Get file path of process execution image
		char proc_path[1024] = "";
		DWORD path_len = 1024;
		BOOL result = QueryFullProcessImageNameA(proc_handle, NULL, proc_path, &path_len);
		LOGFILE("QueryFullProcessImageName: result=%d path=%s len=%d\n", result, proc_path, path_len);
		if (result == FALSE)
			return NULL;

		WORD machine_type = IMAGE_FILE_MACHINE_UNKNOWN;
		result = GetFileMachineType(proc_path, &machine_type);
		LOGFILE("GetFileMachineType: result=%d type=0x%x(0x014C=x86,0x8664=x64)\n", result, machine_type);

		char dll_path64[MAX_PATH];
		char dll_path32[MAX_PATH];
		get_this_module_path(dll_path64, dll_path32);
		LOGFILE("InjectFilePath: x64=%s\n", dll_path64);
		LOGFILE("InjectFilePath: x86=%s\n", dll_path32);

		// If caller process platform target is different with remote process,
		// It can not simply use GetProcAddress to get LoadLibrary method address
		ULONG_PTR func_ptr = NULL;
		PBYTE param_ptr = NULL;
		SIZE_T param_size = 0;
		if (machine_type == IMAGE_FILE_MACHINE_I386)
		{
#if _WIN64
			// x64 platform inject x86 process
			func_ptr = GetImageFuncAddress(proc_handle, "c:\\windows\\syswow64\\kernel32.dll", "LoadLibraryA");
#else
			// x86 platform only can inject x86 process
			func_ptr = GetLoadedFuncAddress("kernel32.dll", "LoadLibraryA");
#endif
			param_ptr = (PBYTE)dll_path32;
			param_size = strlen(dll_path32);
		}
#if _WIN64
		// x64 platform inject x64 process
		if (machine_type == IMAGE_FILE_MACHINE_AMD64)
		{
			func_ptr = GetLoadedFuncAddress("kernel32.dll", "LoadLibraryA");
			param_ptr = (PBYTE)dll_path64;
			param_size = strlen(dll_path64);
		}
#endif
		if (func_ptr == NULL || param_ptr == NULL || param_size == 0)
		{
			LOGFILE("%s: arguments failed func_ptr=0x%p param_ptr=0x%p param_size=%d\n", __func__, func_ptr, param_ptr, param_size);
			return NULL;
		}

		LPVOID alloc_ptr = WriteRemoteParam(proc_handle, param_ptr, param_size);
		if (alloc_ptr == NULL)
			return NULL;

		SetLastError(0);
		HANDLE thread_handle = CreateRemoteThread(proc_handle, NULL, 0, (LPTHREAD_START_ROUTINE)func_ptr, alloc_ptr, 0, NULL);
		LOGFILE("CreateRemoteThread: thread_handle=0x%x err=%d\n", thread_handle, GetLastError());
		return thread_handle;
	}
}