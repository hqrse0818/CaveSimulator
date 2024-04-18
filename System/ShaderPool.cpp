#include "ShaderPool.h"

std::unordered_map<std::string, ShaderLoader*> ShaderPool::mShadermap{};

Shader* ShaderPool::GetShader(const char* _ShaderName)
{
	if (mShadermap.count(_ShaderName) > 0)
	{
		return mShadermap[_ShaderName]->GetShader();
	}
	return nullptr;
}

void ShaderPool::Uninit()
{
	for (auto data : mShadermap)
	{
		if (data.second)
		{
			data.second->Uninit();
			delete data.second;
		}
	}
	mShadermap.clear();
}

void ShaderPool::Push(const char* _name, ShaderLoader* _Shader)
{
	if (mShadermap.count(_name) > 0)
	{
		return;
	}
	mShadermap[_name] = _Shader;
}

