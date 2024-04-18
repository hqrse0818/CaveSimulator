#pragma once
#include "Com_Visual.h"
#include "../Utility.h"

constexpr int VertexInFace = 3;

class Com_Mesh :
    public Com_Visual
{
protected:
    ID3D11Buffer* p_mScaleBuffer{};
    std::string mModelName;
    ShaderResourceView* p_mNormalMap{};
    bool bApplyNormalMap = false;
public:
    Com_Mesh();
    virtual bool SetModelData(const char* _name) = 0;

    void Bind();
    virtual void Uninit();
    std::string GetModelName()
    {
        return mModelName;
    }

    void SetNormalMap(const char* _FileName);
};

