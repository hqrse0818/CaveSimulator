#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

// InputLayoutのタイプ
enum Vertex_Type
{
	Sprite = 0,
	StaticMesh = 1,
	SkeletalMesh = 2,
	Vector = 3,
	Geometory = 4,
	StaticMeshPBR = 5,
	SkeletalMeshPBR = 6,
	BillBoardPBR = 7,
};

struct VERTEX_SPRITE
{
	DirectX::SimpleMath::Vector2 Position;
	DirectX::SimpleMath::Color Diffuse;
	DirectX::SimpleMath::Vector2 TexCoord;
};

struct VERTEX_STATICMESH
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Color Diffuse;
	DirectX::SimpleMath::Vector2 TexCoord;
};

struct VERTEX_STATICMESH_PBR
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Color Diffuse;
	DirectX::SimpleMath::Vector2 TexCoord;
	int MaterialID[4];
};

struct VERTEX_STATICMESH_TANGENT
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Vector3 Tangent;
	DirectX::SimpleMath::Vector3 BiTangent;
	DirectX::SimpleMath::Color Diffuse;
	DirectX::SimpleMath::Vector2 TexCoord;
};

struct VERTEX_SKELETALMESH
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Color	Diffuse;
	DirectX::SimpleMath::Vector2 TexCoord;
	int BoneIndex[4];
	float BoneWeight[4];
};

struct VERTEX_SKELETALMESH_PBR
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Color	Diffuse;
	DirectX::SimpleMath::Vector2 TexCoord;
	int MaterialID[4];
	int BoneIndex[4];
	float BoneWeight[4];
};

// ３Ｄ頂点データ
struct VERTEX_3D
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Color	Diffuse;
	DirectX::SimpleMath::Vector2 TexCoord;
};

struct VERTEX_GEOMETORY
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Color Diffuse;
};

struct VERTEX_BILLBOARD
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Color Diffuse;
	DirectX::SimpleMath::Vector2 TexCoord;
};