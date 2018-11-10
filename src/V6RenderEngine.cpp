#include "..\\inc\\V6RenderEngine.h"
#include "..\\inc\\V6ResourceDictionary.h"

namespace VoltaEngine{
	bool VoltaRenderEngine::Init(HINSTANCE hi, HWND hwnd){
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
		swapChainDesc.BufferDesc.Width = V6WINDOWWIDTH;
		swapChainDesc.BufferDesc.Height = V6WINDOWHEIGHT;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Windowed = true;
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

		ID3D11Texture2D* backBufferTexture;

		result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);

		if (FAILED(result))
		{
			DXTRACE_MSG("Failed to get the swap chain back buffer!");
			return false;
		}

		result = dDevice->CreateRenderTargetView(backBufferTexture, 0, &BackBufferTarget);

		if (backBufferTexture)
			backBufferTexture->Release();

		if (FAILED(result))
		{
			DXTRACE_MSG("Failed to create the render target view!");
			return false;
		}

		dContext->OMSetRenderTargets(1, &BackBufferTarget, 0);

		D3D11_VIEWPORT viewport;
		viewport.Width = static_cast<float>(V6WINDOWWIDTH);
		viewport.Height = static_cast<float>(V6WINDOWHEIGHT);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		dContext->RSSetViewports(1, &viewport);

		InitLib();
		return true;
	}
	void VoltaEngine::InitLib(){
		AddShaderResourceFromFile("Simple", "Basic.fx");
		ID3D11VertexShader* vsdefau;
		ID3D11PixelShader* psdefau;
		VoltaEngine::CompileShaderFromFile("TexBasic.fx", "VS_Main", "vs_4_0", &InputLayoutVSBuffer);
		VoltaRenderEngine::CreateVS(InputLayoutVSBuffer, &vsdefau);
		ID3DBlob* psBuffer = 0;
		VoltaEngine::CompileShaderFromFile("TexBasic.fx", "PS_Main", "ps_4_0", &psBuffer);
		VoltaRenderEngine::CreatePS(psBuffer, &psdefau);
		VSLib.insert(PAIRVS("TexSampler", vsdefau));
		PSLib.insert(PAIRPS("TexSampler", psdefau));
		psBuffer->Release();
		AddTextureFromFile("image1", "image1.dds");

		D3D11_BUFFER_DESC* vertexDesc = new D3D11_BUFFER_DESC();
		ZeroMemory(vertexDesc, sizeof(vertexDesc));
		vertexDesc->Usage = D3D11_USAGE_DEFAULT;
		vertexDesc->BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc->ByteWidth = sizeof(UVVertex)* 4;
		BDescLib.insert(pair<string, D3D11_BUFFER_DESC*>("UVVBD", vertexDesc));

		D3D11_SAMPLER_DESC* colorMapDesc = new D3D11_SAMPLER_DESC();
		ID3D11SamplerState* colorMapSampler;
		ZeroMemory(colorMapDesc, sizeof(colorMapDesc));
		colorMapDesc->AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		colorMapDesc->AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		colorMapDesc->AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		colorMapDesc->ComparisonFunc = D3D11_COMPARISON_NEVER;
		colorMapDesc->Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		colorMapDesc->MaxLOD = D3D11_FLOAT32_MAX;
		MapInsert(&SDescLib, static_cast<string>("WRAP"), colorMapDesc);

		VoltaRenderEngine::dDevice->CreateSamplerState(SDescLib["WRAP"],
			&colorMapSampler);
		MapInsert(&SSLib, static_cast<string>("WRAP"), colorMapSampler);
	}
	// ShaderName:Shader的名称，不应包括扩展名。
	void VoltaEngine::AddShaderResourceFromFile(string ShaderName, LPCSTR file){
		ID3D11VertexShader* vsdefau;
		ID3D11PixelShader* psdefau;

		ID3DBlob* vsBuffer = 0;
		VoltaEngine::CompileShaderFromFile(file, "VS_Main", "vs_4_0", &vsBuffer);
		VoltaRenderEngine::CreateVS(vsBuffer, &vsdefau);
		ID3DBlob* psBuffer = 0;
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

		ID3DBlob* vsBuffer = 0;
		VoltaEngine::CompileShader(ShaderName, "VS_Main", "vs_4_0", &vsBuffer);
		VoltaRenderEngine::CreateVS(vsBuffer, &vsdefau);
		ID3DBlob* psBuffer = 0;
		VoltaEngine::CompileShader(ShaderName, "PS_Main", "ps_4_0", &psBuffer);
		VoltaRenderEngine::CreatePS(psBuffer, &psdefau);
		VSLib.insert(PAIRVS(ShaderName, vsdefau));
		PSLib.insert(PAIRPS(ShaderName, psdefau));
		vsBuffer->Release();
		psBuffer->Release();
	}
	void VoltaEngine::AddTextureFromFile(string TextureName, LPCSTR file){
		ID3D11ShaderResourceView* colorMap;
		D3DX11CreateShaderResourceViewFromFile(VoltaRenderEngine::dDevice,
			file, 0, 0, &colorMap, 0);
		MapInsert(&TLib, TextureName, colorMap);
	}

	void VoltaRenderEngine::Dispose(){
		if (BackBufferTarget) BackBufferTarget->Release();
		if (SwapChain) SwapChain->Release();
		if (dContext) dContext->Release();
		if (dDevice) dDevice->Release();
		Manager.Clear();

		BackBufferTarget = 0;
		SwapChain = 0;
		dContext = 0;
		dDevice = 0;
	}
	void VoltaRenderEngine::Render(){
		if (!dContext)
			return;
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		dContext->ClearRenderTargetView(BackBufferTarget, clearColor);
		Manager.Render();
		SwapChain->Present(0, 0);
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
	}
	void VisualManager::Clear(){
		auto itor = TopControls.begin();
		while (itor != TopControls.end()){
			delete *itor;
			*itor = 0;
			TopControls.erase(itor++);
		}
		itor = Controls.begin();
		while (itor != Controls.end()){
			delete *itor;
			*itor = 0;
			Controls.erase(itor++);
		}
		auto itor2 = Sprites.begin();
		while (itor2 != Sprites.end()){
			delete *itor2;
			*itor2 = 0;
			Sprites.erase(itor2++);
		}
	}
	template<typename T>
	void VoltaEngine::DisposeLib(map<string, T*>* l){
		auto itor = l->begin();
		while (itor != l->end()){
			delete (*itor).second;
			(*itor).second = 0;
			l->erase(itor++);
		}
	}
	void VoltaEngine::DisposeRecourses(){
		DisposeLib(&VSLib);
		DisposeLib(&PSLib);
		DisposeLib(&BDescLib);
		DisposeLib(&IEDescLib);
		DisposeLib(&TLib);
		DisposeLib(&SDescLib);
		DisposeLib(&SSLib);
		InputLayoutVSBuffer->Release();
		InputLayoutVSBuffer = 0;
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

	ID3D11Device* VoltaRenderEngine::dDevice;
	ID3D11DeviceContext* VoltaRenderEngine::dContext;
	IDXGISwapChain* VoltaRenderEngine::SwapChain;
	ID3D11RenderTargetView* VoltaRenderEngine::BackBufferTarget;

	VisualManager VoltaRenderEngine::Manager;

	map<string, ID3D11VertexShader*> VSLib;
	map<string, ID3D11PixelShader*> PSLib;
	// 缓冲描述资源库。
	map<string, D3D11_BUFFER_DESC*> BDescLib;
	// 输入布局描述资源库。
	map<string, D3D11_INPUT_ELEMENT_DESC*> IEDescLib;
	// 纹理描述资源库。
	map<string, ID3D11ShaderResourceView*> TLib;
	// 采样器描述资源库。
	map<string, D3D11_SAMPLER_DESC*> SDescLib;
	// 采样状态资源库。
	map<string, ID3D11SamplerState*> SSLib;
	ID3DBlob* InputLayoutVSBuffer = 0;
}