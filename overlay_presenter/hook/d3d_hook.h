#pragma once
#include "../stdafx.h"
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>

using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")

namespace hook
{
	struct VERTEX_SHADER_INPUT_V4_0 {
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
		XMFLOAT4 Col;
	};

	static HRESULT CompileShaderV4_0(const char *entry, const char *target, ID3DBlob **ppCode)
	{
		const char *hlslcode =
			"Texture2D ShaderTexture : register(t0);\n"
			"SamplerState Sampler : register(s0);\n"
			"struct VS_INPUT {\n"
			"    float3 Pos : POSITION;\n"
			"    float2 Tex : TEXCOORD;\n"
			"    float4 Col : COLOR;\n"
			"};\n"
			"struct PS_INPUT {\n"
			"    float4 Pos : SV_POSITION;\n"
			"    float2 Tex : TEXCOORD;\n"
			"    float4 Col : COLOR;\n"
			"};\n"
			"PS_INPUT vsmain( VS_INPUT input ) {\n"
			"    PS_INPUT output;\n"
			"    output.Pos = float4(input.Pos, 1.0f);\n"
			"    output.Tex = input.Tex;\n"
			"    output.Col = input.Col;\n"
			"    return output;\n"
			"};\n"
			"float4 psmain( PS_INPUT input ) : SV_Target{\n"
			"    return ShaderTexture.Sample(Sampler, input.Tex);\n"
			"};\n"
			"technique11 EffectTech{\n"
			"    pass P0{\n"
			"        SetVertexShader( CompileShader( vs_4_0, vsmain() ) );\n"
			"        SetPixelShader( CompileShader( ps_4_0, psmain() ) );\n"
			"    }\n"
			"};\n"
			;
		DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		shaderFlags |= D3DCOMPILE_DEBUG;
#endif
		ID3DBlob *pError = NULL;
		HRESULT result = D3DCompile(hlslcode, lstrlenA(hlslcode) + 1, "overlay", NULL, NULL, entry, target, shaderFlags, 0, ppCode, &pError);
		if (FAILED(result))
		{
			LOGFILE("%s: D3DCompile failed 0x%x\n", __func__, result);
		}
		if (pError) pError->Release();
		return result;
	}
	
}