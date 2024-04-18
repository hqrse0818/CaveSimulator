#pragma once
#include "../Utility.h"
#include "../DirectX/ShaderResourceView.h"

class GameObject;
class Com_Sprite;

constexpr int CubeMapArraySize = 6;

class CubeMapGenerator
{
public:
	ID3D11ShaderResourceView* GetCubeMap() const
	{
		return pCubeMap;
	};

	// RTVArrayとスプライトオブジェクトの初期化
	void Init(UINT _width, UINT _height);
	// 解放
	void Uninit();
	// 作成
	void Generate(RenderTarget** _texture);

private:
	// レンダーターゲットの幅と高さ
	UINT mWidth, mHeight;

	GameObject* pObject;
	Com_Sprite* pSprite;

	// レンダリング結果の格納先
	ID3D11ShaderResourceView* pCubeMap;
	// arrayのRTV
	ID3D11RenderTargetView* pRTVArray;
	// テクスチャデスクの設定
	D3D11_TEXTURE2D_DESC mDesc;
};

