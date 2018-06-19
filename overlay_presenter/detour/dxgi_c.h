#pragma once
#include <dxgi.h>

#ifdef __cplusplus

// IDXGISwapChain c style interface deplicated from dxgi.h
typedef struct IDXGISwapChainVtbl
{
	BEGIN_INTERFACE

	HRESULT(STDMETHODCALLTYPE *QueryInterface)(
		IDXGISwapChain * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		_COM_Outptr_  void **ppvObject);

	ULONG(STDMETHODCALLTYPE *AddRef)(
		IDXGISwapChain * This);

	ULONG(STDMETHODCALLTYPE *Release)(
		IDXGISwapChain * This);

	HRESULT(STDMETHODCALLTYPE *SetPrivateData)(
		IDXGISwapChain * This,
		/* [annotation][in] */
		_In_  REFGUID Name,
		/* [in] */ UINT DataSize,
		/* [annotation][in] */
		_In_reads_bytes_(DataSize)  const void *pData);

	HRESULT(STDMETHODCALLTYPE *SetPrivateDataInterface)(
		IDXGISwapChain * This,
		/* [annotation][in] */
		_In_  REFGUID Name,
		/* [annotation][in] */
		_In_opt_  const IUnknown *pUnknown);

	HRESULT(STDMETHODCALLTYPE *GetPrivateData)(
		IDXGISwapChain * This,
		/* [annotation][in] */
		_In_  REFGUID Name,
		/* [annotation][out][in] */
		_Inout_  UINT *pDataSize,
		/* [annotation][out] */
		_Out_writes_bytes_(*pDataSize)  void *pData);

	HRESULT(STDMETHODCALLTYPE *GetParent)(
		IDXGISwapChain * This,
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][retval][out] */
		_COM_Outptr_  void **ppParent);

	HRESULT(STDMETHODCALLTYPE *GetDevice)(
		IDXGISwapChain * This,
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][retval][out] */
		_COM_Outptr_  void **ppDevice);

	HRESULT(STDMETHODCALLTYPE *Present)(
		IDXGISwapChain * This,
		/* [in] */ UINT SyncInterval,
		/* [in] */ UINT Flags);

	HRESULT(STDMETHODCALLTYPE *GetBuffer)(
		IDXGISwapChain * This,
		/* [in] */ UINT Buffer,
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][out][in] */
		_COM_Outptr_  void **ppSurface);

	HRESULT(STDMETHODCALLTYPE *SetFullscreenState)(
		IDXGISwapChain * This,
		/* [in] */ BOOL Fullscreen,
		/* [annotation][in] */
		_In_opt_  IDXGIOutput *pTarget);

	HRESULT(STDMETHODCALLTYPE *GetFullscreenState)(
		IDXGISwapChain * This,
		/* [annotation][out] */
		_Out_opt_  BOOL *pFullscreen,
		/* [annotation][out] */
		_COM_Outptr_opt_result_maybenull_  IDXGIOutput **ppTarget);

	HRESULT(STDMETHODCALLTYPE *GetDesc)(
		IDXGISwapChain * This,
		/* [annotation][out] */
		_Out_  DXGI_SWAP_CHAIN_DESC *pDesc);

	HRESULT(STDMETHODCALLTYPE *ResizeBuffers)(
		IDXGISwapChain * This,
		/* [in] */ UINT BufferCount,
		/* [in] */ UINT Width,
		/* [in] */ UINT Height,
		/* [in] */ DXGI_FORMAT NewFormat,
		/* [in] */ UINT SwapChainFlags);

	HRESULT(STDMETHODCALLTYPE *ResizeTarget)(
		IDXGISwapChain * This,
		/* [annotation][in] */
		_In_  const DXGI_MODE_DESC *pNewTargetParameters);

	HRESULT(STDMETHODCALLTYPE *GetContainingOutput)(
		IDXGISwapChain * This,
		/* [annotation][out] */
		_COM_Outptr_  IDXGIOutput **ppOutput);

	HRESULT(STDMETHODCALLTYPE *GetFrameStatistics)(
		IDXGISwapChain * This,
		/* [annotation][out] */
		_Out_  DXGI_FRAME_STATISTICS *pStats);

	HRESULT(STDMETHODCALLTYPE *GetLastPresentCount)(
		IDXGISwapChain * This,
		/* [annotation][out] */
		_Out_  UINT *pLastPresentCount);

	END_INTERFACE
} IDXGISwapChainVtbl;

typedef struct IDXGIDeviceVtbl
{
	BEGIN_INTERFACE

	HRESULT(STDMETHODCALLTYPE *QueryInterface)(
		IDXGIDevice * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		_COM_Outptr_  void **ppvObject);

	ULONG(STDMETHODCALLTYPE *AddRef)(
		IDXGIDevice * This);

	ULONG(STDMETHODCALLTYPE *Release)(
		IDXGIDevice * This);

	HRESULT(STDMETHODCALLTYPE *SetPrivateData)(
		IDXGIDevice * This,
		/* [annotation][in] */
		_In_  REFGUID Name,
		/* [in] */ UINT DataSize,
		/* [annotation][in] */
		_In_reads_bytes_(DataSize)  const void *pData);

	HRESULT(STDMETHODCALLTYPE *SetPrivateDataInterface)(
		IDXGIDevice * This,
		/* [annotation][in] */
		_In_  REFGUID Name,
		/* [annotation][in] */
		_In_opt_  const IUnknown *pUnknown);

	HRESULT(STDMETHODCALLTYPE *GetPrivateData)(
		IDXGIDevice * This,
		/* [annotation][in] */
		_In_  REFGUID Name,
		/* [annotation][out][in] */
		_Inout_  UINT *pDataSize,
		/* [annotation][out] */
		_Out_writes_bytes_(*pDataSize)  void *pData);

	HRESULT(STDMETHODCALLTYPE *GetParent)(
		IDXGIDevice * This,
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][retval][out] */
		_COM_Outptr_  void **ppParent);

	HRESULT(STDMETHODCALLTYPE *GetAdapter)(
		IDXGIDevice * This,
		/* [annotation][out] */
		_COM_Outptr_  IDXGIAdapter **pAdapter);

	HRESULT(STDMETHODCALLTYPE *CreateSurface)(
		IDXGIDevice * This,
		/* [annotation][in] */
		_In_  const DXGI_SURFACE_DESC *pDesc,
		/* [in] */ UINT NumSurfaces,
		/* [in] */ DXGI_USAGE Usage,
		/* [annotation][in] */
		_In_opt_  const DXGI_SHARED_RESOURCE *pSharedResource,
		/* [annotation][out] */
		_COM_Outptr_  IDXGISurface **ppSurface);

	HRESULT(STDMETHODCALLTYPE *QueryResourceResidency)(
		IDXGIDevice * This,
		/* [annotation][size_is][in] */
		_In_reads_(NumResources)  IUnknown *const *ppResources,
		/* [annotation][size_is][out] */
		_Out_writes_(NumResources)  DXGI_RESIDENCY *pResidencyStatus,
		/* [in] */ UINT NumResources);

	HRESULT(STDMETHODCALLTYPE *SetGPUThreadPriority)(
		IDXGIDevice * This,
		/* [in] */ INT Priority);

	HRESULT(STDMETHODCALLTYPE *GetGPUThreadPriority)(
		IDXGIDevice * This,
		/* [annotation][retval][out] */
		_Out_  INT *pPriority);

	END_INTERFACE
} IDXGIDeviceVtbl;

typedef struct IDXGIAdapterVtbl
{
	BEGIN_INTERFACE

	HRESULT(STDMETHODCALLTYPE *QueryInterface)(
		IDXGIAdapter * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		_COM_Outptr_  void **ppvObject);

	ULONG(STDMETHODCALLTYPE *AddRef)(
		IDXGIAdapter * This);

	ULONG(STDMETHODCALLTYPE *Release)(
		IDXGIAdapter * This);

	HRESULT(STDMETHODCALLTYPE *SetPrivateData)(
		IDXGIAdapter * This,
		/* [annotation][in] */
		_In_  REFGUID Name,
		/* [in] */ UINT DataSize,
		/* [annotation][in] */
		_In_reads_bytes_(DataSize)  const void *pData);

	HRESULT(STDMETHODCALLTYPE *SetPrivateDataInterface)(
		IDXGIAdapter * This,
		/* [annotation][in] */
		_In_  REFGUID Name,
		/* [annotation][in] */
		_In_opt_  const IUnknown *pUnknown);

	HRESULT(STDMETHODCALLTYPE *GetPrivateData)(
		IDXGIAdapter * This,
		/* [annotation][in] */
		_In_  REFGUID Name,
		/* [annotation][out][in] */
		_Inout_  UINT *pDataSize,
		/* [annotation][out] */
		_Out_writes_bytes_(*pDataSize)  void *pData);

	HRESULT(STDMETHODCALLTYPE *GetParent)(
		IDXGIAdapter * This,
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][retval][out] */
		_COM_Outptr_  void **ppParent);

	HRESULT(STDMETHODCALLTYPE *EnumOutputs)(
		IDXGIAdapter * This,
		/* [in] */ UINT Output,
		/* [annotation][out][in] */
		_COM_Outptr_  IDXGIOutput **ppOutput);

	HRESULT(STDMETHODCALLTYPE *GetDesc)(
		IDXGIAdapter * This,
		/* [annotation][out] */
		_Out_  DXGI_ADAPTER_DESC *pDesc);

	HRESULT(STDMETHODCALLTYPE *CheckInterfaceSupport)(
		IDXGIAdapter * This,
		/* [annotation][in] */
		_In_  REFGUID InterfaceName,
		/* [annotation][out] */
		_Out_  LARGE_INTEGER *pUMDVersion);

	END_INTERFACE
} IDXGIAdapterVtbl;

typedef struct IDXGIFactoryVtbl
{
	BEGIN_INTERFACE

	HRESULT(STDMETHODCALLTYPE *QueryInterface)(
		IDXGIFactory * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		_COM_Outptr_  void **ppvObject);

	ULONG(STDMETHODCALLTYPE *AddRef)(
		IDXGIFactory * This);

	ULONG(STDMETHODCALLTYPE *Release)(
		IDXGIFactory * This);

	HRESULT(STDMETHODCALLTYPE *SetPrivateData)(
		IDXGIFactory * This,
		/* [annotation][in] */
		_In_  REFGUID Name,
		/* [in] */ UINT DataSize,
		/* [annotation][in] */
		_In_reads_bytes_(DataSize)  const void *pData);

	HRESULT(STDMETHODCALLTYPE *SetPrivateDataInterface)(
		IDXGIFactory * This,
		/* [annotation][in] */
		_In_  REFGUID Name,
		/* [annotation][in] */
		_In_opt_  const IUnknown *pUnknown);

	HRESULT(STDMETHODCALLTYPE *GetPrivateData)(
		IDXGIFactory * This,
		/* [annotation][in] */
		_In_  REFGUID Name,
		/* [annotation][out][in] */
		_Inout_  UINT *pDataSize,
		/* [annotation][out] */
		_Out_writes_bytes_(*pDataSize)  void *pData);

	HRESULT(STDMETHODCALLTYPE *GetParent)(
		IDXGIFactory * This,
		/* [annotation][in] */
		_In_  REFIID riid,
		/* [annotation][retval][out] */
		_COM_Outptr_  void **ppParent);

	HRESULT(STDMETHODCALLTYPE *EnumAdapters)(
		IDXGIFactory * This,
		/* [in] */ UINT Adapter,
		/* [annotation][out] */
		_COM_Outptr_  IDXGIAdapter **ppAdapter);

	HRESULT(STDMETHODCALLTYPE *MakeWindowAssociation)(
		IDXGIFactory * This,
		HWND WindowHandle,
		UINT Flags);

	HRESULT(STDMETHODCALLTYPE *GetWindowAssociation)(
		IDXGIFactory * This,
		/* [annotation][out] */
		_Out_  HWND *pWindowHandle);

	HRESULT(STDMETHODCALLTYPE *CreateSwapChain)(
		IDXGIFactory * This,
		/* [annotation][in] */
		_In_  IUnknown *pDevice,
		/* [annotation][in] */
		_In_  DXGI_SWAP_CHAIN_DESC *pDesc,
		/* [annotation][out] */
		_COM_Outptr_  IDXGISwapChain **ppSwapChain);

	HRESULT(STDMETHODCALLTYPE *CreateSoftwareAdapter)(
		IDXGIFactory * This,
		/* [in] */ HMODULE Module,
		/* [annotation][out] */
		_COM_Outptr_  IDXGIAdapter **ppAdapter);

	END_INTERFACE
} IDXGIFactoryVtbl;

#endif