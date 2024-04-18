#include "LightPool.h"
#include "../Utility.h"
#include "../DirectX/renderer.h"
#include "imguiManager.h"
#include "FileOperation.h"
#include "../Scene/Scene.h"
#include "../System/manager.h"

std::vector<PointLight*> LightPool::mPointLightList{};
std::vector<SpotLight*> LightPool::mSpotLightList{};
DirectionalLight* LightPool::pDirectionLight = nullptr;
ID3D11Buffer* LightPool::p_mDirectionalLightBuf{};
ID3D11Buffer* LightPool::p_mPointLightsBuf{};
ID3D11Buffer* LightPool::p_mSpotLightsBuf{};

using namespace DirectX::SimpleMath;
using namespace std;

HRESULT LightPool::CreatePointLightBuffer()
{
	return CreateConstantBufferWrite(Renderer::GetDevice(), sizeof(PointLights), sizeof(float), &p_mPointLightsBuf);
}

void LightPool::Push(PointLight* _light)
{
	mPointLightList.push_back(_light);
}

void LightPool::UpdatePointLights()
{
	// ポイントライトを一斉更新
	D3D11_MAPPED_SUBRESOURCE ms;
	PointLights* pData = nullptr;

	std::vector<PointLight> vecLights;
	vecLights.resize(mPointLightList.size());
	int index = 0;
	for (auto lig : mPointLightList)
	{
		vecLights[index] = *lig;
		index++;
	}


	HRESULT hr = Renderer::GetDeviceContext()->Map(
		p_mPointLightsBuf,
		0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	if (SUCCEEDED(hr))
	{
		pData = (PointLights*)ms.pData;
		memcpy(pData->lights,
			vecLights.data(),
			sizeof(PointLight) * vecLights.size());
		Renderer::GetDeviceContext()->Unmap(p_mPointLightsBuf, 0);
	}
	else
	{
		NEWDEBUGLOG("ポイントライト定数バッファ更新失敗");
	}

	//Renderer::GetDeviceContext()->UpdateSubresource(p_mPointLightsBuf, 0, NULL, mPointLightList[0], 0, 0);
}

void LightPool::SetPointLightsBuffer(int _slot)
{
	Renderer::GetDeviceContext()->VSSetConstantBuffers(_slot, 1, &p_mPointLightsBuf);
	Renderer::GetDeviceContext()->PSSetConstantBuffers(_slot, 1, &p_mPointLightsBuf);
}

void LightPool::ReleasePointLight()
{
	for (auto lig : mPointLightList)
	{
		SAFE_NULL(lig);
	}
	mPointLightList.clear();
}

HRESULT LightPool::CreateSpotLightBuffer()
{
	return CreateConstantBufferWrite(Renderer::GetDevice(), sizeof(SpotLights), sizeof(float),&p_mSpotLightsBuf);
}

void LightPool::Push(SpotLight* _light)
{
	mSpotLightList.push_back(_light);
}

void LightPool::UpdateSpotLights()
{
	// スポットライトの一斉更新
	D3D11_MAPPED_SUBRESOURCE ms{};
	SpotLights* pData = nullptr;

	std::vector<SpotLight> lights;
	lights.resize(mSpotLightList.size());
	int index = 0;
	for (auto lig : mSpotLightList)
	{
		lights[index] = *lig;
		index++;
	}

	HRESULT  hr = Renderer::GetDeviceContext()->Map(p_mSpotLightsBuf,
		0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	if (SUCCEEDED(hr))
	{
		pData = (SpotLights*)ms.pData;
		memcpy(pData->lights,
			lights.data(),
			sizeof(SpotLight) * lights.size());
		Renderer::GetDeviceContext()->Unmap(p_mSpotLightsBuf, 0);
	}
	else
	{
		NEWDEBUGLOG("スポットライト定数バッファ更新失敗");
	}
}

void LightPool::SetSpotLightBuffer(int _slot)
{
	Renderer::GetDeviceContext()->VSSetConstantBuffers(_slot, 1, &p_mSpotLightsBuf);
	Renderer::GetDeviceContext()->PSSetConstantBuffers(_slot, 1, &p_mSpotLightsBuf);
}

void LightPool::ReleaseSpotLight()
{
	for (auto lig : mSpotLightList)
	{
		SAFE_NULL(lig);
	}
}

void LightPool::UpdateAllLights()
{
	UpdatePointLights();
	UpdateSpotLights();
	UpdateDirectionalLight();
}

DirectionalLight* LightPool::GetDirectioLight()
{
	if (pDirectionLight)
	{
		return pDirectionLight;
	}
	return nullptr;
}

void LightPool::UpdateDirectionalLight()
{
	// ポイントライトの数を設定
	pDirectionLight->PointLightNum = static_cast<int>(mPointLightList.size());
	pDirectionLight->SpotLightNum = static_cast<int>(mSpotLightList.size());
	Renderer::GetDeviceContext()->UpdateSubresource(p_mDirectionalLightBuf, 0, NULL, pDirectionLight, 0, 0);
}

HRESULT LightPool::CreateDirectionalLight()
{
	if (pDirectionLight)
	{
		SAFE_NULL(pDirectionLight);
	}

	pDirectionLight = new DirectionalLight();

	return CreateConstantBuffer(Renderer::GetDevice(), sizeof(DirectionalLight), sizeof(float), &p_mDirectionalLightBuf);
}

void LightPool::SetDirectionalLightBuffer()
{
	Renderer::GetDeviceContext()->VSSetConstantBuffers(2, 1, &p_mDirectionalLightBuf);
	Renderer::GetDeviceContext()->PSSetConstantBuffers(2, 1, &p_mDirectionalLightBuf);
}

void LightPool::Uninit()
{
	for (auto l : mPointLightList)
	{
		SAFE_NULL(l);
	}
	SAFE_NULL(pDirectionLight);

	SAFE_RELEASE(p_mPointLightsBuf);
	SAFE_RELEASE(p_mDirectionalLightBuf);
}

void LightPool::ShowLightGui()
{
	float GUIPosX = 400.0f;
	float GUISizeX = 400.0f;

	ImGui::SetNextWindowPos(ImVec2(GUIPosX, static_cast<float>(MAX_SPRITE_HEIGHT)), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(GUISizeX, static_cast<float>(SCREEN_HEIGHT - MAX_SPRITE_HEIGHT)), ImGuiCond_Once);
	ImGui::Begin("Light");

	// ファイルセーブ
	if (ImGui::Button("Save"))
	{
		string filename = "asset/data/csv/";
		filename += Manager::GetScene()->GetSceneName();
		filename += "LightData.csv";
		SaveLight(filename.c_str());
		GUI::PushLog("Saved Light");
	}

	if (ImGui::TreeNode("DirectionalLight"))
	{
		ImGui::SliderFloat4("Direction", (float*)&pDirectionLight->Direction, -1.0f, 1.0f);
		pDirectionLight->Direction.Normalize();
		ImGui::ColorPicker3("Diffuse", (float*)&pDirectionLight->Diffuse);
		ImGui::InputFloat("LightPower", &pDirectionLight->Diffuse.w);
		ImGui::ColorPicker4("Ambient", (float*)&pDirectionLight->Ambient);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("PointLight"))
	{
		// ポイントライト閾値変数
		static float MaxPLPower = 50.0f;
		if (ImGui::Button("Create"))
		{
			PointLight* light = new PointLight();
			// ライトの強さを5で生成
			light->Position = Vector4(0.0f, 0.0f, 0.0f, 5.0f);
			light->Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
			Push(light);
		}
		if (ImGui::Button("Remove"))
		{
			if (!mPointLightList.empty())
			{
				delete mPointLightList.back();
				mPointLightList.pop_back();
			}
		}

		for (int i = 0; i < mPointLightList.size(); i++)
		{
			string name = "PointLight" + to_string(i);
			if (ImGui::TreeNode(name.c_str()))
			{
				string pos = "Position" + to_string(i);
				string range = "Range" + to_string(i);
				string col = "Color" + to_string(i);
				string power = "Power" + to_string(i);
				ImGui::InputFloat3(pos.c_str(), (float*)&(mPointLightList[i]->Position));
				ImGui::InputFloat(range.c_str(), &mPointLightList[i]->Position.w);
				ImGui::ColorPicker3(col.c_str(), (float*)&(mPointLightList[i]->Diffuse));
				ImGui::SliderFloat(power.c_str(), &mPointLightList[i]->Diffuse.w, 0.0f, MaxPLPower);
				if (ImGui::Button("Copy"))
				{
					PointLight* light = new PointLight();
					light->Position = mPointLightList[i]->Position;
					light->Diffuse = mPointLightList[i]->Diffuse;
					Push(light);
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("SpotLight"))
	{
		// スポットライト閾値変数
		static float MaxRange = 100.0f;
		static float MaxAngle = 90.0f;

		if (ImGui::Button("Create"))
		{
			SpotLight* light = new SpotLight();
			light->Position = Vector3(0.0f, 0.0f, 0.0f);
			light->Direction = Vector3(0.0f, -1.0f, 0.0f);
			light->Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
			light->angle = 0.0f;
			light->range = 50.0f;
			Push(light);
		}

		for (int i = 0; i < mSpotLightList.size(); i++)
		{
			string name = "SpotLight" + to_string(i);
			if (ImGui::TreeNode(name.c_str()))
			{
				string pos = "Position" + to_string(i);
				string direnction = "Direction" + to_string(i);
				string range = "Range" + to_string(i);
				string angle = "Angle" + to_string(i);
				string col = "Color" + to_string(i);
				string power = "Power" + to_string(i);

				ImGui::InputFloat3(pos.c_str(), (float*)&(mSpotLightList[i]->Position));
				ImGui::SliderFloat3(direnction.c_str(), (float*)&(mSpotLightList[i]->Direction), -1.0f, 1.0f);
				mSpotLightList[i]->Direction.Normalize();
				ImGui::SliderFloat(range.c_str(), &mSpotLightList[i]->range, 0.0f, MaxRange);
				ImGui::SliderFloat(angle.c_str(), &mSpotLightList[i]->angle, -MaxAngle, MaxAngle);
				ImGui::ColorPicker3(col.c_str(), (float*)&(mSpotLightList[i]->Diffuse));

				ImGui::TreePop();
			}
			
		}

		ImGui::TreePop();


	}

	ImGui::End();
}

void LightPool::SaveLight(const char* _FileName)
{
	FileOperator* file = new FileOperator(_FileName, true);
	// 平行光源
	string DHeader = "exp,PosX,PosY,PosZ,DirX,DirY,DirZ,DiffuseR,DiffuseG,DiffuseB,LPower,AmbientR,AmbientG,AmbientB,AmbientA,BaseR,BaseG,BaseB,BaseA";
	string DForm = "type,float,float,flat,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float";
	file->Write(DHeader, false);
	file->Write(DForm);
	file->Write("Directional,");
	file->Write(pDirectionLight->mPosition.x, false);
	file->Write(",", false);
	file->Write(pDirectionLight->mPosition.y, false);
	file->Write(",", false);
	file->Write(pDirectionLight->mPosition.z, false);
	file->Write(",", false);
	file->Write(pDirectionLight->Direction.x, false);
	file->Write(",", false);
	file->Write(pDirectionLight->Direction.y, false);
	file->Write(",", false);
	file->Write(pDirectionLight->Direction.z, false);
	file->Write(",", false);
	file->Write(pDirectionLight->Diffuse.x, false);
	file->Write(",", false);
	file->Write(pDirectionLight->Diffuse.y, false);
	file->Write(",", false);
	file->Write(pDirectionLight->Diffuse.z, false);
	file->Write(",", false);
	file->Write(pDirectionLight->Diffuse.w, false);
	file->Write(",", false);
	file->Write(pDirectionLight->Ambient.x, false);
	file->Write(",", false);
	file->Write(pDirectionLight->Ambient.y, false);
	file->Write(",", false);
	file->Write(pDirectionLight->Ambient.z, false);
	file->Write(",", false);
	file->Write(pDirectionLight->BaseColor.x, false);
	file->Write(",", false);
	file->Write(pDirectionLight->BaseColor.y, false);
	file->Write(",", false);
	file->Write(pDirectionLight->BaseColor.z, false);

	string PHeader = "exp,Posx,Posy,Posz,Range,R,G,B,LightPower";
	string PForm = "type,float,float,float,float,float,float,float,float";
	file->Write(PHeader);
	file->Write(PForm);
	for (int i = 0; i < mPointLightList.size(); i++)
	{
		file->Write("Point,");
		file->Write(mPointLightList[i]->Position.x, false);
		file->Write(",", false);
		file->Write(mPointLightList[i]->Position.y, false);
		file->Write(",", false);
		file->Write(mPointLightList[i]->Position.z, false);
		file->Write(",", false);
		file->Write(mPointLightList[i]->Position.w, false);
		file->Write(",", false);
		file->Write(mPointLightList[i]->Diffuse.x, false);
		file->Write(",", false);
		file->Write(mPointLightList[i]->Diffuse.y, false);
		file->Write(",", false);
		file->Write(mPointLightList[i]->Diffuse.z, false);
		file->Write(",", false);
		file->Write(mPointLightList[i]->Diffuse.w, false);
	}

	string SHeader = "exp,PosX,PosY,PosZ,R,G,B,LightPower,Range,DirX,DirY,DirZ,Angle";
	string SForm = "type,float,float,float,float,float,float,float,float,float,float,float";
	file->Write(SHeader);
	file->Write(SForm);
	for (int i = 0; i < mSpotLightList.size(); i++)
	{
		file->Write("Spot,");
		file->Write(mSpotLightList[i]->Position.x, false);
		file->Write(",", false);
		file->Write(mSpotLightList[i]->Position.y, false);
		file->Write(",", false);
		file->Write(mSpotLightList[i]->Position.z, false);
		file->Write(",", false);
		file->Write(mSpotLightList[i]->Diffuse.x, false);
		file->Write(",", false);
		file->Write(mSpotLightList[i]->Diffuse.y, false);
		file->Write(",", false);
		file->Write(mSpotLightList[i]->Diffuse.z, false);
		file->Write(",", false);
		file->Write(mSpotLightList[i]->Diffuse.w, false);
		file->Write(",", false);
		file->Write(mSpotLightList[i]->range, false);
		file->Write(",", false);
		file->Write(mSpotLightList[i]->Direction.x, false);
		file->Write(",", false);
		file->Write(mSpotLightList[i]->Direction.y, false);
		file->Write(",", false);
		file->Write(mSpotLightList[i]->Direction.z, false);
		file->Write(",", false);
		file->Write(mSpotLightList[i]->angle, false);
	}

	file->Close();
	delete file;
}
