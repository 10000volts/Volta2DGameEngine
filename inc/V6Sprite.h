#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include "stdafx.h"

namespace VoltaEngine{
	// ���в�����������Ķ���ṹ������ʹ�������ͼƬ�����Shader���顣
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
		Sprite() : ShouldResetVertex(false){}
		virtual void Render() = 0;
	protected:
		virtual void ResetVertex() = 0;

		bool ShouldResetVertex;
	};

	class RectSprite : public Sprite{
		friend class VoltaRenderEngine;
	public:
		// ����������
		RectSprite(int x, int y, int w, int h, float p = 0.5f) : Sprite(), X(x), Y(y), Width(w), Height(h), Priority(p),
			Angle(0.0f){
			Init();
		}
		~RectSprite(){
			inputLayout->Release(); 
			inputLayout = 0;
			vertexBuffer->Release();
			vertexBuffer = 0;
		}
		virtual void Render() = 0;
		inline void SetX(int x){ X = x; ShouldResetVertex = true; }
		inline void SetY(int x){ Y = x; ShouldResetVertex = true; }
		inline void SetZ(float x){ Priority = x; ShouldResetVertex = true; }
		inline void SetWidth(int x){ Width = x; ShouldResetVertex = true; }
		inline void SetHeight(int x){ Height = x; ShouldResetVertex = true; }
		inline void SetSize(int w, int h){ Width = w; Height = h; ShouldResetVertex = true; }
		inline void SetPosition(int x, int y){ X = x; Y = y; ShouldResetVertex = true; }
		inline void SetMargin(int x, int y, int w, int h){ X = x; Y = y; Width = w; Height = h; ShouldResetVertex = true; }
		inline void SetAngle(float x){ Angle = x; ShouldResetVertex = true;}

		// Ч�����û�н��з�װ��ֱ���޸�ʱ���㻺�沢û�и��£���ʹ��SetXXX������
		int X;
		// Ч�����û�н��з�װ��ֱ���޸�ʱ���㻺�沢û�и��£���ʹ��SetXXX������
		int Y;
		// Ч�����û�н��з�װ��ֱ���޸�ʱ���㻺�沢û�и��£���ʹ��SetXXX������
		int Width;
		// Ч�����û�н��з�װ��ֱ���޸�ʱ���㻺�沢û�и��£���ʹ��SetXXX������
		int Height;
		// Ч�����û�н��з�װ��ֱ���޸�ʱ���㻺�沢û�и��£���ʹ��SetXXX������
		float Priority;
		// Ч�����û�н��з�װ��ֱ���޸�ʱ���㻺�沢û�и��£���ʹ��SetXXX������
		float Angle;
	protected:
		void ResetVertex();
		// ���Ĭ�϶��㡣�ڹ��캯���лᱻ�Զ����á�
		void Init();

		ID3D11InputLayout* inputLayout;
		ID3D11Buffer* vertexBuffer;
		UVVertex Vertexs;
	};
	class TexSprite : public RectSprite{
		friend class VoltaRenderEngine;
	public:
		TexSprite(string TextureName, int x, int y, int w, int h, float p = 0.5f);
		~TexSprite(){ ColorMap = 0; }
		void Render();

	protected:

		ID3D11ShaderResourceView* ColorMap;

	};
	class ShaderSprite : public Sprite{
		friend class VoltaRenderEngine;
	public:
		void Render();
	protected:
		void ResetVertex();
	};
	class Control : public RectSprite{

	};
	class TextControl : public Control{

	};
	class ImageControl : public Control{

	};
}