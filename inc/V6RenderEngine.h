#pragma once

/*
#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include "stdafx.h"
*/
#include "V6Sprite.h"
#include <DxErr.h>
#include <D3Dcompiler.h>

using namespace VoltaEngine;
namespace VoltaEngine{

	class VisualManager{
	public:
		void Render();
		void Clear();
		void Click(int x, int y);
		void ClickRight(int x, int y);
		inline void AddSprite(Sprite* s){ Sprites.push_back(s); }
		inline void AddControl(Control* c){ Controls.push_back(c); }
		inline void AddTopControl(Control* c){ TopControls.push_back(c); }
		inline void RemoveSprite(Sprite* s){ Sprites.remove(s); delete s; s = 0; }
		inline void RemoveControl(Control* c){ Controls.remove(c); delete c; c = 0; }
		inline void RemoveTopControl(Control* c){ TopControls.remove(c); delete c; c = 0; }
	protected:
		list<Sprite*> Sprites;
		list<Control*> Controls;
		list<Control*> TopControls;
	};
	class VoltaRenderEngine{
	public:
		// ��д���뷽��û�н��з�װ����Ҫ����������Ϊ�Ѿ���VoltaEngine::Init�б����ù�����
		static bool Init(HINSTANCE, HWND);
		static void Dispose();
		// ��Ⱦ��
		static void Render();

		static void LoadResource();
		static inline void CreateVS(ID3DBlob* vsBuffer, ID3D11VertexShader** vs){
			dDevice->CreateVertexShader(vsBuffer->GetBufferPointer(),
				vsBuffer->GetBufferSize(), 0, vs);
		}
		static inline void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* descs, unsigned int num,
			ID3DBlob* buffer, ID3D11InputLayout** il){
			dDevice->CreateInputLayout(descs, num,
				buffer->GetBufferPointer(), buffer->GetBufferSize(), il);
		}
		static inline void CreatePS(ID3DBlob* psBuffer, ID3D11PixelShader** ps){
			dDevice->CreatePixelShader(psBuffer->GetBufferPointer(),
				psBuffer->GetBufferSize(), 0, ps);
		}
		static inline void CreateBuffer(D3D11_BUFFER_DESC* bdesc, D3D11_SUBRESOURCE_DATA* sr, ID3D11Buffer** buffer){
			dDevice->CreateBuffer(bdesc, sr, buffer);
		}
		// ����ȷ����ȷʹ��D3D�豸�����ĵ�ǰ����ʹ���������ڳ������ʱ�Զ��ͷš�
		static ID3D11DeviceContext* dContext;
		// ����ȷ����ȷʹ��D3D�豸��ǰ����ʹ���������ڳ������ʱ�Զ��ͷš�
		static ID3D11Device* dDevice;
		// ����ȷ����ȷʹ��DXGI��������ǰ����ʹ���������ڳ������ʱ�Զ��ͷš�
		static IDXGISwapChain* SwapChain;
		// ��ʵӦ������װ��
		static VisualManager Manager;
	protected:

		static D3D_DRIVER_TYPE DriverType;
		static D3D_FEATURE_LEVEL FeatureLevel;
		static ID3D11RenderTargetView* BackBufferTarget;
	};
}