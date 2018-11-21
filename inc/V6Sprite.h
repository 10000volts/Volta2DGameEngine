#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include "stdafx.h"

namespace VoltaEngine{
	// 带有采样纹理坐标的顶点结构。用于使用纹理的图片精灵和Shader精灵。
	struct UVVertex{
		UVVertex() : Position(0, 0, 0), UV(0, 0){}
		UVVertex(float x, float y, float p, float u, float v) : Position(x, y, p), UV(u, v){}
		UVVertex(XMFLOAT3 p, XMFLOAT2 uv) : Position(p), UV(uv){}

		XMFLOAT3 Position;
		XMFLOAT2 UV;
	};
	class VoltaRenderEngine;
	class Sprite{
		friend class VoltaRenderEngine;
	public:
		Sprite(float p) : Priority(p){}
		virtual void Render() = 0;
	protected:
		virtual XMMATRIX Transform() = 0;

		float Priority;
	};

	class RectSprite : public Sprite{
		friend class VoltaRenderEngine;
	public:
		// 隐含内联。
		RectSprite(float x, float y, float ws = 1.0f, float hs = 1.0f, float p = 0.5f, string psname = "TexSampler", string ss = "BORDER");
		~RectSprite(){ RelatedPS = nullptr; SamplerState = nullptr; }
		virtual void Render() = 0;

		XMFLOAT2 Position;
		float TexWidth;
		float TexHeight;
		XMFLOAT2 Scale;
		float Angle;
		bool Highlight;
	protected:
		XMMATRIX Transform();

		ID3D11PixelShader* RelatedPS;
		ID3D11SamplerState* SamplerState;
	};
	class TexSprite : public RectSprite{
		friend class VoltaRenderEngine;
	public:
		TexSprite(string TextureName, float x, float y, float ws = 1.0f, float hs = 1.0f, float p = 0.5f, string psname = "TexSampler", string ss = "BORDER");
		~TexSprite(){ ColorMap = nullptr; }
		void Render();

		ID3D11ShaderResourceView* ColorMap;
	protected:
		ID3D11Buffer* ModelVertexBuffer;
	};
	class BatchTexSprite : Sprite{
	public:

		void Render();
	protected:
		ID3D11ShaderResourceView* ColorMap;
		ID3D11Buffer* ModelVertexBuffer;
	};
	class ShaderSprite : public Sprite{
		friend class VoltaRenderEngine;
	public:
		ShaderSprite(string psname, float x, float y, float w, float h, float p = 0.5f, string ss = "BORDER");
		~ShaderSprite(){
			RelatedPS = nullptr;
			VertexBuffer->Release();
			VertexBuffer = nullptr;
			SamplerState = nullptr;
		}
		void Render();
	protected:
		XMMATRIX Transform();

		ID3D11PixelShader* RelatedPS;
		// 中心位于原地处时，该Shader的边界的顶点缓存。
		ID3D11Buffer* VertexBuffer;
		ID3D11SamplerState* SamplerState;

		XMFLOAT2 Position;
		float TexWidth;
		float TexHeight;
		float Angle;
		bool Highlight;
	};
	class Control : public RectSprite{

	};
	class TextControl : public Control{

	};
	class ImageControl : public Control{

	};
}