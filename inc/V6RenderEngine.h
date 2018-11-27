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
		struct CBufferPerO{
			XMFLOAT4 size;
			XMMATRIX mvp;
			// �������ȡ�ֵ��0~1��
			XMFLOAT4 progress;
		};
		// ���²�Ƶ������Ҫʱ����������CBuffer��
		struct CBufferLoose{

		};

		// ��д���뷽��û�н��з�װ����Ҫ����������Ϊ�Ѿ���VoltaEngine::Init�б����ù�����
		static bool Init(HINSTANCE, HWND, float, float, bool window = true);
		static void Dispose();
		// ��Ⱦ��
		static void Render();

		static void LoadResource();
		static inline void CreateVS(ID3DBlob* vsBuffer, ID3D11VertexShader** vs){
			m_d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(),
				vsBuffer->GetBufferSize(), 0, vs);
		}
		static inline void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* descs, unsigned int num,
			ID3DBlob* buffer, ID3D11InputLayout** il){
			m_d3dDevice_->CreateInputLayout(descs, num,
				buffer->GetBufferPointer(), buffer->GetBufferSize(), il);
		}
		static inline void CreatePS(ID3DBlob* psBuffer, ID3D11PixelShader** ps){
			m_d3dDevice_->CreatePixelShader(psBuffer->GetBufferPointer(),
				psBuffer->GetBufferSize(), 0, ps);
		}
		static inline void CreateBuffer(D3D11_BUFFER_DESC* bdesc, D3D11_SUBRESOURCE_DATA* sr, ID3D11Buffer** buffer){
			m_d3dDevice_->CreateBuffer(bdesc, sr, buffer);
		}
		
		static ID3D11ShaderResourceView* GetTempShaderResourceView();
		
		static inline void SetHighlight(bool h){ if (highlight_ != h){ highlight_ = h; ChangeHighlightState();  } }

		// ����ȷ����ȷʹ��D3D�豸�����ĵ�ǰ����ʹ���������ڳ������ʱ�Զ��ͷš�
		static ID3D11DeviceContext* m_d3dContext_;
		// ����ȷ����ȷʹ��D3D�豸��ǰ����ʹ���������ڳ������ʱ�Զ��ͷš�
		static ID3D11Device* m_d3dDevice_;
		// ����ȷ����ȷʹ��DXGI��������ǰ����ʹ���������ڳ������ʱ�Զ��ͷš�
		static IDXGISwapChain* m_swapchain_;
		// ָ��Ԥ��Ⱦ�Ľ��������Pixel Shader�Ļ��������������������Ϊ�˱�����⣬ʵ��Ӧ�ó�ΪInputSRV��Ч�����û�н��з�װ����Ҫ�޸ġ�
		static ID3D11ShaderResourceView* m_ScreenTexture_;
		// ����仯ʱ���£��������m_ShaderCBPerO_�����������MVP����Ͷ���ߴ硣
		static CBufferPerO CBPerOContent_;
		// ����仯ʱ���µ���ɫ�����������顣����MVP����Ͷ���ߴ硣
		static ID3D11Buffer* m_ShaderCBPerO_;
		// ÿ֡���£��������m_ShaderCBPerO_������ȫ��ʱ�䡣
		static CBufferPerF CBPerFContent_;
		// ÿ֡���µ���ɫ�����������顣����ȫ��ʱ�䡣
		static ID3D11Buffer* m_ShaderCBPerF_;
		// ��Ƶ�����£��������m_ShaderCBL_��
		static CBufferLoose CBLContent_;
		// ��Ƶ�����µ���ɫ�����������顣
		static ID3D11Buffer* m_ShaderCBL_;
		// View Project����
		static XMMATRIX m_vpMatrix_;
		// ɫ�ʻ��ģʽ������
		static D3D11_BLEND_DESC m_blend_desc_;
		// ɫ�ʻ��ģʽ��
		static ID3D11BlendState* m_alpha_blend_state_;
		// Ч�����û�н��з�װ��
		static VisualManager Manager; 

		static float render_width_;
		static float render_height_;
	protected:
		static bool InitDeviceAndSwapChain(HWND hwnd, float width, float height, bool window);
		// ��ʼ����Ⱦ����������Ԥ��ȾĿ�ꡢ��Ļ���㻺�桢�������󻺳�������ȾĿ�ꡢ�ӿڡ��������漰�����������ݡ���������ϵ��ɫ�ʻ��ģʽ��
		static bool InitRenderEnvironment(float width, float height);

		// ����Ԥ��Ⱦ�Ľ����
		static void Present();
		// ������ȾĿ�꣬ʹ����ȾĿ����󶨵�����ͬ��
		static void SwapRenderTarget();
		inline static void SwapInputSRV(){
			if (m_oppoRTV_ == m_tempbufferRTV1_)
				m_ScreenTexture_ = m_tempbufferSRV2_;
			else
				m_ScreenTexture_ = m_tempbufferSRV1_;
		};

		static void ChangeHighlightState();

		// ������ȾĿ�ꡣ
		static ID3D11RenderTargetView* m_oppoRTV_;
		static ID3D11RenderTargetView* m_backbufferRTV_;
		static ID3D11RenderTargetView* m_tempbufferRTV2_;
		static ID3D11ShaderResourceView* m_tempbufferSRV2_;
		// ����Ԥ��Ⱦ����ȾĿ�ꡣ����Pixel Shader�Ļ�����������
		static ID3D11RenderTargetView* m_tempbufferRTV1_;
		static ID3D11ShaderResourceView* m_tempbufferSRV1_;
		// ��Ļ���㻺�档���ڻ���Ԥ��Ⱦ�Ľ����
		static ID3D11Buffer* m_screenVB_;

		static D3D_DRIVER_TYPE m_driver_type_;
		static D3D_FEATURE_LEVEL m_feature_level_;

		static bool highlight_;
	};
}