#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include "stdafx.h"

namespace VoltaEngine{
	class VoltaRenderEngine;
	class LogicSprite;

	typedef ID3D11PixelShader* Animation;
	typedef ID3D11PixelShader* Shader;
	
	struct TimeFixedAnimation{
		TimeFixedAnimation() : ani(nullptr), last_time(0){}
		TimeFixedAnimation(Animation, int);
		TimeFixedAnimation(string, int);
		Animation ani;
		int last_time;
	};
	// ���в�����������Ķ���ṹ������ʹ�������ͼƬ�����Shader���顣
	struct UVVertex{
		UVVertex() : position(0, 0, 0), uv(0, 0){}
		UVVertex(float x, float y, float p, float u, float v) : position(x, y, p), uv(u, v){}
		UVVertex(XMFLOAT3 p, XMFLOAT2 uv) : position(p), uv(uv){}

		XMFLOAT3 position;
		XMFLOAT2 uv;
	};

	class Sprite{
		friend class VoltaRenderEngine;
		friend class LogicSprite;
	public:
		Sprite(ID3D11PixelShader* rps, float p, float x, float y) : relatedPS_(rps), priority_(p), position_(x, y), animating_(false){}
		virtual void Render() = 0;
		inline void StartAnimation(Animation a, int lt){
			relatedPS_ = a;
			last_time_ = lt;
			time_ = 0;
			animating_ = true;
		}
		inline void StartAnimation(int lt){
			last_time_ = lt;
			time_ = 0;
			animating_ = true;
		}

		XMFLOAT2 position_;
		bool animating_;
	protected:
		inline void UpdateAnimation(int t){
			time_ += t;
			if (time_ >= last_time_){
				animating_ = false;
			}
		}

		ID3D11PixelShader* relatedPS_;
		float priority_;
		// time, used in animation implemented by pixel shader.
		int time_;
		int last_time_;
	};

	class RectSprite : public Sprite{
		friend class VoltaRenderEngine;
	public:
		// ����������
		RectSprite(float x, float y, string psname = "TexSampler", string ss = "BORDER", float ws = 1.0f, float hs = 1.0f, float p = 0.5f);
		~RectSprite(){ relatedPS_ = nullptr; m_sampler_state_ = nullptr; }
		virtual void Render() = 0;

		float tex_width_;
		float tex_height_;
		XMFLOAT2 scale_;
		float angle_;
		bool highlight_;
	protected:
		XMMATRIX Transform();

		ID3D11SamplerState* m_sampler_state_;
	};

	class SelectBoard;
	class TexSprite : public RectSprite{
		friend class VoltaRenderEngine;
		friend class SelectBoard;
	public:
		TexSprite(string TextureName, float x, float y, string psname = "TexSampler", string ss = "BORDER", float ws = 1.0f, float hs = 1.0f, float p = 0.5f);
		TexSprite(string TextureName, string psname = "TexSampler", string ss = "BORDER", float ws = 1.0f, float hs = 1.0f, float p = 0.5f);
		~TexSprite(){ color_map_ = nullptr; }
		void Render();

		ID3D11ShaderResourceView* color_map_;
	protected:
		ID3D11Buffer* m_model_vertex_buffer_;
	};

	class BatchTexSprite : Sprite{
	public:

		void Render();
	protected:
		ID3D11ShaderResourceView* color_map_;
		ID3D11Buffer* m_model_vertex_buffer_;
	};

	class TextSprite : public Sprite{

	};

	class LSelectBoard;
	// ��ͼƬΪ�����ѡ��塣
	class SelectBoard : public Sprite{
		friend class VoltaRenderEngine;
		friend class LSelectBoard;
	public:
		// stridex:��������Ԫ�ؼ�ĺ�����֮�op: ѡ����ͼ�� br: �߿���ͼ�� maxn:�������ʾ��Ԫ��������
		SelectBoard(string op[], string br, float x, float y, float stridex, float stridey, int maxn,
			string psname = "TexSampler", string ss = "BORDER", float ws = 1.0f, float hs = 1.0f, float p = 0.5f);
		SelectBoard(string op[], string br, float stridex, float stridey, int maxn,
			string psname = "TexSampler", string ss = "BORDER", float ws = 1.0f, float hs = 1.0f, float p = 0.5f);
		~SelectBoard(){
			curr_ = nullptr;
			children_.clear();
			delete border_;
			border_ = nullptr;
		}
		void Render(){
			Transform();
			for (int i = 0; i < count_; ++i){
				children_[i]->Render();
			}
			border_->Render();
		}
	protected:
		void Transform(){
			for (int i = 0; i < count_; ++i){
				children_[i]->Transform();
			}
			border_->Transform();
		}

		vector<TexSprite*> children_;
		TexSprite* curr_;
		TexSprite* border_;
		// Count of children.
		int count_;

		float startx_;
		float starty_;
		float stridex_;
		float stridey_;
	};

	class ShaderSprite : public Sprite{
		friend class VoltaRenderEngine;
	public:
		ShaderSprite(string psname, float x, float y, float w, float h, float p = 0.5f, string ss = "BORDER");
		~ShaderSprite(){
			relatedPS_ = nullptr;
			m_vertex_buffer_->Release();
			m_vertex_buffer_ = nullptr;
			m_sampler_state_ = nullptr;
		}
		void Render();

		bool highlight_;
	protected:
		XMMATRIX Transform();

		// ����λ��ԭ�ش�ʱ����Shader�ı߽�Ķ��㻺�档
		ID3D11Buffer* m_vertex_buffer_;
		ID3D11SamplerState* m_sampler_state_;

		float tex_width_;
		float tex_height_;
		float angle_;
	};
}