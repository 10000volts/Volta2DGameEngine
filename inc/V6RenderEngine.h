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
		inline void AddScreenShader(ShaderSprite* s){ ScreenShaders.push_back(s); }
		inline void RemoveSprite(Sprite* s){ Sprites.remove(s); delete s; s = nullptr; }
		inline void RemoveControl(Control* c){ Controls.remove(c); delete c; c = nullptr; }
		inline void RemoveTopControl(Control* c){ TopControls.remove(c); delete c; c = nullptr; }
		inline void RemoveScreenShader(ShaderSprite* s){ ScreenShaders.remove(s); delete s; s = nullptr; }
	protected:
		// 过渡使用。
		list<Sprite*> Sprites;
		list<Control*> Controls;
		list<Control*> TopControls;
		// 需要从缓冲区采样的Shader。
		list<ShaderSprite*> ScreenShaders;
		// Batch

		// 帧缓冲区的采样结果。

	};
	class VoltaRenderEngine{
		friend class VisualManager;
	public:
		// 每帧(Frame)更新的CBuffer。
		struct CBufferPerF{
			XMFLOAT4 g_time;
		};
		// 每个对象(Object)都会重新填充的CBuffer。
		struct CBufferPreO{
			XMFLOAT4 size;
			XMMATRIX mvp;
		};

		// 编写代码方便没有进行封装，不要调用它（因为已经在VoltaEngine::Init中被调用过）。
		static bool Init(HINSTANCE, HWND, float, float, bool window = true);
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
		
		static ID3D11ShaderResourceView* GetTempShaderResourceView();
		
		static inline void SetHighlight(bool h){ if (Highlight != h){ Highlight = h; ChangeHighlightState();  } }

		// 请在确保正确使用D3D设备上下文的前提下使用它。会在程序结束时自动释放。
		static ID3D11DeviceContext* dContext;
		// 请在确保正确使用D3D设备的前提下使用它。会在程序结束时自动释放。
		static ID3D11Device* dDevice;
		// 请在确保正确使用DXGI交换链的前提下使用它。会在程序结束时自动释放。
		static IDXGISwapChain* SwapChain;
		// 指向预渲染的结果，用于Pixel Shader的缓冲区采样。这个命名是为了便于理解，实际应该称为InputSRV。效率起见没有进行封装，不要修改。
		static ID3D11ShaderResourceView* ScreenTexture;
		// 用于填充ShaderCBuffer。包含MVP矩阵，分辨率和全局时间。
		static CBufferPreO CBufferContent;
		// 着色器常量缓冲组。包含MVP矩阵，分辨率和全局时间。
		static ID3D11Buffer* ShaderCBuffer;
		// View Project矩阵。
		static XMMATRIX vpMatrix;
		// 色彩混合模式描述。
		static D3D11_BLEND_DESC BlendDesc;
		// 色彩混合模式。
		static ID3D11BlendState* AlphaBlendState;
		// 效率起见没有进行封装。
		static VisualManager Manager; 

		static float RenderWidth;
		static float RenderHeight;
	protected:
		static bool InitDeviceAndSwapChain(HWND hwnd, float width, float height, bool window);
		// 初始化渲染环境。包括预渲染目标、屏幕顶点缓存、交换链后缓冲区、渲染目标、视口、常量缓存及常量缓存内容、世界坐标系和色彩混合模式。
		static bool InitRenderEnvironment(float width, float height);

		// 呈现预渲染的结果。
		static void Present();
		// 交换渲染目标，使得渲染目标与绑定的纹理不同。
		static void SwapRenderTarget();
		inline static void SwapInputSRV(){
			if (OppoRTV == TempBufferRTV1)
				ScreenTexture = TempBufferSRV2;
			else
				ScreenTexture = TempBufferSRV1;
		};

		static void ChangeHighlightState();

		// 缓冲渲染目标。
		static ID3D11RenderTargetView* OppoRTV;
		static ID3D11RenderTargetView* BackBufferRTV;
		static ID3D11RenderTargetView* TempBufferRTV2;
		static ID3D11ShaderResourceView* TempBufferSRV2;
		// 进行预渲染的渲染目标。用于Pixel Shader的缓冲区采样。
		static ID3D11RenderTargetView* TempBufferRTV1;
		static ID3D11ShaderResourceView* TempBufferSRV1;
		// 屏幕顶点缓存。用于绘制预渲染的结果。
		static ID3D11Buffer* ScreenVB;

		static D3D_DRIVER_TYPE DriverType;
		static D3D_FEATURE_LEVEL FeatureLevel;

		static bool Highlight;
	};
}