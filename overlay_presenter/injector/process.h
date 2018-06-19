#pragma once
#include "../stdafx.h"

namespace injector
{
	DLLEXPORT BOOL EnableDebugPrivilege(HANDLE proc_handle, TOKEN_PRIVILEGES *prev_tp_ptr);
	DLLEXPORT BOOL DisableDebugPrivilege(HANDLE proc_handle, TOKEN_PRIVILEGES prev_tp);

#if _WIN64
	DLLEXPORT BOOL InvokeRemoteThread64(HANDLE proc_handle, ULONG_PTR func_ptr, PBYTE param_ptr, SIZE_T param_size);
#endif
	DLLEXPORT BOOL InvokeRemoteThread32(HANDLE proc_handle, ULONG_PTR func_ptr, PBYTE param_ptr, SIZE_T param_size);

	DLLEXPORT BOOL InjectProcessHandle(HANDLE proc_handle);
	DLLEXPORT BOOL InjectProcess(DWORD proc_id, HANDLE proc_handle);

	DLLEXPORT HANDLE AttachProcessHandle(HANDLE proc_handle);
}
