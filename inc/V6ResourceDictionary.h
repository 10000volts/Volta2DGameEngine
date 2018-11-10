#pragma once

#include <D3D11.h>
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include "stdafx.h"
namespace VoltaEngine{
	// �����ȵ��õĺ�����
	inline bool Init(HINSTANCE hi, HWND hw){
		return VoltaRenderEngine::Init(hi, hw);
	};
	void InitLib();
	// ����ʱʹ�á�Ӧ��ɾ���ĺ�����
	void AddShaderResourceFromFile(string ShaderName, LPCSTR file);
	// ShaderName:Shader�����ơ���Ӧ������չ����
	void AddShaderResource(string ShaderName);
	// ����ʱʹ�á�Ӧ��ɾ���ĺ�����
	void AddTextureFromFile(string TextureName, LPCSTR file);
	// ��Ӧ������չ����
	void AddTexture(string TextureName);
	inline void CompileShaderFromFile(LPCSTR file, LPCSTR entry, LPCSTR shaderModel, ID3DBlob** buffer){
		D3DX11CompileFromFile(file, 0, 0, entry, shaderModel,
			D3DCOMPILE_ENABLE_STRICTNESS, 0, 0, buffer, 0, 0);
	}
	// ���ڴ����Shader��
	inline void CompileShader(string code, LPCSTR entry, LPCSTR shaderModel, ID3DBlob** buffer){
		//D3DCompile(&code, strlen(code.c_str()), nullptr, nullptr, nullptr, entry, shaderModel, D3DCOMPILE_ENABLE_STRICTNESS,
		//	0, buffer, 0);
	}
	
	// ��Ҫ�����������������DisposeLibs�е��á�
	template<typename T>
	void DisposeLib(map<string, T*>* l);
	// ��Ҫ�����������������Unload�е��á�
	void DisposeRecourses();
	inline void Unload(){
		VoltaRenderEngine::Dispose();
		DisposeRecourses();
	}

	extern map<string, ID3D11VertexShader*> VSLib;
	extern map<string, ID3D11PixelShader*> PSLib;
	// ����������Դ�⡣
	extern map<string, D3D11_BUFFER_DESC*> BDescLib;
	// ���벼��������Դ�⡣
	extern map<string, D3D11_INPUT_ELEMENT_DESC*> IEDescLib;
	// ����������Դ�⡣
	extern map<string, ID3D11ShaderResourceView*> TLib;
	// ������������Դ�⡣
	extern map<string, D3D11_SAMPLER_DESC*> SDescLib;
	// ����״̬��Դ�⡣
	extern map<string, ID3D11SamplerState*> SSLib;
	extern ID3DBlob* InputLayoutVSBuffer;
}