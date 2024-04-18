#pragma once
#include "Com_Visual.h"
#include "Com_Camera.h"
#include "../Utility.h"
#include "../DirectX/ShaderResourceView.h"

constexpr int BillBoardVertexNum = 4;

// ビルボードコンポーネント
class Com_BillBoard :
    public Com_Visual
    {
    private:
        // カメラへのポインタ
        static Com_Camera* p_TargetCamera;

        static ID3D11Buffer* m_VertexBuffer;
        ShaderResourceView* m_Texture{};

        DirectX::SimpleMath::Vector2 mSize = DirectX::SimpleMath::Vector2(10.0f, 10.0f);

        DirectX::SimpleMath::Color mColor = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);

        // UV算出
        float fCurrentX;
        float fCurrentY;
        float fCurrentZ;
        float fCurrentW;
        // 分割数
        int iSeparateU = 1;
        int iSeparateV = 1;
        // 表示位置
        int iCurrent = 1;

        bool bRelease = true;

        // カメラに基づいた向きの更新をしたか
        static bool bUpdated;
        static DirectX::SimpleMath::Matrix mViewMtx;
        static DirectX::SimpleMath::Matrix mInvertViewMtx;

        MATERIAL mMaterial{};

    public:
        // ターゲットとするカメラの設定
        static void SetCamera(Com_Camera* _cam)
        {
            p_TargetCamera = _cam;
        }
        // 初期化
        static void InitMain();
        // 解放
        static void UninitMain();

    public:
        void Update()override;
        void Draw()override;

        // UV分割数の設定
        void SetSeparate(int _u, int _v)
        {
            if (_u != 0)
                iSeparateU = _u;
            if (_v != 0)
                iSeparateV = _v;
        }
        // 表示位置の設定
        void SetCurrent(int _val);
        // テクスチャの解放boolの設定
        void SetRelease(bool _val = false)
        {
            bRelease = _val;
        }
        // テクスチャの解放
        void ReleaseTexture()
        {
            m_Texture->Uninit();
            SAFE_NULL(m_Texture);
        }

        // テクスチャの設定
        void SetTexture(const char* _FileName);
        void SetTexture(ShaderResourceView* _psrv);

        void SetSize(float _x, float _y)
        {
            mSize.x = _x;
            mSize.y = _y;
        }

        void SetColor(float _r, float _g, float _b, float _a)
        {
            mColor.x = _r;
            mColor.y = _g;
            mColor.z = _b;
            mColor.w = _a;
        }

        ShaderResourceView* GetSRV()
        {
            return m_Texture;
        }

        void SetAlbedoColor(float _r, float _g, float _b, float _a)
        {
            mMaterial.AlbedoColor.x = _r;
            mMaterial.AlbedoColor.y = _g;
            mMaterial.AlbedoColor.z = _b;
            mMaterial.AlbedoColor.w = _a;
        }
    };

