#pragma once
#include "../DirectX/Shader.h"

// シェーダーの読み込みクラス

class ShaderLoader
{
public:
	virtual Shader* GetShader() = 0;
	virtual HRESULT Load(const char* _name) = 0;
	virtual void Uninit() = 0;

	void Push(const char* _Name);
};

class VertexShaderLoader
	: public ShaderLoader
{
private:
	VertexShader* p_mVertexShader;
public:
	VertexShaderLoader();
	VertexShader* GetShader()override
	{
		return p_mVertexShader;
	}

	HRESULT Load(const char* _name);
	HRESULT Load(const char* _name, Vertex_Type _type);

	void Uninit()override;
};

class PixelShaderLoader
	: public ShaderLoader
{
private:
	PixelShader* p_mPixelShader;
public:
	PixelShaderLoader();
	PixelShader* GetShader()override
	{
		return p_mPixelShader;
	}

	HRESULT Load(const char* _name);

	void Uninit()override;
};

class ComputeShaderLoader
	: public ShaderLoader
{
private:
	ComputeShader* p_mComputeShader;

public:
	ComputeShaderLoader();
	ComputeShader* GetShader()override
	{
		return p_mComputeShader;
	}

	HRESULT Load(const char* _name);

	void Uninit();
};