#include "StaticModelPool.h"
#include "manager.h"
#include "../Scene/Scene.h"
#include "imguiManager.h"
#include "../main.h"
#include "FileOperation.h"

std::unordered_map<std::string, StaticMeshLoader*> StaticModelPool::p_mModelmap{};
std::list<LoadThread*>StaticModelPool::ThreadList{};

using namespace std;

STATICMESH_DATA* StaticModelPool::GetModelData(const char* _ModelName)
{
    if (p_mModelmap.count(_ModelName) > 0)
    {
        return p_mModelmap[_ModelName]->GetModelData();
    }

    return nullptr;
}

StaticMeshLoader* StaticModelPool::GetLoaderData(const char* _Name)
{
    if (p_mModelmap.count(_Name) > 0)
    {
        return p_mModelmap[_Name];
    }
    return nullptr;
}

void StaticModelPool::ShowStaticModelGui()
{
    constexpr unsigned int stringbuffer = 30;
    static float GUIPosY = 520.0f;

    ImGui::SetNextWindowPos(ImVec2(static_cast<float>(MAX_SPRITE_WIDTH), GUIPosY), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(SCREEN_WIDTH - MAX_SPRITE_WIDTH), static_cast<float>(SCREEN_HEIGHT) - GUIPosY), ImGuiCond_Once);

    ImGui::Begin("StaticModelList");
    
    if (ImGui::CollapsingHeader("LoadModel"))
    {
        // ���[�h���郂�f���̃t�@�C����
        static char FileName[] = "asset\\model\\examplefolder\\examplefilename.fbx";
        ImGui::InputText("FileName", FileName, sizeof(FileName) + stringbuffer);
        // ���[�h���郂�f���̃e�N�X�`���Ɋ��蓖�Ă�擪�̖��O
        static char TextureName[] = "ExampleTextureName";
        ImGui::InputText("TextureName", TextureName, sizeof(TextureName) + stringbuffer);
        // ���[�h���郂�f���Ɋ��蓖�Ă郂�f����
        static char ModelName[] = "ExampleModelName";
        ImGui::InputText("ModelName", ModelName, sizeof(ModelName) + stringbuffer);

        // ���f�����[�h�㑦�\������ۂɎg�p����
        //// ���[�h���郂�f���Ɏg��VertexShader
        //static char VSName[] = "VS_GBufferPBR";
        //ImGui::InputText("VertexShader", VSName, sizeof(VSName) + stringbuffer);
        //// ���[�h���郂�f���Ɏg��PixelShader
        //static char PSName[] = "PS_GBufferPBR";
        //ImGui::InputText("PixelShader", PSName, sizeof(PSName) + stringbuffer);

        if (ImGui::Button("Load"))
        {
             LoadThread* newTh = new LoadThread();
             newTh->bWorking = true;
             newTh->ModelName = ModelName;
             newTh->FileName = FileName;
             newTh->TextureName = TextureName;
             ThreadList.push_back(newTh);
             newTh->pLoader = new StaticMeshLoader();
             newTh->th = std::thread(&StaticMeshLoader::LoadModelPBR, newTh->pLoader, FileName, TextureName, true);
        }

        if (ThreadList.size() > 0)
        {
            for (auto th : ThreadList)
            {
                if (th->th.joinable())
                {
                    th->th.join();
                    bool result = Push(th->ModelName.c_str(), th->pLoader);
                    int index = 1;
                    std::string OriginName = th->ModelName;
                    while (!result)
                    {
                        // ���f���l�[���ɃC���f�b�N�X��t��(���f�������d�Ȃ�Ȃ��悤��)
                        th->ModelName = OriginName + to_string(index);
                        result = Push(th->ModelName.c_str(), th->pLoader);
                        index++;
                    }
                    string log = "registered Model ";
                    log += th->ModelName;
                    GUI::PushLog(log);
                    FileOperator* File = new FileOperator("asset/data/csv/LoadModelList.csv", false);
                    File->Write("data,");
                    File->Write(th->FileName, false);
                    File->Write(",", false);
                    File->Write(th->ModelName, false);
                    File->Write(",", false);
                    File->Write(th->TextureName, false);
                    File->Close();
                    delete File;
                }
            }
        }

    }

    // ���X�g�̕\���ƃI�u�W�F�N�g�̓o�^
    if (ImGui::CollapsingHeader("ModelList"))
    {
        static int Layer = 1;
        ImGui::InputInt("Layer", &Layer);

        static char VSName[] = "VS_GBufferPBR";
        static char PSName[] = "PS_GBufferPBR";

        ImGui::InputText("VertexShader", VSName, sizeof(VSName) + stringbuffer);
        ImGui::InputText("PixelShader", PSName, sizeof(PSName) + stringbuffer);

        for (auto& obj : p_mModelmap)
        {
            string name = obj.first.c_str();
            if (ImGui::TreeNode(name.c_str()))
            {
                ImGui::Text("VertexNum : %d", obj.second->GetVertexNum());

                if (ImGui::Button("Add"))
                {
                    GameObject* Obj = new GameObject("Created");
                    Com_StaticMesh* MeshBuf = new Com_StaticMesh();
                    MeshBuf->SetModelData(name.c_str());
                    MeshBuf->SetVertex(VSName);
                    MeshBuf->SetPixel(PSName);
                    Obj->AddComponent(MeshBuf);
                    Manager::GetScene()->AddGameObject(Obj, Layer);
                    string log = name.c_str();
                    log += " Add";
                    GUI::PushLog(log);
                }
                ImGui::TreePop();
            }
        }
    }

    ImGui::End();
}

void StaticModelPool::Uninit()
{
    for (auto data : p_mModelmap)
    {
        data.second->Uninit();
        delete data.second;
    }

    for (auto Lo : ThreadList)
    {
        delete Lo;
    }
    p_mModelmap.clear();
}

bool StaticModelPool::Push(const char* _Name, StaticMeshLoader* _Data)
{
    if (p_mModelmap.count(_Name) > 0)
    {
        NEWDEBUGLOG("���ꖼ�̃��f�����o�^����悤�Ƃ��܂���");
        GUI::PushLog("An attempt was made to register a model with the same name");
        return false;
    }
    p_mModelmap[_Name] = _Data;
    return true;
}
