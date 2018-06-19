#pragma once
#include "../stdafx.h"

namespace injector
{
	DLLEXPORT LPVOID MapFileView(const char *file_path, HANDLE *file_handle_ptr, HANDLE *mapping_handle_ptr);
	DLLEXPORT BOOL UnmapFileView(HANDLE file_handle, HANDLE mapping_handle, LPVOID view_ptr);
	DLLEXPORT BOOL GetFileMachineType(const char *file_path, WORD* machine_type_ptr);

	DLLEXPORT ULONG_PTR GetSymbolAddress(HANDLE proc_handle, const char *module_name, const char *func_name);
	DLLEXPORT HMODULE GetLoadedModule(HANDLE proc_handle, const char *file_name);
	DLLEXPORT ULONG_PTR GetImageFuncAddress(HANDLE proc_handle, const char *module_path, const char *func_name);
	DLLEXPORT ULONG_PTR GetLoadedFuncAddress(const char *module_name, const char *func_name);

	DLLEXPORT BOOL get_this_module_path(char path_64[MAX_PATH], char path_32[MAX_PATH]);
}