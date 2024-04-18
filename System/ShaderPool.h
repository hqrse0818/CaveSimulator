#pragma once
#include "ShaderLoader.h"
#include <unordered_map>
#include <string>

// シェーダー管理配列
class ShaderPool
{
private:
	static std::unordered_map<std::string, ShaderLoader*> mShadermap;

public:
	static Shader* GetShader(const char* _ShaderName);

	static void Uninit();

	static void Push(const char* _name, ShaderLoader* _Shader);
};

