#include "../main.h"
#include "renderer.h"
#include "../Utility.h"
#include "../Debug.h"
#include "DirectViewList.h"
#include "../System/LightPool.h"
#include "../System/MaterialPool.h"
#include <io.h>

using namespace DirectX::SimpleMath;


D3D_FEATURE_LEVEL       Renderer::mFeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*           Renderer::p_mDevice{};
ID3D11DeviceContext*    Renderer::p_mDeviceContext{};
IDXGISwapChain*         Renderer::p_mSwapChain{};
// �܂Ƃߌ�s��̒萔�o�b�t�@�f�[�^
ID3D11Buffer* Renderer::p_mMatrixBuffer{};
MatrixBufferData Renderer::mMatrixData{};

ID3D11Buffer*			Renderer::p_mMaterialBuffer{};


ID3D11DepthStencilState* Renderer::p_mDepthState[DEPTH_MAX]{};

ID3D11RasterizerState* Renderer::p_mRasterState[3]{};

ID3D11BlendState*		Renderer::p_mBlendState[BLEND_MAX]{};

Application*			Renderer::p_mApplication;

ID3D11RenderTargetView* Renderer::p_mRTVs[MaxMRT]{};


void Renderer::Init(Application* ap)
{
	HRESULT hr = S_OK;

	// ������I�u�W�F�N�g�̃A�h���X���Z�b�g
	p_mApplication = ap;

	// �f�o�C�X�A�X���b�v�`�F�[���쐬
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = p_mApplication->GetWidth();
	swapChainDesc.BufferDesc.Height = p_mApplication->GetHeight();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = p_mApplication->GetWindow();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&p_mSwapChain,
		&p_mDevice,
		&mFeatureLevel,
		&p_mDeviceContext);

	//===============//
	/*		�����_�[�^�[�Q�b�g		*/
	//===============//
	RenderTarget* pRTVBack = new RenderTarget();

	// �o�b�N�o�b�t�@
	hr = pRTVBack->CreateFromScreen();
	if (FAILED(hr))
	{
		NEWDEBUGLOG("�o�b�N�o�b�t�@�쐬���s");
	}
	else
	{
		DirectViewList::PushRTV("BackBuffer", pRTVBack);
	}

	//--------
	// GBuffer
	//--------

	// �@���}�b�v
	RenderTarget* pRTVNormal = new RenderTarget();
	hr = pRTVNormal->Create(DXGI_FORMAT_R16G16B16A16_FLOAT, ap->GetWidth(), ap->GetHeight());
	if (FAILED(hr))
	{
		NEWDEBUGLOG("RTV�@���}�b�v�쐬���s");
	}
	else
	{
		DirectViewList::PushRTV("RTVNormal", pRTVNormal);
	}

	// �A���x�h�}�b�v
	RenderTarget* pRTVAlbed = new RenderTarget();
	hr = pRTVAlbed->Create(DXGI_FORMAT_R8G8B8A8_UNORM, ap->GetWidth(), ap->GetHeight());
	if (FAILED(hr))
	{
		NEWDEBUGLOG("RTV�A���x�h�}�b�v�쐬���s");
	}
	else
	{
		DirectViewList::PushRTV("RTVAlbedo", pRTVAlbed);
	}

	// ���[���h���W�}�b�v
	RenderTarget* pRTVWorldPos = new RenderTarget();
	hr = pRTVWorldPos->Create(DXGI_FORMAT_R32G32B32A32_FLOAT, ap->GetWidth(), ap->GetHeight());
	if (FAILED(hr))
	{
		NEWDEBUGLOG("RTV���[���h�}�b�v�쐬���s");
	}
	else
	{
		DirectViewList::PushRTV("RTVWorld", pRTVWorldPos);
	}

	// �}�e���A��ID�����_�[�^�[�Q�b�g
	RenderTarget* pRTVMaterial = new RenderTarget();
	hr = pRTVMaterial->Create(DXGI_FORMAT_R16_UNORM, ap->GetWidth(), ap->GetHeight());
	if (FAILED(hr))
	{
		NEWDEBUGLOG("RTV�}�e���A��ID�}�b�v�쐬���s");
	}
	else
	{
		DirectViewList::PushRTV("RTVMaterial", pRTVMaterial);
	}

	// �X�N���[�����W
	RenderTarget* pRTVScreen = new RenderTarget();
	hr = pRTVScreen->Create(DXGI_FORMAT_R32G32B32A32_FLOAT, ap->GetWidth(), ap->GetHeight());
	if (FAILED(hr))
	{
		NEWDEBUGLOG("�X�N���[�����W�}�b�v�쐻���s");
	}
	else
	{
		DirectViewList::PushRTV("RTVScreen", pRTVScreen);
	}

	//RenderTarget* pRTVVertex = new RenderTarget();
	//hr = pRTVVertex->Create(DXGI_FORMAT_R32G32B32A32_FLOAT, ap->GetWidth(), ap->GetHeight())

	// �V���h�E�}�b�v
	RenderTarget* pRTVShadowMap = new RenderTarget();
	hr = pRTVShadowMap->Create(DXGI_FORMAT_R32_FLOAT, ap->GetWidth(), ap->GetHeight());
	if(FAILED(hr))
	{
		NEWDEBUGLOG("�V���h�E�}�b�v�쐻���s");
	}
	else
	{
		DirectViewList::PushRTV("RTVShadow", pRTVShadowMap);
	}

	//------------------
	// �f�B�t�@�[�h�pRTV
	//------------------
	RenderTarget* pDeffered = new RenderTarget();
	hr = pDeffered->Create(DXGI_FORMAT_R32G32B32A32_FLOAT, ap->GetWidth(), ap->GetHeight());
	if (FAILED(hr))
	{
		NEWDEBUGLOG("�f�B�t�@�[�h�����_�[�^�[�Q�b�g�쐬���s");
	}
	else
	{
		DirectViewList::PushRTV("RTVDeffered", pDeffered);
	}

	RenderTarget* pLightMap = new RenderTarget();
	hr = pLightMap->Create(DXGI_FORMAT_R16G16B16A16_FLOAT, ap->GetWidth(), ap->GetHeight());
	if (FAILED(hr))
	{
		NEWDEBUGLOG("���C�g�}�b�v�쐬���s");
	}
	else
	{
		DirectViewList::PushRTV("RTVLightMap", pLightMap);
	}

	//-----------------------
	// �f�v�X�X�e���V���r���[
	//-----------------------

	// �W���f�v�X�X�e���V���쐬
	DepthStencil* pDSV = new DepthStencil();
	hr = pDSV->Create(ap->GetWidth(), ap->GetHeight(), false);
	if (FAILED(hr))
	{
		NEWDEBUGLOG("�W��DSV�쐬���s");
	}
	else
	{
		DirectViewList::PushDSV("DSV", pDSV);
	}
	
	// �����_�[�^�[�Q�b�g�̐ݒ�
	SetRenderTargets(1, &pRTVBack, pDSV);

	// ���X�^���C�U�X�e�[�g�ݒ�
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID; 
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE; 

	// �����J�����O
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	p_mDevice->CreateRasterizerState( &rasterizerDesc, &p_mRasterState[0]);
	// �\���J�����O
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	p_mDevice->CreateRasterizerState(&rasterizerDesc, &p_mRasterState[1]);
	// �J�����O�Ȃ�
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	p_mDevice->CreateRasterizerState(&rasterizerDesc, &p_mRasterState[2]);
	
	// �����J�����O��W���ݒ�
	p_mDeviceContext->RSSetState( p_mRasterState[0]);




	// �u�����h�X�e�[�g�ݒ�
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3D11_BLEND blend[BlendMode::BLEND_MAX][2] =
	{
		{D3D11_BLEND_ONE, D3D11_BLEND_ZERO},
		{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA},
		{D3D11_BLEND_ONE, D3D11_BLEND_ONE},
		{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE},
		{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA},
		{D3D11_BLEND_ZERO, D3D11_BLEND_INV_SRC_COLOR},
		{D3D11_BLEND_INV_DEST_COLOR, D3D11_BLEND_ONE},
	};

	for (int i = 0; i < BLEND_MAX; i++)
	{
		if (static_cast<BlendMode>(i) == BLEND_ATC)
		{
			blendDesc.AlphaToCoverageEnable = TRUE;
		}
		else
		{
			blendDesc.AlphaToCoverageEnable = FALSE;
		}
		blendDesc.RenderTarget[0].SrcBlend = blend[i][0];
		blendDesc.RenderTarget[0].DestBlend = blend[i][1];
		p_mDevice->CreateBlendState(&blendDesc, &p_mBlendState[i]);
	}

	float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	p_mDeviceContext->OMSetBlendState(p_mBlendState[1], blendFactor, 0xffffffff);





	// �f�v�X�X�e���V���X�e�[�g�ݒ�
	// https://tositeru.github.io/ImasaraDX11/part/ZBuffer-and-depth-stencil
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
	dsDesc.BackFace = dsDesc.FrontFace;
	bool enablePattern[] = { true, true, false };
	D3D11_DEPTH_WRITE_MASK maskPattern[] = {
		D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_DEPTH_WRITE_MASK_ZERO,
		D3D11_DEPTH_WRITE_MASK_ZERO,
	};
	for (int i = 0; i < DEPTH_MAX; ++i)
	{
		dsDesc.DepthEnable = dsDesc.StencilEnable = enablePattern[i];
		dsDesc.DepthWriteMask = maskPattern[i];
		hr = p_mDevice->CreateDepthStencilState(&dsDesc, &p_mDepthState[i]);
		if (FAILED(hr))
		{
			NEWDEBUGLOG(i << "�[�x�X�e���V���X�e�[�g�쐬���s")
		}
	}
	SetDepthTest(DEPTH_ENABLE_WRITE_TEST);

	//D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	//depthStencilDesc.DepthEnable = TRUE;
	//depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	//depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	//depthStencilDesc.StencilEnable = FALSE;

	//p_mDevice->CreateDepthStencilState( &depthStencilDesc, &p_mDepthStateEnable );//�[�x�L���X�e�[�g

	////depthStencilDesc.DepthEnable = FALSE;
	//depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	//p_mDevice->CreateDepthStencilState( &depthStencilDesc, &p_mDepthStateDisable );//�[�x�����X�e�[�g

	//p_mDeviceContext->OMSetDepthStencilState( p_mDepthStateEnable, NULL );




	// �T���v���[�X�e�[�g�ݒ�
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	// ���b�v���[�h�̃T���v���[�X�e�[�g
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	
	ID3D11SamplerState* samplerState{};
	p_mDevice->CreateSamplerState(&samplerDesc, &samplerState);

	p_mDeviceContext->PSSetSamplers(0, 1, &samplerState);

	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	p_mDevice->CreateSamplerState(&samplerDesc, &samplerState);

	p_mDeviceContext->PSSetSamplers(1, 1, &samplerState);

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 6;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	p_mDevice->CreateSamplerState(&samplerDesc, &samplerState);
	p_mDeviceContext->PSSetSamplers(2, 1, &samplerState);

	// �o�b�t�@�̃��W�X�^
	const int MatrixRegister = 0;
	const int MaterialRegister = 1;
	const int PointLightRegister = 4;
	const int SpotLightRegister = 5;
	const int PBRMaterialRegister = 6;


	// �萔�o�b�t�@����
	hr = CreateConstantBuffer(p_mDevice, sizeof(MatrixBufferData), sizeof(float), &p_mMatrixBuffer);
	if (SUCCEEDED(hr))
	{
		p_mDeviceContext->VSSetConstantBuffers(MatrixRegister, 1, &p_mMatrixBuffer);
		p_mDeviceContext->PSSetConstantBuffers(MatrixRegister, 1, &p_mMatrixBuffer);
	}
	else
	{
		NEWDEBUGLOG("�}�g���b�N�X�o�b�t�@�쐬���s");
		return;
	}
	hr = CreateConstantBuffer(p_mDevice, sizeof(MATERIAL), sizeof(float), &p_mMaterialBuffer);
	if (SUCCEEDED(hr))
	{
		p_mDeviceContext->VSSetConstantBuffers(MaterialRegister, 1, &p_mMaterialBuffer);
		p_mDeviceContext->PSSetConstantBuffers(MaterialRegister, 1, &p_mMaterialBuffer);
	}
	else
	{
		NEWDEBUGLOG("�}�e���A���o�b�t�@�쐬���s");
		return;
	}
	
	hr = LightPool::CreateDirectionalLight();
	if (SUCCEEDED(hr))
	{
		LightPool::SetDirectionalLightBuffer();
	}
	else
	{
		return;
	}

	hr = LightPool::CreatePointLightBuffer();
	if (SUCCEEDED(hr))
	{
		LightPool::SetPointLightsBuffer(PointLightRegister);
	}
	else
	{
		NEWDEBUGLOG("�|�C���g���C�g�萔�o�b�t�@�쐬���s");
		return;
	}

	hr = LightPool::CreateSpotLightBuffer();
	if (SUCCEEDED(hr))
	{
		LightPool::SetSpotLightBuffer(SpotLightRegister);
	}
	else
	{
		NEWDEBUGLOG("�X�|�b�g���C�g�萔�o�b�t�@�쐬���s")
	}
	hr = MaterialPool::CreateMaterialBuffer();
	if (SUCCEEDED(hr))
	{
		MaterialPool::SetMaterialBuffer(PBRMaterialRegister);
	}
	else
	{
		NEWDEBUGLOG("PBR�}�e���A���o�b�t�@�쐬���s");
	}

	// ���s����������(�V�[�����Ƃɕۑ��f�[�^��ǂݍ���)
	DirectionalLight* light = LightPool::GetDirectioLight();
	light->Enable = true;
	light->Direction = Vector4(0.5f, 1.0f, -0.8f, 0.0f);
	light->Direction.Normalize();
	light->Ambient = Color(0.2f, 0.2f, 0.2f, 1.0f);
	light->Diffuse = Color(0.3f, 0.3f, 0.3f, 1.0f);
	LightPool::UpdateDirectionalLight();



	// �}�e���A��������
	MATERIAL material{};
	material.AlbedoColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

}



void Renderer::Uninit()
{
	SAFE_RELEASE(p_mMatrixBuffer);
	SAFE_RELEASE(p_mMaterialBuffer);
	SAFE_RELEASE(p_mRasterState[2]);
	SAFE_RELEASE(p_mRasterState[1]);
	SAFE_RELEASE(p_mRasterState[0]);
	for (int i = 0; i < DEPTH_MAX; i++)
	{
		SAFE_RELEASE(p_mDepthState[i]);
	}
	for (int i = 0; i < BLEND_MAX; i++)
	{
		SAFE_RELEASE(p_mBlendState[i]);
	}
	

	p_mDeviceContext->ClearState();

	SAFE_RELEASE(p_mSwapChain);
	SAFE_RELEASE(p_mDeviceContext);
	SAFE_RELEASE(p_mDevice);
}




void Renderer::Begin()
{
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	// RTV�o�b�N�o�b�t�@�ƕW����DSV��h��Ԃ�
	DirectViewList::GetRTV("BackBuffer")->Clear(ClearColor);
	DirectViewList::GetDSV("DSV")->Clear();
}



void Renderer::End()
{
	p_mSwapChain->Present( 1, 0 );
}

void Renderer::Render(UINT _VertexCnt, UINT _StartLocation)
{
	p_mDeviceContext->Draw(_VertexCnt, _StartLocation);
}




void Renderer::SetDepthEnable( bool Enable )
{
	/*if( Enable )
		p_mDeviceContext->OMSetDepthStencilState( p_mDepthStateEnable, NULL );
	else
		p_mDeviceContext->OMSetDepthStencilState( p_mDepthStateDisable, NULL );*/

}



void Renderer::SetBlendMode(BlendMode _mode)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	
	p_mDeviceContext->OMSetBlendState(p_mBlendState[_mode], blendFactor, 0xffffffff);
}

void Renderer::SetDepthTest(DepthState state)
{
	p_mDeviceContext->OMSetDepthStencilState(p_mDepthState[state], 0);
}

void Renderer::SetWorldViewProjection2D()
{
	Matrix world;
	world = Matrix::Identity;			// �P�ʍs��ɂ���
	world = world.Transpose();			// �]�u


	mMatrixData.World = Matrix::Identity;
	mMatrixData.World = mMatrixData.World.Transpose();


	Matrix view;
	view = Matrix::Identity;
	view = view.Transpose();

	mMatrixData.View = Matrix::Identity;
	mMatrixData.View = mMatrixData.View.Transpose();


	Matrix projection;
/*
// update �E��n������������n  �iDIRECTXTK�̃��\�b�h�͉E��n�������j 20230511 update by tomoki.suzuki�@
	projection = projection.CreateOrthographicOffCenter(
		static_cast<float>(m_Application->GetWidth() * -0.5f),			// �r���[�{�����[���̍ŏ��w
		static_cast<float>(m_Application->GetWidth() * 0.5f),			// �r���[�{�����[���̍ő�w
		static_cast<float>(m_Application->GetHeight() * 0.5f),			// �r���[�{�����[���̍ŏ��x
		static_cast<float>(m_Application->GetHeight() * -0.5f),			// �r���[�{�����[���̍ő�x
		0.0f,
		1000.0f);
*/

/*
	// update �E��n������������n  �iDIRECTXTK�̃��\�b�h�͉E��n�������j 20230511 update by tomoki.suzuki�@
	projection = DirectX::XMMatrixOrthographicOffCenterLH(
		static_cast<float>(m_Application->GetWidth() * -0.5f),			// �r���[�{�����[���̍ŏ��w
		static_cast<float>(m_Application->GetWidth() * 0.5f),			// �r���[�{�����[���̍ő�w
		static_cast<float>(m_Application->GetHeight() * 0.5f),			// �r���[�{�����[���̍ŏ��x
		static_cast<float>(m_Application->GetHeight() * -0.5f),			// �r���[�{�����[���̍ő�x
		0.0f,
		1000.0f);
*/

	// 2D�`������㌴�_�ɂ���
	projection = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f,
		static_cast<float>(p_mApplication->GetWidth()),					// �r���[�{�����[���̍ŏ��w
		static_cast<float>(p_mApplication->GetHeight()),					// �r���[�{�����[���̍ŏ��x
		0.0f,															// �r���[�{�����[���̍ő�x
		0.0f,
		1.0f);


	projection = projection.Transpose();

	mMatrixData.Projection = projection;

	p_mDeviceContext->UpdateSubresource(p_mMatrixBuffer, 0, NULL, &mMatrixData, 0, 0);
}


void Renderer::SetWorldMatrix( Matrix* WorldMatrix )
{
	Matrix world;
	world = WorldMatrix->Transpose();					// �]�u

	mMatrixData.World = WorldMatrix->Transpose();	// �܂Ƃߗp

	p_mDeviceContext->UpdateSubresource(p_mMatrixBuffer, 0, NULL, &mMatrixData, 0, 0);
}

void Renderer::SetViewMatrix(Matrix* ViewMatrix )
{
	Matrix view;
	view = ViewMatrix->Transpose();						// �]�u

	mMatrixData.View = ViewMatrix->Transpose();

	p_mDeviceContext->UpdateSubresource(p_mMatrixBuffer, 0, NULL, &mMatrixData, 0, 0);
}

void Renderer::SetProjectionMatrix( Matrix* ProjectionMatrix )
{
	Matrix projection;
	projection = ProjectionMatrix->Transpose();

	mMatrixData.Projection = ProjectionMatrix->Transpose();

	p_mDeviceContext->UpdateSubresource(p_mMatrixBuffer, 0, NULL, &mMatrixData, 0, 0);
}

void Renderer::SetScale(DirectX::SimpleMath::Vector4 _Scale)
{
	mMatrixData.Scale = _Scale;
	p_mDeviceContext->UpdateSubresource(p_mMatrixBuffer, 0, NULL, &mMatrixData, 0, 0);
}

void Renderer::SetCameraPos(DirectX::SimpleMath::Vector3 _Pos)
{
	Vector4 pos = Vector4(_Pos.x, _Pos.y, _Pos.z, 1.0f);
	mMatrixData.CameraPos = pos;
	p_mDeviceContext->UpdateSubresource(p_mMatrixBuffer, 0, NULL, &mMatrixData, 0, 0);
}

const MatrixBufferData Renderer::GetMatrixData()
{
	return mMatrixData;
}



void Renderer::SetMaterial( MATERIAL Material )
{
	p_mDeviceContext->UpdateSubresource( p_mMaterialBuffer, 0, NULL, &Material, 0, 0 );
}

void Renderer::SetTopologyTriangleStrip()
{
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Renderer::SetTopologyTriangleList()
{
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::SetTopologyLineList()
{
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void Renderer::SetTopologyLineStrip()
{
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
}

void Renderer::SetCullMode(D3D11_CULL_MODE _mode)
{
	switch (_mode)
	{
	case D3D11_CULL_NONE:
		p_mDeviceContext->RSSetState(p_mRasterState[2]);
		break;
	case D3D11_CULL_FRONT:
		p_mDeviceContext->RSSetState(p_mRasterState[1]);
		break;
	case D3D11_CULL_BACK:
		p_mDeviceContext->RSSetState(p_mRasterState[0]);
		break;
	}
}

void Renderer::SetRenderTargets(UINT num, RenderTarget** ppViews, DepthStencil* pView)
{
	// �}���`�p�X�����_�����O���
	if (num > 8)num = 8;
	for (unsigned int i = 0; i < num; i++)
	{
		p_mRTVs[i] = ppViews[i]->GetView();
	}

	// ���ۂ̃Z�b�g
	p_mDeviceContext->OMSetRenderTargets(num, p_mRTVs, pView ? pView->GetView() :  nullptr);

	// �r���[�|�[�g�̍Đݒ�
	D3D11_VIEWPORT viewport;
	UINT viewportnum = 1;
	p_mDeviceContext->RSGetViewports(&viewportnum, &viewport);
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (float)ppViews[0]->GetWidth();
	viewport.Height = (float)ppViews[0]->GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	p_mDeviceContext->RSSetViewports(1, &viewport);
}

void Renderer::SetRenderTargets(UINT num, ID3D11RenderTargetView** ppViews, ID3D11DepthStencilView* pView, float _width, float _height)
{
	if (num > 8)num = 8;
	for (unsigned int i = 0; i < num; i++)
	{
		p_mRTVs[i] = ppViews[i];
	}
	p_mDeviceContext->OMSetRenderTargets(num, p_mRTVs, pView ? pView : nullptr);

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = _width;
	viewport.Height = _height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	p_mDeviceContext->RSSetViewports(1, &viewport);
}

void Renderer::SetRenderTargetsNull()
{
	for (unsigned int i = 0; i < 8; i++)
	{
		p_mRTVs[i] = NULL;
	}
	p_mDeviceContext->OMSetRenderTargets(8, p_mRTVs, nullptr);

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = 1.0f;
	viewport.Height = 1.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	p_mDeviceContext->RSSetViewports(1, &viewport);
}

HRESULT Renderer::CreateBufferVX(unsigned int _size, D3D11_USAGE _usage, D3D11_BIND_FLAG _flg, D3D11_CPU_ACCESS_FLAG _cflg, ID3D11Buffer& _ret)
{
	D3D11_BUFFER_DESC bd{};
	bd.Usage = _usage;
	bd.ByteWidth = _size;
	bd.BindFlags = _flg;
	bd.CPUAccessFlags = _cflg;



	return E_NOTIMPL;
}
