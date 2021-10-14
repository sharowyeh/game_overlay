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
		IDXGIAdapterVtbl *m_pDXGIAdapterVtbl = NULL;

		HRESULT STDMETHODCALLTYPE Detour_DXGIAdapter_QueryInterface(
			IDXGIAdapter * This,
			/* [in] */ REFIID riid,
			/* [annotation][iid_is][out] */
			_COM_Outptr_  void **ppvObject)
		{
			HRESULT result = m_pDXGIAdapterVtbl->QueryInterface(This, riid, ppvObject);
			LOGFILE("%s: result=0x%x adapter=0x%p vobj=0x%p\n", __func__, result, This,
				(ppvObject == NULL ? NULL : (*ppvObject)));
			return result;
		}

		ULONG STDMETHODCALLTYPE Detour_DXGIAdapter_AddRef(
			IDXGIAdapter * This)
		{
			ULONG result = m_pDXGIAdapterVtbl->AddRef(This);
			LOGFILE("%s: result=0x%x adapter=0x%p\n", __func__, result, This);
			return result;
		}

		ULONG STDMETHODCALLTYPE Detour_DXGIAdapter_Release(
			IDXGIAdapter * This)
		{
			ULONG result = m_pDXGIAdapterVtbl->Release(This);
			LOGFILE("%s: result=0x%x adapter=0x%p\n", __func__, result, This);
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_DXGIAdapter_GetParent(
			IDXGIAdapter * This,
			/* [annotation][in] */
			_In_  REFIID riid,
			/* [annotation][retval][out] */
			_COM_Outptr_  void **ppParent)
		{
			HRESULT result = m_pDXGIAdapterVtbl->GetParent(This, riid, ppParent);
			LOGFILE("%s: result=0x%x adapter=0x%p parent=0x%p\n", __func__, result, This,
				(ppParent == NULL ? NULL : (*ppParent)));
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_DXGIAdapter_EnumOutputs(
			IDXGIAdapter * This,
			/* [in] */ UINT Output,
			/* [annotation][out][in] */
			_COM_Outptr_  IDXGIOutput **ppOutput)
		{
			HRESULT result = m_pDXGIAdapterVtbl->EnumOutputs(This, Output, ppOutput);
			LOGFILE("%s: result=0x%x adapter=0x%p output=%d pout=0x%p\n", __func__, result, This, Output,
				(ppOutput == NULL ? NULL : (*ppOutput)));
			return result;
		}

#ifdef __cplusplus
	}
#endif

	BOOL AttachDXGIAdapter(IDXGIAdapter *pAdapter)
	{
		// Create a list of virtual table address
		m_pDXGIAdapterVtbl = new IDXGIAdapterVtbl();
		// Copy virtual table addresses from referenced dll which loaded from current process
		memcpy(m_pDXGIAdapterVtbl, *reinterpret_cast<IDXGIAdapterVtbl **>(pAdapter), sizeof(IDXGIAdapterVtbl));
		LOGFILE("%s: vtable=0x%p pid=%5d tid=%5d\n", __func__, m_pDXGIAdapterVtbl, GetCurrentProcessId(), GetCurrentThreadId());
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		HRESULT result = S_OK;
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());
		// Attach is usually succeeded except address is NULL or pointer of address is access violation.
		result = DetourAttach(
			&(PVOID &)m_pDXGIAdapterVtbl->QueryInterface,
			Detour_DXGIAdapter_QueryInterface);
		LOGFILE("%s: AddRef result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIAdapterVtbl->QueryInterface, Detour_DXGIAdapter_QueryInterface);
		result = DetourAttach(
			&(PVOID &)m_pDXGIAdapterVtbl->AddRef,
			Detour_DXGIAdapter_AddRef);
		LOGFILE("%s: AddRef result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIAdapterVtbl->AddRef, Detour_DXGIAdapter_AddRef);
		result = DetourAttach(
			&(PVOID &)m_pDXGIAdapterVtbl->Release,
			Detour_DXGIAdapter_Release);
		LOGFILE("%s: Release result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIAdapterVtbl->Release, Detour_DXGIAdapter_Release);
		result = DetourAttach(
			&(PVOID &)m_pDXGIAdapterVtbl->GetParent,
			Detour_DXGIAdapter_GetParent);
		LOGFILE("%s: GetParent result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIAdapterVtbl->GetParent, Detour_DXGIAdapter_GetParent);
		result = DetourAttach(
			&(PVOID &)m_pDXGIAdapterVtbl->EnumOutputs,
			Detour_DXGIAdapter_EnumOutputs);
		LOGFILE("%s: EnumOutputs result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGIAdapterVtbl->EnumOutputs, Detour_DXGIAdapter_EnumOutputs);
		// Then exception will be occurred when transaction commit with invalid address.
		result = DetourTransactionCommit();
		return result == 0 ? TRUE : FALSE;
	}

	BOOL DetachDXGIAdapter()
	{
		if (m_pDXGIAdapterVtbl == NULL)
			return TRUE;

		long result = 0;
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());
		result = DetourDetach(
			&(PVOID &)m_pDXGIAdapterVtbl->QueryInterface,
			Detour_DXGIAdapter_QueryInterface);
		result = DetourDetach(
			&(PVOID &)m_pDXGIAdapterVtbl->AddRef,
			Detour_DXGIAdapter_AddRef);
		result = DetourDetach(
			&(PVOID &)m_pDXGIAdapterVtbl->Release,
			Detour_DXGIAdapter_Release);
		result = DetourDetach(
			&(PVOID &)m_pDXGIAdapterVtbl->GetParent,
			Detour_DXGIAdapter_GetParent);
		result = DetourDetach(
			&(PVOID &)m_pDXGIAdapterVtbl->EnumOutputs,
			Detour_DXGIAdapter_EnumOutputs);
		result = DetourTransactionCommit();
		return TRUE;
	}
}
