// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <string>
#include "detour/kernel.h"
#include "detour/d3d11_detour.h"

HMODULE ThisModuleHandle = NULL;
HANDLE AttachThread = NULL;

DWORD WINAPI AttachThreadRun(LPVOID lpParam)
{
	detour::AttachD3D11();
	return 0;
}

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
		AttachThread = CreateThread(NULL, 0, AttachThreadRun, NULL, 0, NULL);
		//detour::AttachD3D11();
	}
	else if (reson_call == DLL_PROCESS_DETACH) {
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

