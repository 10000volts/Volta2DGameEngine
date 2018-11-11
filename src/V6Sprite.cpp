#include "..\\inc\\V6RenderEngine.h"
#include "..\\inc\\V6ResourceDictionary.h"

namespace VoltaEngine{
	void RectSprite::Init(){
		D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);
		VoltaRenderEngine::CreateInputLayout(solidColorLayout, totalLayoutElements,
			InputLayoutVSBuffer, &inputLayout);
		ID3DBlob* psBuffer = 0;

		ResetVertex();
	}
	void RectSprite::ResetVertex(){
		float xx = -1.0f + static_cast<float>(X) / V6WINDOWWIDTH * 2.0f, yyy = 1.0f - static_cast<float>(Y) / V6WINDOWHEIGHT * 2.0f;
		float xxx = -1.0f + static_cast<float>(X + Width) / V6WINDOWWIDTH * 2.0f, yy = 1.0f - static_cast<float>(Y + Height) / V6WINDOWHEIGHT * 2.0f;
		UVVertex vertices[] =
		{
			{ xx, yy, Priority, 0.0f, 1.0f },
			{ xx, yyy, Priority, 0.0f, 0.0f },
			{ xxx, yy, Priority, 1.0f, 1.0f },
			{ xxx, yyy, Priority, 1.0f, 0.0f },
		};

		D3D11_SUBRESOURCE_DATA resourceData;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = vertices;

		VoltaRenderEngine::CreateBuffer(BDescLib["UVVBD"], &resourceData, &vertexBuffer);
	}

	TexSprite::TexSprite(string TextureName, int x, int y, int w, int h, float p) : RectSprite(x, y, w, h, p){
		ColorMap = TLib[TextureName];
	}
	void TexSprite::Render(){
		if (ShouldResetVertex){
			ResetVertex();
			ShouldResetVertex = false;
		}
		unsigned int stride = sizeof(UVVertex);
		unsigned int offset = 0;

		VoltaRenderEngine::dContext->IASetInputLayout(inputLayout);
		VoltaRenderEngine::dContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		VoltaRenderEngine::dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		VoltaRenderEngine::dContext->VSSetShader(VSLib["TexSampler"], 0, 0);
		VoltaRenderEngine::dContext->PSSetShader(PSLib["TexSampler"], 0, 0);
		VoltaRenderEngine::dContext->PSSetShaderResources(0, 1, &ColorMap);
		VoltaRenderEngine::dContext->PSSetSamplers(0, 1, &SSLib["WRAP"]);
		//XMMATRIX world = GetWorldMatrix();
		//XMMATRIX mvp = XMMatrixMultiply(world, vpMatrix_);
		//mvp = XMMatrixTranspose(mvp);

		//d3dContext_->UpdateSubresource(mvpCB_, 0, 0, &mvp, 0, 0);
		//d3dContext_->VSSetConstantBuffers(0, 1, &mvpCB_);
		VoltaRenderEngine::dContext->Draw(4, 0);
	}
}
