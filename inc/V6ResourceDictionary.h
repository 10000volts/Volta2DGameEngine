#pragma once

#include <D3D11.h>
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include "stdafx.h"
namespace VoltaEngine{
	// �����ȵ��õĺ�����
	inline bool Init(HINSTANCE hi, HWND hw, int width, int height, bool window){
		return VoltaRenderEngine::Init(hi, hw, (float)width, (float)height, window);
	};
	void InitLib();
	// ����ʱʹ�á�Ӧ��ɾ���ĺ�����
	void AddShaderResourceFromFile(string ShaderName, LPCSTR file);
	// ShaderName:Shader�����ơ���Ӧ������չ����
	void AddShaderResource(string ShaderName);
	// ����ʱʹ�á�Ӧ��ɾ���ĺ�����
	void AddTextureFromFile(string TextureName, LPCSTR file);
	// ���������ģ�Ϳռ䶥�㻺���Լ���ߴ絽����⡣
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
	template<typename T1, typename T2>
	void DisposeLib(map<T1, T2*>* l);
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
	// ������Դ�⡣
	extern map<string, ID3D11ShaderResourceView*> TLib;
	// �����Ӧ��ģ�Ϳռ䶥�㡣
	extern map<ID3D11ShaderResourceView*, ID3D11Buffer*> TVBLib;
	// ����ߴ�⡣
	extern map<ID3D11ShaderResourceView*, XMFLOAT2> TSLib;
	// ������������Դ�⡣
	extern map<string, D3D11_SAMPLER_DESC*> SDescLib;
	// ����״̬��Դ�⡣
	extern map<string, ID3D11SamplerState*> SSLib;
	// Ԥ������벼�֡�
	extern ID3D11InputLayout* InputLayout;
}