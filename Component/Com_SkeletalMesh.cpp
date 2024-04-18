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
	case Com_SkeletalMesh::AnimationState::Stop:// アニメーション停止中
		break;
	case Com_SkeletalMesh::AnimationState::Play:// アニメーション再生中
		UpdateAnimation();
		break;
	case Com_SkeletalMesh::AnimationState::Blend:// ブレンド中
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

	// レジスタに定数バッファをセット
	Renderer::GetDeviceContext()->VSSetConstantBuffers(BoneBufferRegister, 1, &p_mCombBuffer);

	for (unsigned int m = 0; m < mModelData->p_mAiScene->mNumMeshes; m++)
	{
		aiMesh* p_mesh = mModelData->p_mAiScene->mMeshes[m];

		// 元のマテリアルを使用する
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

		// テクスチャ設定
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &mModelData->map_mTexture[path.data]);

		// 頂点バッファ設定
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

		// インデックスバッファ
		Renderer::GetDeviceContext()->IASetIndexBuffer(mModelData->pp_mIndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		//メッシュのフェース分描画
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

		NEWDEBUGLOG("モデル取得成功");
		// 定数バッファを作成
		D3D11_BUFFER_DESC bd{};
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(BoneCompMatrix);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


		HRESULT hr = Renderer::GetDevice()->CreateBuffer(&bd, nullptr, &p_mCombBuffer);
		if (FAILED(hr))
		{
			NEWDEBUGLOG("ボーンコンビネーション行列バッファ作成失敗");
			return false;
		}
		assert(p_mCombBuffer);

		// ボーンのコンビネーション行列の配列を生成
		std::vector<aiMatrix4x4> BoneCombContainer;
		BoneCombContainer.resize(mModelData->map_mBone.size());
		for (auto data : mModelData->map_mBone)
		{
			BoneCombContainer[data.second.mIndex] = data.second.Matrix;
		}

		// 転置
		for (auto& mtx : BoneCombContainer)
		{
			mtx.Transpose();
		}

		// 定数バッファ更新
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
			NEWDEBUGLOG("ボーン定数バッファ作成失敗");
		}
	}

	NEWDEBUGLOG("モデルの取得に失敗 : モデル名 " << _ModelName);
	return false;
}

void Com_SkeletalMesh::PlayAnimation(const char* _name, bool _loop)
{
	// アニメーション名が登録されているかチェック
	if (CheckRegisterdAnimation(_name))
	{
		// アニメーションを所持しているかチェック
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
	// 再生中のアニメーションがあるかチェック
	if (p_cPlayAnimationName == "")
	{
		return PlayAnimation(_name, _loop);
	}
	else
	{
		// ブレンドするアニメーション名が登録されているかチェック
		if (CheckRegisterdAnimation(_name))
		{
			// ブレンドするアニメーションがあるか確認
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
	// 引数で渡されたノード名をキーとしてボーン情報を取得
	BONE* bone = &mModelData->map_mBone[_Node->mName.C_Str()];

	// マトリクスの乗算順番に注意する
	aiMatrix4x4 worldMatrix;

	// 親の姿勢と位置を初期状態にする
	worldMatrix = _Matrix;
	// 引数で渡された行列を掛ける
	worldMatrix *= bone->AnimationMatrix;

	// プログラム内に用意している行列に反映
	bone->Matrix = worldMatrix;
	// オフセット行列を反映
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
	// アニメーションを取得
	aiAnimation* animation = mModelData->map_mAnimation[p_cPlayAnimationName].pAnimation->mAnimations[0];

	for (unsigned int i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim* nodeAnim = animation->mChannels[i];

		BONE* bone = &mModelData->map_mBone[nodeAnim->mNodeName.C_Str()];

		int f;
		// フレームに対するアニメーションの回転を取得
		f = iCurrentAnimFrame % nodeAnim->mNumRotationKeys;
		if (f == iLastKey - 1)
		{
			bLastAnimKey = true;
		}

		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		// フレームに対するポジションの位置を取得
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
	// ボーン行列の更新
	aiMatrix4x4 rootMatrix = aiMatrix4x4(
		aiVector3D(1.0f, 1.0f, 1.0f),
		aiQuaternion(static_cast<ai_real>(AI_MATH_PI), 0.0f, 0.0f),
		aiVector3D(0.0f, 0.0f, 0.0f));

	// 再帰的に処理
	UpdateBoneMatrix(mModelData->p_mAiScene->mRootNode, rootMatrix);

	// ボーンのコンビネーション行列の配列を生成
	std::vector<aiMatrix4x4> BoneCombContainer;
	BoneCombContainer.resize(mModelData->map_mBone.size());
	for (auto data : mModelData->map_mBone)
	{
		BoneCombContainer[data.second.mIndex] = data.second.Matrix;
	}

	// 転置
	for (auto& mtx : BoneCombContainer)
	{
		mtx.Transpose();
	}

	// 定数バッファ更新
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
	// アニメーションデータからボーンマトリクス算出
	aiAnimation* AnimCurrent = mModelData->map_mAnimation[p_cPlayAnimationName].pAnimation->mAnimations[0];
	aiAnimation* AnimBlend = mModelData->map_mAnimation[p_cBlendAnimationName].pAnimation->mAnimations[0];

	// 現在再生中のアニメーションに関連するボーンの更新
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

	// ブレンドするアニメーションに関連するボーンの更新
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

	// ブレンド処理
	for (unsigned int c = 0; c < AnimBlend->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = AnimBlend->mChannels[c];

		BONE* bone = &mModelData->map_mBone[nodeAnim->mNodeName.C_Str()];

		// 位置をブレンド
		aiVector3D posCur = bone->BlendPosFrom;
		aiVector3D posBle = bone->BlendPosTo;

		// 線形補間
		aiVector3D resultPos = posCur * (1.0f - fBlendRate) + posCur * fBlendRate;

		// 姿勢のブレンド
		aiQuaternion rotCur = bone->BlendRotFrom;
		aiQuaternion rotBle = bone->BlendRotTo;

		// 球面線形補間
		aiQuaternion resultRot;
		aiQuaternion::Interpolate(resultRot, rotCur, rotBle, fBlendRate);

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), resultRot, resultPos);
	}

	// ボーン行列の更新
	aiMatrix4x4 rootMatrix = aiMatrix4x4(
		aiVector3D(1.0f, 1.0f, 1.0f),
		aiQuaternion(static_cast<ai_real>(AI_MATH_PI), 0.0f, 0.0f),
		aiVector3D(0.0f, 0.0f, 0.0f));

	// 再帰的に処理
	UpdateBoneMatrix(mModelData->p_mAiScene->mRootNode, rootMatrix);

	// ボーンのコンビネーション行列の配列を生成
	std::vector<aiMatrix4x4> BoneCombContainer;
	BoneCombContainer.resize(mModelData->map_mBone.size());
	for (auto data : mModelData->map_mBone)
	{
		BoneCombContainer[data.second.mIndex] = data.second.Matrix;
	}

	// 転置
	for (auto& mtx : BoneCombContainer)
	{
		mtx.Transpose();
	}

	// 定数バッファ更新
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
