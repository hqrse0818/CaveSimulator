#pragma once
#include "Vertex.h"
#include "ConstantBuffers.h"

#include <d3d11.h>

// objデータ用

// モデルのマテリアル構造体
struct MODEL_MATERIAL
{
	char						name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView* Texture;
};

// 描画サブセット構造体
struct SUBSET
{
	unsigned int				StartIndex;
	unsigned int				IndexNum;
	MODEL_MATERIAL				Material;
};

// モデル構造体
struct MODEL_OBJ
{
	VERTEX_3D* VertexArray;
	unsigned int				VertexNum;

	unsigned int* IndexArray;
	unsigned int				IndexNum;
	SUBSET* SubsetArray;
	unsigned int				SubsetNum;
};

// モデル
struct MODEL
{
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;

	SUBSET* SubsetArray;
	unsigned int				SubsetNum;
};