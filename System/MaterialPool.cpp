#include "MaterialPool.h"
#include "../Utility.h"
#include "../DirectX/renderer.h"
#include "imguiManager.h"
#include "FileOperation.h"
#include "SolutionCSV.h"

std::vector<PBR_MATERIAL*> MaterialPool::MaterialList{};
std::vector<std::string> MaterialPool::MeshNameList{};
ID3D11Buffer* MaterialPool::p_mMaterialBuffer = nullptr;

using namespace DirectX::SimpleMath;
using namespace std;

HRESULT MaterialPool::CreateMaterialBuffer()
{
	return CreateConstantBufferWrite(Renderer::GetDevice(), sizeof(Materials), sizeof(float), &p_mMaterialBuffer);
}

void MaterialPool::UpdateMaterials()
{
	D3D11_MAPPED_SUBRESOURCE ms;
	Materials* pData = nullptr;
	std::vector<PBR_MATERIAL> vecMaterials;
	vecMaterials.resize(MaterialList.size());
	int index = 0;
	for (auto mat : MaterialList)
	{
		vecMaterials[index] = *mat;
		index++;
	}

	HRESULT hr = Renderer::GetDeviceContext()->Map(
		p_mMaterialBuffer,
		0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	if (SUCCEEDED(hr))
	{
		pData = (Materials*)ms.pData;
		memcpy(pData->Materials,
			vecMaterials.data(),
			sizeof(PBR_MATERIAL) * vecMaterials.size());
		Renderer::GetDeviceContext()->Unmap(p_mMaterialBuffer, 0);
	}
	else
	{
		NEWDEBUGLOG("マテリアルバッファ更新失敗");
	}
}

void MaterialPool::SetMaterialBuffer(int _slot)
{
	return Renderer::GetDeviceContext()->VSSetConstantBuffers(_slot, 1, &p_mMaterialBuffer),
		Renderer::GetDeviceContext()->PSSetConstantBuffers(_slot, 1, &p_mMaterialBuffer);
}

void MaterialPool::Push(PBR_MATERIAL* _mat)
{
	MaterialList.push_back(_mat);
	// マテリアルIDの登録
	_mat->MaterialID = MaterialList.size() - 1;
}

void MaterialPool::PushName(std::string _Name)
{
	MeshNameList.emplace_back(_Name);
}

void MaterialPool::Uninit()
{
	for (auto mat : MaterialList)
	{
		delete mat;
	}
	MeshNameList.clear();
	MaterialList.clear();
}

void MaterialPool::Init()
{
	PBR_MATERIAL* mat = new PBR_MATERIAL();
	mat->AlbedoColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
	mat->MaterialID = 0;
	mat->Roughness = 0.5f;
	mat->Metalic = 0.04f;
	MaterialList.push_back(mat);
	PushName("BaseMaterial");
}

void MaterialPool::ShowMaterialGui()
{
	constexpr unsigned int stringbuffer = 30;

	static float GUIPosX = 0.0f;
	static float GUISizeX = 400.0f;

	static float MaxShinness = 50.0f;
	static float MaxNormalMapIntensity = 10.0f;

	ImGui::SetNextWindowPos(ImVec2(GUIPosX, static_cast<float>(MAX_SPRITE_HEIGHT)), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(GUISizeX, static_cast<float>(SCREEN_HEIGHT - MAX_SPRITE_HEIGHT)), ImGuiCond_Once);

	ImGui::Begin("Material");

	static char filename[] = "asset/data/Materials/EditMaterialData.csv";
	ImGui::InputText("FileName", filename, sizeof(filename) + stringbuffer);
	if (ImGui::Button("Save"))
	{
		SaveMaterialData(filename);
		GUI::PushLog("Saved Material");
	}

	for (int i = 0; i < MaterialList.size(); i++)
	{
		string name = MeshNameList[i];
		if (ImGui::TreeNode(name.c_str()))
		{
			string color = "Color" + to_string(i);
			string emissive = "Emission" + to_string(i);
			string metal = "Metallic" + to_string(i);
			string roufh = "Roufhness" + to_string(i);
			string micro = "MicroFacet" + to_string(i);
			string shinness = "Shinness" + to_string(i);
			string Normal = "NormalMap_Intensity" + to_string(i);
			ImGui::ColorPicker4(color.c_str(), (float*)&MaterialList[i]->AlbedoColor);
			//ImGui::ColorPicker4(emissive.c_str(), (float*)&MaterialList[i]->Emission);
			ImGui::SliderFloat(metal.c_str(), &MaterialList[i]->Metalic, 0.0f, 1.0f);
			ImGui::SliderFloat(roufh.c_str(), &MaterialList[i]->Roughness, 0.0f, 1.0f);
			ImGui::SliderFloat(micro.c_str(), &MaterialList[i]->MicroFacet, 0.0f, 1.0f);
			ImGui::SliderFloat(shinness.c_str(), &MaterialList[i]->Shinness, 0.0f, MaxShinness);
			ImGui::SliderFloat(Normal.c_str(), &MaterialList[i]->NormalMap_Intensity, 0.0f, MaxNormalMapIntensity);

			ImGui::TreePop();
		}
	}

	ImGui::End();

	UpdateMaterials();
}

void MaterialPool::SaveMaterialData(const char* _FileName)
{
	string header = "exp,meshname,MaterialID,AlbedoR,AlbedoG,AlbedoB,AlbedoA,EmissionR,EmissionG,EmissionB,EmissionA,Metallic,Roufhness,MicroFacet,Shinness,FacetBlend,PhotoReal";
	string form = "type,char,uint,float,float,float,float,float,float,float,float,float,float,float,float,float,float";
	FileOperator* file = new FileOperator(_FileName, true);
	file->Write(header, false);
	file->Write(form);
	for (int i = 0; i < MaterialList.size(); i++)
	{
		file->Write("data,");
		file->Write(MeshNameList[i], false);
		file->Write(",", false);
		file->Write(MaterialList[i]->MaterialID, false);
		file->Write(",", false);
		file->Write(MaterialList[i]->AlbedoColor.x, false);
		file->Write(",", false);
		file->Write(MaterialList[i]->AlbedoColor.y, false);
		file->Write(",", false);
		file->Write(MaterialList[i]->AlbedoColor.z, false);
		file->Write(",", false);
		file->Write(MaterialList[i]->AlbedoColor.w, false);
		file->Write(",", false);
		file->Write(MaterialList[i]->Emission.x, false);
		file->Write(",", false);
		file->Write(MaterialList[i]->Emission.y, false);
		file->Write(",", false);
		file->Write(MaterialList[i]->Emission.z, false);
		file->Write(",", false);
		file->Write(MaterialList[i]->Emission.w, false);
		file->Write(",", false);
		file->Write(MaterialList[i]->Metalic, false);
		file->Write(",", false);
		file->Write(MaterialList[i]->Roughness, false);
		file->Write(",", false);
		file->Write(MaterialList[i]->MicroFacet, false);
		file->Write(",", false);
		file->Write(MaterialList[i]->Shinness, false);
		file->Write(",", false);
		file->Write(MaterialList[i]->NormalMap_Intensity, false);
	}
	file->Close();
	delete file;
}

void MaterialPool::ReadMaterialData(const char* _FileName)
{
	// 設定されたマテリアルデータの読み込み(修正)
	vector<string> MaterialStr = ReadDataCSVAll(_FileName);

	// マテリアルはvectorで管理されている
	// 修正するマテリアルのインデックスデータ〇
	// or マテリアル名で修正するか?
	unsigned int index = 0;

	for (auto str : MaterialStr)
	{
		vector<string> matstr = SeparateString(str, ',');
		if (matstr[0] == "data")
		{
			// マテリアル名が正しいかチェック
			if (matstr[1] == MaterialPool::GetMeshName(index))
			{
				// 正しい場合は書き換え
				PBR_MATERIAL* material = MaterialPool::GetMaterial(index);
				material->AlbedoColor = Color(stof(matstr[3]), stof(matstr[4]), stof(matstr[5]), stof(matstr[6]));
				material->Emission = Color(stof(matstr[7]), stof(matstr[8]), stof(matstr[9]), stof(matstr[10]));
				material->Metalic = stof(matstr[11]);
				material->Roughness = stof(matstr[12]);
				material->MicroFacet = stof(matstr[13]);
				material->Shinness = stof(matstr[14]);
				material->NormalMap_Intensity = stof(matstr[15]);
				index++;
			}
		}
	}

	MaterialPool::UpdateMaterials();
}

PBR_MATERIAL* MaterialPool::GetMaterial(int _index)
{
	if (MaterialList.size() > _index)
	{
		return MaterialList[_index];
	}
	return nullptr;
}

std::string MaterialPool::GetMeshName(int _index)
{
	if (MeshNameList.size() > _index)
	{
		return MeshNameList[_index];
	}
	return "";
}

int MaterialPool::GetMaterialSize()
{
	return MaterialList.size();
}
