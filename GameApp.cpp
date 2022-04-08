#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
using namespace DirectX;

int Img();

GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance),
	m_IndexCount(),	
	m_CurrFrame(),
	m_CurrMode(ShowMode::WoodCrate1),
	m_VSConstantBuffer(),
	m_PSConstantBuffer()
{
}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

	if (!InitEffect())
		return false;

	if (!InitResource())
		return false;

	// 初始化鼠标，键盘不需要
	m_pMouse->SetWindow(m_hMainWnd);
	m_pMouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);

	return true;
}

void GameApp::OnResize()
{
	assert(m_pd2dFactory);
	assert(m_pdwriteFactory);
	// 释放D2D的相关资源
	m_pColorBrush.Reset();
	m_pd2dRenderTarget.Reset();

	D3DApp::OnResize();

	
	
}

void GameApp::UpdateScene(float dt)
{
	
	Keyboard::State state = m_pKeyboard->GetState();
	Mouse::State mouseState = m_pMouse->GetState();
	Mouse::State lastMouseState = m_MouseTracker.GetLastState();
	m_KeyboardTracker.Update(state);	
	m_MouseTracker.Update(mouseState);
	// 键盘切换模式
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::D1) && m_CurrMode != ShowMode::WoodCrate1)
	{
		// 播放木箱动画
		m_CurrMode = ShowMode::WoodCrate1;
		m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());
		auto meshData = Geometry::CreateBox();
		ResetMesh(meshData);
		m_pd3dImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
		m_pd3dImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
		m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pWoodCrate[static_cast<size_t>(0)].GetAddressOf());
		
	}
	else if (m_KeyboardTracker.IsKeyPressed(Keyboard::D2) && m_CurrMode != ShowMode::FireAnim)
	{
		m_CurrMode = ShowMode::FireAnim;
		m_CurrFrame = 0;
		m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout2D.Get());
		auto meshData = Geometry::Create2DShow();
		ResetMesh(meshData);
		m_pd3dImmediateContext->VSSetShader(m_pVertexShader2D.Get(), nullptr, 0);
		m_pd3dImmediateContext->PSSetShader(m_pPixelShader2D.Get(), nullptr, 0);
		m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pFireAnims[0].GetAddressOf());
	}
	
	else if (m_KeyboardTracker.IsKeyPressed(Keyboard::D3) && m_CurrMode != ShowMode::WoodCrate2)
	{
		m_CurrMode = ShowMode::WoodCrate2;
		m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());
		auto meshData = Geometry::CreateBox();
		ResetMesh(meshData);
		m_pd3dImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
		m_pd3dImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
		
	}
		
	
		
	

	if (m_CurrMode == ShowMode::WoodCrate1|| m_CurrMode == ShowMode::WoodCrate2)
	{
		static float cubePhi = 0.0f, cubeTheta = 0.0f;
		
		if (mouseState.leftButton == true && m_MouseTracker.leftButton == m_MouseTracker.HELD)
		{
			cubeTheta -= (mouseState.x - lastMouseState.x) * 0.01f;
			cubePhi -= (mouseState.y - lastMouseState.y) * 0.01f;
			
		}
		if (state.IsKeyDown(Keyboard::W))
		{
			cubePhi += dt * 2;
		}
		if (state.IsKeyDown(Keyboard::S))
		{
			cubePhi -= dt * 2;
		}
		if (state.IsKeyDown(Keyboard::A))
		{
			cubeTheta += dt * 2;
		}
		if (state.IsKeyDown(Keyboard::D))
		{
			cubeTheta -= dt * 2;
		}
		if (mouseState.leftButton == true && m_MouseTracker.leftButton == m_MouseTracker.HELD)
		{
			cubeTheta -= (mouseState.x - lastMouseState.x) * 0.01f;
			cubePhi -= (mouseState.y - lastMouseState.y) * 0.01f;
		}
		if (state.IsKeyDown(Keyboard::W))
		{
			cubePhi += dt * 2;
		}
		if (state.IsKeyDown(Keyboard::S))
		{
			cubePhi -= dt * 2;
		}
		if (state.IsKeyDown(Keyboard::A))
		{
			cubeTheta += dt * 2;
		}
		if (state.IsKeyDown(Keyboard::D))
		{
			cubeTheta -= dt * 2;
		}
		if (m_CurrMode == ShowMode::WoodCrate1)
		{
			static float phi = 0.0f;
			phi += 0.001f;
			m_VSConstantBuffer.circle = XMMatrixTranslation(-0.5f, -0.5f, 0.0f) * XMMatrixRotationZ(phi) * XMMatrixTranslation(0.5f, 0.5f, 0.0f);
		}
		XMMATRIX W = XMMatrixRotationX(cubePhi) * XMMatrixRotationY(cubeTheta);
		m_VSConstantBuffer.world = XMMatrixTranspose(XMMatrixRotationY(cubeTheta) * XMMatrixRotationX(cubePhi));
		m_VSConstantBuffer.worldInvTranspose = XMMatrixTranspose(InverseTranspose(W));
		
		D3D11_MAPPED_SUBRESOURCE mappedData;
		HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
		memcpy_s(mappedData.pData, sizeof(VSConstantBuffer), &m_VSConstantBuffer, sizeof(VSConstantBuffer));
		m_pd3dImmediateContext->Unmap(m_pConstantBuffers[0].Get(), 0);

		HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
		memcpy_s(mappedData.pData, sizeof(PSConstantBuffer), &m_PSConstantBuffer, sizeof(PSConstantBuffer));
		m_pd3dImmediateContext->Unmap(m_pConstantBuffers[1].Get(), 0);
	}
	else if (m_CurrMode == ShowMode::FireAnim)
	{
		// 用于限制在1秒60帧
		static float totDeltaTime = 0;

		totDeltaTime += dt;
		if (totDeltaTime > 1.0f / 60)
		{
			totDeltaTime -= 1.0f / 60;
			m_CurrFrame = (m_CurrFrame + 1) % 120;
			m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pFireAnims[m_CurrFrame].GetAddressOf());
		}		
	}
}

void GameApp::DrawScene()
{
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);

	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), reinterpret_cast<const float*>(&Colors::Green));
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	// 绘制几何模型
	if (m_CurrMode == ShowMode::WoodCrate1||m_CurrMode==ShowMode::FireAnim)
	{
		m_pd3dImmediateContext->DrawIndexed(m_IndexCount, 0, 0);
	}
	
	if (m_CurrMode == ShowMode::WoodCrate2)
	{
		for (int i = 0; i < m_IndexCount; i += 3)
		{
			if (i == 0)
			{
				m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pWoodCrate[static_cast<size_t>(2)].GetAddressOf());
			}
			else if (i == 6)
			{
				m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pWoodCrate[static_cast<size_t>(3)].GetAddressOf());
			}
			else if (i == 12)
			{
				m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pWoodCrate[static_cast<size_t>(4)].GetAddressOf());
			}
			else if (i == 18)
			{
				m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pWoodCrate[static_cast<size_t>(5)].GetAddressOf());
			}
			else if (i == 24)
			{
				m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pWoodCrate[static_cast<size_t>(6)].GetAddressOf());
			}
			else if (i == 30)
			{
				m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pWoodCrate[static_cast<size_t>(7)].GetAddressOf());
			}
			m_pd3dImmediateContext->DrawIndexed(3, i, 0);
		}
	}

	
	
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	static bool f = true;
	ImGui::Begin("Window", &f);   
	ImGui::Text("Press 1,2,3 to change.");
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	
	HR(m_pSwapChain->Present(0, 0));
}


bool GameApp::InitEffect()
{
	ComPtr<ID3DBlob> blob;

	// 创建顶点着色器(2D)
	HR(CreateShaderFromFile(L"HLSL\\Basic_2D_VS.cso", L"HLSL\\Basic_2D_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader2D.GetAddressOf()));
	// 创建顶点布局(2D)
	HR(m_pd3dDevice->CreateInputLayout(VertexPosTex::inputLayout, ARRAYSIZE(VertexPosTex::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout2D.GetAddressOf()));

	// 创建像素着色器(2D)
	HR(CreateShaderFromFile(L"HLSL\\Basic_2D_PS.cso", L"HLSL\\Basic_2D_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader2D.GetAddressOf()));

	// 创建顶点着色器(3D)
	HR(CreateShaderFromFile(L"HLSL\\Basic_3D_VS.cso", L"HLSL\\Basic_3D_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader3D.GetAddressOf()));
	// 创建顶点布局(3D)
	HR(m_pd3dDevice->CreateInputLayout(VertexPosNormalTex::inputLayout, ARRAYSIZE(VertexPosNormalTex::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout3D.GetAddressOf()));

	// 创建像素着色器(3D)
	HR(CreateShaderFromFile(L"HLSL\\Basic_3D_PS.cso", L"HLSL\\Basic_3D_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader3D.GetAddressOf()));

	return true;
}

bool GameApp::InitResource()
{
	// 初始化网格模型并设置到输入装配阶段
	auto meshData = Geometry::CreateBox();
	ResetMesh(meshData);

	// ******************
	// 设置常量缓冲区描述
	//
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(VSConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// 新建用于VS和PS的常量缓冲区
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[0].GetAddressOf()));
	cbd.ByteWidth = sizeof(PSConstantBuffer);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[1].GetAddressOf()));

	// ******************
	// 初始化纹理和采样器状态
	//

	// 初始化木箱纹理
	m_pWoodCrate.resize(8);
	HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\flare.dds", nullptr, m_pWoodCrate[static_cast<size_t>(0)].GetAddressOf()));
	HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\flarealpha.dds", nullptr, m_pWoodCrate[static_cast<size_t>(1)].GetAddressOf()));
	HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\1.png", nullptr, m_pWoodCrate[static_cast<size_t>(2)].GetAddressOf()));
	HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\2.jpg", nullptr, m_pWoodCrate[static_cast<size_t>(3)].GetAddressOf()));
	HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\3.jpg", nullptr, m_pWoodCrate[static_cast<size_t>(4)].GetAddressOf()));
	HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\4.jpg", nullptr, m_pWoodCrate[static_cast<size_t>(5)].GetAddressOf()));
	HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\5.jpg", nullptr, m_pWoodCrate[static_cast<size_t>(6)].GetAddressOf()));
	HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\6.jpg", nullptr, m_pWoodCrate[static_cast<size_t>(7)].GetAddressOf()));
	
	// 初始化火焰纹理
	WCHAR strFile[40];
	m_pFireAnims.resize(120);
	for (int i = 1; i <= 120; ++i)
	{
		wsprintf(strFile, L"..\\Texture\\FireAnim\\Fire%03d.bmp", i);
		HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), strFile, nullptr, m_pFireAnims[static_cast<size_t>(i) - 1].GetAddressOf()));
	}
		
	// 初始化采样器状态
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR(m_pd3dDevice->CreateSamplerState(&sampDesc, m_pSamplerState.GetAddressOf()));

	
	// ******************
	// 初始化常量缓冲区的值
	//

	// 初始化用于VS的常量缓冲区的值
	m_VSConstantBuffer.world = XMMatrixIdentity();			
	m_VSConstantBuffer.view = XMMatrixTranspose(XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
	m_VSConstantBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
	m_VSConstantBuffer.worldInvTranspose = XMMatrixIdentity();
	
	// 初始化用于PS的常量缓冲区的值
	
	m_PSConstantBuffer.dirLight[0].ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_PSConstantBuffer.dirLight[0].diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_PSConstantBuffer.dirLight[0].specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_PSConstantBuffer.dirLight[0].direction = XMFLOAT3(-0.577f, -0.577f, 0.577f);
	m_PSConstantBuffer.numDirLight = 1;
	m_PSConstantBuffer.numPointLight = 0;
	m_PSConstantBuffer.numSpotLight = 0;
	// 初始化材质
	m_PSConstantBuffer.material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_PSConstantBuffer.material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_PSConstantBuffer.material.specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 5.0f);
	// 注意不要忘记设置此处的观察位置，否则高亮部分会有问题
	m_PSConstantBuffer.eyePos = XMFLOAT4(0.0f, 0.0f, -5.0f, 0.0f);

	// 更新PS常量缓冲区资源
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(PSConstantBuffer), &m_PSConstantBuffer, sizeof(PSConstantBuffer));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffers[1].Get(), 0);

	// ******************
	// 给渲染管线各个阶段绑定好所需资源
	// 设置图元类型，设定输入布局
	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout3D.Get());
	// 默认绑定3D着色器
	m_pd3dImmediateContext->VSSetShader(m_pVertexShader3D.Get(), nullptr, 0);
	// VS常量缓冲区对应HLSL寄存于b0的常量缓冲区
	m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, m_pConstantBuffers[0].GetAddressOf());
	// PS常量缓冲区对应HLSL寄存于b1的常量缓冲区
	m_pd3dImmediateContext->PSSetConstantBuffers(1, 1, m_pConstantBuffers[1].GetAddressOf());
	// 像素着色阶段设置好采样器
	m_pd3dImmediateContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
	m_pd3dImmediateContext->PSSetShaderResources(0, 8, m_pWoodCrate[static_cast<size_t>(0)(1)(2)(3)(4)(5)(6)(7)].GetAddressOf());
	//m_pd3dImmediateContext->PSSetShaderResources(0, 2, m_pWoodCrate[static_cast<size_t>(0)(1)].GetAddressOf());
	m_pd3dImmediateContext->PSSetShader(m_pPixelShader3D.Get(), nullptr, 0);
	
	// ******************
	// 设置调试对象名
	//
	D3D11SetDebugObjectName(m_pVertexLayout2D.Get(), "VertexPosTexLayout");
	D3D11SetDebugObjectName(m_pVertexLayout3D.Get(), "VertexPosNormalTexLayout");
	D3D11SetDebugObjectName(m_pConstantBuffers[0].Get(), "VSConstantBuffer");
	D3D11SetDebugObjectName(m_pConstantBuffers[1].Get(), "PSConstantBuffer");
	D3D11SetDebugObjectName(m_pVertexShader2D.Get(), "Basic_2D_VS");
	D3D11SetDebugObjectName(m_pVertexShader3D.Get(), "Basic_3D_VS");
	D3D11SetDebugObjectName(m_pPixelShader2D.Get(), "Basic_2D_PS");
	D3D11SetDebugObjectName(m_pPixelShader3D.Get(), "Basic_3D_PS");
	D3D11SetDebugObjectName(m_pSamplerState.Get(), "SSLinearWrap");

	return true;
}
template<class VertexType>
bool GameApp::ResetMesh(const Geometry::MeshData<VertexType>& meshData)
{
	// 释放旧资源
	m_pVertexBuffer.Reset();
	m_pIndexBuffer.Reset();



	// 设置顶点缓冲区描述
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = (UINT)meshData.vertexVec.size() * sizeof(VertexType);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// 新建顶点缓冲区
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = meshData.vertexVec.data();
	HR(m_pd3dDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));

	// 输入装配阶段的顶点缓冲区设置
	UINT stride = sizeof(VertexType);			// 跨越字节数
	UINT offset = 0;							// 起始偏移量

	m_pd3dImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);



	// 设置索引缓冲区描述
	m_IndexCount = (UINT)meshData.indexVec.size();
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * m_IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// 新建索引缓冲区
	InitData.pSysMem = meshData.indexVec.data();
	HR(m_pd3dDevice->CreateBuffer(&ibd, &InitData, m_pIndexBuffer.GetAddressOf()));
	// 输入装配阶段的索引缓冲区设置
	m_pd3dImmediateContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);



	// 设置调试对象名
	D3D11SetDebugObjectName(m_pVertexBuffer.Get(), "VertexBuffer");
	D3D11SetDebugObjectName(m_pIndexBuffer.Get(), "IndexBuffer");

	return true;
}
