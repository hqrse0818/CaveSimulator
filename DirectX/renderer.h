#pragma once

#include	<d3d11.h>
#include	<SimpleMath.h>
#include "../System/Application.h"
#include	"../Format.h"
#include "ShaderResourceView.h"
#include <vector>

constexpr int MaxMRT = 8;

// ブレンドモード
enum BlendMode
{
	BLEND_NONE,
	BLEND_ALPHA,
	BLEND_ADD,
	BLEND_ADDALPHA,
	BLEND_ATC,
	BLEND_SUB,
	BLEND_SCREEN,
	BLEND_MAX
};

// 深度ステート
enum DepthState
{
	DEPTH_ENABLE_WRITE_TEST,
	DEPTH_ENABLE_TEST,
	DEPTH_DISABLE,
	DEPTH_MAX
};


// レンダラー
class Renderer
{
private:

	static D3D_FEATURE_LEVEL       mFeatureLevel;

	static ID3D11Device*           p_mDevice;
	static ID3D11DeviceContext*    p_mDeviceContext;
	static IDXGISwapChain*         p_mSwapChain;

	// まとめ先
	static ID3D11Buffer* p_mMatrixBuffer;
	static MatrixBufferData mMatrixData;

	// マテリアル用定数バッファ
	static ID3D11Buffer*			p_mMaterialBuffer;

	// デプスステンシルステート
	static ID3D11DepthStencilState* p_mDepthState[DEPTH_MAX];

	// ラスターライザステート
	static ID3D11RasterizerState* p_mRasterState[3];

	static ID3D11BlendState*		p_mBlendState[BLEND_MAX];

	static Application* p_mApplication;

	static ID3D11RenderTargetView* p_mRTVs[8];

public:
	static void Init(Application* ap);
	static void Uninit();
	static void Begin();
	static void End();
	static void Render(UINT _VertexCnt, UINT _StartLocation);

	static void SetDepthEnable(bool Enable);
	// ブレンドモード設定
	static void SetBlendMode(BlendMode _mode);
	// 深度ステンシルテスト設定
	static void SetDepthTest(DepthState state);

	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(DirectX::SimpleMath::Matrix* WorldMatrix);
	static void SetViewMatrix(DirectX::SimpleMath::Matrix* ViewMatrix);
	static void SetProjectionMatrix(DirectX::SimpleMath::Matrix* ProjectionMatrix);
	static void SetScale(DirectX::SimpleMath::Vector4 _Scale);
	static void SetCameraPos(DirectX::SimpleMath::Vector3 _Pos);
	const static MatrixBufferData GetMatrixData();
	static void SetMaterial(MATERIAL Material);

	static ID3D11Device* GetDevice( void ){ return p_mDevice; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return p_mDeviceContext; }
	static IDXGISwapChain* GetSwapChain() { return p_mSwapChain; }

	// トポロジ設定
	static void SetTopologyTriangleStrip();
	static void SetTopologyTriangleList();
	static void SetTopologyLineList();
	static void SetTopologyLineStrip();

	// カリングモード設定
	static void SetCullMode(D3D11_CULL_MODE _mode);

	static void SetRenderTargets(UINT num, RenderTarget** ppViews, DepthStencil* pView);
	static void SetRenderTargets(UINT num, ID3D11RenderTargetView** ppViews, ID3D11DepthStencilView* pView, float _width, float _height);
	static void SetRenderTargetsNull();

	// 頂点バッファ作成
	static HRESULT CreateBufferVX(unsigned int _size, D3D11_USAGE _usage, D3D11_BIND_FLAG _flg, D3D11_CPU_ACCESS_FLAG _cflg, ID3D11Buffer& _ret);
};
