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
		// ����ʹ�á�
		list<Sprite*> Sprites;
		list<Control*> Controls;
		list<Control*> TopControls;
		// ��Ҫ�ӻ�����������Shader��
		list<ShaderSprite*> ScreenShaders;
		// Batch

		// ֡�������Ĳ��������

	};
	class VoltaRenderEngine{
		friend class VisualManager;
	public:
		// ÿ֡(Frame)���µ�CBuffer��
		struct CBufferPerF{
			XMFLOAT4 g_time;
		};
		// ÿ������(Object)������������CBuffer��
		struct CBufferPreO{
			XMFLOAT4 size;
			XMMATRIX mvp;
		};

		// ��д���뷽��û�н��з�װ����Ҫ����������Ϊ�Ѿ���VoltaEngine::Init�б����ù�����
		static bool Init(HINSTANCE, HWND, float, float, bool window = true);
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
		
		static ID3D11ShaderResourceView* GetTempShaderResourceView();
		
		static inline void SetHighlight(bool h){ if (Highlight != h){ Highlight = h; ChangeHighlightState();  } }

		// ����ȷ����ȷʹ��D3D�豸�����ĵ�ǰ����ʹ���������ڳ������ʱ�Զ��ͷš�
		static ID3D11DeviceContext* dContext;
		// ����ȷ����ȷʹ��D3D�豸��ǰ����ʹ���������ڳ������ʱ�Զ��ͷš�
		static ID3D11Device* dDevice;
		// ����ȷ����ȷʹ��DXGI��������ǰ����ʹ���������ڳ������ʱ�Զ��ͷš�
		static IDXGISwapChain* SwapChain;
		// ָ��Ԥ��Ⱦ�Ľ��������Pixel Shader�Ļ��������������������Ϊ�˱�����⣬ʵ��Ӧ�ó�ΪInputSRV��Ч�����û�н��з�װ����Ҫ�޸ġ�
		static ID3D11ShaderResourceView* ScreenTexture;
		// �������ShaderCBuffer������MVP���󣬷ֱ��ʺ�ȫ��ʱ�䡣
		static CBufferPreO CBufferContent;
		// ��ɫ�����������顣����MVP���󣬷ֱ��ʺ�ȫ��ʱ�䡣
		static ID3D11Buffer* ShaderCBuffer;
		// View Project����
		static XMMATRIX vpMatrix;
		// ɫ�ʻ��ģʽ������
		static D3D11_BLEND_DESC BlendDesc;
		// ɫ�ʻ��ģʽ��
		static ID3D11BlendState* AlphaBlendState;
		// Ч�����û�н��з�װ��
		static VisualManager Manager; 

		static float RenderWidth;
		static float RenderHeight;
	protected:
		static bool InitDeviceAndSwapChain(HWND hwnd, float width, float height, bool window);
		// ��ʼ����Ⱦ����������Ԥ��ȾĿ�ꡢ��Ļ���㻺�桢�������󻺳�������ȾĿ�ꡢ�ӿڡ��������漰�����������ݡ���������ϵ��ɫ�ʻ��ģʽ��
		static bool InitRenderEnvironment(float width, float height);

		// ����Ԥ��Ⱦ�Ľ����
		static void Present();
		// ������ȾĿ�꣬ʹ����ȾĿ����󶨵�����ͬ��
		static void SwapRenderTarget();
		inline static void SwapInputSRV(){
			if (OppoRTV == TempBufferRTV1)
				ScreenTexture = TempBufferSRV2;
			else
				ScreenTexture = TempBufferSRV1;
		};

		static void ChangeHighlightState();

		// ������ȾĿ�ꡣ
		static ID3D11RenderTargetView* OppoRTV;
		static ID3D11RenderTargetView* BackBufferRTV;
		static ID3D11RenderTargetView* TempBufferRTV2;
		static ID3D11ShaderResourceView* TempBufferSRV2;
		// ����Ԥ��Ⱦ����ȾĿ�ꡣ����Pixel Shader�Ļ�����������
		static ID3D11RenderTargetView* TempBufferRTV1;
		static ID3D11ShaderResourceView* TempBufferSRV1;
		// ��Ļ���㻺�档���ڻ���Ԥ��Ⱦ�Ľ����
		static ID3D11Buffer* ScreenVB;

		static D3D_DRIVER_TYPE DriverType;
		static D3D_FEATURE_LEVEL FeatureLevel;

		static bool Highlight;
	};
}