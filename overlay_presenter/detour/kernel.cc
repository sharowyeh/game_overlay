#include "kernel.h"
#include <Psapi.h>
#include "../injector/process.h"
#include "../injector/image.h"
#include "../../Detours/include/detours.h"

#ifdef _WIN64
#pragma comment(lib, "../Detours/lib.X64/detours.lib")
#else
#pragma comment(lib, "../Detours/lib.X86/detours.lib")
#endif

// Detour functions will follow old style Windows naming convention for variables name consistent with MSDN

// Function type definition
typedef BOOL(WINAPI *CreateProcessAType)(__in_opt LPCSTR, __inout_opt LPSTR, __in_opt LPSECURITY_ATTRIBUTES, __in_opt LPSECURITY_ATTRIBUTES, __in BOOL, __in DWORD, __in_opt LPVOID, __in_opt LPCSTR, __in LPSTARTUPINFOA, __out LPPROCESS_INFORMATION);
typedef BOOL(WINAPI *CreateProcessWType)(__in_opt LPCWSTR, __inout_opt LPWSTR, __in_opt LPSECURITY_ATTRIBUTES, __in_opt LPSECURITY_ATTRIBUTES, __in BOOL, __in DWORD, __in_opt LPVOID, __in_opt LPCWSTR, __in LPSTARTUPINFOW, __out LPPROCESS_INFORMATION);

// Original function address
CreateProcessAType Kernel32_CreateProcessA = NULL;
CreateProcessWType Kernel32_CreateProcessW = NULL;

// Detour function address
BOOL WINAPI Detour_CreateProcessA(
	_In_opt_ LPCSTR lpApplicationName,
	_Inout_opt_ LPSTR lpCommandLine,
	_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ BOOL bInheritHandles,
	_In_ DWORD dwCreationFlags,
	_In_opt_ LPVOID lpEnvironment,
	_In_opt_ LPCSTR lpCurrentDirectory,
	_In_ LPSTARTUPINFOA lpStartupInfo,
	_Out_ LPPROCESS_INFORMATION lpProcessInformation
)
{
	// create the process suspended
	if ((dwCreationFlags & CREATE_SUSPENDED) != CREATE_SUSPENDED)
		dwCreationFlags |= CREATE_SUSPENDED;

	// call original CreateProcessW
	BOOL bResult = Kernel32_CreateProcessA(
		lpApplicationName, 
		lpCommandLine, 
		lpProcessAttributes, 
		lpThreadAttributes, 
		bInheritHandles, 
		dwCreationFlags, 
		lpEnvironment, 
		lpCurrentDirectory, 
		lpStartupInfo, 
		lpProcessInformation);
	if (bResult)
	{
		LOGFILE("Detour_CreateProcessA: app=%s cmdline=%s hth=%p\n", lpApplicationName, lpCommandLine, lpProcessInformation->hThread);
		// Lets method find process platform target and given matched dll module handle
		HANDLE thread_handle = injector::AttachProcessHandle(lpProcessInformation->hProcess);
		LOGFILE("AttachProcessHandle: thread_handle=0x%x\n", thread_handle);
		// Original code
		//HANDLE hRemoteThread = LoadModuleEx(lpProcessInformation->hProcess, hCurrentModule);
		//LOGFILE("LoadModuleEx: %p\n", hRemoteThread);

		// resume thread
		ResumeThread(lpProcessInformation->hThread);
	}

	return bResult;
}

BOOL WINAPI Detour_CreateProcessW(
	__in_opt LPCWSTR lpApplicationName,
	__inout_opt LPWSTR lpCommandLine,
	__in_opt LPSECURITY_ATTRIBUTES lpProcessAttributes,
	__in_opt LPSECURITY_ATTRIBUTES lpThreadAttributes,
	__in BOOL bInheritHandles,
	__in DWORD dwCreationFlags,
	__in_opt LPVOID lpEnvironment,
	__in_opt LPCWSTR lpCurrentDirectory,
	__in LPSTARTUPINFOW lpStartupInfo,
	__out LPPROCESS_INFORMATION lpProcessInformation)
{
	// create the process suspended
	if ((dwCreationFlags & CREATE_SUSPENDED) != CREATE_SUSPENDED)
		dwCreationFlags |= CREATE_SUSPENDED;

	// call original CreateProcessW
	BOOL bResult = Kernel32_CreateProcessW(
		lpApplicationName,
		lpCommandLine,
		lpProcessAttributes,
		lpThreadAttributes,
		bInheritHandles,
		dwCreationFlags,
		lpEnvironment,
		lpCurrentDirectory,
		lpStartupInfo,
		lpProcessInformation);
	if (bResult)
	{
		LOGFILE("Detour_CreateProcessW: app=%S cmdline=%S hth=%p\n", lpApplicationName, lpCommandLine, lpProcessInformation->hThread);
		// Lets method find process platform target and given matched dll module handle
		HANDLE thread_handle = injector::AttachProcessHandle(lpProcessInformation->hProcess);
		LOGFILE("AttachProcessHandle: thread_handle=0x%x\n", thread_handle);
		// Original code
		//HANDLE hRemoteThread = LoadModuleEx(lpProcessInformation->hProcess, hCurrentModule);
		//LOGFILE("LoadModuleEx: %p\n", hRemoteThread);

		// resume thread
		ResumeThread(lpProcessInformation->hThread);
	}

	return bResult;
}

namespace detour
{
	BOOL AttachKernel()
	{
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		LONG result = 0;
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());
		Kernel32_CreateProcessA = reinterpret_cast<CreateProcessAType>(
			injector::GetLoadedFuncAddress("kernel32.dll", "CreateProcessA"));
		Kernel32_CreateProcessW = reinterpret_cast<CreateProcessWType>(
			injector::GetLoadedFuncAddress("kernel32.dll", "CreateProcessW"));
		result = DetourAttach(&(PVOID &)Kernel32_CreateProcessA, Detour_CreateProcessA);
		LOGFILE("AttachKernel: CreateProcessA result=0x%x 0x%p -> 0x%p\n", result,
			Kernel32_CreateProcessA, Detour_CreateProcessA);
		if (result != 0)
			Kernel32_CreateProcessA = NULL;
		result = DetourAttach(&(PVOID &)Kernel32_CreateProcessW, Detour_CreateProcessW);
		LOGFILE("AttachKernel: CreateProcessW result=0x%x 0x%p -> 0x%p\n", result,
			Kernel32_CreateProcessW, Detour_CreateProcessW);
		if (result != 0)
			Kernel32_CreateProcessW = NULL;
		result = DetourTransactionCommit();
		return result == 0 ? TRUE : FALSE;
	}

	BOOL DetachKernel()
	{
		long result = 0;
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());
		if (Kernel32_CreateProcessA)
		{
			result = DetourDetach(&(PVOID &)Kernel32_CreateProcessA, Detour_CreateProcessA);
			Kernel32_CreateProcessA = NULL;
		}
		if (Kernel32_CreateProcessW)
		{
			result = DetourDetach(&(PVOID &)Kernel32_CreateProcessW, Detour_CreateProcessW);
			Kernel32_CreateProcessW = NULL;
		}
		result = DetourTransactionCommit();
		return result == 0 ? TRUE : FALSE;
	}
}
