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
		RenderWidth = width;
		RenderHeight = height;

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
				D3D11_SDK_VERSION, &swapChainDesc, &SwapChain,
				&dDevice, &FeatureLevel, &dContext);

			if (SUCCEEDED(result))
			{
				DriverType = driverTypes[driver];
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
		result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBufferTexture);

		if (FAILED(result))
		{
			DXTRACE_MSG("Failed to get the swap chain back buffer!");
			return false;
		}

		result = dDevice->CreateRenderTargetView(BackBufferTexture, 0, &BackBufferRTV);

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
		dDevice->CreateTexture2D(&textureDesc, NULL, &TempTexture);
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		dDevice->CreateRenderTargetView(TempTexture, &renderTargetViewDesc, &TempBufferRTV1);
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		dDevice->CreateShaderResourceView(TempTexture, &shaderResourceViewDesc, &TempBufferSRV1);
		TempTexture->Release();

		ID3D11Texture2D* TempTexture2;
		dDevice->CreateTexture2D(&textureDesc, NULL, &TempTexture2);
		dDevice->CreateRenderTargetView(TempTexture2, &renderTargetViewDesc, &TempBufferRTV2);
		dDevice->CreateShaderResourceView(TempTexture2, &shaderResourceViewDesc, &TempBufferSRV2);
		TempTexture2->Release();

		// 设置初始渲染环境。
		OppoRTV = TempBufferRTV2;

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

		VoltaRenderEngine::dDevice->CreateBuffer(&vertexDesc, &resourceData, &ScreenVB);

		// 设置视口。
		D3D11_VIEWPORT viewport;
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		dContext->RSSetViewports(1, &viewport);

		// 初始化CBuffer和CBufferContent。
		D3D11_BUFFER_DESC constDesc;
		ZeroMemory(&constDesc, sizeof(constDesc));
		constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constDesc.ByteWidth = sizeof(CBufferPreO);
		constDesc.Usage = D3D11_USAGE_DEFAULT;

		dDevice->CreateBuffer(&constDesc, nullptr, &ShaderCBuffer);

		//CBufferContent.g_time = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		ZeroMemory(&CBufferContent.mvp, sizeof(CBufferContent.mvp));

		dContext->VSSetConstantBuffers(0, 1, &VoltaRenderEngine::ShaderCBuffer);
		dContext->PSSetConstantBuffers(0, 1, &VoltaRenderEngine::ShaderCBuffer);

		// 设置世界坐标系。
		XMMATRIX view = XMMatrixIdentity();
		XMMATRIX projection = XMMatrixOrthographicOffCenterLH(0.0f, width, 0.0f, height, 0.0f, 1.0f);
		vpMatrix = XMMatrixMultiply(view, projection);

		// 设置色彩混合模式。
		ChangeHighlightState();
	}
	void VoltaEngine::InitLib(){
		AddShaderResourceFromFile("Simple", "Basic.fx");
		AddShaderResourceFromFile("Blur", "Blur2.fx");
		AddShaderResourceFromFile("Test", "test.fx");

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
			InputLayoutVSBuffer, &InputLayout);
		ID3DBlob* psBuffer = nullptr;
		VoltaEngine::CompileShaderFromFile("TexBasic.fx", "PS_Main", "ps_4_0", &psBuffer);
		VoltaRenderEngine::CreatePS(psBuffer, &psdefau);
		InputLayoutVSBuffer->Release();

		VSLib.insert(PAIRVS("TexSampler", vsdefau));
		PSLib.insert(PAIRPS("TexSampler", psdefau));
		psBuffer->Release();
		AddTextureFromFile("image1", "image1.dds");
		AddTextureFromFile("net", "net.dds");

		D3D11_BUFFER_DESC* vertexDesc = new D3D11_BUFFER_DESC();
		ZeroMemory(vertexDesc, sizeof(vertexDesc));
		vertexDesc->Usage = D3D11_USAGE_DEFAULT;
		vertexDesc->BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc->ByteWidth = sizeof(UVVertex)* 4;
		BDescLib.insert(pair<string, D3D11_BUFFER_DESC*>("UVVBD", vertexDesc));

		D3D11_SAMPLER_DESC* colorMapDesc = new D3D11_SAMPLER_DESC();
		ID3D11SamplerState* colorMapSampler;
		ZeroMemory(colorMapDesc, sizeof(colorMapDesc));
		colorMapDesc->AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		colorMapDesc->AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		colorMapDesc->AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		colorMapDesc->ComparisonFunc = D3D11_COMPARISON_NEVER;
		colorMapDesc->Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		colorMapDesc->MaxLOD = D3D11_FLOAT32_MAX;
		MapInsert(&SDescLib, static_cast<string>("BORDER"), colorMapDesc);

		VoltaRenderEngine::dDevice->CreateSamplerState(SDescLib["BORDER"],
			&colorMapSampler);
		MapInsert(&SSLib, static_cast<string>("BORDER"), colorMapSampler);

		colorMapDesc = new D3D11_SAMPLER_DESC();
		ZeroMemory(colorMapDesc, sizeof(colorMapDesc));
		colorMapDesc->AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		colorMapDesc->AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		colorMapDesc->AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		colorMapDesc->ComparisonFunc = D3D11_COMPARISON_NEVER;
		colorMapDesc->Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		colorMapDesc->MaxLOD = D3D11_FLOAT32_MAX;
		MapInsert(&SDescLib, static_cast<string>("CLAMP"), colorMapDesc);

		VoltaRenderEngine::dDevice->CreateSamplerState(SDescLib["CLAMP"],
			&colorMapSampler);
		MapInsert(&SSLib, static_cast<string>("CLAMP"), colorMapSampler);
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
		VSLib.insert(PAIRVS(ShaderName, vsdefau));
		PSLib.insert(PAIRPS(ShaderName, psdefau));
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
		VSLib.insert(PAIRVS(ShaderName, vsdefau));
		PSLib.insert(PAIRPS(ShaderName, psdefau));
		vsBuffer->Release();
		psBuffer->Release();
	}
	void VoltaEngine::AddTextureFromFile(string TextureName, LPCSTR file){
		// 添加纹理的拷贝至库。
		ID3D11ShaderResourceView* colorMap;
		D3DX11CreateShaderResourceViewFromFile(VoltaRenderEngine::dDevice,
			file, 0, 0, &colorMap, 0);
		MapInsert(&TLib, TextureName, colorMap);
		// 创建模型空间对应的顶点缓存并将其拷贝、添加至库。
		ID3D11Resource* colorTex;
		colorMap->GetResource(&colorTex);
		D3D11_TEXTURE2D_DESC colorTexDesc;
		((ID3D11Texture2D*)colorTex)->GetDesc(&colorTexDesc);
		colorTex->Release();

		XMFLOAT2 size((float)colorTexDesc.Width, (float)colorTexDesc.Height);
		MapInsert(&TSLib, colorMap, size);

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
		VoltaRenderEngine::dDevice->CreateBuffer(&vertexDesc, &resourceData, &vertexBuffer);
		MapInsert(&TVBLib, colorMap, vertexBuffer);
	}

	void VoltaRenderEngine::Dispose(){
		if (ScreenVB) ScreenVB->Release();
		if (TempBufferRTV1) TempBufferRTV1->Release();
		if (TempBufferSRV1) TempBufferSRV1->Release();
		if (TempBufferRTV2) TempBufferRTV2->Release();
		if (TempBufferSRV2) TempBufferSRV2->Release();
		if (BackBufferRTV) BackBufferRTV->Release();
		if (SwapChain) SwapChain->Release();
		if (dContext) dContext->Release();
		if (dDevice) dDevice->Release();
		if (ShaderCBuffer) ShaderCBuffer->Release();
		if (AlphaBlendState) AlphaBlendState->Release();
		Manager.Clear();
		ScreenVB = nullptr;
		TempBufferSRV1 = nullptr;
		TempBufferRTV1 = nullptr;
		TempBufferSRV2 = nullptr;
		TempBufferRTV2 = nullptr;
		ScreenTexture = nullptr;
		// TargetSRV = nullptr;
		OppoRTV = nullptr;
		BackBufferRTV = nullptr;
		SwapChain = nullptr;
		dContext = nullptr;
		dDevice = nullptr;
		ShaderCBuffer = nullptr;
		AlphaBlendState = nullptr;
	}

	void VoltaRenderEngine::Render(){
		if (!dContext)
			return;
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		//CBufferContent.g_time = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		dContext->ClearRenderTargetView(TempBufferRTV1, clearColor);
		dContext->ClearRenderTargetView(TempBufferRTV2, clearColor);
		dContext->ClearRenderTargetView(BackBufferRTV, clearColor);

		SwapRenderTarget();
		Manager.Render();
		dContext->OMSetRenderTargets(1, &BackBufferRTV, 0);
		SwapInputSRV();
		Present();
		SwapChain->Present(0, 0);
	}
	void VoltaRenderEngine::Present(){
		unsigned int stride = sizeof(UVVertex);
		unsigned int offset = 0;

		dContext->IASetInputLayout(InputLayout);
		dContext->IASetVertexBuffers(0, 1, &ScreenVB, &stride, &offset);
		dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		dContext->VSSetShader(VSLib["TexSampler"], 0, 0);
		dContext->PSSetShader(PSLib["TexSampler"], 0, 0);
		dContext->PSSetShaderResources(0, 1, &ScreenTexture);
		dContext->PSSetSamplers(0, 1, &SSLib["BORDER"]);

		CBufferContent.size = XMFLOAT4(RenderWidth, RenderHeight, 0.0, 0.0);
		CBufferContent.mvp = XMMatrixTranspose(vpMatrix);

		dContext->UpdateSubresource(ShaderCBuffer, 0, 0, &CBufferContent, 0, 0);
		dContext->VSSetConstantBuffers(0, 1, &ShaderCBuffer);

		dContext->Draw(4, 0);
	}
	void VoltaRenderEngine::SwapRenderTarget(){
		dContext->OMSetRenderTargets(1, &OppoRTV, 0);
		if (OppoRTV == TempBufferRTV1){
			OppoRTV = TempBufferRTV2;
			ScreenTexture = TempBufferSRV2;
		}
		else{
			OppoRTV = TempBufferRTV1;
			ScreenTexture = TempBufferSRV1;
		}
	}
	void VisualManager::Render(){

		for_each(Sprites.begin(), Sprites.end(), [](Sprite* one){
			one->Render();
		});
		for_each(Controls.begin(), Controls.end(), [](Control* one){
			one->Render();
		});
		for_each(TopControls.begin(), TopControls.end(), [](Control* one){
			one->Render();
		});
		for_each(ScreenShaders.begin(), ScreenShaders.end(), [](ShaderSprite* one){
			VoltaRenderEngine::SwapRenderTarget();
			one->Render();
		});
	}
	void VisualManager::Clear(){
		DisposeList(&TopControls);
		DisposeList(&Controls);
		DisposeList(&ScreenShaders);
		DisposeList(&Sprites);
	}
	ID3D11ShaderResourceView* VoltaRenderEngine::GetTempShaderResourceView(){
		return nullptr;
	}

	template<typename T1, typename T2>
	void VoltaEngine::DisposeLib(map<T1, T2*>* l){
		auto itor = l->begin();
		while (itor != l->end()){
			delete (*itor).second;
			(*itor).second = nullptr;
			l->erase(itor++);
		}
	}
	void VoltaEngine::DisposeRecourses(){
		DisposeLib(&VSLib);
		DisposeLib(&PSLib);
		DisposeLib(&BDescLib);
		DisposeLib(&IEDescLib);
		DisposeLib(&TLib);
		TSLib.clear();
		DisposeLib(&TVBLib);
		DisposeLib(&SDescLib);
		DisposeLib(&SSLib);
		InputLayout->Release();
		InputLayout = nullptr;
	}

	void VoltaRenderEngine::ChangeHighlightState(){
		if (Highlight){
			ZeroMemory(&BlendDesc, sizeof(BlendDesc));
			BlendDesc.RenderTarget[0].BlendEnable = TRUE;
			BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_ALPHA;
			BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
			BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
			BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
			BlendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

			dDevice->CreateBlendState(&BlendDesc, &AlphaBlendState);
			dContext->OMSetBlendState(AlphaBlendState, blendFactor, 0xFFFFFFFF);
		}
		else{
			ZeroMemory(&BlendDesc, sizeof(BlendDesc));
			BlendDesc.RenderTarget[0].BlendEnable = TRUE;
			BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
			BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
			BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
			BlendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

			dDevice->CreateBlendState(&BlendDesc, &AlphaBlendState);
			dContext->OMSetBlendState(AlphaBlendState, blendFactor, 0xFFFFFFFF);
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

	D3D_DRIVER_TYPE VoltaRenderEngine::DriverType;
	D3D_FEATURE_LEVEL VoltaRenderEngine::FeatureLevel;

	ID3D11Device* VoltaRenderEngine::dDevice = nullptr;
	ID3D11DeviceContext* VoltaRenderEngine::dContext = nullptr;
	IDXGISwapChain* VoltaRenderEngine::SwapChain = nullptr;
	ID3D11RenderTargetView* VoltaRenderEngine::OppoRTV = nullptr;
	ID3D11RenderTargetView* VoltaRenderEngine::BackBufferRTV = nullptr;
	ID3D11RenderTargetView* VoltaRenderEngine::TempBufferRTV1 = nullptr;
	ID3D11ShaderResourceView* VoltaRenderEngine::TempBufferSRV1 = nullptr;
	ID3D11RenderTargetView* VoltaRenderEngine::TempBufferRTV2 = nullptr;
	ID3D11ShaderResourceView* VoltaRenderEngine::TempBufferSRV2 = nullptr;
	ID3D11ShaderResourceView* VoltaRenderEngine::ScreenTexture = nullptr;
	ID3D11Buffer* VoltaRenderEngine::ScreenVB = nullptr;
	VoltaRenderEngine::CBufferPreO VoltaRenderEngine::CBufferContent;
	ID3D11Buffer* VoltaRenderEngine::ShaderCBuffer = nullptr;
	XMMATRIX VoltaRenderEngine::vpMatrix;
	D3D11_BLEND_DESC VoltaRenderEngine::BlendDesc;
	ID3D11BlendState* VoltaRenderEngine::AlphaBlendState = nullptr;

	VisualManager VoltaRenderEngine::Manager;

	float VoltaRenderEngine::RenderWidth;
	float VoltaRenderEngine::RenderHeight;

	bool VoltaRenderEngine::Highlight = false;

	map<string, ID3D11VertexShader*> VSLib;
	map<string, ID3D11PixelShader*> PSLib;
	// 缓冲描述资源库。
	map<string, D3D11_BUFFER_DESC*> BDescLib;
	// 输入布局描述资源库。
	map<string, D3D11_INPUT_ELEMENT_DESC*> IEDescLib;
	// 纹理描述资源库。
	map<string, ID3D11ShaderResourceView*> TLib;
	// 纹理对应的模型空间顶点。
	map<ID3D11ShaderResourceView*, ID3D11Buffer*> TVBLib;
	// 纹理尺寸库。
	map<ID3D11ShaderResourceView*, XMFLOAT2> TSLib;
	// 采样器描述资源库。
	map<string, D3D11_SAMPLER_DESC*> SDescLib;
	// 采样状态资源库。
	map<string, ID3D11SamplerState*> SSLib;
	ID3D11InputLayout* InputLayout = nullptr;
}