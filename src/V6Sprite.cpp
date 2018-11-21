#include "..\\inc\\V6RenderEngine.h"
#include "..\\inc\\V6ResourceDictionary.h"

namespace VoltaEngine{

	RectSprite::RectSprite(float x, float y, float ws, float hs, float p, string psname, string ss) : Sprite(p), Position(x, y), Scale(ws, hs),
		Angle(0.0f), Highlight(false), RelatedPS(PSLib[psname]), SamplerState(SSLib[ss]){}
	XMMATRIX RectSprite::Transform(){
		XMMATRIX scale = XMMatrixScaling(Scale.x, Scale.y, 1.0f);
		XMMATRIX rotationZ = XMMatrixRotationZ(Angle);
		XMMATRIX translation = XMMatrixTranslation(Position.x, Position.y, 0.0f);

		return scale * rotationZ * translation;
	}

	TexSprite::TexSprite(string TextureName, float x, float y, float w, float h, float p, string psname, string ss) : RectSprite(x, y, w, h, p, psname, ss){
		ColorMap = TLib[TextureName];
		ModelVertexBuffer = TVBLib[ColorMap];
		TexWidth = TSLib[ColorMap].x;
		TexHeight = TSLib[ColorMap].y;
	}
	void TexSprite::Render(){
		unsigned int stride = sizeof(UVVertex);
		unsigned int offset = 0;

		VoltaRenderEngine::SetHighlight(Highlight);

		VoltaRenderEngine::dContext->IASetInputLayout(InputLayout);
		VoltaRenderEngine::dContext->IASetVertexBuffers(0, 1, &ModelVertexBuffer, &stride, &offset);
		VoltaRenderEngine::dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		VoltaRenderEngine::dContext->VSSetShader(VSLib["TexSampler"], 0, 0);
		VoltaRenderEngine::dContext->PSSetShader(RelatedPS, 0, 0);
		VoltaRenderEngine::dContext->PSSetShaderResources(0, 1, &ColorMap);
		VoltaRenderEngine::dContext->PSSetSamplers(0, 1, &SamplerState);

		VoltaRenderEngine::CBufferContent.size = XMFLOAT4(TexWidth, TexHeight, 0, 0);
		XMMATRIX t = Transform();
		XMMATRIX mvp = XMMatrixMultiply(t, VoltaRenderEngine::vpMatrix);
		VoltaRenderEngine::CBufferContent.mvp = XMMatrixTranspose(mvp);

		VoltaRenderEngine::dContext->UpdateSubresource(VoltaRenderEngine::ShaderCBuffer, 0, 0, &VoltaRenderEngine::CBufferContent, 0, 0);

		VoltaRenderEngine::dContext->Draw(4, 0);
	}

	ShaderSprite::ShaderSprite(string psname, float x, float y, float w, float h, float p, string ss) : Sprite(p), RelatedPS(PSLib[psname]), SamplerState(SSLib[ss]),
		Position(x, y), TexWidth(w), TexHeight(h){
		float halfWidth = TexWidth / 2.0f;
		float halfHeight = TexHeight / 2.0f;
		UVVertex vertices[] =
		{
			{ XMFLOAT3(halfWidth, halfHeight, 0.5f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(halfWidth, -halfHeight, 0.5f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(-halfWidth, halfHeight, 0.5f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(-halfWidth, -halfHeight, 0.5f), XMFLOAT2(0.0f, 1.0f) }
		};

		D3D11_BUFFER_DESC vertexDesc;
		ZeroMemory(&vertexDesc, sizeof(vertexDesc));
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = sizeof(UVVertex)* 4;

		D3D11_SUBRESOURCE_DATA resourceData;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = vertices;

		VoltaRenderEngine::dDevice->CreateBuffer(&vertexDesc, &resourceData, &VertexBuffer);
	}
	XMMATRIX ShaderSprite::Transform(){
		XMMATRIX rotationZ = XMMatrixRotationZ(Angle);
		XMMATRIX translation = XMMatrixTranslation(Position.x, Position.y, 0.0f);

		return rotationZ * translation;
	}
	void ShaderSprite::Render(){
		unsigned int stride = sizeof(UVVertex);
		unsigned int offset = 0;

		VoltaRenderEngine::SetHighlight(Highlight);

		VoltaRenderEngine::dContext->IASetInputLayout(InputLayout);
		VoltaRenderEngine::dContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
		VoltaRenderEngine::dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		VoltaRenderEngine::dContext->VSSetShader(VSLib["TexSampler"], 0, 0);
		VoltaRenderEngine::dContext->PSSetShader(RelatedPS, 0, 0);
		VoltaRenderEngine::dContext->PSSetShaderResources(0, 1, &VoltaRenderEngine::ScreenTexture);
		VoltaRenderEngine::dContext->PSSetSamplers(0, 1, &SamplerState);

		VoltaRenderEngine::CBufferContent.size = XMFLOAT4(TexWidth, TexHeight, 0, 0);
		XMMATRIX t = Transform();
		XMMATRIX mvp = XMMatrixMultiply(t, VoltaRenderEngine::vpMatrix);
		VoltaRenderEngine::CBufferContent.mvp = XMMatrixTranspose(mvp);

		VoltaRenderEngine::dContext->UpdateSubresource(VoltaRenderEngine::ShaderCBuffer, 0, 0, &VoltaRenderEngine::CBufferContent, 0, 0);

		VoltaRenderEngine::dContext->Draw(4, 0);
	}
}