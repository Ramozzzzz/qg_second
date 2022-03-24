### QG训练营图形组作业1

#### 问题描述：

绘制正六边形

#### 思路分析：

1.使用TRIANGLELIST图元类型，输入18个顶点绘制6个独立的三角形。

2.使用TRIANGLESTRIP图元类型输入9个顶点绘制六个相连的三角形。

#### 代码实现：

##### 1.TRIANGLELIST图元类型：

`// 设置六边形顶点
	VertexPosColor vertices[] =
	{
		{ XMFLOAT3(-0.25f, sqrt(0.1875), 0.5f), XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f)},
		{ XMFLOAT3(0.25f, sqrt(0.1875), 0.5f), XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f)},
		{ XMFLOAT3(0.0f, 0.0f, 0.5f), XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.25f, sqrt(0.1875), 0.5f), XMFLOAT4(0.3f, 0.0f, 1.0f, 1.0f)},
		{ XMFLOAT3(0.5f, 0.0f, 0.5f), XMFLOAT4(0.4f, 0.0f, 1.0f, 1.0f)},
		{ XMFLOAT3(0.0f, 0.0f, 0.5f), XMFLOAT4(1.0f, 0.4f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, 0.0f, 0.5f), XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f)},
		{ XMFLOAT3(0.25f, -sqrt(0.1875), 0.5f), XMFLOAT4(0.1f, 0.0f, 1.0f, 1.0f)},
		{ XMFLOAT3(0.0f, 0.0f, 0.5f), XMFLOAT4(1.0f, 0.6f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.25f, -sqrt(0.1875), 0.5f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)},
		{ XMFLOAT3(-0.25f, -sqrt(0.1875), 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{ XMFLOAT3(0.0f, 0.0f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.25f, -sqrt(0.1875), 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{ XMFLOAT3(-0.5f, 0.0f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{ XMFLOAT3(0.0f, 0.0f, 0.5f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, 0.0f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{ XMFLOAT3(-0.25f, sqrt(0.1875), 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{ XMFLOAT3(0.0f, 0.0f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }
	};`

`// 设置图元类型，设定输入布局
	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout.Get());`

`// 绘制六边形
	m_pd3dImmediateContext->Draw(18, 0);
	HR(m_pSwapChain->Present(0, 0));`

##### 2.TRIANGLESTRIP图元类型：

`// 设置六边形顶点
	VertexPosColor vertices[] =
	{
		{ XMFLOAT3(-0.25f, sqrt(0.1875), 0.5f), XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f)},
		{ XMFLOAT3(0.25f, sqrt(0.1875), 0.5f), XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f)},
		{ XMFLOAT3(0.0f, 0.0f, 0.5f), XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, 0.0f, 0.5f), XMFLOAT4(0.4f, 0.0f, 1.0f, 1.0f)},	
		{ XMFLOAT3(0.25f, -sqrt(0.1875), 0.5f), XMFLOAT4(0.1f, 0.0f, 1.0f, 1.0f)},	
		{ XMFLOAT3(-0.25f, -sqrt(0.1875), 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{ XMFLOAT3(0.0f, 0.0f, 0.5f), XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, 0.0f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{ XMFLOAT3(-0.25f, sqrt(0.1875), 0.5f), XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f)},
	};`

`// 设置图元类型，设定输入布局
	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout.Get());`

`// 绘制六边形
	m_pd3dImmediateContext->Draw(9, 0);
	HR(m_pSwapChain->Present(0, 0));`

#### 测试结果：

##### 1.TRIANGLELIST图元类型：

![](https://img-blog.csdnimg.cn/284cfd68decb4caca42a009c86098cd1.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAUmFtb3p6eg==,size_19,color_FFFFFF,t_70,g_se,x_16)



##### 2.TRIANGLESTRIP图元类型：

![](https://img-blog.csdnimg.cn/49270679d40f449392c2b4192a3db59b.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAUmFtb3p6eg==,size_19,color_FFFFFF,t_70,g_se,x_16)



