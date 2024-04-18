#include "Com_Visual.h"
#include "../System/ShaderPool.h"
#include "../DirectX/renderer.h"

void Com_Visual::ShadowDraw()
{
	if (bShadowEnable)
	{
		p_PS = static_cast<PixelShader*>(ShaderPool::GetShader("PS_Depth"));
		Draw();
		p_PS = static_cast<PixelShader*>(ShaderPool::GetShader(PSName));
	}
}

void Com_Visual::UpdateScale(float _x, float _y, float _z)
{
	mScale.x = _x;
	mScale.y = _y;
	mScale.z = _z;
	mScale.w = 1.0f;
}

void Com_Visual::SetVertex(const char* _name)
{
	p_VS = static_cast<VertexShader*>(ShaderPool::GetShader(_name));
	VSName = _name;
}

void Com_Visual::SetPixel(const char* _name)
{
	p_PS = static_cast<PixelShader*>(ShaderPool::GetShader(_name));
	PSName = _name;
}

void Com_Visual::SetVertexGBuffer(const char* _name)
{
	p_VS = static_cast<VertexShader*>(ShaderPool::GetShader(_name));
}

void Com_Visual::SetPixelGbuffer(const char* _name)
{
	p_PS = static_cast<PixelShader*>(ShaderPool::GetShader(_name));
}

void Com_Visual::Bind()
{
	Renderer::SetScale(mScale);

	VSName;
	PSName;

	if (p_VS)
	{
		p_VS->Bind();
	}
	if (p_PS)
	{
		p_PS->Bind();
	}
}