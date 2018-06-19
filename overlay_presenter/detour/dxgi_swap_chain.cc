#include "d3d11_detour.h"
#include "dxgi_c.h"
#include "../hook/dxgi_hook.h"

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
		IDXGISwapChainVtbl *m_pDXGISwapChainVtbl = NULL;

		HRESULT STDMETHODCALLTYPE Detour_DXGISwapChain_QueryInterface(
			IDXGISwapChain * This,
			/* [in] */ REFIID riid,
			/* [annotation][iid_is][out] */
			_COM_Outptr_  void **ppvObject)
		{
			HRESULT result = m_pDXGISwapChainVtbl->QueryInterface(This, riid, ppvObject);
			LOGFILE("%s: result=0x%x swapchain=0x%p vobj=0x%p\n", __func__, result, This, (*ppvObject));
			return result;
		}

		ULONG STDMETHODCALLTYPE Detour_DXGISwapChain_AddRef(
			IDXGISwapChain * This)
		{
			ULONG result = m_pDXGISwapChainVtbl->AddRef(This);
			LOGFILE("%s: result=0x%x swapchain=0x%p\n", __func__, result, This);
			return result;
		}

		hook::DXGIHook *m_pDXGIHook = NULL;
		hook::DXGI_HOOK_RECT m_DXGIOverlayRect = {};

		ULONG STDMETHODCALLTYPE Detour_DXGISwapChain_Release(
			IDXGISwapChain * This)
		{
			ULONG result = m_pDXGISwapChainVtbl->Release(This);
			LOGFILE("%s: result=0x%x swapchain=0x%p\n", __func__, result, This);
			if (result == 0 && m_pDXGIHook != NULL)
			{
				delete m_pDXGIHook;
				m_pDXGIHook = NULL;
			}
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_DXGISwapChain_Present(
			IDXGISwapChain * This,
			/* [in] */ UINT SyncInterval,
			/* [in] */ UINT Flags)
		{
			if (m_pDXGIHook == NULL)
				m_pDXGIHook = new hook::DXGIHook(This);
			else
			{
				if (m_DXGIOverlayRect.width == 0)
					m_pDXGIHook->UpdateOverlayRect(20, 20, 100, 150, &m_DXGIOverlayRect);
				else
				{
					size_t length = m_DXGIOverlayRect.width * m_DXGIOverlayRect.height * 4;
					BYTE *pbData = new BYTE[length];
					for (size_t i = 0; i < length; i++)
					{
						if (i % 4 == 0 || i % 4 == 3)
							pbData[i] = (BYTE)255;
						else
							pbData[i] = (BYTE)0;
					}
					m_pDXGIHook->DrawTestD3D11Overlay(pbData, m_DXGIOverlayRect);
					delete pbData;
				}
			}
			HRESULT result = m_pDXGISwapChainVtbl->Present(This, SyncInterval, Flags);
			LOGFILE("%s: result=0x%x swapchain=0x%p interval=%d flags=%d\n", __func__, result, This, SyncInterval, Flags);
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_DXGISwapChain_SetFullscreenState(
			IDXGISwapChain * This,
			/* [in] */ BOOL Fullscreen,
			/* [annotation][in] */
			_In_opt_  IDXGIOutput *pTarget)
		{
			HRESULT result = m_pDXGISwapChainVtbl->SetFullscreenState(This, Fullscreen, pTarget);
			LOGFILE("%s: result=0x%x swapchain=0x%p full=%d target=0x%p\n", __func__, result, This, Fullscreen, pTarget);
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_DXGISwapChain_ResizeBuffers(
			IDXGISwapChain * This,
			/* [in] */ UINT BufferCount,
			/* [in] */ UINT Width,
			/* [in] */ UINT Height,
			/* [in] */ DXGI_FORMAT NewFormat,
			/* [in] */ UINT SwapChainFlags)
		{
			HRESULT result = m_pDXGISwapChainVtbl->ResizeBuffers(This, BufferCount, Width, Height, NewFormat, SwapChainFlags);
			LOGFILE("%s: result=0x%x swapchain=0x%p count=%d width=%d height=%d format=%d flags=%d\n", __func__, result, This, BufferCount, Width, Height, NewFormat, SwapChainFlags);
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_DXGISwapChain_ResizeTarget(
			IDXGISwapChain * This,
			/* [annotation][in] */
			_In_  const DXGI_MODE_DESC *pNewTargetParameters)
		{
			HRESULT result = m_pDXGISwapChainVtbl->ResizeTarget(This, pNewTargetParameters);
			LOGFILE("%s: result=0x%x swapchain=0x%p width=%d height=%d format=%d\n", __func__, result, This, pNewTargetParameters->Width, pNewTargetParameters->Height, pNewTargetParameters->Format);
			return result;
		}

#ifdef __cplusplus
	}
#endif

	BOOL AttachDXGISwapChain(IDXGISwapChain *pSwapChain)
	{
		// Create a list of virtual table address
		m_pDXGISwapChainVtbl = new IDXGISwapChainVtbl();
		// Copy virtual table addresses from referenced dll which loaded from current process
		memcpy(m_pDXGISwapChainVtbl, *reinterpret_cast<IDXGISwapChainVtbl **>(pSwapChain), sizeof(IDXGISwapChainVtbl));
		LOGFILE("%s: vtable=0x%p pid=%5d tid=%5d\n", __func__, m_pDXGISwapChainVtbl, GetCurrentProcessId(), GetCurrentThreadId());
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		HRESULT result = S_OK;
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());
		// Attach is usually succeeded except address is NULL or pointer of address is access violation.
		result = DetourAttach(
			&(PVOID &)m_pDXGISwapChainVtbl->QueryInterface,
			Detour_DXGISwapChain_QueryInterface);
		LOGFILE("%s: AddRef result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGISwapChainVtbl->QueryInterface, Detour_DXGISwapChain_QueryInterface);
		result = DetourAttach(
			&(PVOID &)m_pDXGISwapChainVtbl->AddRef,
			Detour_DXGISwapChain_AddRef);
		LOGFILE("%s: AddRef result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGISwapChainVtbl->AddRef, Detour_DXGISwapChain_AddRef);
		result = DetourAttach(
			&(PVOID &)m_pDXGISwapChainVtbl->Release,
			Detour_DXGISwapChain_Release);
		LOGFILE("%s: Release result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGISwapChainVtbl->Release, Detour_DXGISwapChain_Release);
		result = DetourAttach(
			&(PVOID &)m_pDXGISwapChainVtbl->Present,
			Detour_DXGISwapChain_Present);
		LOGFILE("%s: Present result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGISwapChainVtbl->Present, Detour_DXGISwapChain_Present);
		result = DetourAttach(
			&(PVOID &)m_pDXGISwapChainVtbl->SetFullscreenState,
			Detour_DXGISwapChain_SetFullscreenState);
		LOGFILE("%s: SetFullscreenState result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGISwapChainVtbl->SetFullscreenState, Detour_DXGISwapChain_SetFullscreenState);
		result = DetourAttach(
			&(PVOID &)m_pDXGISwapChainVtbl->ResizeBuffers,
			Detour_DXGISwapChain_ResizeBuffers);
		LOGFILE("%s: ResizeBuffers result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGISwapChainVtbl->ResizeBuffers, Detour_DXGISwapChain_ResizeBuffers);
		result = DetourAttach(
			&(PVOID &)m_pDXGISwapChainVtbl->ResizeTarget,
			Detour_DXGISwapChain_ResizeTarget);
		LOGFILE("%s: ResizeTarget result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pDXGISwapChainVtbl->ResizeTarget, Detour_DXGISwapChain_ResizeTarget);
		// Then exception will be occurred when transaction commit with invalid address.
		result = DetourTransactionCommit();
		return result == 0 ? TRUE : FALSE;
	}

	BOOL DetachDXGISwapChain()
	{
		if (m_pDXGISwapChainVtbl == NULL)
			return TRUE;

		long result = 0;
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());
		//TODO: check m_pDXGISwapChainVtbl->method address is the same with previous given pSwapChain?
		result = DetourDetach(
			&(PVOID &)m_pDXGISwapChainVtbl->QueryInterface,
			Detour_DXGISwapChain_QueryInterface);
		result = DetourDetach(
			&(PVOID &)m_pDXGISwapChainVtbl->AddRef,
			Detour_DXGISwapChain_AddRef);
		result = DetourDetach(
			&(PVOID &)m_pDXGISwapChainVtbl->Release,
			Detour_DXGISwapChain_Release);
		result = DetourDetach(
			&(PVOID &)m_pDXGISwapChainVtbl->Present,
			Detour_DXGISwapChain_Present);
		result = DetourDetach(
			&(PVOID &)m_pDXGISwapChainVtbl->SetFullscreenState,
			Detour_DXGISwapChain_SetFullscreenState);
		result = DetourDetach(
			&(PVOID &)m_pDXGISwapChainVtbl->ResizeBuffers,
			Detour_DXGISwapChain_ResizeBuffers);
		result = DetourDetach(
			&(PVOID &)m_pDXGISwapChainVtbl->ResizeTarget,
			Detour_DXGISwapChain_ResizeTarget);
		result = DetourTransactionCommit();
		return TRUE;
	}
}