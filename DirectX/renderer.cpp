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
// まとめ後行列の定数バッファデータ
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

	// 持ち主オブジェクトのアドレスをセット
	p_mApplication = ap;

	// デバイス、スワップチェーン作成
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
	/*		レンダーターゲット		*/
	//===============//
	RenderTarget* pRTVBack = new RenderTarget();

	// バックバッファ
	hr = pRTVBack->CreateFromScreen();
	if (FAILED(hr))
	{
		NEWDEBUGLOG("バックバッファ作成失敗");
	}
	else
	{
		DirectViewList::PushRTV("BackBuffer", pRTVBack);
	}

	//--------
	// GBuffer
	//--------

	// 法線マップ
	RenderTarget* pRTVNormal = new RenderTarget();
	hr = pRTVNormal->Create(DXGI_FORMAT_R16G16B16A16_FLOAT, ap->GetWidth(), ap->GetHeight());
	if (FAILED(hr))
	{
		NEWDEBUGLOG("RTV法線マップ作成失敗");
	}
	else
	{
		DirectViewList::PushRTV("RTVNormal", pRTVNormal);
	}

	// アルベドマップ
	RenderTarget* pRTVAlbed = new RenderTarget();
	hr = pRTVAlbed->Create(DXGI_FORMAT_R8G8B8A8_UNORM, ap->GetWidth(), ap->GetHeight());
	if (FAILED(hr))
	{
		NEWDEBUGLOG("RTVアルベドマップ作成失敗");
	}
	else
	{
		DirectViewList::PushRTV("RTVAlbedo", pRTVAlbed);
	}

	// ワールド座標マップ
	RenderTarget* pRTVWorldPos = new RenderTarget();
	hr = pRTVWorldPos->Create(DXGI_FORMAT_R32G32B32A32_FLOAT, ap->GetWidth(), ap->GetHeight());
	if (FAILED(hr))
	{
		NEWDEBUGLOG("RTVワールドマップ作成失敗");
	}
	else
	{
		DirectViewList::PushRTV("RTVWorld", pRTVWorldPos);
	}

	// マテリアルIDレンダーターゲット
	RenderTarget* pRTVMaterial = new RenderTarget();
	hr = pRTVMaterial->Create(DXGI_FORMAT_R16_UNORM, ap->GetWidth(), ap->GetHeight());
	if (FAILED(hr))
	{
		NEWDEBUGLOG("RTVマテリアルIDマップ作成失敗");
	}
	else
	{
		DirectViewList::PushRTV("RTVMaterial", pRTVMaterial);
	}

	// スクリーン座標
	RenderTarget* pRTVScreen = new RenderTarget();
	hr = pRTVScreen->Create(DXGI_FORMAT_R32G32B32A32_FLOAT, ap->GetWidth(), ap->GetHeight());
	if (FAILED(hr))
	{
		NEWDEBUGLOG("スクリーン座標マップ作製失敗");
	}
	else
	{
		DirectViewList::PushRTV("RTVScreen", pRTVScreen);
	}

	//RenderTarget* pRTVVertex = new RenderTarget();
	//hr = pRTVVertex->Create(DXGI_FORMAT_R32G32B32A32_FLOAT, ap->GetWidth(), ap->GetHeight())

	// シャドウマップ
	RenderTarget* pRTVShadowMap = new RenderTarget();
	hr = pRTVShadowMap->Create(DXGI_FORMAT_R32_FLOAT, ap->GetWidth(), ap->GetHeight());
	if(FAILED(hr))
	{
		NEWDEBUGLOG("シャドウマップ作製失敗");
	}
	else
	{
		DirectViewList::PushRTV("RTVShadow", pRTVShadowMap);
	}

	//------------------
	// ディファード用RTV
	//------------------
	RenderTarget* pDeffered = new RenderTarget();
	hr = pDeffered->Create(DXGI_FORMAT_R32G32B32A32_FLOAT, ap->GetWidth(), ap->GetHeight());
	if (FAILED(hr))
	{
		NEWDEBUGLOG("ディファードレンダーターゲット作成失敗");
	}
	else
	{
		DirectViewList::PushRTV("RTVDeffered", pDeffered);
	}

	RenderTarget* pLightMap = new RenderTarget();
	hr = pLightMap->Create(DXGI_FORMAT_R16G16B16A16_FLOAT, ap->GetWidth(), ap->GetHeight());
	if (FAILED(hr))
	{
		NEWDEBUGLOG("ライトマップ作成失敗");
	}
	else
	{
		DirectViewList::PushRTV("RTVLightMap", pLightMap);
	}

	//-----------------------
	// デプスステンシルビュー
	//-----------------------

	// 標準デプスステンシル作成
	DepthStencil* pDSV = new DepthStencil();
	hr = pDSV->Create(ap->GetWidth(), ap->GetHeight(), false);
	if (FAILED(hr))
	{
		NEWDEBUGLOG("標準DSV作成失敗");
	}
	else
	{
		DirectViewList::PushDSV("DSV", pDSV);
	}
	
	// レンダーターゲットの設定
	SetRenderTargets(1, &pRTVBack, pDSV);

	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID; 
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE; 

	// 裏側カリング
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	p_mDevice->CreateRasterizerState( &rasterizerDesc, &p_mRasterState[0]);
	// 表側カリング
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	p_mDevice->CreateRasterizerState(&rasterizerDesc, &p_mRasterState[1]);
	// カリングなし
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	p_mDevice->CreateRasterizerState(&rasterizerDesc, &p_mRasterState[2]);
	
	// 裏側カリングを標準設定
	p_mDeviceContext->RSSetState( p_mRasterState[0]);




	// ブレンドステート設定
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





	// デプスステンシルステート設定
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
			NEWDEBUGLOG(i << "深度ステンシルステート作成失敗")
		}
	}
	SetDepthTest(DEPTH_ENABLE_WRITE_TEST);

	//D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	//depthStencilDesc.DepthEnable = TRUE;
	//depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	//depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	//depthStencilDesc.StencilEnable = FALSE;

	//p_mDevice->CreateDepthStencilState( &depthStencilDesc, &p_mDepthStateEnable );//深度有効ステート

	////depthStencilDesc.DepthEnable = FALSE;
	//depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	//p_mDevice->CreateDepthStencilState( &depthStencilDesc, &p_mDepthStateDisable );//深度無効ステート

	//p_mDeviceContext->OMSetDepthStencilState( p_mDepthStateEnable, NULL );




	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	// ラップモードのサンプラーステート
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

	// バッファのレジスタ
	const int MatrixRegister = 0;
	const int MaterialRegister = 1;
	const int PointLightRegister = 4;
	const int SpotLightRegister = 5;
	const int PBRMaterialRegister = 6;


	// 定数バッファ生成
	hr = CreateConstantBuffer(p_mDevice, sizeof(MatrixBufferData), sizeof(float), &p_mMatrixBuffer);
	if (SUCCEEDED(hr))
	{
		p_mDeviceContext->VSSetConstantBuffers(MatrixRegister, 1, &p_mMatrixBuffer);
		p_mDeviceContext->PSSetConstantBuffers(MatrixRegister, 1, &p_mMatrixBuffer);
	}
	else
	{
		NEWDEBUGLOG("マトリックスバッファ作成失敗");
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
		NEWDEBUGLOG("マテリアルバッファ作成失敗");
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
		NEWDEBUGLOG("ポイントライト定数バッファ作成失敗");
		return;
	}

	hr = LightPool::CreateSpotLightBuffer();
	if (SUCCEEDED(hr))
	{
		LightPool::SetSpotLightBuffer(SpotLightRegister);
	}
	else
	{
		NEWDEBUGLOG("スポットライト定数バッファ作成失敗")
	}
	hr = MaterialPool::CreateMaterialBuffer();
	if (SUCCEEDED(hr))
	{
		MaterialPool::SetMaterialBuffer(PBRMaterialRegister);
	}
	else
	{
		NEWDEBUGLOG("PBRマテリアルバッファ作成失敗");
	}

	// 平行光源初期化(シーンごとに保存データを読み込む)
	DirectionalLight* light = LightPool::GetDirectioLight();
	light->Enable = true;
	light->Direction = Vector4(0.5f, 1.0f, -0.8f, 0.0f);
	light->Direction.Normalize();
	light->Ambient = Color(0.2f, 0.2f, 0.2f, 1.0f);
	light->Diffuse = Color(0.3f, 0.3f, 0.3f, 1.0f);
	LightPool::UpdateDirectionalLight();



	// マテリアル初期化
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
	// RTVバックバッファと標準のDSVを塗りつぶす
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
	world = Matrix::Identity;			// 単位行列にする
	world = world.Transpose();			// 転置


	mMatrixData.World = Matrix::Identity;
	mMatrixData.World = mMatrixData.World.Transpose();


	Matrix view;
	view = Matrix::Identity;
	view = view.Transpose();

	mMatrixData.View = Matrix::Identity;
	mMatrixData.View = mMatrixData.View.Transpose();


	Matrix projection;
/*
// update 右手系＝＝＝＞左手系  （DIRECTXTKのメソッドは右手系だった） 20230511 update by tomoki.suzuki　
	projection = projection.CreateOrthographicOffCenter(
		static_cast<float>(m_Application->GetWidth() * -0.5f),			// ビューボリュームの最小Ｘ
		static_cast<float>(m_Application->GetWidth() * 0.5f),			// ビューボリュームの最大Ｘ
		static_cast<float>(m_Application->GetHeight() * 0.5f),			// ビューボリュームの最小Ｙ
		static_cast<float>(m_Application->GetHeight() * -0.5f),			// ビューボリュームの最大Ｙ
		0.0f,
		1000.0f);
*/

/*
	// update 右手系＝＝＝＞左手系  （DIRECTXTKのメソッドは右手系だった） 20230511 update by tomoki.suzuki　
	projection = DirectX::XMMatrixOrthographicOffCenterLH(
		static_cast<float>(m_Application->GetWidth() * -0.5f),			// ビューボリュームの最小Ｘ
		static_cast<float>(m_Application->GetWidth() * 0.5f),			// ビューボリュームの最大Ｘ
		static_cast<float>(m_Application->GetHeight() * 0.5f),			// ビューボリュームの最小Ｙ
		static_cast<float>(m_Application->GetHeight() * -0.5f),			// ビューボリュームの最大Ｙ
		0.0f,
		1000.0f);
*/

	// 2D描画を左上原点にする
	projection = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f,
		static_cast<float>(p_mApplication->GetWidth()),					// ビューボリュームの最小Ｘ
		static_cast<float>(p_mApplication->GetHeight()),					// ビューボリュームの最小Ｙ
		0.0f,															// ビューボリュームの最大Ｙ
		0.0f,
		1.0f);


	projection = projection.Transpose();

	mMatrixData.Projection = projection;

	p_mDeviceContext->UpdateSubresource(p_mMatrixBuffer, 0, NULL, &mMatrixData, 0, 0);
}


void Renderer::SetWorldMatrix( Matrix* WorldMatrix )
{
	Matrix world;
	world = WorldMatrix->Transpose();					// 転置

	mMatrixData.World = WorldMatrix->Transpose();	// まとめ用

	p_mDeviceContext->UpdateSubresource(p_mMatrixBuffer, 0, NULL, &mMatrixData, 0, 0);
}

void Renderer::SetViewMatrix(Matrix* ViewMatrix )
{
	Matrix view;
	view = ViewMatrix->Transpose();						// 転置

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
	// マルチパスレンダリング上限
	if (num > 8)num = 8;
	for (unsigned int i = 0; i < num; i++)
	{
		p_mRTVs[i] = ppViews[i]->GetView();
	}

	// 実際のセット
	p_mDeviceContext->OMSetRenderTargets(num, p_mRTVs, pView ? pView->GetView() :  nullptr);

	// ビューポートの再設定
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
