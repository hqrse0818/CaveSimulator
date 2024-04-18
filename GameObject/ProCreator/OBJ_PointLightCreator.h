#pragma once
#include "../GameObject.h"
#include "../../Format.h"
class OBJ_PointLightCreator :
    public GameObject
{
public:
    OBJ_PointLightCreator();
    OBJ_PointLightCreator(const char* _Name);
    OBJ_PointLightCreator(const char* _Name, const char* _FileName);
};

class OBJ_CustomPointLightCreator :
    public GameObject
{
private:
    std::vector<CustomPointLight*> mLightList;
public:
    OBJ_CustomPointLightCreator();
    OBJ_CustomPointLightCreator(const char* _Name);
    OBJ_CustomPointLightCreator(const char* _Name, const char* _FileName);

    void Update();
    void Uninit()override;
};