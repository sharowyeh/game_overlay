#include "dxgi_hook.h"
#include "d3d11_hook.h"

#pragma comment(lib, "dxgi.lib")

namespace hook
{
	D3D11Hook *m_pD3D11Hook = NULL;

	DXGIHook::DXGIHook(IDXGISwapChain *pSwapChain)
	{
		LOGFILE("%s: Constructor begin\n", __func__);
		if (pSwapChain == NULL)
		{
			LOGFILE("%s: swapchain=NULL\n", __func__);
			return;
		}
		// Assign detoured swap chain to local variable
		m_pSwapChain = pSwapChain;
		m_pHookDesc = new DXGI_HOOK_DESC();
		m_pD3D11Hook = new D3D11Hook(pSwapChain);

		CreateResource();
	}

	DXGIHook::~DXGIHook()
	{
		ReleaseResource();

		delete m_pD3D11Hook;
		delete m_pHookDesc;
		LOGFILE("%s: Destructor end\n", __func__);
	}

	void DXGIHook::CreateResource()
	{
		// Check swapchain
		if (m_pSwapChain == NULL)
			return;
		// Check resource has been initialized
		if (IsInitialized == true)
		{
			LOGFILE("%s: IsInitialized=true relase previous resources\n", __func__);
			ReleaseResource();
		}
		
		// Always update output boundary which resource needs to be created or re-created
		UpdateOutputRect();

		//HRESULT result = CreateD3D11Resource(m_pSwapChain);
		HRESULT result = m_pD3D11Hook->D3D11CreateResource();
		LOGFILE("%s: CreateD3D11Resource result=0x%x\n", __func__, result);
		if (SUCCEEDED(result))
			IsInitialized = true;
	}

	void DXGIHook::ReleaseResource()
	{
		// Check resource has been initialized
		if (IsInitialized == false)
			return;

		HRESULT result = m_pD3D11Hook->D3D11ReleaseResource();
		LOGFILE("%s: ReleaseD3D11Resource result=0x%x\n", __func__, result);
		if (SUCCEEDED(result))
			IsInitialized = false;
	}

	// Update exist overlay rect
	//TODO: it needs to scale content source or no show if overlay size large then output
	//TODO: it needs to notify global mouse hook if overlay rect has been changed by output
	void DXGIHook::UpdateOverlayRect(DXGI_HOOK_RECT *pOverlayRect)
	{
		// width and height can not large then signed long max
		if (m_pHookDesc->OutputRect.width > LONG_MAX ||
			m_pHookDesc->OutputRect.height > LONG_MAX)
		{
			LOGFILE("%s: OutputRect size too large that rect bits overflow\n", __func__);
			m_pHookDesc->OverlayRect = { 0 };
			if (pOverlayRect != NULL)
				*pOverlayRect = m_pHookDesc->OverlayRect;
			return;
		}
		if (m_pHookDesc->OverlayRect.width > m_pHookDesc->OutputRect.width)
		{
			m_pHookDesc->OverlayRect.left = 0;
			m_pHookDesc->OverlayRect.right = m_pHookDesc->OutputRect.width;
			m_pHookDesc->OverlayRect.width = m_pHookDesc->OutputRect.width;
		}
		if (m_pHookDesc->OverlayRect.height > m_pHookDesc->OutputRect.height)
		{
			m_pHookDesc->OverlayRect.top = 0;
			m_pHookDesc->OverlayRect.bottom = m_pHookDesc->OutputRect.width;
			m_pHookDesc->OverlayRect.height = m_pHookDesc->OutputRect.height;
		}
		if (m_pHookDesc->OverlayRect.right > 0 &&
			m_pHookDesc->OverlayRect.right > static_cast<long>(m_pHookDesc->OutputRect.width))
		{
			m_pHookDesc->OverlayRect.right = m_pHookDesc->OutputRect.width;
			m_pHookDesc->OverlayRect.left = m_pHookDesc->OverlayRect.right - m_pHookDesc->OverlayRect.width;
		}
		if (m_pHookDesc->OverlayRect.bottom > 0 &&
			m_pHookDesc->OverlayRect.bottom > static_cast<long>(m_pHookDesc->OutputRect.height))
		{
			m_pHookDesc->OverlayRect.bottom = m_pHookDesc->OutputRect.height;
			m_pHookDesc->OverlayRect.top = m_pHookDesc->OverlayRect.bottom - m_pHookDesc->OverlayRect.height;
		}
		if (m_pHookDesc->OverlayRect.left < 0)
		{
			m_pHookDesc->OverlayRect.left = 0;
			m_pHookDesc->OverlayRect.right = m_pHookDesc->OverlayRect.left + m_pHookDesc->OverlayRect.width;
		}
		if (m_pHookDesc->OverlayRect.top < 0)
		{
			m_pHookDesc->OverlayRect.top = 0;
			m_pHookDesc->OverlayRect.bottom = m_pHookDesc->OverlayRect.top + m_pHookDesc->OverlayRect.height;
		}
		if (pOverlayRect != NULL)
			*pOverlayRect = m_pHookDesc->OverlayRect;
	}

	// Update output and output window boundaries from swapchain description
	// Notice: it also effected overlay rectangle
	void DXGIHook::UpdateOutputRect(DXGI_HOOK_RECT *pOutputRect, DXGI_HOOK_RECT *pWindowRect, DXGI_HOOK_RECT *pOverlayRect)
	{
		if (m_pSwapChain == NULL)
			return;

		//TODO: may query interface with higher version of swapchain getting desc which we need
		DXGI_SWAP_CHAIN_DESC desc = {};
		HRESULT result = m_pSwapChain->GetDesc(&desc);
		LOGFILE("%s: GetDesc result=0x%x width=%d height=%d windowed=%d window_handle=0x%x\n", __func__, result,
			desc.BufferDesc.Width, desc.BufferDesc.Height, desc.Windowed, desc.OutputWindow);
		if (FAILED(result))
		{
			LOGFILE("%s: GetDesc failed 0x%x\n", __func__, result);
			return;
		}

		m_pHookDesc->OutputRect.width = desc.BufferDesc.Width;
		m_pHookDesc->OutputRect.height = desc.BufferDesc.Height;
		m_pHookDesc->OutputIsWindowed = desc.Windowed == TRUE;
		m_pHookDesc->WindowHandle = desc.OutputWindow;
		// It may uses GetClientRect and GetWindowRect win32 API if output is in windowed mode, otherwise
		// rect of output and window will be consistent if it is in fullscreen mode.
		// Notice: The output rect and window rect not only using related to overlay coordinates,
		//         but also effected global mouse hook with top-left coordinates of display screen.
		if (m_pHookDesc->OutputIsWindowed && m_pHookDesc->WindowHandle)
		{
			RECT rc = {};
			BOOL rect_result = FALSE;
			// First 4 members(left,top,right,bottom) of DXGI_HOOK_RECT can be cast to pointer of RECT struct
			rect_result = GetClientRect(m_pHookDesc->WindowHandle, reinterpret_cast<RECT *>(&m_pHookDesc->OutputRect.left));
			if (rect_result == TRUE)
			{
				// Convert window coordinates to display screen coordinate
				// Each 2 coords(left-top, right-bottom) of DXGI_HOOK_RECT can be cast to pointer of POINT struct
				ClientToScreen(m_pHookDesc->WindowHandle, reinterpret_cast<POINT *>(&m_pHookDesc->OutputRect.left));
				ClientToScreen(m_pHookDesc->WindowHandle, reinterpret_cast<POINT *>(&m_pHookDesc->OutputRect.right));

				rect_result = GetWindowRect(m_pHookDesc->WindowHandle, reinterpret_cast<RECT *>(&m_pHookDesc->WindowRect.left));
			}
			// It will return negative value when window is minimized or invisible
			if (rect_result == FALSE ||
				m_pHookDesc->OutputRect.right < 0 || m_pHookDesc->OutputRect.bottom < 0 ||
				m_pHookDesc->WindowRect.right < 0 || m_pHookDesc->WindowRect.bottom < 0)
			{
				LOGFILE("%s: GetClientRect failed\n", __func__);
				ZeroMemory(&m_pHookDesc->OutputRect, sizeof(RECT));
				ZeroMemory(&m_pHookDesc->WindowRect, sizeof(RECT));
			}
			else
			{
				m_pHookDesc->WindowRect.width = m_pHookDesc->WindowRect.right - m_pHookDesc->WindowRect.left;
				m_pHookDesc->WindowRect.height = m_pHookDesc->WindowRect.bottom - m_pHookDesc->WindowRect.top;
			}
		}
		else
		{
			m_pHookDesc->OutputRect.left = 0;
			m_pHookDesc->OutputRect.top = 0;
			m_pHookDesc->OutputRect.right = m_pHookDesc->OutputRect.width;
			m_pHookDesc->OutputRect.bottom = m_pHookDesc->OutputRect.height;
			m_pHookDesc->WindowRect = m_pHookDesc->OutputRect;
		}
		LOGFILE("%s: OutputRect left=%d top=%d right=%d bottom=%d width=%d height=%d\n", __func__,
			m_pHookDesc->OutputRect.left, m_pHookDesc->OutputRect.top,
			m_pHookDesc->OutputRect.right, m_pHookDesc->OutputRect.bottom,
			m_pHookDesc->OutputRect.width, m_pHookDesc->OutputRect.height);
		LOGFILE("%s: WindowRect left=%d top=%d right=%d bottom=%d width=%d height=%d\n", __func__,
			m_pHookDesc->WindowRect.left, m_pHookDesc->WindowRect.top,
			m_pHookDesc->WindowRect.right, m_pHookDesc->WindowRect.bottom,
			m_pHookDesc->WindowRect.width, m_pHookDesc->WindowRect.height);
		// Assign to return parameters
		if (pOutputRect != NULL)
			*pOutputRect = m_pHookDesc->OutputRect;
		if (pWindowRect != NULL)
			*pWindowRect = m_pHookDesc->WindowRect;
		// Also effected overlay rect if output size has been changed
		UpdateOverlayRect(pOverlayRect);
	}

	// Update overlay rect by given left-top coord and size
	void DXGIHook::UpdateOverlayRect(long x, long y, unsigned long width, unsigned long height, DXGI_HOOK_RECT *pOverlayRect)
	{
		m_pHookDesc->OverlayRect.left = x;
		m_pHookDesc->OverlayRect.top = y;
		m_pHookDesc->OverlayRect.right = x + width;
		m_pHookDesc->OverlayRect.bottom = y + height;
		m_pHookDesc->OverlayRect.width = width;
		m_pHookDesc->OverlayRect.height = height;
		// Update with output rect
		UpdateOverlayRect(pOverlayRect);
	}

	// Map view of file mapping from content source for buffer creation or overlay size changed
	// Notice: memory leak should be concerned but it is super fast communication way between processes
	void DXGIHook::MapOverlayBuffer(const char *target)
	{
		if (m_pHookDesc->BufferMappingHandle == NULL)
		{
			m_pHookDesc->BufferMappingHandle = OpenFileMappingA(FILE_MAP_READ, FALSE, target);
			LOGFILE("%s: OpenFileMapping handle=0x%p target=%s\n", __func__, m_pHookDesc->BufferMappingHandle, target);
			if (m_pHookDesc->BufferMappingHandle == NULL)
				return;
		}
		// Check buffer length of map view if overlay size was changed
		// (buffer length = width * height * 4, 4 is 32bit color channels)
		size_t length = m_pHookDesc->OverlayRect.width * m_pHookDesc->OverlayRect.height * 4;
		if (m_pHookDesc->BufferLength != length)
		{
			// Unmap exist view
			BOOL result = TRUE;
			if (m_pHookDesc->BufferViewHandle != NULL)
			{
				result = UnmapViewOfFile(m_pHookDesc->BufferViewHandle);
				m_pHookDesc->BufferViewHandle = NULL;
			}
			LOGFILE("%s: UnmapViewOfFile result=%d length=%ul -> %ul\n", __func__, result, m_pHookDesc->BufferLength, length);
			m_pHookDesc->BufferLength = length;
		}
		// Map view of file mapping
		if (m_pHookDesc->BufferViewHandle == NULL && m_pHookDesc->BufferLength > 0)
		{
			m_pHookDesc->BufferViewHandle = MapViewOfFile(m_pHookDesc->BufferMappingHandle, FILE_MAP_READ, 0, 0, m_pHookDesc->BufferLength);
			LOGFILE("%s: MapViewOfFile handle=0x%p buffer width=%d height=%d\n", __func__, m_pHookDesc->BufferViewHandle,
				m_pHookDesc->OverlayRect.width, m_pHookDesc->OverlayRect.height);
			if (m_pHookDesc->BufferViewHandle == NULL)
				return;
		}
	}

	void DXGIHook::UnmapOverlayBuffer()
	{
		if (m_pHookDesc->BufferViewHandle)
		{
			UnmapViewOfFile(m_pHookDesc->BufferViewHandle);
			m_pHookDesc->BufferViewHandle = NULL;
		}
		if (m_pHookDesc->BufferMappingHandle)
		{
			CloseHandle(m_pHookDesc->BufferMappingHandle);
			m_pHookDesc->BufferMappingHandle = NULL;
		}
	}

	void DXGIHook::UpdateOverlayState(bool enabled, float alpha)
	{
		m_pHookDesc->OverlayEnabled = enabled;
		m_pHookDesc->OverlayAlpha = min(max(alpha, 0.0f), 1.0f);
	}

	void DXGIHook::DrawTestD3D11Overlay(unsigned char *pbData, DXGI_HOOK_RECT rect)
	{
		m_pD3D11Hook->D3D11DrawOverlayState(pbData, m_pHookDesc->OverlayRect);
	}

	void DXGIHook::DrawOverlay()
	{
		// Check required data: resource created, buffer mapped, overlay size valid
		if (IsInitialized == false ||
			m_pHookDesc->BufferViewHandle == NULL ||
			m_pHookDesc->OverlayRect.width == 0 || m_pHookDesc->OverlayRect.height == 0)
			return;

		// BGRA32(B8G8R8A8) from buffer
		size_t length = m_pHookDesc->OverlayRect.width * m_pHookDesc->OverlayRect.height * 4;
		BYTE *pbData = new BYTE[length];
		// Overlay enabled state indicates draw a transparent rectangle or buffered rectangle
		//TODO: or cleanup texture2d resource reducing memory access
		if (m_pHookDesc->OverlayEnabled)
			CopyMemory(pbData, m_pHookDesc->BufferViewHandle, length);
		else
			ZeroMemory(pbData, length);
		// And operator alpha bits
		//TODO: It may be another solution using Context->OMSetBlendState reducing memory access
		if (m_pHookDesc->OverlayAlpha < 1.0f)
		{
			BYTE alpha = (BYTE)(m_pHookDesc->OverlayAlpha * 255);
			for (size_t i = 3; i < length; i += 4)
				pbData[i] &= alpha;
		}
		
		//DrawD3D11Overlay(pbData, m_pHookDesc->OverlayRect);
		m_pD3D11Hook->D3D11DrawOverlay(pbData, m_pHookDesc->OverlayRect);

		delete pbData;
	}
}