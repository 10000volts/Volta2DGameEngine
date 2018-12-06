#pragma once

#include <D3D11.h>
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include "stdafx.h"

namespace VoltaEngine{
	// 必须先调用的函数。
	bool Init(HINSTANCE hi, HWND hw, int width, int height, bool window);
	bool InitLib();
	// 过渡时使用。应该删除的函数。
	void AddShaderResourceFromFile(string ShaderName, LPCSTR file);
	// ShaderName:Shader的名称。不应包括扩展名。
	void AddShaderResource(string ShaderName);
	// 过渡时使用。应该删除的函数。
	void AddTextureFromFile(string TextureName, LPCSTR file);
	// 添加纹理、其模型空间顶点缓存以及其尺寸到纹理库。
	void AddTexture(string TextureName);
	inline void CompileShaderFromFile(LPCSTR file, LPCSTR entry, LPCSTR shaderModel, ID3DBlob** buffer){
		D3DX11CompileFromFile(file, 0, 0, entry, shaderModel,
			D3DCOMPILE_ENABLE_STRICTNESS, 0, 0, buffer, 0, 0);
	}
	// 从内存编译Shader。
	inline void CompileShader(string code, LPCSTR entry, LPCSTR shaderModel, ID3DBlob** buffer){
		//D3DCompile(&code, strlen(code.c_str()), nullptr, nullptr, nullptr, entry, shaderModel, D3DCOMPILE_ENABLE_STRICTNESS,
		//	0, buffer, 0);
	}
	
	// 不要调用这个函数。已在Unload中调用。
	void DisposeRecourses();
	void Unload();

	class LogicStep;
	extern map<string, LogicStep*> LS_lib_;

	extern map<string, ID3D11VertexShader*> VS_lib_;
	extern map<string, ID3D11PixelShader*> PS_lib_;
	// 缓冲描述资源库。
	extern map<string, D3D11_BUFFER_DESC*> BDesc_lib_;
	// 输入布局描述资源库。
	extern map<string, D3D11_INPUT_ELEMENT_DESC*> IEDesc_lib_;
	// 纹理资源库。
	extern map<string, ID3D11ShaderResourceView*> T_lib_;
	// 纹理对应的模型空间顶点。
	extern map<ID3D11ShaderResourceView*, ID3D11Buffer*> TVB_lib_;
	// 纹理尺寸库。
	extern map<ID3D11ShaderResourceView*, XMFLOAT2> TS_lib_;
	// 采样器描述资源库。
	extern map<string, D3D11_SAMPLER_DESC*> SDesc_lib_;
	// 采样状态资源库。
	extern map<string, ID3D11SamplerState*> SS_lib_;

	extern map<string, ID3D11PixelShader*>& AniLib;
	// 预设的输入布局。
	extern ID3D11InputLayout* input_layout_;
}