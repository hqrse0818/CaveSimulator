#pragma once
#include "../Format.h"
#include <vector>

constexpr unsigned int MaxPointLightNum = 1024;
constexpr unsigned int MaxSpotLightNum = 1000;

struct PointLights
{
	PointLight lights[MaxPointLightNum];
};

struct SpotLights
{
	SpotLight lights[MaxSpotLightNum];
};


class LightPool
{
private:
	// 平行光源
	static DirectionalLight* pDirectionLight;
	// ポイントライトのリスト
	static std::vector<PointLight*> mPointLightList;
	// スポットライトのリスト
	static std::vector<SpotLight*> mSpotLightList;

	static ID3D11Buffer* p_mDirectionalLightBuf;
	static ID3D11Buffer* p_mPointLightsBuf;
	static ID3D11Buffer* p_mSpotLightsBuf;

public:
	// ポイントライトの定数バッファ作成
	static HRESULT CreatePointLightBuffer();
	// ポイントライトの追加
	static void Push(PointLight* _light);
	// ポイントライト更新
	static void UpdatePointLights();
	// ポイントライトの定数バッファ登録
	static void SetPointLightsBuffer(int _slot);
	// ポイントライトのみの削除
	static void ReleasePointLight();

	// スポットライトの定数バッファ作成
	static HRESULT CreateSpotLightBuffer();
	// スポットライトの追加
	static void Push(SpotLight* _light);
	// スポットライト更新
	static void UpdateSpotLights();
	// スポットライトの定数バッファ登録
	static void SetSpotLightBuffer(int _slot);
	// スポットライトのみ削除
	static void ReleaseSpotLight();

	static void UpdateAllLights();
	// 平行光源作成
	static HRESULT CreateDirectionalLight();
	// 平行光源取得
	static DirectionalLight* GetDirectioLight();
	// 平行光源更新
	static void UpdateDirectionalLight();
	// 平行光源の定数バッファ登録
	static void SetDirectionalLightBuffer();

	// 解放
	static void Uninit();

	static void ShowLightGui();

	static void SaveLight(const char* _FileName);
};

