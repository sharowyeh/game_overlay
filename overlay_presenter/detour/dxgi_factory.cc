#include "d3d_detour.h"
#include "dxgi_c.h"

#ifdef LOGFILE
#undef LOGFILE
#define LOGFILE(fmt, ...) WriteLog("detour.txt", fmt, ##__VA_ARGS__)
#endif

#if _DEBUG
#define D3D_DEBUG_INFO
#endif

#include <dxgi.h>
#include "../../Detours/include/detours.h"

#pragma comment(lib, "dxgi.lib")
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
		IDXGIFactoryVtbl *m_pDXGIFactoryVtbl = NULL;

		HRESULT STDMETHODCALLTYPE Detour_DXGIFactory_QueryInterface(
			IDXGIFactory * This,
			/* [in] */ REFIID riid,
			/* [annotation][iid_is][out] */
			_COM_Outptr_  void **ppvObject)
		{
			HRESULT result = m_pDXGIFactoryVtbl->QueryInterface(This, riid, ppvObject);
			LOGFILE("%s: result=0x%x factory=0x%p vobj=0x%p\n", __func__, result, This, (*ppvObject));
			return result;
		}

		ULONG STDMETHODCALLTYPE Detour_DXGIFactory_AddRef(
			IDXGIFactory * This)
		{
			ULONG result = m_pDXGIFactoryVtbl->AddRef(This);
			LOGFILE("%s: result=0x%x factory=0x%p\n", __func__, result, This);
			return result;
		}

		ULONG STDMETHODCALLTYPE Detour_DXGIFactory_Release(
			IDXGIFactory * This)
		{
			ULONG result = m_pDXGIFactoryVtbl->Release(This);
			LOGFILE("%s: result=0x%x factory=0x%p\n", __func__, result, This);
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_DXGIFactory_GetParent(
			IDXGIFactory * This,
			/* [annotation][in] */
			_In_  REFIID riid,
			/* [annotation][retval][out] */
			_COM_Outptr_  void **ppParent)
		{
			HRESULT result = m_pDXGIFactoryVtbl->GetParent(This, riid, ppParent);
			LOGFILE("%s: result=0x%x factory=0x%p parent=0x%p\n", __func__, result, This, (*ppParent));
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_DXGIFactory_EnumAdapters(
			IDXGIFactory * This,
			/* [in] */ UINT Adapter,
			/* [annotation][out] */
			_COM_Outptr_  IDXGIAdapter **ppAdapter)
		{
			HRESULT result = m_pDXGIFactoryVtbl->EnumAdapters(This, Adapter, ppAdapter);
			LOGFILE("%s: result=0x%x factory=0x%p adapter=%d padpt=0x%p\n", __func__, result, This, Adapter, (*ppAdapter));
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_DXGIFactory_MakeWindowAssociation(
			IDXGIFactory * This,
			HWND WindowHandle,
			UINT Flags)
		{
			HRESULT result = m_pDXGIFactoryVtbl->MakeWindowAssociation(This, WindowHandle, Flags);
			LOGFILE("%s: result=0x%x factory=0x%p hwnd=0x%x flags=0x%x\n", __func__, result, This, WindowHandle, Flags);
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_DXGIFactory_CreateSwapChain(
			IDXGIFactory * This,
			/* [annotation][in] */
			_In_  IUnknown *pDevice,
			/* [annotation][in] */
			_In_  DXGI_SWAP_CHAIN_DESC *pDesc,
			/* [annotation][out] */
			_COM_Outptr_  IDXGISwapChain **ppSwapChain)
		{
			HRESULT result = m_pDXGIFactoryVtbl->CreateSwapChain(This, pDevice, pDesc, ppSwapChain);
			LOGFILE("%s: result=0x%x factory=0x%p device=0x%p w=%d h=%d fmt=%d swapchain=0x%p\n", __func__, result, This, pDevice,
				pDesc->BufferDesc.Width, pDesc->BufferDesc.Height, pDesc->BufferDesc.Format, (*ppSwapChain));
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_DXGIFactory_CreateSoftwareAdapter(
			IDXGIFactory * This,
			/* [in] */ HMODULE Module,
			/* [annotation][out] */
			_COM_Outptr_  IDXGIAdapter **ppAdapter)
		{
			HRESULT result = m_pDXGIFactoryVtbl->CreateSoftwareAdapter(This, Module, ppAdapter);
			LOGFILE("%s: result=0x%x factory=0x%p module=0x%p adapter=0x%p\n", __func__, result, This, Module, (*ppAdapter));
			return result;
		}

#ifdef __cplusplus
	}
#endif

	BOOL AttachDXGIFactory(IDXGIFactory *pFactory)
	{
		// Create a list of virtual table address
		m_pDXGIFactoryVtbl = new IDXGIFactoryVtbl();
		// Copy virtual table addresses from referenced dll which loaded from current process
		memcpy(m_pDXGIFactoryVtbl, *reinterpret_cast<IDXGIFactoryVtbl **>(pFactory), sizeof(IDXGIFactoryVtbl));
		LOGFILE("%s: vtable=0x%p pid=%5d tid=%5d\n", __func__, m_pDXGIFactoryVtbl, GetCurrentProcessId(), GetCurrentThreadId());
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		HRESULT result = S_OK;
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());
		// Attach is usually succeeded except address is NULL or pointer of address is access violation.
		result = DetourAttach(
			&(PVOID &)m_pDXGIFactoryVtbl->QueryInterface,
			Detour_DXGIFactory_QueryInterface);
		LOGFILE("%s: AddRef result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIFactoryVtbl->QueryInterface, Detour_DXGIFactory_QueryInterface);
		result = DetourAttach(
			&(PVOID &)m_pDXGIFactoryVtbl->AddRef,
			Detour_DXGIFactory_AddRef);
		LOGFILE("%s: AddRef result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIFactoryVtbl->AddRef, Detour_DXGIFactory_AddRef);
		result = DetourAttach(
			&(PVOID &)m_pDXGIFactoryVtbl->Release,
			Detour_DXGIFactory_Release);
		LOGFILE("%s: Release result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIFactoryVtbl->Release, Detour_DXGIFactory_Release);
		result = DetourAttach(
			&(PVOID &)m_pDXGIFactoryVtbl->GetParent,
			Detour_DXGIFactory_GetParent);
		LOGFILE("%s: GetParent result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIFactoryVtbl->GetParent, Detour_DXGIFactory_GetParent);
		result = DetourAttach(
			&(PVOID &)m_pDXGIFactoryVtbl->EnumAdapters,
			Detour_DXGIFactory_EnumAdapters);
		LOGFILE("%s: EnumAdapters result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIFactoryVtbl->EnumAdapters, Detour_DXGIFactory_EnumAdapters);
		result = DetourAttach(
			&(PVOID &)m_pDXGIFactoryVtbl->MakeWindowAssociation,
			Detour_DXGIFactory_MakeWindowAssociation);
		LOGFILE("%s: MakeWindowAssociation result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIFactoryVtbl->MakeWindowAssociation, Detour_DXGIFactory_MakeWindowAssociation);
		result = DetourAttach(
			&(PVOID &)m_pDXGIFactoryVtbl->CreateSwapChain,
			Detour_DXGIFactory_CreateSwapChain);
		LOGFILE("%s: CreateSwapChain result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIFactoryVtbl->CreateSwapChain, Detour_DXGIFactory_CreateSwapChain);
		result = DetourAttach(
			&(PVOID &)m_pDXGIFactoryVtbl->CreateSoftwareAdapter,
			Detour_DXGIFactory_CreateSoftwareAdapter);
		LOGFILE("%s: CreateSoftwareAdapter result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIFactoryVtbl->CreateSoftwareAdapter, Detour_DXGIFactory_CreateSoftwareAdapter);
		// Then exception will be occurred when transaction commit with invalid address.
		result = DetourTransactionCommit();
		return result == 0 ? TRUE : FALSE;
	}

	BOOL DetachDXGIFactory()
	{
		if (m_pDXGIFactoryVtbl == NULL)
			return TRUE;

		long result = 0;
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());
		result = DetourDetach(
			&(PVOID &)m_pDXGIFactoryVtbl->QueryInterface,
			Detour_DXGIFactory_QueryInterface);
		result = DetourDetach(
			&(PVOID &)m_pDXGIFactoryVtbl->AddRef,
			Detour_DXGIFactory_AddRef);
		result = DetourDetach(
			&(PVOID &)m_pDXGIFactoryVtbl->Release,
			Detour_DXGIFactory_Release);
		result = DetourDetach(
			&(PVOID &)m_pDXGIFactoryVtbl->GetParent,
			Detour_DXGIFactory_GetParent);
		result = DetourDetach(
			&(PVOID &)m_pDXGIFactoryVtbl->EnumAdapters,
			Detour_DXGIFactory_EnumAdapters);
		result = DetourDetach(
			&(PVOID &)m_pDXGIFactoryVtbl->MakeWindowAssociation,
			Detour_DXGIFactory_MakeWindowAssociation);
		result = DetourDetach(
			&(PVOID &)m_pDXGIFactoryVtbl->CreateSwapChain,
			Detour_DXGIFactory_CreateSwapChain);
		result = DetourDetach(
			&(PVOID &)m_pDXGIFactoryVtbl->CreateSoftwareAdapter,
			Detour_DXGIFactory_CreateSoftwareAdapter);
		result = DetourTransactionCommit();
		return TRUE;
	}
}
