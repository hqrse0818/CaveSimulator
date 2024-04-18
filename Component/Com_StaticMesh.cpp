#include "Com_StaticMesh.h"
#include "../System/StaticModelPool.h"
#include "../DirectX/renderer.h"
#include "../System/Time.h"
#include "../GameObject/GameObject.h"
#include "../Debug.h"
#include "../System/ShaderPool.h"

#include <iostream>

int Com_StaticMesh::mNumModels = 0;
int Com_StaticMesh::mNumTotalVertices = 0;

using namespace std;
using namespace DirectX::SimpleMath;

void Com_StaticMesh::Draw()
{
	Bind();

	Renderer::SetTopologyTriangleList();

	for (unsigned int m = 0; m < mModelData->p_mAiScene->mNumMeshes; m++)
	{
		aiMesh* p_mesh = mModelData->p_mAiScene->mMeshes[m];

		aiMaterial* material = mModelData->p_mAiScene->mMaterials[p_mesh->mMaterialIndex];

		// �e�N�X�`���ݒ�
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &mModelData->map_mTexture[path.data]);

		// ���_�o�b�t�@�ݒ�
		UINT stride = 0;
		switch (mModelData->mType)
		{
		case STATICMESH_TYPE::Normal:
		{
			// ���̃}�e���A�����g�p����
			if (bUseMaterial)
			{
				Renderer::SetMaterial(mModelData->vec_material[m]);
			}
			else
			{
				MATERIAL mat;
				ZeroMemory(&mat, sizeof(mat));
				mat.AlbedoColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
				mat.TextureEnable = true;
				Renderer::SetMaterial(mat);
			}
			stride = sizeof(VERTEX_STATICMESH);
		}
		break;
		case STATICMESH_TYPE::PBR:
		{
			stride = sizeof(VERTEX_STATICMESH_PBR);
		}
		break;
		case STATICMESH_TYPE::Tangent:
		{
			// ���̃}�e���A�����g�p����
			if (bUseMaterial)
			{
				Renderer::SetMaterial(mModelData->vec_material[m]);
			}
			else
			{
				MATERIAL mat;
				ZeroMemory(&mat, sizeof(mat));
				mat.AlbedoColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
				mat.TextureEnable = true;
				Renderer::SetMaterial(mat);
			}
			stride = sizeof(VERTEX_STATICMESH_TANGENT);
		}
		break;
		default:
			break;
		}

		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &mModelData->pp_mVertexBuffer[m], &stride, &offset);

		// �C���f�b�N�X�o�b�t�@
		Renderer::GetDeviceContext()->IASetIndexBuffer(mModelData->pp_mIndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		//���b�V���̃t�F�[�X���`��
		Renderer::GetDeviceContext()->DrawIndexed(p_mesh->mNumFaces * VertexInFace, 0, 0);
	}
}

void Com_StaticMesh::ShadowDraw()
{
	if (bShadowEnable)
	{
		p_VS = static_cast<VertexShader*>(ShaderPool::GetShader("VS_ShadowMap"));
		p_PS = static_cast<PixelShader*>(ShaderPool::GetShader("PS_ShadowMap"));
		Draw();
		p_VS = static_cast<VertexShader*>(ShaderPool::GetShader(VSName));
		p_PS = static_cast<PixelShader*>(ShaderPool::GetShader(PSName));
	}
}

void Com_StaticMesh::Uninit()
{
	mNumTotalVertices -= mNumVertices;
	mNumModels--;
}

bool Com_StaticMesh::SetModelData(const char* _ModelName)
{
	mModelData = StaticModelPool::GetModelData(_ModelName);
	mModelName = _ModelName;

	if (mModelData)
	{
		NEWDEBUGLOG("���f���擾����");

		mNumVertices = StaticModelPool::GetLoaderData(_ModelName)->GetVertexNum();
		mNumTotalVertices += mNumVertices;

		return true;
	}
	NEWDEBUGLOG("���f���擾���s");
	return false;
}

