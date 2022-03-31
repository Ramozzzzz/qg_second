### QG训练营图形组作业2

#### 问题描述：

绘制梯台，且通过鼠标和键盘输入来旋转梯台。

#### 思路分析：

使用鼠标类和键盘类实现控制。通过索引缓冲区的应用减少顶点数量，通过常量缓冲区的不断更新实现梯台的旋转。

#### 代码实现：

##### 鼠标和键盘输入：

`static float cubePhi = 0.0f, cubeTheta = 0.0f;
	Mouse::State mouseState = m_pMouse->GetState();
	Mouse::State lastMouseState = m_MouseTracker.GetLastState();
	Keyboard::State keyState = m_pKeyboard->GetState();
	Keyboard::State lastKeyState = m_KeyboardTracker.GetLastState();

	m_MouseTracker.Update(mouseState);
	m_KeyboardTracker.Update(keyState);
	if (mouseState.leftButton == true && m_MouseTracker.leftButton == m_MouseTracker.HELD)
	{
		cubeTheta -= (mouseState.x - lastMouseState.x) * 0.01f;
		cubePhi -= (mouseState.y - lastMouseState.y) * 0.01f;
	}
	if (keyState.IsKeyDown(Keyboard::W))
	{
		cubePhi += dt * 2;
	}
	if (keyState.IsKeyDown(Keyboard::S))
	{
		cubePhi -= dt * 2;
	}
	if (keyState.IsKeyDown(Keyboard::A))
	{
		cubeTheta += dt * 2;
	}
	if (keyState.IsKeyDown(Keyboard::D))
	{
		cubeTheta -= dt * 2;
	}
	if (keyState.IsKeyDown(Keyboard::Up))
	{
		cubePhi += dt * 2;
	}
	if (keyState.IsKeyDown(Keyboard::Down))
	{
		cubePhi -= dt * 2;
	}
	if (keyState.IsKeyDown(Keyboard::Left))
	{
		cubeTheta += dt * 2;
	}
	if (keyState.IsKeyDown(Keyboard::Right))
	{
		cubeTheta -= dt * 2;
	}
	
	m_CBuffer.world = XMMatrixTranspose(XMMatrixRotationY(cubeTheta) * XMMatrixRotationX(cubePhi));
	
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(m_CBuffer), &m_CBuffer, sizeof(m_CBuffer));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffer.Get(), 0);
}`

##### 梯台顶点和索引：

`VertexPosColor vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, 1.0f, -0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, 1.0f, -0.5f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.2f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.3f, 0.0f, 1.5f, 1.0f) },
		{ XMFLOAT3(-0.5f, 1.0f, 0.5f), XMFLOAT4(1.0f, 0.2f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.5f, 1.0f, 0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.6f, 1.0f, 1.0f, 1.0f) }
	};`

##### 顶点着色器：

`VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posH = mul(float4(vIn.posL, 1.0f), g_World); // mul 才是矩阵乘法, 运算符*要求操作对象为
    vOut.posH = mul(vOut.posH, g_View); // 行列数相等的两个矩阵，结果为
    vOut.posH = mul(vOut.posH, g_Proj); // Cij = Aij * Bij
    vOut.color = vIn.color; // 这里alpha通道的值默认为1.0
    return vOut;
}`

#### 测试结果：

![](https://img-blog.csdnimg.cn/afee1986401844bfb1a0ce0c09883196.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAUmFtb3p6eg==,size_20,color_FFFFFF,t_70,g_se,x_16)

![](https://img-blog.csdnimg.cn/843f6c2b83e14fca95bcf12d271db729.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAUmFtb3p6eg==,size_20,color_FFFFFF,t_70,g_se,x_16)