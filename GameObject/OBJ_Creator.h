#pragma once
#include "GameObject.h"

#define StartLine (2)

class OBJ_Creator :
    public GameObject
{
private:
    std::vector<GameObject*> mObjList;

public:
    OBJ_Creator();
    OBJ_Creator(const char* _FileName, const char* _ModelName);
    OBJ_Creator(const char* _FileName, const char* _ModelName, bool _usematerial);
    OBJ_Creator(const char* _FileName, const char* _ModelName, const char* _VSName, const char* _PSName);

    void Init();
    void Start();
    void Update();
    void Draw();
    void ShadowDraw()override;
    void Uninit();
};

