#include "..\\inc\\V6RenderEngine.h"
#include "..\\inc\\V6ResourceDictionary.h"

namespace VoltaEngine{
	bool VoltaRenderEngine::Init(HINSTANCE hi, HWND hwnd, float width, float height, bool window){
		if (!InitDeviceAndSwapChain(hwnd, width, height, window)) return false;
		if (!InitRenderEnvironment(width, height)) return false;
		InitLib();
		return true;
	}
	bool VoltaRenderEngine::InitDeviceAndSwapChain(HWND hwnd, float width, float height, bool window){
		render_width_ = width;
		render_height_ = height;

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE
		};

		unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Windowed = window;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		unsigned int creationFlags = 0;

#ifdef _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT result;
		unsigned int driver = 0;

		for (driver = 0; driver < totalDriverTypes; ++driver)
		{
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0, creationFlags,
				featureLevels, totalFeatureLevels,
				D3D11_SDK_VERSION, &swapChainDesc, &m_swapchain_,
				&m_d3dDevice_, &m_feature_level_, &m_d3dContext_);

			if (SUCCEEDED(result))
			{
				m_driver_type_ = driverTypes[driver];
				break;
			}
		}

		if (FAILED(result))
		{
			DXTRACE_MSG("Failed to create the Direct3D device!");
			return false;
		}

		return true;
	}
	bool VoltaRenderEngine::InitRenderEnvironment(float width, float height){
		HRESULT result;
		// 创建交换链后缓冲区。
		ID3D11Texture2D* BackBufferTexture;
		result = m_swapchain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBufferTexture);

		if (FAILED(result))
		{
			DXTRACE_MSG("Failed to get the swap chain back buffer!");
			return false;
		}

		result = m_d3dDevice_->CreateRenderTargetView(BackBufferTexture, 0, &m_backbufferRTV_);

		if (FAILED(result))
		{
			DXTRACE_MSG("Failed to create the render target view!");
			return false;
		}
		if (BackBufferTexture)
			BackBufferTexture->Release();

		// 设置两个缓冲渲染目标。
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // R32G32B32A32_FLOAT, R8G8B8A8_UNORM
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		ID3D11Texture2D* TempTexture;
		m_d3dDevice_->CreateTexture2D(&textureDesc, NULL, &TempTexture);
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		m_d3dDevice_->CreateRenderTargetView(TempTexture, &renderTargetViewDesc, &m_tempbufferRTV1_);
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		m_d3dDevice_->CreateShaderResourceView(TempTexture, &shaderResourceViewDesc, &m_tempbufferSRV1_);
		TempTexture->Release();

		ID3D11Texture2D* TempTexture2;
		m_d3dDevice_->CreateTexture2D(&textureDesc, NULL, &TempTexture2);
		m_d3dDevice_->CreateRenderTargetView(TempTexture2, &renderTargetViewDesc, &m_tempbufferRTV2_);
		m_d3dDevice_->CreateShaderResourceView(TempTexture2, &shaderResourceViewDesc, &m_tempbufferSRV2_);
		TempTexture2->Release();

		// 设置初始渲染环境。
		m_oppoRTV_ = m_tempbufferRTV2_;

		// 创建屏幕顶点缓存。
		float halfWidth = width;
		float halfHeight = height;
		UVVertex vertices[] =
		{
			{ XMFLOAT3(halfWidth, halfHeight, 0.5f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(halfWidth, 0.0f, 0.5f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(0.0f, halfHeight, 0.5f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(0.0f, 0.0f, 0.5f), XMFLOAT2(0.0f, 1.0f) }
		};

		D3D11_BUFFER_DESC vertexDesc;
		ZeroMemory(&vertexDesc, sizeof(vertexDesc));
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = sizeof(UVVertex)* 4;

		D3D11_SUBRESOURCE_DATA resourceData;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = vertices;

		VoltaRenderEngine::m_d3dDevice_->CreateBuffer(&vertexDesc, &resourceData, &m_screenVB_);

		// 设置视口。
		D3D11_VIEWPORT viewport;
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		m_d3dContext_->RSSetViewports(1, &viewport);

		// 初始化CBuffer和m_CBContent_。
		D3D11_BUFFER_DESC constDesc;
		ZeroMemory(&constDesc, sizeof(constDesc));
		constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constDesc.ByteWidth = sizeof(CBufferPerO);
		constDesc.Usage = D3D11_USAGE_DEFAULT;

		m_d3dDevice_->CreateBuffer(&constDesc, nullptr, &m_ShaderCBPerO_);

		ZeroMemory(&CBPerOContent_, sizeof(CBPerOContent_));

		m_d3dContext_->VSSetConstantBuffers(0, 1, &VoltaRenderEngine::m_ShaderCBPerO_);
		m_d3dContext_->PSSetConstantBuffers(0, 1, &VoltaRenderEngine::m_ShaderCBPerO_);

		// PerF

		m_d3dDevice_->CreateBuffer(&constDesc, nullptr, &m_ShaderCBPerF_);

		ZeroMemory(&CBPerFContent_, sizeof(CBPerFContent_));

		m_d3dContext_->VSSetConstantBuffers(1, 1, &VoltaRenderEngine::m_ShaderCBPerF_);
		m_d3dContext_->PSSetConstantBuffers(1, 1, &VoltaRenderEngine::m_ShaderCBPerF_);

		// L
		m_d3dDevice_->CreateBuffer(&constDesc, nullptr, &m_ShaderCBL_);

		ZeroMemory(&CBLContent_, sizeof(CBLContent_));

		m_d3dContext_->VSSetConstantBuffers(2, 1, &VoltaRenderEngine::m_ShaderCBL_); 
		m_d3dContext_->PSSetConstantBuffers(2, 1, &VoltaRenderEngine::m_ShaderCBL_);

		// 设置世界坐标系。
		XMMATRIX view = XMMatrixIdentity();
		XMMATRIX projection = XMMatrixOrthographicOffCenterLH(0.0f, width, 0.0f, height, 0.0f, 1.0f);
		m_vpMatrix_ = XMMatrixMultiply(view, projection);

		// 设置色彩混合模式。
		ChangeHighlightState();
	}
	void loadresource(string file){
		if (file.find(".fx") != string::npos){
			AddShaderResourceFromFile(GetFileName(file), file.c_str());
		}
		else if (file.find(".mp3") != string::npos){

		}
		else if (file.find(".dds") != string::npos){
			AddTextureFromFile(GetFileName(file), file.c_str());
		}
	}
	void LoadAll(){
		WIN32_FIND_DATA p;
		HANDLE h = FindFirstFile("*.*", &p);
		string file = p.cFileName;
		loadresource(file);
		while (FindNextFile(h, &p)) {
			file = p.cFileName;
			loadresource(file);
		}
		return;
	}
	bool VoltaEngine::InitLib(){
		// 添加用于读取纹理的着色器和输入布局。
		ID3D11VertexShader* vsdefau;
		ID3D11PixelShader* psdefau;
		ID3DBlob* InputLayoutVSBuffer;
		VoltaEngine::CompileShaderFromFile("TexBasic.fx", "VS_Main", "vs_4_0", &InputLayoutVSBuffer);
		VoltaRenderEngine::CreateVS(InputLayoutVSBuffer, &vsdefau);	
		D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);
		VoltaRenderEngine::CreateInputLayout(solidColorLayout, totalLayoutElements,
			InputLayoutVSBuffer, &input_layout_);
		ID3DBlob* psBuffer = nullptr;
		VoltaEngine::CompileShaderFromFile("TexBasic.fx", "PS_Main", "ps_4_0", &psBuffer);
		VoltaRenderEngine::CreatePS(psBuffer, &psdefau);
		InputLayoutVSBuffer->Release();

		VS_lib_.insert(PAIRVS("TexSampler", vsdefau));
		PS_lib_.insert(PAIRPS("TexSampler", psdefau));
		psBuffer->Release();

		D3D11_BUFFER_DESC* vertexDesc = new D3D11_BUFFER_DESC();
		ZeroMemory(vertexDesc, sizeof(vertexDesc));
		vertexDesc->Usage = D3D11_USAGE_DEFAULT;
		vertexDesc->BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc->ByteWidth = sizeof(UVVertex)* 4;
		BDesc_lib_.insert(pair<string, D3D11_BUFFER_DESC*>("UVVBD", vertexDesc));

		D3D11_SAMPLER_DESC* colorMapDesc = new D3D11_SAMPLER_DESC();
		ID3D11SamplerState* colorMapSampler;
		ZeroMemory(colorMapDesc, sizeof(colorMapDesc));
		colorMapDesc->AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		colorMapDesc->AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		colorMapDesc->AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		colorMapDesc->ComparisonFunc = D3D11_COMPARISON_NEVER;
		colorMapDesc->Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		colorMapDesc->MaxLOD = D3D11_FLOAT32_MAX;
		MapInsert(&SDesc_lib_, static_cast<string>("BORDER"), colorMapDesc);

		VoltaRenderEngine::m_d3dDevice_->CreateSamplerState(SDesc_lib_["BORDER"],
			&colorMapSampler);
		MapInsert(&SS_lib_, static_cast<string>("BORDER"), colorMapSampler);

		colorMapDesc = new D3D11_SAMPLER_DESC();
		ZeroMemory(colorMapDesc, sizeof(colorMapDesc));
		colorMapDesc->AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		colorMapDesc->AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		colorMapDesc->AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		colorMapDesc->ComparisonFunc = D3D11_COMPARISON_NEVER;
		colorMapDesc->Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		colorMapDesc->MaxLOD = D3D11_FLOAT32_MAX;
		MapInsert(&SDesc_lib_, static_cast<string>("CLAMP"), colorMapDesc);

		VoltaRenderEngine::m_d3dDevice_->CreateSamplerState(SDesc_lib_["CLAMP"],
			&colorMapSampler);
		MapInsert(&SS_lib_, static_cast<string>("CLAMP"), colorMapSampler);
		
		LoadAll();

		return true;
	}

	// ShaderName:Shader的名称，不应包括扩展名。
	void VoltaEngine::AddShaderResourceFromFile(string ShaderName, LPCSTR file){
		ID3D11VertexShader* vsdefau;
		ID3D11PixelShader* psdefau;

		ID3DBlob* vsBuffer = nullptr;
		VoltaEngine::CompileShaderFromFile(file, "VS_Main", "vs_4_0", &vsBuffer);
		VoltaRenderEngine::CreateVS(vsBuffer, &vsdefau);
		ID3DBlob* psBuffer = nullptr;
		VoltaEngine::CompileShaderFromFile(file, "PS_Main", "ps_4_0", &psBuffer);
		VoltaRenderEngine::CreatePS(psBuffer, &psdefau);
		VS_lib_.insert(PAIRVS(ShaderName, vsdefau));
		PS_lib_.insert(PAIRPS(ShaderName, psdefau));
		vsBuffer->Release();
		psBuffer->Release();
	}
	void VoltaEngine::AddShaderResource(string ShaderName){
		ID3D11VertexShader* vsdefau;
		ID3D11PixelShader* psdefau;

		ID3DBlob* vsBuffer = nullptr;
		VoltaEngine::CompileShader(ShaderName, "VS_Main", "vs_4_0", &vsBuffer);
		VoltaRenderEngine::CreateVS(vsBuffer, &vsdefau);
		ID3DBlob* psBuffer = nullptr;
		VoltaEngine::CompileShader(ShaderName, "PS_Main", "ps_4_0", &psBuffer);
		VoltaRenderEngine::CreatePS(psBuffer, &psdefau);
		VS_lib_.insert(PAIRVS(ShaderName, vsdefau));
		PS_lib_.insert(PAIRPS(ShaderName, psdefau));
		vsBuffer->Release();
		psBuffer->Release();
	}
	void VoltaEngine::AddTextureFromFile(string TextureName, LPCSTR file){
		// 添加纹理的拷贝至库。
		ID3D11ShaderResourceView* colorMap;
		D3DX11CreateShaderResourceViewFromFile(VoltaRenderEngine::m_d3dDevice_,
			file, 0, 0, &colorMap, 0);
		MapInsert(&T_lib_, TextureName, colorMap);
		// 创建模型空间对应的顶点缓存并将其拷贝、添加至库。
		ID3D11Resource* colorTex;
		colorMap->GetResource(&colorTex);
		D3D11_TEXTURE2D_DESC colorTexDesc;
		((ID3D11Texture2D*)colorTex)->GetDesc(&colorTexDesc);
		colorTex->Release();

		XMFLOAT2 size((float)colorTexDesc.Width, (float)colorTexDesc.Height);
		MapInsert(&TS_lib_, colorMap, size);

		float halfWidth = size.x / 2.0f;
		float halfHeight = size.y / 2.0f;
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
		vertexDesc.ByteWidth = sizeof(UVVertex) * 4;

		D3D11_SUBRESOURCE_DATA resourceData;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = vertices;

		ID3D11Buffer* vertexBuffer;
		VoltaRenderEngine::m_d3dDevice_->CreateBuffer(&vertexDesc, &resourceData, &vertexBuffer);
		MapInsert(&TVB_lib_, colorMap, vertexBuffer);
	}

	void VoltaRenderEngine::Dispose(){
		if (m_screenVB_) m_screenVB_->Release();
		if (m_tempbufferRTV1_) m_tempbufferRTV1_->Release();
		if (m_tempbufferSRV1_) m_tempbufferSRV1_->Release();
		if (m_tempbufferRTV2_) m_tempbufferRTV2_->Release();
		if (m_tempbufferSRV2_) m_tempbufferSRV2_->Release();
		if (m_backbufferRTV_) m_backbufferRTV_->Release();
		if (m_swapchain_) m_swapchain_->Release();
		if (m_d3dContext_) m_d3dContext_->Release();
		if (m_d3dDevice_) m_d3dDevice_->Release();
		if (m_ShaderCBPerO_) m_ShaderCBPerO_->Release();
		if (m_ShaderCBPerF_) m_ShaderCBPerF_->Release();
		if (m_ShaderCBL_) m_ShaderCBL_->Release();
		if (m_alpha_blend_state_) m_alpha_blend_state_->Release();
		Manager.Clear();
		m_screenVB_ = nullptr;
		m_tempbufferSRV1_ = nullptr;
		m_tempbufferRTV1_ = nullptr;
		m_tempbufferSRV2_ = nullptr;
		m_tempbufferRTV2_ = nullptr;
		m_ScreenTexture_ = nullptr;
		m_oppoRTV_ = nullptr;
		m_backbufferRTV_ = nullptr;
		m_swapchain_ = nullptr;
		m_d3dContext_ = nullptr;
		m_d3dDevice_ = nullptr;
		m_ShaderCBPerO_ = nullptr;
		m_ShaderCBPerF_ = nullptr;
		m_ShaderCBL_ = nullptr;
		m_alpha_blend_state_ = nullptr;
	}

	void VoltaRenderEngine::Render(){
		if (!m_d3dContext_)
			return;
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_d3dContext_->UpdateSubresource(m_ShaderCBPerF_, 0, 0, &CBPerFContent_, 0, 0);

		m_d3dContext_->ClearRenderTargetView(m_tempbufferRTV1_, clearColor);
		m_d3dContext_->ClearRenderTargetView(m_tempbufferRTV2_, clearColor);
		m_d3dContext_->ClearRenderTargetView(m_backbufferRTV_, clearColor);

		SwapRenderTarget();
		Manager.Render();
		m_d3dContext_->OMSetRenderTargets(1, &m_backbufferRTV_, 0);
		SwapInputSRV();
		Present();
		m_swapchain_->Present(0, 0);
	}
	void VoltaRenderEngine::Present(){
		unsigned int stride = sizeof(UVVertex);
		unsigned int offset = 0;

		m_d3dContext_->IASetInputLayout(input_layout_);
		m_d3dContext_->IASetVertexBuffers(0, 1, &m_screenVB_, &stride, &offset);
		m_d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_d3dContext_->VSSetShader(VS_lib_["TexSampler"], 0, 0);
		m_d3dContext_->PSSetShader(PS_lib_["TexSampler"], 0, 0);
		m_d3dContext_->PSSetShaderResources(0, 1, &m_ScreenTexture_);
		m_d3dContext_->PSSetSamplers(0, 1, &SS_lib_["BORDER"]);

		CBPerOContent_.size = XMFLOAT4(render_width_, render_height_, 0.0, 0.0);
		CBPerOContent_.mvp = XMMatrixTranspose(m_vpMatrix_);

		m_d3dContext_->UpdateSubresource(m_ShaderCBPerO_, 0, 0, &CBPerOContent_, 0, 0);

		m_d3dContext_->Draw(4, 0);
	}
	void VoltaRenderEngine::SwapRenderTarget(){
		m_d3dContext_->OMSetRenderTargets(1, &m_oppoRTV_, 0);
		if (m_oppoRTV_ == m_tempbufferRTV1_){
			m_oppoRTV_ = m_tempbufferRTV2_;
			m_ScreenTexture_ = m_tempbufferSRV2_;
		}
		else{
			m_oppoRTV_ = m_tempbufferRTV1_;
			m_ScreenTexture_ = m_tempbufferSRV1_;
		}
	}
	void VisualManager::Render(){

		for_each(Sprites.begin(), Sprites.end(), [](Sprite* one){
			one->Render();
		});
		for_each(ScreenShaders.begin(), ScreenShaders.end(), [](ShaderSprite* one){
			VoltaRenderEngine::SwapRenderTarget();
			one->Render();
		});
	}
	void VisualManager::Clear(){
		DisposeList(&ScreenShaders);
		DisposeList(&Sprites);
	}
	ID3D11ShaderResourceView* VoltaRenderEngine::GetTempShaderResourceView(){
		return nullptr;
	}

	void VoltaEngine::DisposeRecourses(){
		DisposeCOMLib(&VS_lib_);
		DisposeCOMLib(&PS_lib_);
		DisposeLib(&BDesc_lib_);
		DisposeLib(&IEDesc_lib_);
		DisposeCOMLib(&TVB_lib_);
		TS_lib_.clear();
		DisposeCOMLib(&T_lib_);
		DisposeLib(&SDesc_lib_);
		DisposeCOMLib(&SS_lib_);
		input_layout_->Release();
		input_layout_ = nullptr;
	}

	void VoltaRenderEngine::ChangeHighlightState(){
		if (highlight_){
			ZeroMemory(&m_blend_desc_, sizeof(m_blend_desc_));
			m_blend_desc_.RenderTarget[0].BlendEnable = TRUE;
			m_blend_desc_.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			m_blend_desc_.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			m_blend_desc_.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_ALPHA;
			m_blend_desc_.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
			m_blend_desc_.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
			m_blend_desc_.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
			m_blend_desc_.RenderTarget[0].RenderTargetWriteMask = 0x0F;

			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

			m_d3dDevice_->CreateBlendState(&m_blend_desc_, &m_alpha_blend_state_);
			m_d3dContext_->OMSetBlendState(m_alpha_blend_state_, blendFactor, 0xFFFFFFFF);
		}
		else{
			ZeroMemory(&m_blend_desc_, sizeof(m_blend_desc_));
			m_blend_desc_.RenderTarget[0].BlendEnable = TRUE;
			m_blend_desc_.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			m_blend_desc_.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			m_blend_desc_.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			m_blend_desc_.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
			m_blend_desc_.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
			m_blend_desc_.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
			m_blend_desc_.RenderTarget[0].RenderTargetWriteMask = 0x0F;

			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

			m_d3dDevice_->CreateBlendState(&m_blend_desc_, &m_alpha_blend_state_);
			m_d3dContext_->OMSetBlendState(m_alpha_blend_state_, blendFactor, 0xFFFFFFFF);
		}
	}
	//void VoltaRenderEngine::CompileShader(LPCSTR file, LPCSTR entry, LPCSTR shaderModel, ID3DBlob** buffer){
	//	//	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	//	//
	//	//#if defined( DEBUG ) || defined( _DEBUG )
	//	//	shaderFlags |= D3DCOMPILE_DEBUG;
	//	//#endif
	//	//ID3DBlob* errorBuffer = 0;
	//	D3DX11CompileFromFile(file, 0, 0, entry, shaderModel,
	//		D3DCOMPILE_ENABLE_STRICTNESS, 0, 0, buffer, 0, 0); // 倒数第二个参数是errorBuffer
	//}

	D3D_DRIVER_TYPE VoltaRenderEngine::m_driver_type_;
	D3D_FEATURE_LEVEL VoltaRenderEngine::m_feature_level_;

	ID3D11Device* VoltaRenderEngine::m_d3dDevice_ = nullptr;
	ID3D11DeviceContext* VoltaRenderEngine::m_d3dContext_ = nullptr;
	IDXGISwapChain* VoltaRenderEngine::m_swapchain_ = nullptr;
	ID3D11RenderTargetView* VoltaRenderEngine::m_oppoRTV_ = nullptr;
	ID3D11RenderTargetView* VoltaRenderEngine::m_backbufferRTV_ = nullptr;
	ID3D11RenderTargetView* VoltaRenderEngine::m_tempbufferRTV1_ = nullptr;
	ID3D11ShaderResourceView* VoltaRenderEngine::m_tempbufferSRV1_ = nullptr;
	ID3D11RenderTargetView* VoltaRenderEngine::m_tempbufferRTV2_ = nullptr;
	ID3D11ShaderResourceView* VoltaRenderEngine::m_tempbufferSRV2_ = nullptr;
	ID3D11ShaderResourceView* VoltaRenderEngine::m_ScreenTexture_ = nullptr;
	ID3D11Buffer* VoltaRenderEngine::m_screenVB_ = nullptr;
	VoltaRenderEngine::CBufferPerO VoltaRenderEngine::CBPerOContent_;
	ID3D11Buffer* VoltaRenderEngine::m_ShaderCBPerO_ = nullptr;
	VoltaRenderEngine::CBufferPerF VoltaRenderEngine::CBPerFContent_;
	ID3D11Buffer* VoltaRenderEngine::m_ShaderCBPerF_ = nullptr;
	VoltaRenderEngine::CBufferLoose VoltaRenderEngine::CBLContent_;
	ID3D11Buffer* VoltaRenderEngine::m_ShaderCBL_ = nullptr;
	XMMATRIX VoltaRenderEngine::m_vpMatrix_;
	D3D11_BLEND_DESC VoltaRenderEngine::m_blend_desc_;
	ID3D11BlendState* VoltaRenderEngine::m_alpha_blend_state_ = nullptr;

	VisualManager VoltaRenderEngine::Manager;

	float VoltaRenderEngine::render_width_;
	float VoltaRenderEngine::render_height_;

	bool VoltaRenderEngine::highlight_ = false;

	map<string, ID3D11VertexShader*> VS_lib_;
	map<string, ID3D11PixelShader*> PS_lib_;
	// 缓冲描述资源库。
	map<string, D3D11_BUFFER_DESC*> BDesc_lib_;
	// 输入布局描述资源库。
	map<string, D3D11_INPUT_ELEMENT_DESC*> IEDesc_lib_;
	// 纹理描述资源库。
	map<string, ID3D11ShaderResourceView*> T_lib_;
	// 纹理对应的模型空间顶点。
	map<ID3D11ShaderResourceView*, ID3D11Buffer*> TVB_lib_;
	// 纹理尺寸库。
	map<ID3D11ShaderResourceView*, XMFLOAT2> TS_lib_;
	// 采样器描述资源库。
	map<string, D3D11_SAMPLER_DESC*> SDesc_lib_;
	// 采样状态资源库。
	map<string, ID3D11SamplerState*> SS_lib_;

	// 动画库。实质上是Pixel Shader库的别名。
	map<string, ID3D11PixelShader*>& AniLib = PS_lib_;
	ID3D11InputLayout* input_layout_ = nullptr;
}