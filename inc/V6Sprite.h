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
		Sprite() : ShouldResetVertex(false){}
		virtual void Render() = 0;
	protected:
		virtual void ResetVertex() = 0;

		bool ShouldResetVertex;
	};

	class RectSprite : public Sprite{
		friend class VoltaRenderEngine;
	public:
		// 隐含内联。
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

		// 效率起见没有进行封装。直接修改时顶点缓存并没有更新，请使用SetXXX方法。
		int X;
		// 效率起见没有进行封装。直接修改时顶点缓存并没有更新，请使用SetXXX方法。
		int Y;
		// 效率起见没有进行封装。直接修改时顶点缓存并没有更新，请使用SetXXX方法。
		int Width;
		// 效率起见没有进行封装。直接修改时顶点缓存并没有更新，请使用SetXXX方法。
		int Height;
		// 效率起见没有进行封装。直接修改时顶点缓存并没有更新，请使用SetXXX方法。
		float Priority;
		// 效率起见没有进行封装。直接修改时顶点缓存并没有更新，请使用SetXXX方法。
		float Angle;
	protected:
		void ResetVertex();
		// 添加默认顶点。在构造函数中会被自动调用。
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