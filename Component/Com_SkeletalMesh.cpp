#include "Com_SkeletalMesh.h"
#include "../System/SkeletalModelPool.h"
#include "../DirectX/renderer.h"
#include "../System/Time.h"
#include "../GameObject/GameObject.h"
#include "../Debug.h"
#include "../System/ShaderPool.h"

#include <iostream>

int Com_SkeletalMesh::mNumModels = 0;
int Com_SkeletalMesh::mNumTotalVertices = 0;

using namespace std;
using namespace DirectX::SimpleMath;

Com_SkeletalMesh::Com_SkeletalMesh()
{
	mNumModels++;
}

void Com_SkeletalMesh::Update()
{
	bLastAnimKey = false;

	switch (mState)
	{
	case Com_SkeletalMesh::AnimationState::Stop:// �A�j���[�V������~��
		break;
	case Com_SkeletalMesh::AnimationState::Play:// �A�j���[�V�����Đ���
		UpdateAnimation();
		break;
	case Com_SkeletalMesh::AnimationState::Blend:// �u�����h��
		UpdateBlend();
		break;
	default:
		break;
	}
}

void Com_SkeletalMesh::Draw()
{
	Bind();
	//Time->CountStart();
	Renderer::SetTopologyTriangleList();

	// ���W�X�^�ɒ萔�o�b�t�@���Z�b�g
	Renderer::GetDeviceContext()->VSSetConstantBuffers(BoneBufferRegister, 1, &p_mCombBuffer);

	for (unsigned int m = 0; m < mModelData->p_mAiScene->mNumMeshes; m++)
	{
		aiMesh* p_mesh = mModelData->p_mAiScene->mMeshes[m];

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
			Renderer::SetMaterial(mat);
		}

		aiMaterial* material = mModelData->p_mAiScene->mMaterials[p_mesh->mMaterialIndex];

		// �e�N�X�`���ݒ�
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &mModelData->map_mTexture[path.data]);

		// ���_�o�b�t�@�ݒ�
		UINT stride = 0;
		switch (mModelData->mType)
		{
		case SKELETALMESH_TYPE::Normal:
			stride = sizeof(VERTEX_SKELETALMESH);
			break;
		case SKELETALMESH_TYPE::PBR:
			stride = sizeof(VERTEX_SKELETALMESH_PBR);
			break;
		case SKELETALMESH_TYPE::Tangent:
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
	//float f = Time->CountStop();
	//DEBUG_LOG("Time : " << f << "sec");
}

void Com_SkeletalMesh::ShadowDraw()
{
	if (bShadowEnable)
	{
		p_VS = static_cast<VertexShader*>(ShaderPool::GetShader("VS_ShadowMapAnim"));
		p_PS = static_cast<PixelShader*>(ShaderPool::GetShader("PS_ShadowMap"));
		Draw();
		p_VS = static_cast<VertexShader*>(ShaderPool::GetShader(VSName));
		p_PS = static_cast<PixelShader*>(ShaderPool::GetShader(PSName));
	}
}

void Com_SkeletalMesh::Uninit()
{
	Com_Mesh::Uninit();
	if (p_mCombBuffer)
	{
		p_mCombBuffer->Release();
		p_mCombBuffer = nullptr;
	}

	mNumTotalVertices -= mNumVertices;
	mNumModels--;
}


bool Com_SkeletalMesh::SetModelData(const char* _ModelName)
{
	mModelData = SkeletalModelPool::GetModelData(_ModelName);
	mModelName = _ModelName;

	if (mModelData)
	{
		mNumVertices = SkeletalModelPool::GetLoaderData(_ModelName)->GetVertexNum();
		mNumTotalVertices += mNumVertices;

		NEWDEBUGLOG("���f���擾����");
		// �萔�o�b�t�@���쐬
		D3D11_BUFFER_DESC bd{};
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(BoneCompMatrix);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


		HRESULT hr = Renderer::GetDevice()->CreateBuffer(&bd, nullptr, &p_mCombBuffer);
		if (FAILED(hr))
		{
			NEWDEBUGLOG("�{�[���R���r�l�[�V�����s��o�b�t�@�쐬���s");
			return false;
		}
		assert(p_mCombBuffer);

		// �{�[���̃R���r�l�[�V�����s��̔z��𐶐�
		std::vector<aiMatrix4x4> BoneCombContainer;
		BoneCombContainer.resize(mModelData->map_mBone.size());
		for (auto data : mModelData->map_mBone)
		{
			BoneCombContainer[data.second.mIndex] = data.second.Matrix;
		}

		// �]�u
		for (auto& mtx : BoneCombContainer)
		{
			mtx.Transpose();
		}

		// �萔�o�b�t�@�X�V
		D3D11_MAPPED_SUBRESOURCE ms;
		BoneCompMatrix* pData = nullptr;

		hr = Renderer::GetDeviceContext()->Map(
			p_mCombBuffer,
			0, D3D11_MAP_WRITE_DISCARD,
			0, &ms);
		if (SUCCEEDED(hr))
		{
			pData = (BoneCompMatrix*)ms.pData;
			memcpy(pData->BoneCombMtx,
				BoneCombContainer.data(),
				sizeof(aiMatrix4x4) * BoneCombContainer.size());
			Renderer::GetDeviceContext()->Unmap(p_mCombBuffer, 0);
			return true;
		}
		else
		{
			NEWDEBUGLOG("�{�[���萔�o�b�t�@�쐬���s");
		}
	}

	NEWDEBUGLOG("���f���̎擾�Ɏ��s : ���f���� " << _ModelName);
	return false;
}

void Com_SkeletalMesh::PlayAnimation(const char* _name, bool _loop)
{
	// �A�j���[�V���������o�^����Ă��邩�`�F�b�N
	if (CheckRegisterdAnimation(_name))
	{
		// �A�j���[�V�������������Ă��邩�`�F�b�N
		if (!CheckHasAnimation(_name))
		{
			mState = AnimationState::Stop;
			return;
		}
		mState = AnimationState::Play;
		p_cPlayAnimationName = _name;
		iLastKey = mModelData->map_mAnimation[_name].LastKey;
		bLoop = _loop;
	}
	else
	{
		mState = AnimationState::Stop;
	}
}

void Com_SkeletalMesh::BlendAnimation(const char* _name, bool _loop)
{
	// �Đ����̃A�j���[�V���������邩�`�F�b�N
	if (p_cPlayAnimationName == "")
	{
		return PlayAnimation(_name, _loop);
	}
	else
	{
		// �u�����h����A�j���[�V���������o�^����Ă��邩�`�F�b�N
		if (CheckRegisterdAnimation(_name))
		{
			// �u�����h����A�j���[�V���������邩�m�F
			if (!CheckHasAnimation(_name))
			{
				if (mState == AnimationState::Play)
				{
					bLoop = _loop;
				}
				return;
			}
			mState = AnimationState::Blend;
			p_cBlendAnimationName = _name;
			iLastKey = mModelData->map_mAnimation[_name].LastKey;
			bLoop = _loop;
			iBlendAnimFrame = 0;
			fBlendRate = 0.0f;
		}
	}
}

void Com_SkeletalMesh::SetBlendRate(float _val)
{
	fBlendPerFrame = _val;
}

void Com_SkeletalMesh::SetPlayAnimation(bool _val)
{
	if (_val)
	{
		mState = AnimationState::Play;
	}
	else
	{
		mState = AnimationState::Stop;
	}
}

void Com_SkeletalMesh::UpdateCurrentAnimFrame()
{
	fAnimCnt += Time->GetDeltaTime();

	if (fAnimCnt > fAnimSpeed)
	{
		if (bPlayOrder)
		{
			iCurrentAnimFrame++;
		}
		else
		{
			iCurrentAnimFrame--;
		}
		fAnimCnt = 0;
	}
}

void Com_SkeletalMesh::UpdateBlendAnimFrame()
{
	fBlendCnt += Time->GetDeltaTime();
	if (fBlendCnt > fBlendSpeed)
	{
		if (bPlayOrder)
		{
			iBlendAnimFrame++;
		}
		else
		{
			iBlendAnimFrame--;
		}
		fBlendRate += fBlendPerFrame;
		if (fBlendRate > 1.0f)
		{
			mState = AnimationState::Play;
			p_cPlayAnimationName = p_cBlendAnimationName;
			p_cBlendAnimationName = "";
		}
	}
}



void Com_SkeletalMesh::UpdateBoneMatrix(aiNode* _Node, aiMatrix4x4 _Matrix)
{
	// �����œn���ꂽ�m�[�h�����L�[�Ƃ��ă{�[�������擾
	BONE* bone = &mModelData->map_mBone[_Node->mName.C_Str()];

	// �}�g���N�X�̏�Z���Ԃɒ��ӂ���
	aiMatrix4x4 worldMatrix;

	// �e�̎p���ƈʒu��������Ԃɂ���
	worldMatrix = _Matrix;
	// �����œn���ꂽ�s����|����
	worldMatrix *= bone->AnimationMatrix;

	// �v���O�������ɗp�ӂ��Ă���s��ɔ��f
	bone->Matrix = worldMatrix;
	// �I�t�Z�b�g�s��𔽉f
	bone->Matrix *= bone->OffsetMatrix;

	for (unsigned int index = 0; index < _Node->mNumChildren; index++)
	{
		UpdateBoneMatrix(_Node->mChildren[index], worldMatrix);
	}
}

bool Com_SkeletalMesh::CheckRegisterdAnimation(const char* _AnimName)
{
	if (mModelData->map_mAnimation.count(_AnimName) > 0)
	{
		return true;
	}
	return false;
}

bool Com_SkeletalMesh::CheckHasAnimation(const char* _AnimName)
{
	if (mModelData->map_mAnimation[_AnimName].pAnimation->HasAnimations())
	{
		return true;
	}
	return false;
}

void Com_SkeletalMesh::UpdateAnimation()
{
	// �A�j���[�V�������擾
	aiAnimation* animation = mModelData->map_mAnimation[p_cPlayAnimationName].pAnimation->mAnimations[0];

	for (unsigned int i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim* nodeAnim = animation->mChannels[i];

		BONE* bone = &mModelData->map_mBone[nodeAnim->mNodeName.C_Str()];

		int f;
		// �t���[���ɑ΂���A�j���[�V�����̉�]���擾
		f = iCurrentAnimFrame % nodeAnim->mNumRotationKeys;
		if (f == iLastKey - 1)
		{
			bLastAnimKey = true;
		}

		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		// �t���[���ɑ΂���|�W�V�����̈ʒu���擾
		f = iCurrentAnimFrame % nodeAnim->mNumPositionKeys;
		if (f == iLastKey - 1)
		{
			bLastAnimKey = true;
		}

		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->AnimationMatrix =
			aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f),
				rot, pos);
	}


	if (bLastAnimKey)
	{
		if (!bLoop)
		{
			mState = AnimationState::Stop;
		}
		iCurrentAnimFrame = 0;
	}
	// �{�[���s��̍X�V
	aiMatrix4x4 rootMatrix = aiMatrix4x4(
		aiVector3D(1.0f, 1.0f, 1.0f),
		aiQuaternion(static_cast<ai_real>(AI_MATH_PI), 0.0f, 0.0f),
		aiVector3D(0.0f, 0.0f, 0.0f));

	// �ċA�I�ɏ���
	UpdateBoneMatrix(mModelData->p_mAiScene->mRootNode, rootMatrix);

	// �{�[���̃R���r�l�[�V�����s��̔z��𐶐�
	std::vector<aiMatrix4x4> BoneCombContainer;
	BoneCombContainer.resize(mModelData->map_mBone.size());
	for (auto data : mModelData->map_mBone)
	{
		BoneCombContainer[data.second.mIndex] = data.second.Matrix;
	}

	// �]�u
	for (auto& mtx : BoneCombContainer)
	{
		mtx.Transpose();
	}

	// �萔�o�b�t�@�X�V
	D3D11_MAPPED_SUBRESOURCE ms;
	BoneCompMatrix* pData = nullptr;

	HRESULT hr = Renderer::GetDeviceContext()->Map(
		p_mCombBuffer,
		0, D3D11_MAP_WRITE_DISCARD,
		0, &ms);
	if (SUCCEEDED(hr))
	{
		pData = (BoneCompMatrix*)ms.pData;
		memcpy(pData->BoneCombMtx,
			BoneCombContainer.data(),
			sizeof(aiMatrix4x4) * BoneCombContainer.size());
		Renderer::GetDeviceContext()->Unmap(p_mCombBuffer, 0);
	}

	UpdateCurrentAnimFrame();
}

void Com_SkeletalMesh::UpdateBlend()
{
	// �A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	aiAnimation* AnimCurrent = mModelData->map_mAnimation[p_cPlayAnimationName].pAnimation->mAnimations[0];
	aiAnimation* AnimBlend = mModelData->map_mAnimation[p_cBlendAnimationName].pAnimation->mAnimations[0];

	// ���ݍĐ����̃A�j���[�V�����Ɋ֘A����{�[���̍X�V
	for (unsigned int c = 0; c < AnimCurrent->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = AnimCurrent->mChannels[c];

		BONE* bone = &mModelData->map_mBone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = iCurrentAnimFrame & nodeAnim->mNumRotationKeys;
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = iCurrentAnimFrame % nodeAnim->mNumPositionKeys;
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->BlendPosFrom = pos;
		bone->BlendRotFrom = rot;
	}

	// �u�����h����A�j���[�V�����Ɋ֘A����{�[���̍X�V
	for (unsigned int b = 0; b < AnimBlend->mNumChannels; b++)
	{
		aiNodeAnim* nodeAnim = AnimBlend->mChannels[b];

		BONE* bone = &mModelData->map_mBone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = iCurrentAnimFrame & nodeAnim->mNumRotationKeys;
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = iCurrentAnimFrame % nodeAnim->mNumPositionKeys;
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->BlendPosTo = pos;
		bone->BlendRotTo = rot;

	}

	// �u�����h����
	for (unsigned int c = 0; c < AnimBlend->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = AnimBlend->mChannels[c];

		BONE* bone = &mModelData->map_mBone[nodeAnim->mNodeName.C_Str()];

		// �ʒu���u�����h
		aiVector3D posCur = bone->BlendPosFrom;
		aiVector3D posBle = bone->BlendPosTo;

		// ���`���
		aiVector3D resultPos = posCur * (1.0f - fBlendRate) + posCur * fBlendRate;

		// �p���̃u�����h
		aiQuaternion rotCur = bone->BlendRotFrom;
		aiQuaternion rotBle = bone->BlendRotTo;

		// ���ʐ��`���
		aiQuaternion resultRot;
		aiQuaternion::Interpolate(resultRot, rotCur, rotBle, fBlendRate);

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), resultRot, resultPos);
	}

	// �{�[���s��̍X�V
	aiMatrix4x4 rootMatrix = aiMatrix4x4(
		aiVector3D(1.0f, 1.0f, 1.0f),
		aiQuaternion(static_cast<ai_real>(AI_MATH_PI), 0.0f, 0.0f),
		aiVector3D(0.0f, 0.0f, 0.0f));

	// �ċA�I�ɏ���
	UpdateBoneMatrix(mModelData->p_mAiScene->mRootNode, rootMatrix);

	// �{�[���̃R���r�l�[�V�����s��̔z��𐶐�
	std::vector<aiMatrix4x4> BoneCombContainer;
	BoneCombContainer.resize(mModelData->map_mBone.size());
	for (auto data : mModelData->map_mBone)
	{
		BoneCombContainer[data.second.mIndex] = data.second.Matrix;
	}

	// �]�u
	for (auto& mtx : BoneCombContainer)
	{
		mtx.Transpose();
	}

	// �萔�o�b�t�@�X�V
	D3D11_MAPPED_SUBRESOURCE ms;
	BoneCompMatrix* pData = nullptr;

	HRESULT hr = Renderer::GetDeviceContext()->Map(
		p_mCombBuffer,
		0, D3D11_MAP_WRITE_DISCARD,
		0, &ms);
	if (SUCCEEDED(hr))
	{
		pData = (BoneCompMatrix*)ms.pData;
		memcpy(pData->BoneCombMtx,
			BoneCombContainer.data(),
			sizeof(aiMatrix4x4) * BoneCombContainer.size());
		Renderer::GetDeviceContext()->Unmap(p_mCombBuffer, 0);
	}

	//UpdateCurrentAnimFrame();
	UpdateBlendAnimFrame();
}
