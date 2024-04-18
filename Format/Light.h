#pragma once
#include <SimpleMath.h>

class DirectionalLight
{
public:
	BOOL Enable;
	DirectX::SimpleMath::Vector3 mPosition;
	DirectX::SimpleMath::Vector4	Direction;
	// x,y,z カラー w ライトの強さ
	DirectX::SimpleMath::Color	Diffuse;
	// 環境光
	DirectX::SimpleMath::Color	Ambient;
	DirectX::SimpleMath::Matrix WorldMatrix;
	DirectX::SimpleMath::Matrix ViewMatrix;
	DirectX::SimpleMath::Matrix ProjectionMatrix;
	DirectX::SimpleMath::Color BaseColor;
	int PointLightNum;
	int SpotLightNum;
	float dummy[2];
	
public:

	DirectX::SimpleMath::Matrix WorldIdentity();
	// ワールド変換行列の計算
	DirectX::SimpleMath::Matrix CalcWorldMatrix();
	// ビュー変換行列の計算
	DirectX::SimpleMath::Matrix CalcView();
	// プロジェクション変換行列作成
	DirectX::SimpleMath::Matrix CalcProjection(float fovx, float fovy, float clipnear, float clipfar);
};

class SpotLight
{
public:
	DirectX::SimpleMath::Color Diffuse;
	DirectX::SimpleMath::Vector3 Position;
	float range;
	DirectX::SimpleMath::Vector3 Direction;
	float angle;
};

class PointLight
{
public:
	// x,y,z 位置情報 w 影響範囲
	DirectX::SimpleMath::Vector4 Position;
	// x,y,z カラー w ライトの強さ
	DirectX::SimpleMath::Color Diffuse;
};


// 動くポイントライト
class CustomPointLight
{
private:
	// 配列に格納する実際のデータ
	PointLight* pLight;

public:
	// 範囲関連
	float mInitRange = 0;
	float mMaxRange = 0;
	float mMinRange = 0;
	float mGrowSpeed = 0;

	// ポジション関連
	float mAngleXZ = 0;
	float mAngleY = 0;
	float mAngleSpeedXZ = 0;
	float mAngleSpeedY = 0;
	float mLength = 0;

	bool bReturn = false;
	DirectX::SimpleMath::Vector3  AxisPos = DirectX::SimpleMath::Vector3(0.0f,0.0f, 0.0f);

public:
	CustomPointLight();
	void SetColor(float r, float g, float b, float a);
	void InitPointLight();
	void UpdatePointLight();

	PointLight* GetLight()
	{
		return pLight;
	}
};