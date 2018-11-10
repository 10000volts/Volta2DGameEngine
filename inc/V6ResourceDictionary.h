#pragma once

#include <D3D11.h>
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include "stdafx.h"
namespace VoltaEngine{
	// 必须先调用的函数。
	inline bool Init(HINSTANCE hi, HWND hw){
		return VoltaRenderEngine::Init(hi, hw);
	};
	void InitLib();
	// 过渡时使用。应该删除的函数。
	void AddShaderResourceFromFile(string ShaderName, LPCSTR file);
	// ShaderName:Shader的名称。不应包括扩展名。
	void AddShaderResource(string ShaderName);
	// 过渡时使用。应该删除的函数。
	void AddTextureFromFile(string TextureName, LPCSTR file);
	// 不应包括扩展名。
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
	
	// 不要调用这个函数。已在DisposeLibs中调用。
	template<typename T>
	void DisposeLib(map<string, T*>* l);
	// 不要调用这个函数。已在Unload中调用。
	void DisposeRecourses();
	inline void Unload(){
		VoltaRenderEngine::Dispose();
		DisposeRecourses();
	}

	extern map<string, ID3D11VertexShader*> VSLib;
	extern map<string, ID3D11PixelShader*> PSLib;
	// 缓冲描述资源库。
	extern map<string, D3D11_BUFFER_DESC*> BDescLib;
	// 输入布局描述资源库。
	extern map<string, D3D11_INPUT_ELEMENT_DESC*> IEDescLib;
	// 纹理描述资源库。
	extern map<string, ID3D11ShaderResourceView*> TLib;
	// 采样器描述资源库。
	extern map<string, D3D11_SAMPLER_DESC*> SDescLib;
	// 采样状态资源库。
	extern map<string, ID3D11SamplerState*> SSLib;
	extern ID3DBlob* InputLayoutVSBuffer;
}