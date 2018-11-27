#include "..\\inc\\V6RenderEngine.h"
#include "..\\inc\\V6ResourceDictionary.h"

namespace VoltaEngine{

	Animation::Animation(string psname) : relatedPS_(PS_lib_[psname]), last_time_(1000){}
	Animation::Animation(string psname, int lt) : relatedPS_(PS_lib_[psname]), last_time_(lt){}

	RectSprite::RectSprite(float x, float y, string psname, string ss, float ws, float hs, float p) : Sprite(PS_lib_[psname], p, x, y), scale_(ws, hs),
		angle_(0.0f), highlight_(false), m_sampler_state_(SS_lib_[ss]){}
	XMMATRIX RectSprite::Transform(){
		XMMATRIX scale = XMMatrixScaling(scale_.x, scale_.y, 1.0f);
		XMMATRIX rotationZ = XMMatrixRotationZ(angle_);
		XMMATRIX translation = XMMatrixTranslation(position_.x, position_.y, 0.0f);

		return scale * rotationZ * translation;
	}

	TexSprite::TexSprite(string TextureName, float x, float y, string psname, string ss, float w, float h, float p) : RectSprite(x, y, psname, ss, w, h, p){
		color_map_ = T_lib_[TextureName];
		m_model_vertex_buffer_ = TVB_lib_[color_map_];
		tex_width_ = TS_lib_[color_map_].x;
		tex_height_ = TS_lib_[color_map_].y;
	}
	void TexSprite::Render(){
		unsigned int stride = sizeof(UVVertex);
		unsigned int offset = 0;

		VoltaRenderEngine::SetHighlight(highlight_);

		VoltaRenderEngine::m_d3dContext_->IASetInputLayout(input_layout_);
		VoltaRenderEngine::m_d3dContext_->IASetVertexBuffers(0, 1, &m_model_vertex_buffer_, &stride, &offset);
		VoltaRenderEngine::m_d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		VoltaRenderEngine::m_d3dContext_->VSSetShader(VS_lib_["TexSampler"], 0, 0);
		VoltaRenderEngine::m_d3dContext_->PSSetShader(relatedPS_, 0, 0);
		VoltaRenderEngine::m_d3dContext_->PSSetShaderResources(0, 1, &color_map_);
		VoltaRenderEngine::m_d3dContext_->PSSetSamplers(0, 1, &m_sampler_state_);

		VoltaRenderEngine::CBPerOContent_.size = XMFLOAT4(tex_width_, tex_height_, 0, 0);
		XMMATRIX t = Transform();
		XMMATRIX mvp = XMMatrixMultiply(t, VoltaRenderEngine::m_vpMatrix_);
		VoltaRenderEngine::CBPerOContent_.mvp = XMMatrixTranspose(mvp);
		if (animating_) VoltaRenderEngine::CBPerOContent_.progress = XMFLOAT4(float(time_) / last_time_, 0.0f, 0.0f, 0.0f);

		VoltaRenderEngine::m_d3dContext_->UpdateSubresource(VoltaRenderEngine::m_ShaderCBPerO_, 0, 0, &VoltaRenderEngine::CBPerOContent_, 0, 0);

		VoltaRenderEngine::m_d3dContext_->Draw(4, 0);
	}

	ShaderSprite::ShaderSprite(string psname, float x, float y, float w, float h, float p, string ss) : Sprite(PS_lib_[psname], p, x, y),
		m_sampler_state_(SS_lib_[ss]), tex_width_(w), tex_height_(h){
		float halfWidth = tex_width_ / 2.0f;
		float halfHeight = tex_height_ / 2.0f;
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

		VoltaRenderEngine::m_d3dDevice_->CreateBuffer(&vertexDesc, &resourceData, &m_vertex_buffer_);
	}
	XMMATRIX ShaderSprite::Transform(){
		XMMATRIX rotationZ = XMMatrixRotationZ(angle_);
		XMMATRIX translation = XMMatrixTranslation(position_.x, position_.y, 0.0f);

		return rotationZ * translation;
	}
	void ShaderSprite::Render(){
		if (relatedPS_ != nullptr){
			unsigned int stride = sizeof(UVVertex);
			unsigned int offset = 0;

			VoltaRenderEngine::SetHighlight(highlight_);

			VoltaRenderEngine::m_d3dContext_->IASetInputLayout(input_layout_);
			VoltaRenderEngine::m_d3dContext_->IASetVertexBuffers(0, 1, &m_vertex_buffer_, &stride, &offset);
			VoltaRenderEngine::m_d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			VoltaRenderEngine::m_d3dContext_->VSSetShader(VS_lib_["TexSampler"], 0, 0);
			VoltaRenderEngine::m_d3dContext_->PSSetShader(relatedPS_, 0, 0);
			VoltaRenderEngine::m_d3dContext_->PSSetShaderResources(0, 1, &VoltaRenderEngine::m_ScreenTexture_);
			VoltaRenderEngine::m_d3dContext_->PSSetSamplers(0, 1, &m_sampler_state_);

			VoltaRenderEngine::CBPerOContent_.size = XMFLOAT4(tex_width_, tex_height_, 0, 0);
			XMMATRIX t = Transform();
			XMMATRIX mvp = XMMatrixMultiply(t, VoltaRenderEngine::m_vpMatrix_);
			VoltaRenderEngine::CBPerOContent_.mvp = XMMatrixTranspose(mvp);
			if (animating_) VoltaRenderEngine::CBPerOContent_.progress = XMFLOAT4(float(time_) / last_time_, 0.0f, 0.0f, 0.0f);

			VoltaRenderEngine::m_d3dContext_->UpdateSubresource(VoltaRenderEngine::m_ShaderCBPerO_, 0, 0, &VoltaRenderEngine::CBPerOContent_, 0, 0);

			VoltaRenderEngine::m_d3dContext_->Draw(4, 0);
		}
	}
}