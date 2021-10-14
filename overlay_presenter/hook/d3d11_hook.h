#pragma once
#include "../stdafx.h"
#include "dxgi_hook.h"
#include <d3d11.h>

namespace hook
{
	class D3D11HookContextState
	{
	public:
		D3D11HookContextState();
		virtual ~D3D11HookContextState();
		void SetState(ID3D11DeviceContext *pContext);
		void GetState(ID3D11DeviceContext *pContext);

	private:
		ID3D11VertexShader * pVertexShader = NULL;
		ID3D11PixelShader *pPixelShader = NULL;
		ID3D11SamplerState *ppSamplerStates[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
		ID3D11ShaderResourceView *ppShaderResourceViews[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
		ID3D11InputLayout *pInputLayout = NULL;
		ID3D11Buffer *pIndexBuffer = NULL;
		DXGI_FORMAT indexBufferFormat = DXGI_FORMAT_UNKNOWN;
		UINT indexBufferOffset = 0;
		D3D11_PRIMITIVE_TOPOLOGY primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
		ID3D11Buffer *ppVertexBuffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
		UINT vertexStrides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
		UINT vertexOffsets[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
		ID3D11RenderTargetView *ppRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		ID3D11DepthStencilView *pDepthStencilView = NULL;
		ID3D11BlendState *pBlendState = NULL;
		float blendStateFactor[4] = {};
		UINT blendMask = 0;
		ID3D11RasterizerState *pRasterizerState = NULL;
		UINT viewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
		D3D11_VIEWPORT pViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	};

	class D3D11Hook
	{
	public:
		D3D11Hook(IDXGISwapChain *pSwapChain);
		virtual ~D3D11Hook();

		HRESULT D3D11CreateResource();
		HRESULT D3D11ReleaseResource();
		void D3D11DrawOverlayState(unsigned char *pbData, DXGI_HOOK_RECT rect);
		void D3D11DrawOverlay(unsigned char *pbData, DXGI_HOOK_RECT rect);
		void D3D11CreateShaderResource(unsigned char *pbData, DXGI_HOOK_RECT rect);
		void D3D11UpdateShaderResource(unsigned char *pbData, DXGI_HOOK_RECT rect);
		void D3D11ReleaseShaderResource();
	private:
		IDXGISwapChain * m_pSwapChain = NULL;
		ID3D11Device *m_pDevice = NULL;
		ID3D11DeviceContext *m_pDeferredContext = NULL;
		ID3D11DeviceContext *m_pImmediateContext = NULL;
		D3D11HookContextState *m_pImmediateState = NULL;
		ID3D11RenderTargetView *m_pRenderTargetView = NULL;
		ID3D11VertexShader *m_pVertexShader = NULL;
		ID3D11PixelShader *m_pPixelShader = NULL;
		ID3D11InputLayout *m_pInputLayout = NULL;
		ID3D11Buffer *m_pVertexBuffer = NULL;
		ID3D11Buffer *m_pIndexBuffer = NULL;
		ID3D11SamplerState *m_pSamplerState = NULL;
		ID3D11BlendState *m_pBlendState = NULL;
		ID3D11Texture2D *m_pTexture2D = NULL;
		ID3D11ShaderResourceView *m_pShaderResourceView = NULL;
	};
}