//#pragma once
//#include "Scene.h"
//#include "../main.h"
//#include "../GameObject/OBJ_Creator.h"
//#include "../GameObject/OBJ_Player.h"
//
//class Scene_PBR :
//    public Scene
//{
//public:
//    enum class DrawType
//    {
//        GBuffer,
//        Deffered,
//        Max,
//    };
//
//private:
//    DirectX::SimpleMath::Vector3 mTargetPosition = DirectX::SimpleMath::Vector3(0.0f,0.0f, 10.0f);
//    OBJ_Player* Player{};
//
//    GameObject* pNormal;
//    GameObject* pAlbedo;
//    GameObject* pWorld;
//    GameObject* pMaterial;
//    GameObject* pPBR;
//    Com_Sprite* pNormalSprite;
//    Com_Sprite* pAlbedoSprite;
//    Com_Sprite* pWorldSprite;
//    Com_Sprite* pMaterialSprite;
//    Com_Sprite* pPBRSprite;
//
//    GameObject* pTitleMoji;
//    Com_Sprite* pTitleSprite;
//
//    DrawType mDrawWhich = DrawType::GBuffer;
//
//    PointLight* SceneLight;
//
//    bool bShowSceneData = false;
//    bool bShowObjectData = true;
//
//    float fCameraAngle = 0.0f;
//
//    float fTime = 0.0f;
//
//    bool bRange = true;
//    float fRange =50.0f;
//    float fMaxRange = 100.0f;
//    float fMinRange = 20.0f;
//    bool bPower = true;
//    float fPower = 3.0f;
//    float fMaxPower = 10.0f;
//    float fMinPower = 1.0f;
//
//public:
//    void Init();
//    void Start();
//    void Update();
//    void Draw2D() override;
//    void Uninit();
//
//private:
//    void ReadLevelData(const char* _FileName);
//    void ReadMaterialData(const char* _FileName);
//    void SceneGUIUpdate();
//};
//
