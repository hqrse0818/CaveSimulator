#pragma once
#include <SimpleMath.h>
// マテリアル
struct MATERIAL
{
	// 拡散反射
	DirectX::SimpleMath::Color AlbedoColor;
	// 自家発光成分
	DirectX::SimpleMath::Color	 Emission;
	// テクスチャの有効化
	BOOL		TextureEnable;
	// 金属度
	float		Metalic;
	// 粗さ
	float		Roughness;
	// スペキュラ強度
	float		Shinness;

	MATERIAL()
	{
		AlbedoColor = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);
		Emission = DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f, 0.0f);
		Metalic = 1.0f;
		TextureEnable = true;
		Roughness = 0.0f;
		Shinness = 0.0f;
	}
};

struct PBR_MATERIAL
{
	// 拡散反射光
	DirectX::SimpleMath::Color AlbedoColor;
	// エミッション
	DirectX::SimpleMath::Color Emission;
	// マテリアルID
	unsigned int MaterialID;
	// メタリック
	float Metalic;
	// 粗さ
	float Roughness;
	// マイクロファセット
	float MicroFacet;
	// スペキュラ強度
	float Shinness;
	// 法線マップ強度
	float NormalMap_Intensity;
	// ダミー
	float pad[2];

	PBR_MATERIAL()
	{
		AlbedoColor = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);
		Emission = DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f, 0.0f);
		Metalic = 0.04f;
		Roughness = 0.5f;
		MicroFacet = 0.75f;
		MaterialID = 0;
		Shinness = 1.0f;
		NormalMap_Intensity = 0.0f;
		pad[0] = 0.0f;
		pad[1] = 0.0f;
	}
};




// 定数バッファ用構造体
struct MatrixBufferData
{
	// ワールド変換行列
	DirectX::SimpleMath::Matrix World;
	// ビュー変換行列
	DirectX::SimpleMath::Matrix View;
	// プロジェクション変換行列
	DirectX::SimpleMath::Matrix Projection;
	// スケールデータ
	DirectX::SimpleMath::Vector4 Scale;
	// カメラのポジションデータ
	DirectX::SimpleMath::Vector4 CameraPos;
};