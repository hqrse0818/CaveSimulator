#pragma once

#include	<d3d11.h>
#include	<SimpleMath.h>
#include "../System/Application.h"
#include	"../Format.h"
#include "ShaderResourceView.h"
#include <vector>

constexpr int MaxMRT = 8;

// �u�����h���[�h
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

// �[�x�X�e�[�g
enum DepthState
{
	DEPTH_ENABLE_WRITE_TEST,
	DEPTH_ENABLE_TEST,
	DEPTH_DISABLE,
	DEPTH_MAX
};


// �����_���[
class Renderer
{
private:

	static D3D_FEATURE_LEVEL       mFeatureLevel;

	static ID3D11Device*           p_mDevice;
	static ID3D11DeviceContext*    p_mDeviceContext;
	static IDXGISwapChain*         p_mSwapChain;

	// �܂Ƃߐ�
	static ID3D11Buffer* p_mMatrixBuffer;
	static MatrixBufferData mMatrixData;

	// �}�e���A���p�萔�o�b�t�@
	static ID3D11Buffer*			p_mMaterialBuffer;

	// �f�v�X�X�e���V���X�e�[�g
	static ID3D11DepthStencilState* p_mDepthState[DEPTH_MAX];

	// ���X�^�[���C�U�X�e�[�g
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
	// �u�����h���[�h�ݒ�
	static void SetBlendMode(BlendMode _mode);
	// �[�x�X�e���V���e�X�g�ݒ�
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

	// �g�|���W�ݒ�
	static void SetTopologyTriangleStrip();
	static void SetTopologyTriangleList();
	static void SetTopologyLineList();
	static void SetTopologyLineStrip();

	// �J�����O���[�h�ݒ�
	static void SetCullMode(D3D11_CULL_MODE _mode);

	static void SetRenderTargets(UINT num, RenderTarget** ppViews, DepthStencil* pView);
	static void SetRenderTargets(UINT num, ID3D11RenderTargetView** ppViews, ID3D11DepthStencilView* pView, float _width, float _height);
	static void SetRenderTargetsNull();

	// ���_�o�b�t�@�쐬
	static HRESULT CreateBufferVX(unsigned int _size, D3D11_USAGE _usage, D3D11_BIND_FLAG _flg, D3D11_CPU_ACCESS_FLAG _cflg, ID3D11Buffer& _ret);
};
