#pragma once
#include "Com_Visual.h"
#include "../Utility.h"
#include "../DirectX/renderer.h"
#include "../DirectX/Shader.h"

// コライダーラインの表示コンポーネント
class Com_RenderCollider :
    public Com_Visual
{
protected:
    // 頂点バッファ
    ID3D11Buffer* p_mVertexBuffer{};

public:
    Com_RenderCollider();
    virtual void Uninit();
    virtual void Draw();
    void SetPixelShaderNoHit();
    void SetPixelShaderHit();
};

