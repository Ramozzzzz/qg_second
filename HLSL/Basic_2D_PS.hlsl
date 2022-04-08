#include "Basic.hlsli"

// 像素着色器(2D)
float4 PS(VertexPosHTex pIn) : SV_Target
{   	
    return g_Tex[0].Sample(g_SamLinear, pIn.Tex);            
}
        