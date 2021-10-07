#include "d3d11_detour.h"
#include "d3d11_c.h"

#ifdef LOGFILE
#undef LOGFILE
#define LOGFILE(fmt, ...) WriteLog("detour.txt", fmt, ##__VA_ARGS__)
#endif

#if _DEBUG
#define D3D_DEBUG_INFO
#endif

#include <d3d11.h>
#include "../../Detours/include/detours.h"

#pragma comment(lib, "d3d11.lib")
#ifdef _WIN64
#pragma comment(lib, "../Detours/lib.X64/detours.lib")
#else
#pragma comment(lib, "../Detours/lib.X86/detours.lib")
#endif

namespace detour
{
#ifdef __cplusplus
	extern "C"
	{
#endif
		ID3D11DeviceVtbl *m_pD3D11DeviceVtbl = NULL;

		HRESULT STDMETHODCALLTYPE Detour_D3D11Device_QueryInterface(
			ID3D11Device * pDevice,
			REFIID riid,
			_COM_Outptr_  void **ppvObject)
		{
			HRESULT result = m_pD3D11DeviceVtbl->QueryInterface(pDevice, riid, ppvObject);
			LOGFILE("Detour_D3D11Device_QueryInterface: result=0x%x device=0x%p vobj=0x%p\n", result, pDevice, (*ppvObject));
			return result;
		}

		ULONG STDMETHODCALLTYPE Detour_D3D11Device_AddRef(
			ID3D11Device *pDevice)
		{
			ULONG result = m_pD3D11DeviceVtbl->AddRef(pDevice);
			LOGFILE("Detour_D3D11Device_AddRef: result=0x%x device=0x%p\n", result, pDevice);
			return result;
		}

		ULONG STDMETHODCALLTYPE Detour_D3D11Device_Release(
			ID3D11Device *pDevice)
		{
			ULONG result = m_pD3D11DeviceVtbl->Release(pDevice);
			LOGFILE("Detour_D3D11Device_Release: result=0x%x device=0x%p\n", result, pDevice);
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_D3D11Device_CreateBuffer(
			ID3D11Device *pDevice,
			_In_  const D3D11_BUFFER_DESC *pDesc,
			_In_opt_  const D3D11_SUBRESOURCE_DATA *pInitialData,
			_COM_Outptr_opt_  ID3D11Buffer **ppBuffer)
		{
			HRESULT result = m_pD3D11DeviceVtbl->CreateBuffer(pDevice, pDesc, pInitialData, ppBuffer);
			LOGFILE("%s: result=0x%x device=0x%p buffer=0x%p\n", __func__, result, pDevice,
				(ppBuffer == NULL) ? NULL : (*ppBuffer));
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_D3D11Device_CreateRenderTargetView(
			ID3D11Device *pDevice,
			_In_  ID3D11Resource *pResource,
			_In_opt_  const D3D11_RENDER_TARGET_VIEW_DESC *pDesc,
			_COM_Outptr_opt_  ID3D11RenderTargetView **ppRTView)
		{
			// Notice: opt parameter can be null that access violation may be occurred
			HRESULT result = m_pD3D11DeviceVtbl->CreateRenderTargetView(pDevice, pResource, pDesc, ppRTView);
			char desc[128] = "desc=NULL";
			if (pDesc != NULL) sprintf_s(desc, "fmt=%d dim=%d", pDesc->Format, pDesc->ViewDimension);
			LOGFILE("%s: result=0x%x device=0x%p %s rtv=0x%p\n", __func__, result, pDevice, desc,
				(ppRTView == NULL) ? NULL : (*ppRTView));
			//TODO: if(*ppRTView != NULL) (*ppRTView)->GetDesc(&rtvDesc)
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_D3D11Device_CreateDeferredContext(
			ID3D11Device *pDevice,
			UINT ContextFlags,
			_COM_Outptr_opt_  ID3D11DeviceContext **ppDeferredContext)
		{
			HRESULT result = m_pD3D11DeviceVtbl->CreateDeferredContext(pDevice, ContextFlags, ppDeferredContext);
			LOGFILE("%s: result=0x%x device=0x%p flag=%d context=0x%p\n", __func__, result, pDevice, ContextFlags,
				(ppDeferredContext == NULL) ? NULL : (*ppDeferredContext));
			return result;
		}

		void STDMETHODCALLTYPE Detour_D3D11Device_GetImmediateContext(
			ID3D11Device *pDevice,
			_Outptr_  ID3D11DeviceContext **ppImmediateContext)
		{
			m_pD3D11DeviceVtbl->GetImmediateContext(pDevice, ppImmediateContext);
			LOGFILE("%s: device=0x%p context=0x%p\n", __func__, pDevice, (*ppImmediateContext));
		}

#ifdef __cplusplus
	}
#endif

	BOOL AttachD3D11Device(ID3D11Device *pDevice)
	{
		// It's another point querying COM class GUID to get c style interface
		// as virtual address table from referenced DLL.
		// See also: https://stackoverflow.com/questions/45207273/what-is-a-direct-x-virtual-table

		// Use Vtbl struct is a easy way without counting function numbers 
		// and writting function typedef which we need to detour.
		//ULONG_PTR *vtbl = *reinterpret_cast<ULONG_PTR **>(pDevice);
		//OriginalCreateBuffer = reinterpret_cast<CreateBufferType>(vtbl[3]);

		// Create list of device's virtual table address
		m_pD3D11DeviceVtbl = new ID3D11DeviceVtbl();
		// Copy virtual table addresses from referenced dll to detour attach and swap them
		memcpy(m_pD3D11DeviceVtbl, *reinterpret_cast<ID3D11DeviceVtbl **>(pDevice), sizeof(ID3D11DeviceVtbl));
		LOGFILE("AttachD3D11Device: vtable=0x%p pid=%5d tid=%5d\n", m_pD3D11DeviceVtbl, GetCurrentProcessId(), GetCurrentThreadId());
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		HRESULT result = S_OK;
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());
		// Attach is usually succeeded except address is NULL or pointer of address is access violation.
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceVtbl->QueryInterface,
			Detour_D3D11Device_QueryInterface);
		LOGFILE("AttachD3D11Device: QueryInterface result=0x%x 0x%p->0x%p\n", result,
			m_pD3D11DeviceVtbl->QueryInterface, Detour_D3D11Device_QueryInterface);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceVtbl->AddRef,
			Detour_D3D11Device_AddRef);
		LOGFILE("AttachD3D11Device: AddRef result=0x%x 0x%p -> 0x%p\n", result,
			m_pD3D11DeviceVtbl->AddRef, Detour_D3D11Device_AddRef);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceVtbl->Release,
			Detour_D3D11Device_Release);
		LOGFILE("AttachD3D11Device: Release result=0x%x 0x%p -> 0x%p\n", result,
			m_pD3D11DeviceVtbl->Release, Detour_D3D11Device_Release);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceVtbl->CreateBuffer,
			Detour_D3D11Device_CreateBuffer);
		LOGFILE("AttachD3D11Device: CreateBuffer result=0x%x 0x%p -> 0x%p\n", result,
			m_pD3D11DeviceVtbl->CreateBuffer, Detour_D3D11Device_CreateBuffer);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceVtbl->CreateRenderTargetView,
			Detour_D3D11Device_CreateRenderTargetView);
		LOGFILE("AttachD3D11Device: CreateRenderTargetView result=0x%x 0x%p -> 0x%p\n", result,
			m_pD3D11DeviceVtbl->CreateRenderTargetView, Detour_D3D11Device_CreateRenderTargetView);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceVtbl->CreateDeferredContext,
			Detour_D3D11Device_CreateDeferredContext);
		LOGFILE("AttachD3D11Device: CreateDeferredContext result=0x%x 0x%p -> 0x%p\n", result,
			m_pD3D11DeviceVtbl->CreateDeferredContext, Detour_D3D11Device_CreateDeferredContext);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceVtbl->GetImmediateContext,
			Detour_D3D11Device_GetImmediateContext);
		LOGFILE("AttachD3D11Device: GetImmediateContext result=0x%x 0x%p -> 0x%p\n", result,
			m_pD3D11DeviceVtbl->GetImmediateContext, Detour_D3D11Device_GetImmediateContext);
		// Then exception will be occurred when transaction commit.
		result = DetourTransactionCommit();
		return result == 0 ? TRUE : FALSE;
	}

	// Move to d3d11_detour.cc
	//BOOL AttachD3D11Device()
	//{
	//	// Create a dummy window then assign HWND to override method
	//	WNDCLASSEX wc;
	//	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	//	wc.cbSize = sizeof(WNDCLASSEX);
	//	wc.style = CS_CLASSDC;
	//	wc.lpszClassName = L"dummy_class";
	//	wc.lpfnWndProc = DefWindowProc;
	//	RegisterClassEx(&wc);
	//	HWND hwnd = CreateWindow(L"dummy_class", L"dummy_window", NULL, 0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);
	//	if (hwnd == NULL)
	//		return FALSE;
	//	// Notice: almost forgot D3D must needs HWND assigned to swap chain's output window,
	//	// and can not called from dllmain because related dll may not loaded
	//	ID3D11Device *pDevice = NULL;
	//	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	//	HRESULT result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &pDevice, NULL, NULL);
	//	LOGFILE("AttachD3D11Device: CreateDevice result=0x%x device=0x%p\n", result, pDevice);
	//	if (pDevice == NULL)
	//		return FALSE;
	//	result = AttachD3D11Device(pDevice);
	//	// Aftering function attached, d3d device and hwnd were useless
	//	pDevice->Release();
	//	DestroyWindow(hwnd);
	//	return result;
	//}

	BOOL DetachD3D11Device()
	{
		if (m_pD3D11DeviceVtbl == NULL)
			return TRUE;

		long result = 0;
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());
		// There has problem if we use ID3D11DeviceVtbl to attach virtual addresses.
		// It needs to keep something which address attached succeeded before.
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceVtbl->QueryInterface,
			Detour_D3D11Device_QueryInterface);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceVtbl->AddRef,
			Detour_D3D11Device_AddRef);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceVtbl->Release,
			Detour_D3D11Device_Release);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceVtbl->CreateBuffer,
			Detour_D3D11Device_CreateBuffer);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceVtbl->CreateRenderTargetView,
			Detour_D3D11Device_CreateRenderTargetView);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceVtbl->CreateDeferredContext,
			Detour_D3D11Device_CreateDeferredContext);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceVtbl->GetImmediateContext,
			Detour_D3D11Device_GetImmediateContext);

		result = DetourTransactionCommit();
		return TRUE;
	}

	/*void D3D11Device::GetImmediateContext(ID3D11Device *pDevice, ID3D11DeviceContext **ppDeviceContext)
	{
		if (m_pD3D11DeviceVtbl != NULL && m_pD3D11DeviceVtbl->GetImmediateContext != NULL)
		{
			m_pD3D11DeviceVtbl->GetImmediateContext(pDevice, ppDeviceContext);
		}
	};*/
}