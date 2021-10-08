// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <string>
#include "detour/kernel.h"
#include "detour/d3d11_detour.h"

HMODULE ThisModuleHandle = NULL;

bool isPrintable(TCHAR *str, size_t length) {
	if (length == 0) return false;
	int pos = 0;
	while (pos <= length) {
		if (*str != 0 && (*str < 32 || *str > 126))
			return false;
		str++;
		pos++;
	}
	return true;
}

BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lParam)
{
	if (hwnd != NULL) {
		DWORD procId = 0;
		GetWindowThreadProcessId(hwnd, &procId);
		TCHAR name[MAX_PATH] = { 0 };
		if (GetWindowText(hwnd, name, MAX_PATH) == 0 || !isPrintable(name, wcslen(name)))
			wcscpy_s(name, L"(null)");
		LOGFILE("%s: check curr:%d proc:%d name:%S\n", __func__, GetCurrentProcessId(), procId, name);
		if (procId == GetCurrentProcessId())
		{
			detour::AttachD3D11();
			// stop enumeration
			return FALSE;
		}
	}
	Sleep(10);
	// continue enumeration
	return TRUE;
}

DWORD WINAPI GetHwndThreadRun(LPVOID lpParam)
{
	detour::AttachD3D11();
	return 0;
	BOOL *IsRun = (BOOL*)lpParam;
	while (*IsRun)
	{
		HWND hwnd = GetForegroundWindow();
		if (hwnd != NULL) {
			DWORD procId = 0;
			GetWindowThreadProcessId(hwnd, &procId);
			TCHAR name[MAX_PATH] = { 0 };
			if (GetWindowText(hwnd, name, MAX_PATH) == 0 || !isPrintable(name, wcslen(name)))
				wcscpy_s(name, L"(null)");
			//LOGFILE("%s: check curr:%d proc:%d name:%S\n", __func__, GetCurrentProcessId(), procId, name);
			if (procId == GetCurrentProcessId())
			{
				LOGFILE("%s: check curr:%d proc:%d name:%S\n", __func__, GetCurrentProcessId(), procId, name);
				detour::AttachD3D11();
				break;
			}
		}
		Sleep(100);
	}
	return 0;
}

BOOL IsGetHwndThreadRun;
HANDLE GetHwndThread = NULL;
void WithoutInjectorDebugger(DWORD reson_call, const char *reason)
{
	char image_name[MAX_PATH] = "";
	DWORD len = MAX_PATH;
	QueryFullProcessImageNameA(GetCurrentProcess(), NULL, image_name, &len);
	std::string exe_path(image_name);
	if (exe_path.find("overlay_presenter") != std::string::npos)
		return;

	LOGFILE("%s: module=0x%p pid=%5d tid=%5d exe=%s\n", reason, ThisModuleHandle, GetCurrentProcessId(), GetCurrentThreadId(), image_name);

	if (reson_call == DLL_PROCESS_ATTACH) {
		detour::AttachKernel();
		//TODO: swapchain may not create before main process loaded d3d libraries
		IsGetHwndThreadRun = TRUE;
		GetHwndThread = CreateThread(NULL, 0, GetHwndThreadRun, &IsGetHwndThreadRun, 0, NULL);
		//detour::AttachD3D11();
	}
	else if (reson_call == DLL_PROCESS_DETACH) {
		IsGetHwndThreadRun = FALSE;
		detour::DetachKernel();
		detour::DetachD3D11();
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		ThisModuleHandle = hModule;
		WithoutInjectorDebugger(DLL_PROCESS_ATTACH, "DllProcessAttach");
		break;
	case DLL_THREAD_ATTACH:
		//WithoutInjectorDebugger(DLL_THREAD_ATTACH, "DllThreadAttach");
		break;
	case DLL_THREAD_DETACH:
		//WithoutInjectorDebugger(DLL_THREAD_DETACH, "DllThreadDetech");
		break;
	case DLL_PROCESS_DETACH:
		WithoutInjectorDebugger(DLL_PROCESS_DETACH, "DllProcessDetach");
		ThisModuleHandle = NULL;
	    break;
    }
    return TRUE;
}

