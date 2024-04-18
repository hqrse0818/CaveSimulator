#include "StaticMeshLoader.h"
#include "StaticModelPool.h"
#include <SimpleMath.h>
#include <DirectXMath.h>
#include<DirectXTex.h>
#include <iostream>
#include "../main.h"
#include "../System/Time.h"
#include "../System/TextureCreate.h"
#include "../Utility.h"
#include "../Debug.h"
#include "MaterialPool.h"
#include "FileOperation.h"
#include "SolutionCSV.h"

using namespace DirectX::SimpleMath;
using namespace std;

ID3D11ShaderResourceView* StaticMeshLoader::LoadDiffuseTexture(std::string _FileName, std::string _ModelFileName)
{
	// ファイルの拡張子を取得
	std::string fileext = GetFileExt(_FileName);

	// ファイル名(UTF-16)を取得する
	std::wstring widefilename = GetFileNameWide(_FileName.c_str());

	// モデルデータの親ディレクトリを取得
	std::wstring parentdirectory = GetParentDirectoryWide(_ModelFileName);// +L"";

	// テクスチャのパス名取得
	std::wstring filepath = parentdirectory + widefilename;

	// SRV
	ID3D11ShaderResourceView* srv = nullptr;

	// TGA？
	if (fileext == ".tga") {
		// TGAの場合
		DirectX::TexMetadata meta;
		DirectX::GetMetadataFromTGAFile(filepath.c_str(), meta);

		std::unique_ptr<DirectX::ScratchImage> image(new DirectX::ScratchImage);

		HRESULT hr = LoadFromTGAFile(filepath.c_str(), &meta, *image);
		if (FAILED(hr)) {
			NEWDEBUGLOG("LoadFromTGAFile Error (" << _FileName << ")");
			return nullptr;
		}

		ID3D11Resource* tex = nullptr;

		// テクスチャ生成
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

		// シェーダーリソースビュー作成
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
		// テクスチャ読み込み
		TextureCreate::CreateTexture(filepath.c_str(), &srv);
		if (srv == nullptr) {
			WNEWDEBULOG(L"CreateWICTextureFromFile Error (" << filepath.c_str() << widefilename.c_str() << L")");
			return nullptr;
		}
	}

	return srv;
}

StaticMeshLoader::StaticMeshLoader()
{
	p_mModelData = new STATICMESH_DATA();
}

void StaticMeshLoader::LoadModel(const char* _FileName, bool _bLoadMaterial)
{
	const std::string modelPath(_FileName);

	Time->CountStart();

	Assimp::Importer importer;

	unsigned int flg = 0;
	flg |= aiProcess_ConvertToLeftHanded;	// 左手座標系にコンバートする
	flg |= aiProcessPreset_TargetRealtime_MaxQuality;

	p_mModelData->p_mAiScene = aiImportFile(_FileName, flg);

	if (p_mModelData->p_mAiScene == nullptr)
	{
		NEWDEBUGLOG(_FileName << " is Error");
		NEWDEBUGLOG(aiGetErrorString());
	}

	assert(p_mModelData->p_mAiScene);

	// バッファの作成
	p_mModelData->pp_mVertexBuffer = new ID3D11Buffer * [p_mModelData->p_mAiScene->mNumMeshes];
	p_mModelData->pp_mIndexBuffer = new ID3D11Buffer * [p_mModelData->p_mAiScene->mNumMeshes];

	for (unsigned int index = 0; index < p_mModelData->p_mAiScene->mNumMeshes; index++)
	{
		aiMesh* p_mesh = p_mModelData->p_mAiScene->mMeshes[index];

		// 頂点バッファ生成
		if (p_mesh != nullptr)
		{
			VERTEX_STATICMESH* p_vertex = new VERTEX_STATICMESH[p_mesh->mNumVertices];

			for (unsigned int ver = 0; ver < p_mesh->mNumVertices; ver++)
			{
				p_vertex[ver].Position = Vector3(p_mesh->mVertices[ver].x, p_mesh->mVertices[ver].y, p_mesh->mVertices[ver].z);

				if (p_mesh->mNormals != nullptr)
				{
					p_vertex[ver].Normal = Vector3(p_mesh->mNormals[ver].x, p_mesh->mNormals[ver].y, p_mesh->mNormals[ver].z);
				}
				else
				{
					NEWDEBUGLOG(_FileName << " Normal is null !");
				}
				if (p_mesh->mTextureCoords[index])
				{
					p_vertex[ver].TexCoord = Vector2(p_mesh->mTextureCoords[0][ver].x, p_mesh->mTextureCoords[0][ver].y);
				}
				else
				{
					NEWDEBUGLOG(_FileName << " TexCoord is null !");
					p_vertex[ver].TexCoord = Vector2(0, 1);
				}

				p_vertex[ver].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_STATICMESH) * p_mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = p_vertex;

			HRESULT hr = Renderer::GetDevice()->CreateBuffer(&bd, &sd, &p_mModelData->pp_mVertexBuffer[index]);
			if (FAILED(hr))
			{
				NEWDEBUGLOG(_FileName << "頂点バッファの作成に失敗 : " << index);
			}

			delete[] p_vertex;
		}

		// インデックスバッファ生成
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
		}
	}
	// テクスチャの読み込み
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

	// マテリアル読み込み
	if (_bLoadMaterial)
	{
		for (unsigned int i = 0; i < p_mModelData->p_mAiScene->mNumMaterials; i++)
		{
			aiMaterial* mat = p_mModelData->p_mAiScene->mMaterials[i];
			
			aiString name = mat->GetName();

			MATERIAL myMat{};
			PBR_MATERIAL myPBRmat{};

			// 拡散反射
			{
				aiColor3D color(0.0f, 0.0f, 0.0f);
				if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
				{
					myMat.AlbedoColor.x = color.r;
					myMat.AlbedoColor.y = color.g;
					myMat.AlbedoColor.z = color.b;
					myMat.AlbedoColor.w = 1.0f;
					myPBRmat.AlbedoColor.x = color.r;
					myPBRmat.AlbedoColor.y = color.g;
					myPBRmat.AlbedoColor.z = color.b;
					myPBRmat.AlbedoColor.w = 1.0f;
				}
				else
				{
					myMat.AlbedoColor.x = 0.5f;
					myMat.AlbedoColor.y = 0.5f;
					myMat.AlbedoColor.z = 0.5f;
					myMat.AlbedoColor.w = 1.0f;
				}
			}

			// 鏡面反射強度
			{
				float Shinness = 0.0f;
				if (mat->Get(AI_MATKEY_SHININESS, Shinness) == AI_SUCCESS)
				{
					myMat.Shinness = Shinness;
				}
				else
				{
					myMat.Shinness = 0.0f;
				}
			}
			// 金属度
			{
				float metal = 0.04f;
				if (mat->Get(AI_MATKEY_METALLIC_FACTOR, metal) == AI_SUCCESS)
				{
					myMat.Metalic = metal;
				}
				else
				{
					myMat.Metalic = 0.04f;
				}
			}
			// 自家発光成分
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
			// 粗さ
			{
				float Roufhness = 0.0f;
				if (mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, Roufhness) == AI_SUCCESS)
				{
					myPBRmat.Roughness = Roufhness;
					NEWDEBUGLOG("粗さ" << Roufhness);
				}
				else
				{
					myPBRmat.Roughness = 0.5f;
				}
			}
			// メタリック
			{
				float metalic = 0.0f;
				if (mat->Get(AI_MATKEY_METALLIC_FACTOR, metalic) == AI_SUCCESS)
				{
					myPBRmat.Metalic = metalic;
					NEWDEBUGLOG("メタリック" << metalic);
				}
				else
				{
					myPBRmat.Metalic = 0.5f;
				}
			}

			// ディフーズテクスチャ数取得
			aiTextureType type = aiTextureType_DIFFUSE;
			int texnum = mat->GetTextureCount(type);

			// 1メッシュに1毎だけ許可
			assert(texnum <= 1);

			// マテリアル名を取得
			aiString matName;
			mat->GetTexture(type, 0, &matName);

			ID3D11ShaderResourceView* srv = nullptr;

			// 存在しているか確認
			if (p_mModelData->map_mTexture.find(matName.data) == p_mModelData->map_mTexture.end())
			{
				myMat.TextureEnable = FALSE;
				srv = nullptr;

				// ファイル名取得
				std::string fileName = GetFileName(matName.C_Str());

				// テクスチャの読み込みと取得
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
			// マテリアルの保存
			p_mModelData->vec_material.emplace_back(myMat);
		}
	}

	float time = Time->CountStop();
	NEWDEBUGLOG(_FileName);
	NEWDEBUGLOG("モデル読み込み完了");
	NEWDEBUGLOG("Time : " << time << "sec");
}

void StaticMeshLoader::LoadModelPBR(const char* _FileName, const char* _TextName, bool _bLoadMaterial)
{
	p_mModelData->mType = STATICMESH_TYPE::PBR;

	// ファイル名の分割
	vector<string> Namelist = SeparateString(_FileName, '\\');
	int EndIndex = Namelist.size() - 1;
	// .fbxを除いたファイル名の取得
	string LastName = SeparateString(Namelist[EndIndex], '.')[0];

	std::string SecondName = _TextName;
	std::string FirstName = "asset/data/Materials/";
	FirstName = FirstName + SecondName;
	FirstName += ".txt";

	FileOperator* Operator = new FileOperator(FirstName.c_str());

	const std::string modelPath(_FileName);

	Time->CountStart();

	Assimp::Importer importer;

	unsigned int flg = 0;
	flg |= aiProcess_ConvertToLeftHanded;	// 左手座標系にコンバートする
	flg |= aiProcessPreset_TargetRealtime_MaxQuality;

	p_mModelData->p_mAiScene = aiImportFile(_FileName, flg);

	if (p_mModelData->p_mAiScene == nullptr)
	{
		NEWDEBUGLOG(_FileName << " is Error");
		NEWDEBUGLOG(aiGetErrorString());
	}

	assert(p_mModelData->p_mAiScene);

	// バッファの作成
	p_mModelData->pp_mVertexBuffer = new ID3D11Buffer * [p_mModelData->p_mAiScene->mNumMeshes];
	p_mModelData->pp_mIndexBuffer = new ID3D11Buffer * [p_mModelData->p_mAiScene->mNumMeshes];

	for (unsigned int index = 0; index < p_mModelData->p_mAiScene->mNumMeshes; index++)
	{
		aiMesh* p_mesh = p_mModelData->p_mAiScene->mMeshes[index];
		mNumVertices += p_mesh->mNumVertices;

		// 頂点バッファ生成
		if (p_mesh != nullptr)
		{
			VERTEX_STATICMESH_PBR* p_vertex = new VERTEX_STATICMESH_PBR[p_mesh->mNumVertices];

			for (unsigned int ver = 0; ver < p_mesh->mNumVertices; ver++)
			{
				p_vertex[ver].Position = Vector3(p_mesh->mVertices[ver].x, p_mesh->mVertices[ver].y, p_mesh->mVertices[ver].z);

				if (p_mesh->mNormals != nullptr)
				{
					p_vertex[ver].Normal = Vector3(p_mesh->mNormals[ver].x, p_mesh->mNormals[ver].y, p_mesh->mNormals[ver].z);
				}
				else
				{
					NEWDEBUGLOG(_FileName << " Normal is null !");
				}
				if (p_mesh->mTextureCoords[index] != nullptr)
				{
					p_vertex[ver].TexCoord = Vector2(p_mesh->mTextureCoords[0][ver].x, p_mesh->mTextureCoords[0][ver].y);
				}
				else
				{
					NEWDEBUGLOG(_FileName << " TexCoord is null !");
					p_vertex[ver].TexCoord = Vector2(0, 1);
				}

				p_vertex[ver].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);

				// PBR用マテリアルの読み込みとマテリアルID登録
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
						// 反射光
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
						// エミッションカラー
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

						// 粗さ
						float roufh;
						if (aiMat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roufh) == AI_SUCCESS)
						{
							mat->Roughness = roufh;
						}
						else
						{
							mat->Roughness = 0.5f;
						}

						// メタリック
						float met;
						if (aiMat->Get(AI_MATKEY_METALLIC_FACTOR, met) == AI_SUCCESS)
						{
							// 最低の金属度
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

						// スペキュラ強度
						float shinness;
						if (aiMat->Get(AI_MATKEY_SHININESS_STRENGTH, shinness) == AI_SUCCESS)
						{
							mat->Shinness = shinness;
						}
						else
						{
							mat->Shinness = 0.0f;
						}

						mat->MicroFacet = 0.75f;

						MaterialPool::Push(mat);

						string PushName = LastName;
						LastName += p_mesh->mName.C_Str();

						MaterialPool::PushName(LastName);
						int id = mat->MaterialID;
						IDInfo[p_mesh->mMaterialIndex] = id;
						p_vertex[ver].MaterialID[0] = id;
						p_vertex[ver].MaterialID[1] = id;
						p_vertex[ver].MaterialID[2] = id;
						p_vertex[ver].MaterialID[3] = id;
						
						// テキストファイルにデータを書き込む
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
			bd.ByteWidth = sizeof(VERTEX_STATICMESH_PBR) * p_mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = p_vertex;

			HRESULT hr = Renderer::GetDevice()->CreateBuffer(&bd, &sd, &p_mModelData->pp_mVertexBuffer[index]);
			if (FAILED(hr))
			{
				NEWDEBUGLOG(_FileName << "頂点バッファの作成に失敗 : " << index);
			}

			delete[] p_vertex;
		}

		// インデックスバッファ生成
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
		}
	}
	// テクスチャの読み込み
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

	// マテリアル読み込み
	if (_bLoadMaterial)
	{
		for (unsigned int i = 0; i < p_mModelData->p_mAiScene->mNumMaterials; i++)
		{
			aiMaterial* mat = p_mModelData->p_mAiScene->mMaterials[i];

			aiString name = mat->GetName();

			MATERIAL myMat{};
			PBR_MATERIAL myPBRmat{};

			// 拡散反射
			{
				aiColor3D color(0.0f, 0.0f, 0.0f);
				if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
				{
					myMat.AlbedoColor.x = color.r;
					myMat.AlbedoColor.y = color.g;
					myMat.AlbedoColor.z = color.b;
					myMat.AlbedoColor.w = 1.0f;
					myPBRmat.AlbedoColor.x = color.r;
					myPBRmat.AlbedoColor.y = color.g;
					myPBRmat.AlbedoColor.z = color.b;
					myPBRmat.AlbedoColor.w = 1.0f;
				}
				else
				{
					myMat.AlbedoColor.x = 0.5f;
					myMat.AlbedoColor.y = 0.5f;
					myMat.AlbedoColor.z = 0.5f;
					myMat.AlbedoColor.w = 1.0f;
				}
			}
			// 鏡面反射強度
			{
				float Shinness = 0.0f;
				if (mat->Get(AI_MATKEY_SHININESS, Shinness) == AI_SUCCESS)
				{
					myMat.Shinness = Shinness;
				}
				else
				{
					myMat.Shinness = 0.0f;
				}
			}
			// 金属度
			{
				float metal = 0.04f;
				if (mat->Get(AI_MATKEY_METALLIC_FACTOR, metal) == AI_SUCCESS)
				{
					myMat.Metalic = metal;
				}
				else
				{
					myMat.Metalic = 0.04f;
				}
			}

			// 自家発光成分
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

			// 粗さ
			{
				float Roufhness = 0.0f;
				if (mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, Roufhness) == AI_SUCCESS)
				{
					myPBRmat.Roughness = Roufhness;
					NEWDEBUGLOG("粗さ" << Roufhness);
				}
				else
				{
					myPBRmat.Roughness = 0.5f;
				}
			}
			// メタリック
			{
				float metalic = 0.0f;
				if (mat->Get(AI_MATKEY_METALLIC_FACTOR, metalic))
				{
					myPBRmat.Metalic = metalic;
					NEWDEBUGLOG("メタリック" << metalic);
				}
				else
				{
					myPBRmat.Metalic = 0.5f;
				}
			}

			// ディフーズテクスチャ数取得
			aiTextureType type = aiTextureType_DIFFUSE;
			int texnum = mat->GetTextureCount(type);

			// 1メッシュに1毎だけ許可
			assert(texnum <= 1);

			// マテリアル名を取得
			aiString matName;
			mat->GetTexture(type, 0, &matName);

			ID3D11ShaderResourceView* srv = nullptr;

			// 存在しているか確認
			if (p_mModelData->map_mTexture.find(matName.data) == p_mModelData->map_mTexture.end())
			{
				myMat.TextureEnable = FALSE;
				srv = nullptr;

				// ファイル名取得
				std::string fileName = GetFileName(matName.C_Str());

				// テクスチャの読み込みと取得
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
			// マテリアルの保存
			p_mModelData->vec_material.emplace_back(myMat);
		}
	}

	Operator->Close();
	delete Operator;

	float time = Time->CountStop();
	NEWDEBUGLOG(_FileName);
	NEWDEBUGLOG("モデル読み込み完了");
	NEWDEBUGLOG("Time : " << time << "sec");
}

void StaticMeshLoader::LoadModelIncludeTanget(const char* _FileName, bool _bLoadMaterial)
{
	p_mModelData->mType = STATICMESH_TYPE::Tangent;

	const std::string modelPath(_FileName);

	Time->CountStart();

	Assimp::Importer importer;

	unsigned int flg = 0;
	flg |= aiProcess_ConvertToLeftHanded;	// 左手座標系にコンバートする
	flg |= aiProcessPreset_TargetRealtime_MaxQuality;

	p_mModelData->p_mAiScene = aiImportFile(_FileName, flg);

	if (p_mModelData->p_mAiScene == nullptr)
	{
		NEWDEBUGLOG(_FileName << " is Error");
		NEWDEBUGLOG(aiGetErrorString());
	}

	assert(p_mModelData->p_mAiScene);

	// バッファの作成
	p_mModelData->pp_mVertexBuffer = new ID3D11Buffer * [p_mModelData->p_mAiScene->mNumMeshes];
	p_mModelData->pp_mIndexBuffer = new ID3D11Buffer * [p_mModelData->p_mAiScene->mNumMeshes];

	for (unsigned int index = 0; index < p_mModelData->p_mAiScene->mNumMeshes; index++)
	{
		aiMesh* p_mesh = p_mModelData->p_mAiScene->mMeshes[index];

		// 頂点バッファ生成
		if (p_mesh != nullptr)
		{
			VERTEX_STATICMESH_TANGENT* p_vertex = new VERTEX_STATICMESH_TANGENT[p_mesh->mNumVertices];

			for (unsigned int ver = 0; ver < p_mesh->mNumVertices; ver++)
			{
				p_vertex[ver].Position = Vector3(p_mesh->mVertices[ver].x, p_mesh->mVertices[ver].y, p_mesh->mVertices[ver].z);

				if (p_mesh->mNormals != nullptr)
				{
					p_vertex[ver].Normal = Vector3(p_mesh->mNormals[ver].x, p_mesh->mNormals[ver].y, p_mesh->mNormals[ver].z);
				}
				else
				{
					NEWDEBUGLOG(_FileName << " Normal is null !");
				}
				if (p_mesh->mTangents != nullptr)
				{
					p_vertex[ver].Tangent = Vector3(p_mesh->mTangents[ver].x, p_mesh->mTangents[ver].y, p_mesh->mTangents[ver].z);
				}
				else
				{
					NEWDEBUGLOG(_FileName << " Tangent is null !");
				}
				if (p_mesh->mBitangents != nullptr)
				{
					p_vertex[ver].BiTangent = Vector3(p_mesh->mBitangents[ver].x, p_mesh->mBitangents[ver].y, p_mesh->mBitangents[ver].z);
				}
				else
				{
					NEWDEBUGLOG(_FileName << " BiTangent is null !");
				}
				if (p_mesh->mTextureCoords[index])
				{
					p_vertex[ver].TexCoord = Vector2(p_mesh->mTextureCoords[0][ver].x, p_mesh->mTextureCoords[0][ver].y);
				}
				else
				{
					NEWDEBUGLOG(_FileName << " TexCoord is null !");
					p_vertex[ver].TexCoord = Vector2(0, 1);
				}

				p_vertex[ver].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_STATICMESH_TANGENT) * p_mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = p_vertex;

			HRESULT hr = Renderer::GetDevice()->CreateBuffer(&bd, &sd, &p_mModelData->pp_mVertexBuffer[index]);
			if (FAILED(hr))
			{
				NEWDEBUGLOG(_FileName << "頂点バッファの作成に失敗 : " << index);
			}

			delete[] p_vertex;
		}

		// インデックスバッファ生成
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
		}
	}
	// テクスチャの読み込み
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

	// マテリアル読み込み
	if (_bLoadMaterial)
	{
		for (unsigned int i = 0; i < p_mModelData->p_mAiScene->mNumMaterials; i++)
		{
			aiMaterial* mat = p_mModelData->p_mAiScene->mMaterials[i];
			aiString name = mat->GetName();

			MATERIAL myMat{};
			PBR_MATERIAL myPBRmat{};

			// 拡散反射
			{
				aiColor3D color(0.0f, 0.0f, 0.0f);
				if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
				{
					myMat.AlbedoColor.x = color.r;
					myMat.AlbedoColor.y = color.g;
					myMat.AlbedoColor.z = color.b;
					myMat.AlbedoColor.w = 1.0f;
					myPBRmat.AlbedoColor.x = color.r;
					myPBRmat.AlbedoColor.y = color.g;
					myPBRmat.AlbedoColor.z = color.b;
					myPBRmat.AlbedoColor.w = 1.0f;
				}
				else
				{
					myMat.AlbedoColor.x = 0.5f;
					myMat.AlbedoColor.y = 0.5f;
					myMat.AlbedoColor.z = 0.5f;
					myMat.AlbedoColor.w = 1.0f;
				}
			}
			// 鏡面反射強度
			{
				float Shinness = 0.0f;
				if (mat->Get(AI_MATKEY_SHININESS, Shinness) == AI_SUCCESS)
				{
					myMat.Shinness = Shinness;
				}
				else
				{
					myMat.Shinness = 0.0f;
				}
			}
			// 金属度
			{
				float Metallic = 0.0f;
				if (mat->Get(AI_MATKEY_METALLIC_FACTOR, Metallic) == AI_SUCCESS)
				{
					myMat.Metalic = Metallic;
				}
				else
				{
					myMat.Metalic = 0.0f;
				}
			}

			// 自家発光成分
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

			// 粗さ
			{
				float Roufhness = 0.0f;
				if (mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, Roufhness) == AI_SUCCESS)
				{
					myPBRmat.Roughness = Roufhness;
					NEWDEBUGLOG("粗さ" << Roufhness);
				}
				else
				{
					myPBRmat.Roughness = 0.5f;
				}
			}
			// メタリック
			{
				float metalic = 0.0f;
				if (mat->Get(AI_MATKEY_METALLIC_FACTOR, metalic))
				{
					myPBRmat.Metalic = metalic;
					NEWDEBUGLOG("メタリック" << metalic);
				}
				else
				{
					myPBRmat.Metalic = 0.5f;
				}
			}

			// ディフーズテクスチャ数取得
			aiTextureType type = aiTextureType_DIFFUSE;
			int texnum = mat->GetTextureCount(type);

			// 1メッシュに1毎だけ許可
			assert(texnum <= 1);

			// マテリアル名を取得
			aiString matName;
			mat->GetTexture(type, 0, &matName);

			ID3D11ShaderResourceView* srv = nullptr;

			// 存在しているか確認
			if (p_mModelData->map_mTexture.find(matName.data) == p_mModelData->map_mTexture.end())
			{
				myMat.TextureEnable = FALSE;
				srv = nullptr;

				// ファイル名取得
				std::string fileName = GetFileName(matName.C_Str());

				// テクスチャの読み込みと取得
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
			// マテリアルの保存
			p_mModelData->vec_material.emplace_back(myMat);
		}
	}

	float time = Time->CountStop();
	NEWDEBUGLOG(_FileName);
	NEWDEBUGLOG("モデル読み込み完了");
	NEWDEBUGLOG("Time : " << time << "sec");
}

void StaticMeshLoader::Push(const char* _ModelName)
{
	StaticModelPool::Push(_ModelName, this);
}

void StaticMeshLoader::Uninit()
{
	for (unsigned int m = 0; m < p_mModelData->p_mAiScene->mNumMeshes; m++)
	{
		p_mModelData->pp_mVertexBuffer[m]->Release();
		p_mModelData->pp_mIndexBuffer[m]->Release();
	}

	delete[] p_mModelData->pp_mVertexBuffer;
	delete[] p_mModelData->pp_mIndexBuffer;

	for (std::pair<const std::string, ID3D11ShaderResourceView*>pair : p_mModelData->map_mTexture)
	{
		if (pair.second)
			pair.second->Release();
	}

	aiReleaseImport(p_mModelData->p_mAiScene);

	delete p_mModelData;
}
