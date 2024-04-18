#pragma once
#include <d3d11.h>
#include <unordered_map>
#include <string>
#include <WICTextureLoader.h>

// テクスチャ作成クラス
class TextureCreate
{
private:
	static TextureCreate* p_mInstance;
	static CRITICAL_SECTION myCS;
	TextureCreate() {};

public:
	static TextureCreate* GetInstance()
	{
		if (p_mInstance == nullptr)
		{
			p_mInstance = new TextureCreate();
			InitializeCriticalSection(&myCS);
		}
		return p_mInstance;
	}

	static void Uninit()
	{
		if (p_mInstance)
		{
			DeleteCriticalSection(&myCS);
			delete p_mInstance;
		}
	}

	static void CreateTexture(const char* _FileName, ID3D11ShaderResourceView** _pTex);
	static void CreateTexture(const wchar_t* _FileName, ID3D11ShaderResourceView** _pTex);
};