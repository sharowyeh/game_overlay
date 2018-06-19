#include "d3d11_detour.h"
#include "dxgi_c.h"

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
		IDXGIDeviceVtbl *m_pDXGIDeviceVtbl = NULL;

		HRESULT STDMETHODCALLTYPE Detour_DXGIDevice_QueryInterface(
			IDXGIDevice * This,
			/* [in] */ REFIID riid,
			/* [annotation][iid_is][out] */
			_COM_Outptr_  void **ppvObject)
		{
			HRESULT result = m_pDXGIDeviceVtbl->QueryInterface(This, riid, ppvObject);
			LOGFILE("%s: result=0x%x device=0x%p vobj=0x%p\n", __func__, result, This, (*ppvObject));
			return result;
		}

		ULONG STDMETHODCALLTYPE Detour_DXGIDevice_AddRef(
			IDXGIDevice * This)
		{
			ULONG result = m_pDXGIDeviceVtbl->AddRef(This);
			LOGFILE("%s: result=0x%x device=0x%p\n", __func__, result, This);
			return result;
		}

		ULONG STDMETHODCALLTYPE Detour_DXGIDevice_Release(
			IDXGIDevice * This)
		{
			ULONG result = m_pDXGIDeviceVtbl->Release(This);
			LOGFILE("%s: result=0x%x device=0x%p\n", __func__, result, This);
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_DXGIDevice_GetParent(
			IDXGIDevice * This,
			/* [annotation][in] */
			_In_  REFIID riid,
			/* [annotation][retval][out] */
			_COM_Outptr_  void **ppParent)
		{
			HRESULT result = m_pDXGIDeviceVtbl->GetParent(This, riid, ppParent);
			LOGFILE("%s: result=0x%x device=0x%p parent=0x%p\n", __func__, result, This, (*ppParent));
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_DXGIDevice_GetAdapter(
			IDXGIDevice * This,
			/* [annotation][out] */
			_COM_Outptr_  IDXGIAdapter **pAdapter)
		{
			HRESULT result = m_pDXGIDeviceVtbl->GetAdapter(This, pAdapter);
			LOGFILE("%s: result=0x%x device=0x%p adapter=0x%p\n", __func__, result, This, (*pAdapter));
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_DXGIDevice_CreateSurface(
			IDXGIDevice * This,
			/* [annotation][in] */
			_In_  const DXGI_SURFACE_DESC *pDesc,
			/* [in] */ UINT NumSurfaces,
			/* [in] */ DXGI_USAGE Usage,
			/* [annotation][in] */
			_In_opt_  const DXGI_SHARED_RESOURCE *pSharedResource,
			/* [annotation][out] */
			_COM_Outptr_  IDXGISurface **ppSurface)
		{
			HRESULT result = m_pDXGIDeviceVtbl->CreateSurface(This, pDesc, NumSurfaces, Usage, pSharedResource, ppSurface);
			LOGFILE("%s: result=0x%x device=0x%p w=%d h=%d fmt=%d surfaces=%d usage=0x%x\n", __func__, result, This, pDesc->Width, pDesc->Height, pDesc->Format, NumSurfaces, Usage);
			return result;
		}

#ifdef __cplusplus
	}
#endif

	BOOL AttachDXGIDevice(IDXGIDevice *pDevice)
	{
		// Create a list of virtual table address
		m_pDXGIDeviceVtbl = new IDXGIDeviceVtbl();
		// Copy virtual table addresses from referenced dll which loaded from current process
		memcpy(m_pDXGIDeviceVtbl, *reinterpret_cast<IDXGIDeviceVtbl **>(pDevice), sizeof(IDXGIDeviceVtbl));
		LOGFILE("%s: vtable=0x%p pid=%5d tid=%5d\n", __func__, m_pDXGIDeviceVtbl, GetCurrentProcessId(), GetCurrentThreadId());
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		HRESULT result = S_OK;
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());
		// Attach is usually succeeded except address is NULL or pointer of address is access violation.
		result = DetourAttach(
			&(PVOID &)m_pDXGIDeviceVtbl->QueryInterface,
			Detour_DXGIDevice_QueryInterface);
		LOGFILE("%s: AddRef result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIDeviceVtbl->QueryInterface, Detour_DXGIDevice_QueryInterface);
		result = DetourAttach(
			&(PVOID &)m_pDXGIDeviceVtbl->AddRef,
			Detour_DXGIDevice_AddRef);
		LOGFILE("%s: AddRef result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIDeviceVtbl->AddRef, Detour_DXGIDevice_AddRef);
		result = DetourAttach(
			&(PVOID &)m_pDXGIDeviceVtbl->Release,
			Detour_DXGIDevice_Release);
		LOGFILE("%s: Release result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIDeviceVtbl->Release, Detour_DXGIDevice_Release);
		result = DetourAttach(
			&(PVOID &)m_pDXGIDeviceVtbl->GetParent,
			Detour_DXGIDevice_GetParent);
		LOGFILE("%s: GetParent result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIDeviceVtbl->GetParent, Detour_DXGIDevice_GetParent);
		result = DetourAttach(
			&(PVOID &)m_pDXGIDeviceVtbl->GetAdapter,
			Detour_DXGIDevice_GetAdapter);
		LOGFILE("%s: GetAdapter result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIDeviceVtbl->GetAdapter, Detour_DXGIDevice_GetAdapter);
		result = DetourAttach(
			&(PVOID &)m_pDXGIDeviceVtbl->CreateSurface,
			Detour_DXGIDevice_CreateSurface);
		LOGFILE("%s: CreateSurface result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIDeviceVtbl->CreateSurface, Detour_DXGIDevice_CreateSurface);
		// Then exception will be occurred when transaction commit with invalid address.
		result = DetourTransactionCommit();
		return result == 0 ? TRUE : FALSE;
	}

	BOOL DetachDXGIDevice()
	{
		if (m_pDXGIDeviceVtbl == NULL)
			return TRUE;

		long result = 0;
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());
		result = DetourDetach(
			&(PVOID &)m_pDXGIDeviceVtbl->QueryInterface,
			Detour_DXGIDevice_QueryInterface);
		result = DetourDetach(
			&(PVOID &)m_pDXGIDeviceVtbl->AddRef,
			Detour_DXGIDevice_AddRef);
		result = DetourDetach(
			&(PVOID &)m_pDXGIDeviceVtbl->Release,
			Detour_DXGIDevice_Release);
		result = DetourDetach(
			&(PVOID &)m_pDXGIDeviceVtbl->GetParent,
			Detour_DXGIDevice_GetParent);
		result = DetourDetach(
			&(PVOID &)m_pDXGIDeviceVtbl->GetAdapter,
			Detour_DXGIDevice_GetAdapter);
		result = DetourDetach(
			&(PVOID &)m_pDXGIDeviceVtbl->CreateSurface,
			Detour_DXGIDevice_CreateSurface);
		result = DetourTransactionCommit();
		return TRUE;
	}
}