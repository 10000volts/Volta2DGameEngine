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
		// 编写代码方便没有进行封装，不要调用它（因为已经在VoltaEngine::Init中被调用过）。
		static bool Init(HINSTANCE, HWND);
		static void Dispose();
		// 渲染。
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
		// 请在确保正确使用D3D设备上下文的前提下使用它。会在程序结束时自动释放。
		static ID3D11DeviceContext* dContext;
		// 请在确保正确使用D3D设备的前提下使用它。会在程序结束时自动释放。
		static ID3D11Device* dDevice;
		// 请在确保正确使用DXGI交换链的前提下使用它。会在程序结束时自动释放。
		static IDXGISwapChain* SwapChain;
		// 其实应该作封装。
		static VisualManager Manager;
	protected:

		static D3D_DRIVER_TYPE DriverType;
		static D3D_FEATURE_LEVEL FeatureLevel;
		static ID3D11RenderTargetView* BackBufferTarget;
	};
}