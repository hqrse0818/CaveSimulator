#pragma once
#include "GameObject.h"
#include "../Component/Com_Shader.h"
#include "../Component/Com_StaticMesh.h"

class OBJ_RockA :
    public GameObject
{
private:
    Com_StaticMesh* p_mModelCom = nullptr;

public:
    OBJ_RockA();
    OBJ_RockA(const char* _name);

    Com_StaticMesh* GetModelCom()
    {
        return p_mModelCom;
    }
};

