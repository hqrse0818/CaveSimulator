#pragma once
#include "Com_Mesh.h"
#include "../System/StaticMeshLoader.h"

class Com_StaticMesh :
    public Com_Mesh
{
private:
    static int mNumModels;
    static int mNumTotalVertices;

private:
    int mNumVertices = 0;
    STATICMESH_DATA* mModelData = nullptr;
    bool bUseMaterial = false;
public:
    Com_StaticMesh()
    {
        mNumModels++;
    }
    void Draw();
    void ShadowDraw()override;

    void Uninit();

    bool SetModelData(const char* _ModelName);
    void SetUseMaterial(bool _b)
    {
        bUseMaterial = _b;
    }

    static int GetModelNum()
    {
        return mNumModels;
    }

    static int GetVertexNum()
    {
        return mNumTotalVertices;
    }
};

