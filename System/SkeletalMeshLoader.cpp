#include "SkeletalMeshLoader.h"
#include <SimpleMath.h>
#include <DirectXMath.h>
#include<DirectXTex.h>
#include <iostream>
#include "../main.h"
#include "../System/Time.h"
#include "../System/TextureCreate.h"
#include "../Utility.h"
#include "SkeletalModelPool.h"
#include "../Debug.h"
#include "MaterialPool.h"
#include "FileOperation.h"
#include "SolutionCSV.h"

using namespace DirectX::SimpleMath;
using namespace std;

ID3D11ShaderResourceView* SkeletalMeshLoader::LoadDiffuseTexture(std::string _FileName, std::string _ModelFileName)
{
	// �t�@�C���̊g���q���擾
	std::string fileext = GetFileExt(_FileName);

	// �t�@�C����(UTF-16)���擾����
	std::wstring widefilename = GetFileNameWide(_FileName.c_str());

	// ���f���f�[�^�̐e�f�B���N�g�����擾
	std::wstring parentdirectory = GetParentDirectoryWide(_ModelFileName);// +L"";

	// �e�N�X�`���̃p�X���擾
	std::wstring filepath = parentdirectory + widefilename;

	// SRV
	ID3D11ShaderResourceView* srv = nullptr;

	// TGA�H
	if (fileext == ".tga") {
		// TGA�̏ꍇ
		DirectX::TexMetadata meta;
		DirectX::GetMetadataFromTGAFile(filepath.c_str(), meta);

		std::unique_ptr<DirectX::ScratchImage> image(new DirectX::ScratchImage);

		HRESULT hr = LoadFromTGAFile(filepath.c_str(), &meta, *image);
		if (FAILED(hr)) {
			NEWDEBUGLOG("LoadFromTGAFile Error (" << _FileName << ")");
			return nullptr;
		}

		ID3D11Resource* tex = nullptr;

		// �e�N�X�`������
		hr = CreateTexture(
			Renderer::GetDevice(),
			image->GetImages(),
			image->GetImageCount(),
			meta,
			&tex);

		tex->Release();

		if (FAILED(hr)) {
			NEWDEBUGLOG("CreateTexture Error (" << _FileName << ")");
			return nullptr;
		}

		// �V�F�[�_�[���\�[�X�r���[�쐬
		hr = DirectX::CreateShaderResourceView(Renderer::GetDevice(),
			image->GetImages(),
			image->GetImageCount(),
			meta,
			&srv);

		if (FAILED(hr)) {
			WNEWDEBULOG(L"CreateShaderResourceView Error (" << filepath.c_str() << L")");
			return nullptr;
		}
	}
	else {
		// �e�N�X�`���ǂݍ���
		TextureCreate::CreateTexture(filepath.c_str(), &srv);
		if (srv == nullptr) {
			WNEWDEBULOG(L"CreateWICTextureFromFile Error (" << filepath.c_str() << widefilename.c_str() << L")");
			return nullptr;
		}
	}

	return srv;
}

void SkeletalMeshLoader::CreateBone(aiNode* _node)
{
	BONE bone;

	p_mModelData->map_mBone[_node->mName.C_Str()] = bone;

	// �ċA�I�ɌĂяo��
	for (unsigned int index = 0; index < _node->mNumChildren; index++)
	{
		CreateBone(_node->mChildren[index]);
	}
}

SkeletalMeshLoader::SkeletalMeshLoader()
{
	p_mModelData = new SKELETALMESH_DATA();
}

void SkeletalMeshLoader::LoadModel(const char* _FileName, bool _bLoadMaterial)
{
	const std::string modelPath(_FileName);

	Time->CountStart();

	Assimp::Importer importer;

	unsigned int flg = 0;
	//flg |= aiProcess_Triangulate;	// �O�p�`�ɕϊ�
	//flg |= aiProcess_JoinIdenticalVertices;	// ���꒸�_�̌���
	//flg |= aiProcess_PreTransformVertices;	// �S�Ă̒��_���W�����[���h���W�Ƃ��ďo�͂ł��邪�����̏�񂪍폜����邽�߂��܂萄�����Ȃ�
	//flg |= aiProcess_MakeLeftHanded;				// ������W�n�ō��//(UV���W���Ԃ�Ȃ�)
	flg |= aiProcess_ConvertToLeftHanded;	// ������W�n�ɃR���o�[�g����
	//flg |= aiProcess_FlipUVs;								// UV���W�̔��](���Ԃ邯�ǃA�j���[�V����������ƃe�N�X�`������邱�Ƃ�����)
	//flg |= aiProcessPreset_TargetRealtime_Quality;
	flg |= aiProcessPreset_TargetRealtime_MaxQuality;
	//flg |= aiProcess_FindInstances ;
	//flg |= aiProcess_ValidateDataStructure;
	//flg |= aiProcess_OptimizeMeshes;

	p_mModelData->p_mAiScene = aiImportFile(_FileName, flg);

	if (p_mModelData->p_mAiScene == nullptr)
	{
		NEWDEBUGLOG(_FileName << " is Error");
		NEWDEBUGLOG(aiGetErrorString());
	}

	assert(p_mModelData->p_mAiScene);

	// �o�b�t�@�̍쐬
	p_mModelData->pp_mVertexBuffer = new ID3D11Buffer * [p_mModelData->p_mAiScene->mNumMeshes];
	p_mModelData->pp_mIndexBuffer = new ID3D11Buffer * [p_mModelData->p_mAiScene->mNumMeshes];
	p_mModelData->vec_mDeformVertex = new std::vector<DEFORM_VERTEX>[p_mModelData->p_mAiScene->mNumMeshes];

	// �{�[���𐶐�
	CreateBone(p_mModelData->p_mAiScene->mRootNode);

	// �{�[���̔z��ʒu���i�[
	unsigned int num = 0;
	for (auto& bone : p_mModelData->map_mBone)
	{
		bone.second.mIndex = num;
		num++;
	}

	for (unsigned int index = 0; index < p_mModelData->p_mAiScene->mNumMeshes; index++)
	{
		aiMesh* p_mesh = p_mModelData->p_mAiScene->mMeshes[index];

		// ���_�o�b�t�@����
		if (p_mesh != nullptr)
		{
			VERTEX_SKELETALMESH* p_vertex = new VERTEX_SKELETALMESH[p_mesh->mNumVertices];

			for (unsigned int ver = 0; ver < p_mesh->mNumVertices; ver++)
			{
				p_vertex[ver].Position = Vector3(p_mesh->mVertices[ver].x, p_mesh->mVertices[ver].y, p_mesh->mVertices[ver].z);

				if (p_mesh->mNormals != nullptr)
				{
					p_vertex[ver].Normal = Vector3(p_mesh->mNormals[ver].x, p_mesh->mNormals[ver].y, p_mesh->mNormals[ver].z);
				}
				else
				{

				}
				if (p_mesh->mTextureCoords[index])
				{
					p_vertex[ver].TexCoord = Vector2(p_mesh->mTextureCoords[0][ver].x, p_mesh->mTextureCoords[0][ver].y);
				}
				else
				{
					p_vertex[ver].TexCoord = Vector2(0, 1);
				}
				p_vertex[ver].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_SKELETALMESH) * p_mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = p_vertex;

			HRESULT hr = Renderer::GetDevice()->CreateBuffer(&bd, &sd, &p_mModelData->pp_mVertexBuffer[index]);
			if (FAILED(hr))
			{
				NEWDEBUGLOG(_FileName << "���_�o�b�t�@�̍쐬�Ɏ��s : " << index);
			}

			delete[] p_vertex;
		}

		// �C���f�b�N�X�o�b�t�@����
		if (p_mesh != nullptr)
		{
			{
				unsigned int* p_index = new unsigned int[p_mesh->mNumFaces * 3];

				for (unsigned int f = 0; f < p_mesh->mNumFaces; f++)
				{
					const aiFace* p_face = &p_mesh->mFaces[f];

					assert(p_face->mNumIndices == 3);

					p_index[f * 3 + 0] = p_face->mIndices[0];
					p_index[f * 3 + 1] = p_face->mIndices[1];
					p_index[f * 3 + 2] = p_face->mIndices[2];
				}

				D3D11_BUFFER_DESC bd;
				ZeroMemory(&bd, sizeof(bd));
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(unsigned int) * p_mesh->mNumFaces * 3;
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;

				D3D11_SUBRESOURCE_DATA sd;
				ZeroMemory(&sd, sizeof(sd));
				sd.pSysMem = p_index;

				Renderer::GetDevice()->CreateBuffer(&bd, &sd, &p_mModelData->pp_mIndexBuffer[index]);

				delete[] p_index;
			}

			// �ό`�㒸�_�f�[�^������
			for (unsigned int vec = 0; vec < p_mesh->mNumVertices; vec++)
			{
				DEFORM_VERTEX deformVertex;
				deformVertex.Position = p_mesh->mVertices[vec];
				deformVertex.Normal = p_mesh->mNormals[vec];
				deformVertex.BoneNum = 0;

				for (unsigned bone = 0; bone < 4; bone++)
				{
					deformVertex.BoneName[bone] = "";
					deformVertex.BoneWeight[bone] = 0.0f;
				}
				p_mModelData->vec_mDeformVertex[index].emplace_back(deformVertex);
			}


			// �{�[���f�[�^�̏�����
			for (unsigned int bone = 0; bone < p_mesh->mNumBones; bone++)
			{
				aiBone* p_bone = p_mesh->mBones[bone];

				p_mModelData->map_mBone[p_bone->mName.C_Str()].OffsetMatrix = p_bone->mOffsetMatrix;

				// �ό`�㒸�_�Ƀ{�[���f�[�^�i�[
				for (unsigned int w = 0; w < p_bone->mNumWeights; w++)
				{
					aiVertexWeight weight = p_bone->mWeights[w];

					int num = p_mModelData->vec_mDeformVertex[index][weight.mVertexId].BoneNum;

					p_mModelData->vec_mDeformVertex[index][weight.mVertexId].BoneWeight[num] = weight.mWeight;
					p_mModelData->vec_mDeformVertex[index][weight.mVertexId].BoneName[num] = p_bone->mName.C_Str();

					p_mModelData->vec_mDeformVertex[index][weight.mVertexId].BoneIndex[num] = p_mModelData->map_mBone[p_bone->mName.C_Str()].mIndex;

					p_mModelData->vec_mDeformVertex[index][weight.mVertexId].BoneNum++;

					assert(p_mModelData->vec_mDeformVertex[index][weight.mVertexId].BoneNum <= 4);
				}
			}
		}
	}

	// ���_�o�b�t�@�Ƀ{�[���C���f�b�N�X�Əd�ݒl���Z�b�g
	for (unsigned int m = 0; m < p_mModelData->p_mAiScene->mNumMeshes; m++)
	{
		aiMesh* p_mesh = p_mModelData->p_mAiScene->mMeshes[m];

		if (p_mesh != nullptr)
		{
			// ���_�o�b�t�@�����b�N
			D3D11_MAPPED_SUBRESOURCE ms;
			HRESULT hr = Renderer::GetDeviceContext()->Map(p_mModelData->pp_mVertexBuffer[m], 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
			if (SUCCEEDED(hr))
			{
				VERTEX_SKELETALMESH* p_vertex = (VERTEX_SKELETALMESH*)ms.pData;
				for (unsigned int v = 0; v < p_mesh->mNumVertices; v++)
				{
					DEFORM_VERTEX* dvx = &p_mModelData->vec_mDeformVertex[m][v];

					p_vertex->Position = Vector3(p_mesh->mVertices[v].x, p_mesh->mVertices[v].y, p_mesh->mVertices[v].z);
					p_vertex->Normal = Vector3(p_mesh->mNormals[v].x, p_mesh->mNormals[v].y, p_mesh->mNormals[v].z);
					p_vertex->TexCoord = Vector2(p_mesh->mTextureCoords[0][v].x, p_mesh->mTextureCoords[0][v].y);
					p_vertex->Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);

					p_vertex->BoneIndex[0] = dvx->BoneIndex[0];
					p_vertex->BoneIndex[1] = dvx->BoneIndex[1];
					p_vertex->BoneIndex[2] = dvx->BoneIndex[2];
					p_vertex->BoneIndex[3] = dvx->BoneIndex[3];

					p_vertex->BoneWeight[0] = dvx->BoneWeight[0];
					p_vertex->BoneWeight[1] = dvx->BoneWeight[1];
					p_vertex->BoneWeight[2] = dvx->BoneWeight[2];
					p_vertex->BoneWeight[3] = dvx->BoneWeight[3];

					p_vertex++;
				}
				Renderer::GetDeviceContext()->Unmap(p_mModelData->pp_mVertexBuffer[m], 0);
			}
		}
	}

	// �e�N�X�`���̓ǂݍ���
	for (unsigned int i = 0; i < p_mModelData->p_mAiScene->mNumTextures; i++)
	{
		ID3D11ShaderResourceView* p_texture;

		aiTexture* p_aitexture = p_mModelData->p_mAiScene->mTextures[i];

		const size_t size = p_aitexture->mWidth;

		HRESULT hr = DirectX::CreateWICTextureFromMemory(
			Renderer::GetDevice(),
			Renderer::GetDeviceContext(),
			reinterpret_cast<const unsigned char*>(p_aitexture->pcData),
			p_aitexture->mWidth,
			nullptr,
			&p_texture);

		assert(p_texture);

		p_mModelData->map_mTexture[p_aitexture->mFilename.data] = p_texture;
	}

	// �}�e���A���ǂݍ���
	if (_bLoadMaterial)
	{
		for (unsigned int i = 0; i < p_mModelData->p_mAiScene->mNumMaterials; i++)
		{
			aiMaterial* mat = p_mModelData->p_mAiScene->mMaterials[i];
			aiString name = mat->GetName();

			MATERIAL myMat{};

			// �g�U����
			{
				aiColor3D color(0.0f, 0.0f, 0.0f);
				if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
				{
					myMat.AlbedoColor.x = color.r;
					myMat.AlbedoColor.y = color.g;
					myMat.AlbedoColor.z = color.b;
					myMat.AlbedoColor.w = 1.0f;
				}
				else
				{
					myMat.AlbedoColor.x = 0.5f;
					myMat.AlbedoColor.y = 0.5f;
					myMat.AlbedoColor.z = 0.5f;
					myMat.AlbedoColor.w = 1.0f;
				}
			}
			// �����x
			{
				float Metallic = 0.0f;
				if (mat->Get(AI_MATKEY_METALLIC_FACTOR, Metallic) == AI_SUCCESS)
				{
					myMat.Metalic = Metallic;
				}
				else
				{
					myMat.Metalic = 1.0f;
				}
				// ���Ɣ�������
				{
					aiColor3D color(0.0f, 0.0f, 0.0f);
					if (mat->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS)
					{
						myMat.Emission.x = color.r;
						myMat.Emission.y = color.g;
						myMat.Emission.z = color.b;
						myMat.Emission.w = 1.0f;
					}
					else
					{
						myMat.Emission.x = 0.0f;
						myMat.Emission.y = 0.0f;
						myMat.Emission.z = 0.0f;
						myMat.Emission.w = 0.0f;
					}
				}

				// �f�B�t�[�Y�e�N�X�`�����擾
				aiTextureType type = aiTextureType_DIFFUSE;
				int texnum = mat->GetTextureCount(type);

				// 1���b�V����1����������
				assert(texnum <= 1);

				// �}�e���A�������擾
				aiString matName;
				mat->GetTexture(type, 0, &matName);

				ID3D11ShaderResourceView* srv = nullptr;

				// ���݂��Ă��邩�m�F
				if (p_mModelData->map_mTexture.find(matName.data) == p_mModelData->map_mTexture.end())
				{
					myMat.TextureEnable = FALSE;
					srv = nullptr;

					// �t�@�C�����擾
					std::string fileName = GetFileName(matName.C_Str());

					// �e�N�X�`���̓ǂݍ��݂Ǝ擾
					srv = LoadDiffuseTexture(fileName, _FileName);

					if (srv == nullptr)
					{
						myMat.TextureEnable = FALSE;
					}
					else
					{
						myMat.TextureEnable = TRUE;
						p_mModelData->map_mTexture[matName.data] = srv;
					}
				}
				// �}�e���A���̕ۑ�
				p_mModelData->vec_material.emplace_back(myMat);
			}
		}
	}

	float time = Time->CountStop();
	NEWDEBUGLOG(_FileName);
	NEWDEBUGLOG("���f���ǂݍ��݊���");
	NEWDEBUGLOG("Time : " << time << "sec");
}

void SkeletalMeshLoader::LoadModelPBR(const char* _FileName, const char* _TextName, bool _bLoadMaterial)
{
	const std::string modelPath(_FileName);

	// �t�@�C�����̕���
	vector<string> Namelist = SeparateString(_FileName, '\\');
	int EndIndex = Namelist.size() - 1;
	// .fbx���������t�@�C�����̎擾
	string LastName = SeparateString(Namelist[EndIndex], '.')[0];

	Time->CountStart();

	Assimp::Importer importer;

	unsigned int flg = 0;
	//flg |= aiProcess_Triangulate;	// �O�p�`�ɕϊ�
	//flg |= aiProcess_JoinIdenticalVertices;	// ���꒸�_�̌���
	//flg |= aiProcess_PreTransformVertices;	// �S�Ă̒��_���W�����[���h���W�Ƃ��ďo�͂ł��邪�����̏�񂪍폜����邽�߂��܂萄�����Ȃ�
	//flg |= aiProcess_MakeLeftHanded;				// ������W�n�ō��//(UV���W���Ԃ�Ȃ�)
	flg |= aiProcess_ConvertToLeftHanded;	// ������W�n�ɃR���o�[�g����
	//flg |= aiProcess_FlipUVs;								// UV���W�̔��](���Ԃ邯�ǃA�j���[�V����������ƃe�N�X�`������邱�Ƃ�����)
	//flg |= aiProcessPreset_TargetRealtime_Quality;
	flg |= aiProcessPreset_TargetRealtime_MaxQuality;
	//flg |= aiProcess_FindInstances ;
	//flg |= aiProcess_ValidateDataStructure;
	//flg |= aiProcess_OptimizeMeshes;

	p_mModelData->p_mAiScene = aiImportFile(_FileName, flg);

	if (p_mModelData->p_mAiScene == nullptr)
	{
		NEWDEBUGLOG(_FileName << " is Error");
		NEWDEBUGLOG(aiGetErrorString());
	}

	assert(p_mModelData->p_mAiScene);

	// �t�@�C���l�[������t�@�C�����쐬����
	// �}�e���A���̏��������o��
	std::string SecondName = _TextName;
	std::string FirstName = "asset/data/Materials/";
	FirstName = FirstName + SecondName;
	FirstName += ".txt";

	


	// �����o���p�t�@�C���̍쐬or�W�J
	FileOperator* Operator = new FileOperator(FirstName.c_str());

	// �`��^�C�v��PBR�̃X�P���^�����b�V���Ƃ��Ďw��
	p_mModelData->mType = SKELETALMESH_TYPE::PBR;

	// �o�b�t�@�̍쐬
	p_mModelData->pp_mVertexBuffer = new ID3D11Buffer * [p_mModelData->p_mAiScene->mNumMeshes];
	p_mModelData->pp_mIndexBuffer = new ID3D11Buffer * [p_mModelData->p_mAiScene->mNumMeshes];
	p_mModelData->vec_mDeformVertex = new std::vector<DEFORM_VERTEX>[p_mModelData->p_mAiScene->mNumMeshes];

	// �{�[���𐶐�
	CreateBone(p_mModelData->p_mAiScene->mRootNode);

	// �{�[���̔z��ʒu���i�[
	unsigned int num = 0;
	for (auto& bone : p_mModelData->map_mBone)
	{
		bone.second.mIndex = num;
		num++;
	}

	for (unsigned int index = 0; index < p_mModelData->p_mAiScene->mNumMeshes; index++)
	{
		aiMesh* p_mesh = p_mModelData->p_mAiScene->mMeshes[index];

		mNumVertices += p_mesh->mNumVertices;

		// ���_�o�b�t�@����
		if (p_mesh != nullptr)
		{
			VERTEX_SKELETALMESH_PBR* p_vertex = new VERTEX_SKELETALMESH_PBR[p_mesh->mNumVertices];

			for (unsigned int ver = 0; ver < p_mesh->mNumVertices; ver++)
			{
				p_vertex[ver].Position = Vector3(p_mesh->mVertices[ver].x, p_mesh->mVertices[ver].y, p_mesh->mVertices[ver].z);

				if (p_mesh->mNormals != nullptr)
				{
					p_vertex[ver].Normal = Vector3(p_mesh->mNormals[ver].x, p_mesh->mNormals[ver].y, p_mesh->mNormals[ver].z);
				}
				else
				{
					NEWDEBUGLOG(_FileName << " normal is null!");
				}
				if (p_mesh->mTextureCoords[index])
				{
					p_vertex[ver].TexCoord = Vector2(p_mesh->mTextureCoords[0][ver].x, p_mesh->mTextureCoords[0][ver].y);
				}
				else
				{
					NEWDEBUGLOG(_FileName << " TexCoord is null!");
					p_vertex[ver].TexCoord = Vector2(0, 1);
				}
				p_vertex[ver].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);

				// PBR�p�}�e���A���̓ǂݍ��݂ƃ}�e���A��ID�̓o�^
				if (p_mModelData->p_mAiScene->mNumMaterials > 0 && _bLoadMaterial)
				{
					if (IDInfo[p_mesh->mMaterialIndex] > 0)
					{
						p_vertex[ver].MaterialID[0] = IDInfo[p_mesh->mMaterialIndex];
						p_vertex[ver].MaterialID[1] = IDInfo[p_mesh->mMaterialIndex];
						p_vertex[ver].MaterialID[2] = IDInfo[p_mesh->mMaterialIndex];
						p_vertex[ver].MaterialID[3] = IDInfo[p_mesh->mMaterialIndex];
					}
					else
					{
						PBR_MATERIAL* mat = new PBR_MATERIAL();
						aiMaterial* aiMat = p_mModelData->p_mAiScene->mMaterials[p_mesh->mMaterialIndex];
						aiColor4D color;
						// ���ˌ�
						if (aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
						{
							mat->AlbedoColor.x = color.r;
							mat->AlbedoColor.y = color.g;
							mat->AlbedoColor.z = color.b;
							mat->AlbedoColor.w = color.a;
						}
						else
						{
							mat->AlbedoColor.x = 1.0f;
							mat->AlbedoColor.y = 1.0f;
							mat->AlbedoColor.z = 1.0f;
							mat->AlbedoColor.w = 1.0f;
						}
						// �G�~�b�V�����J���[
						aiColor4D emission;
						if (aiMat->Get(AI_MATKEY_COLOR_EMISSIVE, emission) == AI_SUCCESS)
						{
							mat->Emission.x = emission.r;
							mat->Emission.y = emission.g;
							mat->Emission.z = emission.b;
							mat->Emission.w = emission.a;
						}
						else
						{
							mat->Emission = Color(0.0f, 0.0f, 0.0f, 0.0f);
						}
						// �e��
						float roufh;
						if (aiMat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roufh) == AI_SUCCESS)
						{
							mat->Roughness = roufh;
						}
						else
						{
							mat->Roughness = 0.5f;
						}

						// ���^���b�N
						float met;
						if (aiMat->Get(AI_MATKEY_METALLIC_FACTOR, met) == AI_SUCCESS)
						{
							// �Œ�̋����x
							if (met <= 0.0f)
							{
								met = 0.04f;
							}
							mat->Metalic = met;
						}
						else
						{
							mat->Metalic = 0.04f;
						}
						mat->MicroFacet = 0.75f;

						MaterialPool::Push(mat); 

						string PushName = LastName;
						PushName += p_mesh->mName.C_Str();

						MaterialPool::PushName(PushName);
						int id = mat->MaterialID;
						IDInfo[p_mesh->mMaterialIndex] = id;
						p_vertex[ver].MaterialID[0] = id;
						p_vertex[ver].MaterialID[1] = id;
						p_vertex[ver].MaterialID[2] = id;
						p_vertex[ver].MaterialID[3] = id;

						// �e�L�X�g�t�@�C���Ƀf�[�^����������
						Operator->Write(p_mesh->mName.C_Str(), false);
						Operator->Write("Roufhness");
						Operator->Write(mat->Roughness);
						Operator->Write("Metallic");
						Operator->Write(mat->Metalic);
						Operator->Write("MaterialID");
						Operator->Write(mat->MaterialID);
						Operator->NewLine();

					}

				}
				else
				{
					p_vertex[ver].MaterialID[0] = 0;
					p_vertex[ver].MaterialID[1] = 0;
					p_vertex[ver].MaterialID[2] = 0;
					p_vertex[ver].MaterialID[3] = 0;
				}
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_SKELETALMESH_PBR) * p_mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = p_vertex;

			HRESULT hr = Renderer::GetDevice()->CreateBuffer(&bd, &sd, &p_mModelData->pp_mVertexBuffer[index]);
			if (FAILED(hr))
			{
				NEWDEBUGLOG(_FileName << "���_�o�b�t�@�̍쐬�Ɏ��s : " << index);
			}

			delete[] p_vertex;
		}
		Operator->Close();
		delete Operator;

		// �C���f�b�N�X�o�b�t�@����
		if (p_mesh != nullptr)
		{
			{
				unsigned int* p_index = new unsigned int[p_mesh->mNumFaces * 3];

				for (unsigned int f = 0; f < p_mesh->mNumFaces; f++)
				{
					const aiFace* p_face = &p_mesh->mFaces[f];

					assert(p_face->mNumIndices == 3);

					p_index[f * 3 + 0] = p_face->mIndices[0];
					p_index[f * 3 + 1] = p_face->mIndices[1];
					p_index[f * 3 + 2] = p_face->mIndices[2];
				}

				D3D11_BUFFER_DESC bd;
				ZeroMemory(&bd, sizeof(bd));
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(unsigned int) * p_mesh->mNumFaces * 3;
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;

				D3D11_SUBRESOURCE_DATA sd;
				ZeroMemory(&sd, sizeof(sd));
				sd.pSysMem = p_index;

				Renderer::GetDevice()->CreateBuffer(&bd, &sd, &p_mModelData->pp_mIndexBuffer[index]);

				delete[] p_index;
			}

			// �ό`�㒸�_�f�[�^������
			for (unsigned int vec = 0; vec < p_mesh->mNumVertices; vec++)
			{
				DEFORM_VERTEX deformVertex;
				deformVertex.Position = p_mesh->mVertices[vec];
				deformVertex.Normal = p_mesh->mNormals[vec];
				deformVertex.BoneNum = 0;

				for (unsigned bone = 0; bone < 4; bone++)
				{
					deformVertex.BoneName[bone] = "";
					deformVertex.BoneWeight[bone] = 0.0f;
				}
				p_mModelData->vec_mDeformVertex[index].emplace_back(deformVertex);
			}


			// �{�[���f�[�^�̏�����
			for (unsigned int bone = 0; bone < p_mesh->mNumBones; bone++)
			{
				aiBone* p_bone = p_mesh->mBones[bone];

				p_mModelData->map_mBone[p_bone->mName.C_Str()].OffsetMatrix = p_bone->mOffsetMatrix;

				// �ό`�㒸�_�Ƀ{�[���f�[�^�i�[
				for (unsigned int w = 0; w < p_bone->mNumWeights; w++)
				{
					aiVertexWeight weight = p_bone->mWeights[w];

					int num = p_mModelData->vec_mDeformVertex[index][weight.mVertexId].BoneNum;

					p_mModelData->vec_mDeformVertex[index][weight.mVertexId].BoneWeight[num] = weight.mWeight;
					p_mModelData->vec_mDeformVertex[index][weight.mVertexId].BoneName[num] = p_bone->mName.C_Str();

					p_mModelData->vec_mDeformVertex[index][weight.mVertexId].BoneIndex[num] = p_mModelData->map_mBone[p_bone->mName.C_Str()].mIndex;

					p_mModelData->vec_mDeformVertex[index][weight.mVertexId].BoneNum++;

					assert(p_mModelData->vec_mDeformVertex[index][weight.mVertexId].BoneNum <= 4);
				}
			}
		}
	}

	// ���_�o�b�t�@�Ƀ{�[���C���f�b�N�X�Əd�ݒl���Z�b�g
	for (unsigned int m = 0; m < p_mModelData->p_mAiScene->mNumMeshes; m++)
	{
		aiMesh* p_mesh = p_mModelData->p_mAiScene->mMeshes[m];

		if (p_mesh != nullptr)
		{
			// ���_�o�b�t�@�����b�N
			D3D11_MAPPED_SUBRESOURCE ms;
			HRESULT hr = Renderer::GetDeviceContext()->Map(p_mModelData->pp_mVertexBuffer[m], 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
			if (SUCCEEDED(hr))
			{
				VERTEX_SKELETALMESH_PBR* p_vertex = (VERTEX_SKELETALMESH_PBR*)ms.pData;
				for (unsigned int v = 0; v < p_mesh->mNumVertices; v++)
				{
					DEFORM_VERTEX* dvx = &p_mModelData->vec_mDeformVertex[m][v];

					p_vertex->Position = Vector3(p_mesh->mVertices[v].x, p_mesh->mVertices[v].y, p_mesh->mVertices[v].z);
					p_vertex->Normal = Vector3(p_mesh->mNormals[v].x, p_mesh->mNormals[v].y, p_mesh->mNormals[v].z);
					p_vertex->TexCoord = Vector2(p_mesh->mTextureCoords[0][v].x, p_mesh->mTextureCoords[0][v].y);
					p_vertex->Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
					p_vertex->MaterialID[0] = IDInfo[p_mesh->mMaterialIndex];
					p_vertex->MaterialID[1] = IDInfo[p_mesh->mMaterialIndex];
					p_vertex->MaterialID[2] = IDInfo[p_mesh->mMaterialIndex];
					p_vertex->MaterialID[3] = IDInfo[p_mesh->mMaterialIndex];

					p_vertex->BoneIndex[0] = dvx->BoneIndex[0];
					p_vertex->BoneIndex[1] = dvx->BoneIndex[1];
					p_vertex->BoneIndex[2] = dvx->BoneIndex[2];
					p_vertex->BoneIndex[3] = dvx->BoneIndex[3];

					p_vertex->BoneWeight[0] = dvx->BoneWeight[0];
					p_vertex->BoneWeight[1] = dvx->BoneWeight[1];
					p_vertex->BoneWeight[2] = dvx->BoneWeight[2];
					p_vertex->BoneWeight[3] = dvx->BoneWeight[3];

					p_vertex++;
				}
				Renderer::GetDeviceContext()->Unmap(p_mModelData->pp_mVertexBuffer[m], 0);
			}
		}
	}

	// �e�N�X�`���̓ǂݍ���
	for (unsigned int i = 0; i < p_mModelData->p_mAiScene->mNumTextures; i++)
	{
		ID3D11ShaderResourceView* p_texture;

		aiTexture* p_aitexture = p_mModelData->p_mAiScene->mTextures[i];

		const size_t size = p_aitexture->mWidth;

		HRESULT hr = DirectX::CreateWICTextureFromMemory(
			Renderer::GetDevice(),
			Renderer::GetDeviceContext(),
			reinterpret_cast<const unsigned char*>(p_aitexture->pcData),
			p_aitexture->mWidth,
			nullptr,
			&p_texture);

		assert(p_texture);

		p_mModelData->map_mTexture[p_aitexture->mFilename.data] = p_texture;
	}

	// �}�e���A���ǂݍ���
	if (_bLoadMaterial)
	{
		for (unsigned int i = 0; i < p_mModelData->p_mAiScene->mNumMaterials; i++)
		{
			aiMaterial* mat = p_mModelData->p_mAiScene->mMaterials[i];
			aiString name = mat->GetName();

			MATERIAL myMat{};

			// �g�U����
			{
				aiColor3D color(0.0f, 0.0f, 0.0f);
				if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
				{
					myMat.AlbedoColor.x = color.r;
					myMat.AlbedoColor.y = color.g;
					myMat.AlbedoColor.z = color.b;
					myMat.AlbedoColor.w = 1.0f;
				}
				else
				{
					myMat.AlbedoColor.x = 0.5f;
					myMat.AlbedoColor.y = 0.5f;
					myMat.AlbedoColor.z = 0.5f;
					myMat.AlbedoColor.w = 1.0f;
				}
			}
			// ���ʔ��ˋ��x
			{
				float Metallic = 0.0f;
				if (mat->Get(AI_MATKEY_METALLIC_FACTOR, Metallic) == AI_SUCCESS)
				{
					myMat.Metalic = Metallic;
				}
				else
				{
					myMat.Metalic = 1.0f;
				}

				// ���Ɣ�������
				{
					aiColor3D color(0.0f, 0.0f, 0.0f);
					if (mat->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS)
					{
						myMat.Emission.x = color.r;
						myMat.Emission.y = color.g;
						myMat.Emission.z = color.b;
						myMat.Emission.w = 1.0f;
					}
					else
					{
						myMat.Emission.x = 0.0f;
						myMat.Emission.y = 0.0f;
						myMat.Emission.z = 0.0f;
						myMat.Emission.w = 0.0f;
					}
				}

				// �f�B�t�[�Y�e�N�X�`�����擾
				aiTextureType type = aiTextureType_DIFFUSE;
				int texnum = mat->GetTextureCount(type);

				// 1���b�V����1����������
				assert(texnum <= 1);

				// �}�e���A�������擾
				aiString matName;
				mat->GetTexture(type, 0, &matName);

				ID3D11ShaderResourceView* srv = nullptr;

				// ���݂��Ă��邩�m�F
				if (p_mModelData->map_mTexture.find(matName.data) == p_mModelData->map_mTexture.end())
				{
					myMat.TextureEnable = FALSE;
					srv = nullptr;

					// �t�@�C�����擾
					std::string fileName = GetFileName(matName.C_Str());

					// �e�N�X�`���̓ǂݍ��݂Ǝ擾
					srv = LoadDiffuseTexture(fileName, _FileName);

					if (srv == nullptr)
					{
						myMat.TextureEnable = FALSE;
					}
					else
					{
						myMat.TextureEnable = TRUE;
						p_mModelData->map_mTexture[matName.data] = srv;
					}
				}
				// �}�e���A���̕ۑ�
				p_mModelData->vec_material.emplace_back(myMat);
			}
		}
	}

	float time = Time->CountStop();
	NEWDEBUGLOG(_FileName);
	NEWDEBUGLOG("���f���ǂݍ��݊���");
	NEWDEBUGLOG("Time : " << time << "sec");
}

void SkeletalMeshLoader::LoadAnimation(const char* _FileName, const char* _AnimName)
{
	Time->CountStart();
	p_mModelData->map_mAnimation[_AnimName].pAnimation = aiImportFile(_FileName, aiProcess_ConvertToLeftHanded);
	if (p_mModelData->map_mAnimation[_AnimName].pAnimation)
	{
		if (p_mModelData->map_mAnimation[_AnimName].pAnimation->HasAnimations())
		{
			float time = Time->CountStop();
			NEWDEBUGLOG(_FileName << " : " << _AnimName);
			NEWDEBUGLOG("�A�j���[�V�����ǂݍ��ݐ���");
			NEWDEBUGLOG("Time : " << time << "sec");

			aiAnimation* animation = p_mModelData->map_mAnimation[_AnimName].pAnimation->mAnimations[0];
			unsigned int MaxKey = 0;

			for (unsigned int i = 0; i < animation->mNumChannels; i++)
			{
				aiNodeAnim* nodeAnim = animation->mChannels[i];

				unsigned int pkey = nodeAnim->mNumPositionKeys;
				unsigned int rkey = nodeAnim->mNumRotationKeys;

				// �ő�̃L�[�t���[�����擾����
				if (pkey > MaxKey)
				{
					MaxKey = pkey;
				}
				if (rkey > MaxKey)
				{
					MaxKey = rkey;
				}
			}
			p_mModelData->map_mAnimation[_AnimName].LastKey = MaxKey;
		}
	}
	else
	{
		float time = Time->CountStop();
		NEWDEBUGLOG(_FileName << " : " << _AnimName);
		NEWDEBUGLOG(_FileName << " , " << _AnimName);
		NEWDEBUGLOG("�A�j���[�V�����ǂݍ��ݎ��s");
		NEWDEBUGLOG("Time : " << time << "sec");
	}

	assert(p_mModelData->map_mAnimation[_AnimName].pAnimation);
}

void SkeletalMeshLoader::Push(const char* _ModelName)
{
	SkeletalModelPool::Push(_ModelName, this);
}

void SkeletalMeshLoader::Uninit()
{
	for (unsigned int m = 0; m < p_mModelData->p_mAiScene->mNumMeshes; m++)
	{
		p_mModelData->pp_mVertexBuffer[m]->Release();
		p_mModelData->pp_mIndexBuffer[m]->Release();
	}

	delete[] p_mModelData->pp_mVertexBuffer;
	delete[] p_mModelData->pp_mIndexBuffer;

	delete[] p_mModelData->vec_mDeformVertex;

	for (std::pair<const std::string, ID3D11ShaderResourceView*>pair : p_mModelData->map_mTexture)
	{
		if (pair.second)
			pair.second->Release();
	}

	aiReleaseImport(p_mModelData->p_mAiScene);

	for (auto anim : p_mModelData->map_mAnimation)
	{
		aiReleaseImport(anim.second.pAnimation);
	}
	delete p_mModelData;
}

void SkeletalMeshLoader::SetAlbedoColor(DirectX::SimpleMath::Color _color, int index)
{
	if (index < p_mModelData->vec_material.size())
	{
		p_mModelData->vec_material[index].AlbedoColor = _color;
	}
}

void SkeletalMeshLoader::SetEmission(DirectX::SimpleMath::Color _color, int index)
{
	if (index < p_mModelData->vec_material.size())
	{
		p_mModelData->vec_material[index].Emission = _color;
	}
}

void SkeletalMeshLoader::SetMetallic(float _val, int index)
{
	if (index < p_mModelData->vec_material.size())
	{
		p_mModelData->vec_material[index].Metalic = _val;
	}
}

void SkeletalMeshLoader::SetRoughness(float _val, int index)
{
	if (index < p_mModelData->vec_material.size())
	{
		p_mModelData->vec_material[index].Roughness = _val;
	}
}


/*
* // �ǂݍ��ݎ��̍��W�������������f���f�[�^�ɑ΂��Ē��_�ʒu�����[�J�����W�n�̌��_����Ɍv�Z���Ȃ���
* void Com_AssimpAnimation::TransWorldToLocal(aiNode* _node, aiMatrix4x4& parentTransform)
{
	aiMatrix4x4 currentTransform = _node->mTransformation;

	currentTransform *= parentTransform;

	for (unsigned int i = 0; i < _node->mNumMeshes; i++)
	{
		aiMesh* p_mesh = p_mAiScene->mMeshes[_node->mMeshes[i]];
		for (unsigned int j = 0; j < p_mesh->mNumVertices; j++)
		{
			p_mesh->mVertices[j] = currentTransform * p_mesh->mVertices[j];
		}
	}

	// �q�m�[�h�ɑ΂��čċA�I�ɏ���
	for (unsigned int i = 0; i < _node->mNumChildren; i++)
	{
		TransWorldToLocal(_node->mChildren[i], currentTransform);
	}
}
*
*/