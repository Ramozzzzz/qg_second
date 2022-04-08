### QG训练营图形组作业3

#### 问题描述：

给一个立方体六个面贴不同的纹理，并且使用方向光光照

#### 思路分析：

利用DrawIndexed函数，根据索引分别使用不同纹理绘制立方体的六个面。

#### 代码实现：

`HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\1.png", nullptr, m_pWoodCrate[static_cast<size_t>(2)].GetAddressOf()));
	HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\2.jpg", nullptr, m_pWoodCrate[static_cast<size_t>(3)].GetAddressOf()));
	HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\3.jpg", nullptr, m_pWoodCrate[static_cast<size_t>(4)].GetAddressOf()));
	HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\4.jpg", nullptr, m_pWoodCrate[static_cast<size_t>(5)].GetAddressOf()));
	HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\5.jpg", nullptr, m_pWoodCrate[static_cast<size_t>(6)].GetAddressOf()));
	HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\6.jpg", nullptr, m_pWoodCrate[static_cast<size_t>(7)].GetAddressOf()));`



`if (m_CurrMode == ShowMode::WoodCrate2)  //WoodCrate2表示为六面不同纹理的立方体
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
	}`

#### 测试结果：

![](https://img-blog.csdnimg.cn/d040af18a77a403ea8f24b15885c4c10.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAUmFtb3p6eg==,size_20,color_FFFFFF,t_70,g_se,x_16)



#### 问题描述：

利用纹理数组实现2D纹理动画（09项目中的火焰动画）

#### 思路分析：

用一个数组存储火焰纹理图片名，然后在UpdateScene中按次序更新以播放。

#### 代码实现：

`else if (m_KeyboardTracker.IsKeyPressed(Keyboard::D2) && m_CurrMode != ShowMode::FireAnim)
	{
		m_CurrMode = ShowMode::FireAnim;
		m_CurrFrame = 0;
		m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout2D.Get());
		auto meshData = Geometry::Create2DShow();
		ResetMesh(meshData);
		m_pd3dImmediateContext->VSSetShader(m_pVertexShader2D.Get(), nullptr, 0);
		m_pd3dImmediateContext->PSSetShader(m_pPixelShader2D.Get(), nullptr, 0);
		m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pFireAnims[0].GetAddressOf());
	}`

`// 初始化火焰纹理
	WCHAR strFile[40];
	m_pFireAnims.resize(120);
	for (int i = 1; i <= 120; ++i)
	{
		wsprintf(strFile, L"..\\Texture\\FireAnim\\Fire%03d.bmp", i);
		HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), strFile, nullptr, m_pFireAnims[static_cast<size_t>(i) - 1].GetAddressOf()));
	}`

#### 测试结果：

![](https://img-blog.csdnimg.cn/38bf414c0b384aaf8d9ec8a7c5f19528.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAUmFtb3p6eg==,size_20,color_FFFFFF,t_70,g_se,x_16)





#### 问题描述：

给一个立方体六个面贴上同一个纹理，这个纹理利用给定的两张纹理进行合成，并且实现纹理在立方体面上的旋转（提示：顶点着色器），关闭光照，还可以利用上上一节课所写的代码来操控立方体的旋转

#### 思路分析：

利用相关矩阵在顶点着色器中运算，通过顶点常量缓冲区更新数据。

#### 代码实现：

`HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\flare.dds", nullptr, m_pWoodCrate[static_cast<size_t>(0)].GetAddressOf()));
	HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"..\\Texture\\flarealpha.dds", nullptr, m_pWoodCrate[static_cast<size_t>(1)].GetAddressOf()));`

` vOut.NormalW = mul(vIn.NormalL, (float3x3) g_WorldInvTranspose);
    vOut.Tex = mul(g_Circle, float4(vIn.Tex, 0.0f, 1.0f));`

`if (m_CurrMode == ShowMode::WoodCrate1)
		{
			static float phi = 0.0f;
			phi += 0.001f;
			m_VSConstantBuffer.circle = XMMatrixTranslation(-0.5f, -0.5f, 0.0f) * XMMatrixRotationZ(phi) * XMMatrixTranslation(0.5f, 0.5f, 0.0f);
		}`

#### 测试结果：

![](https://img-blog.csdnimg.cn/582099d20c8e4542903f83e353d7c06d.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAUmFtb3p6eg==,size_20,color_FFFFFF,t_70,g_se,x_16)