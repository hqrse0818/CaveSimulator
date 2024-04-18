#include "Com_Shader.h"

Com_Shader::Com_Shader()
{
	p_mVS = new VertexShader();
	p_mPS = new PixelShader();
}

void Com_Shader::Uninit()
{
	p_mVS->Uninit();
	delete p_mVS;
	p_mPS->Uninit();
	delete p_mPS;
}

void Com_Shader::Draw()
{
	// シェーダ設定
	p_mVS->Bind();
	p_mPS->Bind();
}
