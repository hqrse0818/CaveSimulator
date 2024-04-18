#include "Scene_ShaderLoad.h"
#include "../System/ShaderPool.h"
#include "../System/ShaderLoader.h"
#include "../Scene/Scene_ModelLoad.h"
#include "../System/manager.h"
#include "Scene_Title.h"
#include "../DirectX/Geometory.h"

void LoadSpriteShader()
{
	// 通常スプライトVS
	VertexShaderLoader* vsSprite = new VertexShaderLoader();
	vsSprite->Load("asset\\cso\\VS_DefaultSprite.cso", Vertex_Type::Sprite);
	vsSprite->Push("VS_SPRITE");
	// ディファード用スプライトVS
	vsSprite = new VertexShaderLoader();
	vsSprite->Load("asset\\cso\\VS_Defferd.cso", Vertex_Type::Sprite);
	vsSprite->Push("VS_Defferd");

	// 通常スプライトPS
	PixelShaderLoader* psSprite = new PixelShaderLoader();
	psSprite->Load("asset\\cso\\PS_DefaultSprite.cso");
	psSprite->Push("PS_SPRITE");
	// FXAA
	psSprite = new PixelShaderLoader();
	psSprite->Load("asset\\cso\\PS_FXAA.cso");
	psSprite->Push("PS_FXAA");
}


void LoadModelShader()
{
	// VS_GBuffer(Static)
	VertexShaderLoader* vsModel = new VertexShaderLoader();
	vsModel->Load("asset\\cso\\VS_GBuffer.cso", Vertex_Type::StaticMesh);
	vsModel->Push("VS_GBuffer");
	// VS_GBuffer(Skeletal)
	vsModel = new VertexShaderLoader();
	vsModel->Load("asset\\cso\\VS_GBufferAnim.cso", Vertex_Type::SkeletalMesh);
	vsModel->Push("VS_GBufferAnim");
	// VS_ShadowMap(Static)
	vsModel = new VertexShaderLoader();
	vsModel->Load("asset\\cso\\VS_ShadowMap.cso", Vertex_Type::StaticMesh);
	vsModel->Push("VS_ShadowMap");
	// VS_ShadowMap(Skeletal)
	vsModel = new VertexShaderLoader();
	vsModel->Load("asset\\cso\\VS_ShadowMapAnim.cso", Vertex_Type::SkeletalMesh);
	vsModel->Push("VS_ShadowMapAnim");
	// VS_Shadow
	vsModel = new VertexShaderLoader();
	vsModel->Load("asset\\cso\\VS_Shadow.cso", Vertex_Type::StaticMesh);
	vsModel->Push("VS_Shadow");

	// PS_ShadowMap
	PixelShaderLoader* psModel = new PixelShaderLoader();
	psModel->Load("asset\\cso\\PS_ShadowMap.cso");
	psModel->Push("PS_ShadowMap");
	// PS_Shadow
	psModel = new PixelShaderLoader();
	psModel->Load("asset\\cso\\PS_Shadow.cso");
	psModel->Push("PS_Shadow");
}

void LoadHitLine()
{
	PixelShaderLoader* Line = new PixelShaderLoader();
	Line->Load("asset\\cso\\HitLine.cso");
	Line->Push("Hit");
	Line = new PixelShaderLoader();
	Line->Load("asset\\cso\\NoHitLine.cso");
	Line->Push("NoHit");
}

void LoadPBRShader()
{
	// GBufferPBR
	VertexShaderLoader* vsModel = new VertexShaderLoader();
	vsModel->Load("asset\\cso\\VS_GBufferPBR.cso", Vertex_Type::StaticMeshPBR);
	vsModel->Push("VS_GBufferPBR");

	vsModel = new VertexShaderLoader();
	vsModel->Load("asset\\cso\\VS_GBufferPBRAnim.cso", Vertex_Type::SkeletalMeshPBR);
	vsModel->Push("VS_GBufferPBRAnim");

	PixelShaderLoader* psModel = new PixelShaderLoader();
	psModel->Load("asset\\cso\\PS_GBufferPBR.cso");
	psModel->Push("PS_GBufferPBR");

	psModel = new PixelShaderLoader();
	psModel->Load("asset\\cso\\PS_GBufferSampleNormal.cso");
	psModel->Push("PS_GBufferPBRNormal");

	psModel = new PixelShaderLoader();
	psModel->Load("asset\\cso\\PS_DisneyBasePBR.cso");
	psModel->Push("PS_DisneyPBR");

	psModel = new PixelShaderLoader();
	psModel->Load("asset\\cso\\PS_ToonBasePBR.cso");
	psModel->Push("PS_ToonPBR");
}

void Scene_ShaderLoad::PreInit()
{
	LoadSpriteShader();
	//LoadModelShader();
	LoadPBRShader();

	mSceneName = "SceneShaderLoad";
}

void Scene_ShaderLoad::Update()
{
	Manager::SetNextScene<Scene_ModelLoad>();
}
