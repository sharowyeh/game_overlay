#pragma once
#include "../stdafx.h"
#include <dxgi.h>
#include <d3d11.h>

namespace hook
{
	struct DXGI_HOOK_RECT
	{
		long left;
		long top;
		long right;
		long bottom;
		unsigned int width;
		unsigned int height;
	};

	struct DXGI_HOOK_DESC
	{
		bool OverlayEnabled = true;
		float OverlayAlpha = 1.0f;
		// Rect of rendering overlay related to render view target screen
		DXGI_HOOK_RECT OverlayRect = {};
		// Buffer for overlay data
		HANDLE BufferMappingHandle = NULL;
		HANDLE BufferViewHandle = NULL;
		size_t BufferLength = 0;
		// Rect of graphic output(render view) related to display screen
		DXGI_HOOK_RECT OutputRect = {};
		bool OutputIsWindowed = false;
		// Rect of output window related to display screen
		DXGI_HOOK_RECT WindowRect = {};
		HWND WindowHandle = NULL;
	};

	class DXGIHook
	{
	public:
		// Use detoured DXGISwapChain then drawing by D3D11Device(or D3D12Device?)
		DXGIHook(IDXGISwapChain *pSwapChain);
		virtual ~DXGIHook();
		void CreateResource();
		void ReleaseResource();
		bool IsInitialized = false;
		void UpdateOutputRect(DXGI_HOOK_RECT *pOutputRect = NULL, DXGI_HOOK_RECT *pWindowRect = NULL, DXGI_HOOK_RECT *pOverlayRect = NULL);
		void UpdateOverlayRect(long x, long y, unsigned long width, unsigned long height, DXGI_HOOK_RECT *pOverlayRect = NULL);
		void MapOverlayBuffer(const char *target);
		void UnmapOverlayBuffer();
		void UpdateOverlayState(bool enabled, float alpha);
		void DrawOverlay();
		//test
		void DrawTestD3D11Overlay(unsigned char *pbData, DXGI_HOOK_RECT rect);
	private:
		void UpdateOverlayRect(DXGI_HOOK_RECT *pOverlayRect = NULL);

		DXGI_HOOK_DESC *m_pHookDesc = NULL;
		IDXGISwapChain *m_pSwapChain = NULL;
	};
}