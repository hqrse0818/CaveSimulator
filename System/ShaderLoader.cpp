#include "ShaderLoader.h"
#include "ShaderPool.h"

VertexShaderLoader::VertexShaderLoader()
{
	p_mVertexShader = new VertexShader();
}

HRESULT VertexShaderLoader::Load(const char* _name)
{
	return p_mVertexShader->Load(_name);
}

HRESULT VertexShaderLoader::Load(const char* _name, Vertex_Type _type)
{
	return p_mVertexShader->Load(_name, _type);
}

void VertexShaderLoader::Uninit()
{
	p_mVertexShader->Uninit();
	delete p_mVertexShader;
}

PixelShaderLoader::PixelShaderLoader()
{
	p_mPixelShader = new PixelShader();
}

HRESULT PixelShaderLoader::Load(const char* _name)
{
	return p_mPixelShader->Load(_name);
}

void PixelShaderLoader::Uninit()
{
	p_mPixelShader->Uninit();
	delete p_mPixelShader;
}

void ShaderLoader::Push(const char* _Name)
{
	ShaderPool::Push(_Name, this);
}

ComputeShaderLoader::ComputeShaderLoader()
{
	p_mComputeShader = new ComputeShader();
}

HRESULT ComputeShaderLoader::Load(const char* _name)
{
	return p_mComputeShader->Load(_name);
}

void ComputeShaderLoader::Uninit()
{
	p_mComputeShader->Uninit();
	delete p_mComputeShader;
}
