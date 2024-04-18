#pragma once
#include "../Utility.h"

class GameObject;
class Com_Sprite;
class RenderTarget;
class DepthStencil;


// レンダーターゲットの切り替え、クリア、ディファードの結果を格納するスプライトを持つ
class DynamicEnvironmentMap
{
private:
	GameObject* p_mSpriteObj;
	Com_Sprite* p_mSpriteBuf;

	RenderTarget* p_mRTVEnv;

	RenderTarget* p_mRTVNormal;
	RenderTarget* p_mRTVAlbedo;
	RenderTarget* p_mRTVWorld;
	RenderTarget* p_mRTVMaterial;
	DepthStencil* p_mDSV;

public:
	DynamicEnvironmentMap(const char* _ObjectName, float _SpriteWidth, float _SpriteHeight);

	void InitObject();
	HRESULT InitRender(UINT _width, UINT _height, bool _useStencil);
	void SetPosition(float _x, float _y, float _z = 0.0f);
	void SetGBuffer();
	void ExcuteRendering(ID3D11ShaderResourceView* _CubeMap);

	void Uninit();

	RenderTarget* GetDefferedTexture();
	RenderTarget* GetNormalRTV();
	RenderTarget* GetAlbedoRTV();
	RenderTarget* GetWorldRTV();
	RenderTarget* GetMaterialRTV();

};

