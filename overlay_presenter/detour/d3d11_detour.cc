#include "d3d11_detour.h"

#ifdef LOGFILE
#undef LOGFILE
#define LOGFILE(fmt, ...) WriteLog("detour.txt", fmt, ##__VA_ARGS__)
#endif

namespace detour
{
	BOOL AttachD3D11()
	{
		// Create a dummy window then assign HWND to override method
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_CLASSDC;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = L"dummy_class";
		wc.lpfnWndProc = DefWindowProc;
		RegisterClassEx(&wc);
		HWND hwnd = CreateWindow(L"dummy_class", L"dummy_window", NULL, 0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);
		if (hwnd == NULL)
			return FALSE;
		// Notice: almost forgot D3D must needs HWND assigned to swap chain's output window,
		// and can not called from dllmain because related dll may not loaded
		ID3D11Device *pDevice = NULL;
		ID3D11DeviceContext *pContext = NULL;
		IDXGISwapChain *pSwapChain = NULL;
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
		DXGI_MODE_DESC modeDesc = {};
		modeDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		modeDesc.Width = 1;
		modeDesc.Height = 1;
		modeDesc.RefreshRate.Numerator = 60;
		modeDesc.RefreshRate.Denominator = 1;
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferDesc = modeDesc;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		//HRESULT result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
		//	&featureLevel, 1, D3D11_SDK_VERSION, &pDevice, NULL, &pContext);
		//LOGFILE("%s: CreateDevice result=0x%x device=0x%p context=0x%p\n", __func__, result, pDevice, pContext);
		HRESULT result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
			&featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, NULL, &pContext);
		LOGFILE("%s: CreateDeviceAndSwapChain result=0x%x device=0x%p swapchain=0x%p context=0x%p\n", __func__, result, pDevice, pSwapChain, pContext);
		// Create DXGI objects
		IDXGIDevice *pDXGIDevice = NULL;
		IDXGIAdapter *pDXGIAdapter = NULL;
		IDXGIFactory *pDXGIFactory = NULL;
		if (pDevice != NULL)
			result = pDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void **>(&pDXGIDevice));
		if (pDXGIDevice != NULL)
		{
			result = pDXGIDevice->GetAdapter(&pDXGIAdapter);
			result = AttachDXGIDevice(pDXGIDevice);
			pDXGIDevice->Release();
		}
		if (pDXGIAdapter != NULL)
		{
			result = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void **>(&pDXGIFactory));
			result = AttachDXGIAdapter(pDXGIAdapter);
			pDXGIAdapter->Release();
		}
		if (pDXGIFactory != NULL)
		{
			result = AttachDXGIFactory(pDXGIFactory);
			pDXGIFactory->Release();
		}
		if (pSwapChain != NULL)
		{
			result = AttachDXGISwapChain(pSwapChain);
			pSwapChain->Release();
		}
		if (pContext != NULL)
		{
			result = AttachD3D11DeviceContext(pContext);
			pContext->Release();
		}
		if (pDevice != NULL)
		{
			result = AttachD3D11Device(pDevice);
			pDevice->Release();
		}
		// Close window or destory window
		DestroyWindow(hwnd);
		return result;
	}

	BOOL DetachD3D11()
	{
		DetachDXGIDevice();
		DetachDXGIAdapter();
		DetachDXGIFactory();
		DetachDXGISwapChain();
		DetachD3D11DeviceContext();
		DetachD3D11Device();
		return TRUE;
	}
}