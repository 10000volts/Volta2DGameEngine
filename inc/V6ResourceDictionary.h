#pragma once

#include <D3D11.h>
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include "stdafx.h"

namespace VoltaEngine{
	// �����ȵ��õĺ�����
	bool Init(HINSTANCE hi, HWND hw, int width, int height, bool window);
	bool InitLib();
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
	
	// ��Ҫ�����������������Unload�е��á�
	void DisposeRecourses();
	void Unload();

	class LogicStep;
	extern map<string, LogicStep*> LS_lib_;

	extern map<string, ID3D11VertexShader*> VS_lib_;
	extern map<string, ID3D11PixelShader*> PS_lib_;
	// ����������Դ�⡣
	extern map<string, D3D11_BUFFER_DESC*> BDesc_lib_;
	// ���벼��������Դ�⡣
	extern map<string, D3D11_INPUT_ELEMENT_DESC*> IEDesc_lib_;
	// ������Դ�⡣
	extern map<string, ID3D11ShaderResourceView*> T_lib_;
	// �����Ӧ��ģ�Ϳռ䶥�㡣
	extern map<ID3D11ShaderResourceView*, ID3D11Buffer*> TVB_lib_;
	// ����ߴ�⡣
	extern map<ID3D11ShaderResourceView*, XMFLOAT2> TS_lib_;
	// ������������Դ�⡣
	extern map<string, D3D11_SAMPLER_DESC*> SDesc_lib_;
	// ����״̬��Դ�⡣
	extern map<string, ID3D11SamplerState*> SS_lib_;

	extern map<string, ID3D11PixelShader*>& AniLib;
	// Ԥ������벼�֡�
	extern ID3D11InputLayout* input_layout_;
}