#pragma once
#include "Com_Visual.h"
#include "Com_Camera.h"
#include "../Utility.h"
#include "../DirectX/ShaderResourceView.h"

constexpr int BillBoardVertexNum = 4;

// �r���{�[�h�R���|�[�l���g
class Com_BillBoard :
    public Com_Visual
    {
    private:
        // �J�����ւ̃|�C���^
        static Com_Camera* p_TargetCamera;

        static ID3D11Buffer* m_VertexBuffer;
        ShaderResourceView* m_Texture{};

        DirectX::SimpleMath::Vector2 mSize = DirectX::SimpleMath::Vector2(10.0f, 10.0f);

        DirectX::SimpleMath::Color mColor = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);

        // UV�Z�o
        float fCurrentX;
        float fCurrentY;
        float fCurrentZ;
        float fCurrentW;
        // ������
        int iSeparateU = 1;
        int iSeparateV = 1;
        // �\���ʒu
        int iCurrent = 1;

        bool bRelease = true;

        // �J�����Ɋ�Â��������̍X�V��������
        static bool bUpdated;
        static DirectX::SimpleMath::Matrix mViewMtx;
        static DirectX::SimpleMath::Matrix mInvertViewMtx;

        MATERIAL mMaterial{};

    public:
        // �^�[�Q�b�g�Ƃ���J�����̐ݒ�
        static void SetCamera(Com_Camera* _cam)
        {
            p_TargetCamera = _cam;
        }
        // ������
        static void InitMain();
        // ���
        static void UninitMain();

    public:
        void Update()override;
        void Draw()override;

        // UV�������̐ݒ�
        void SetSeparate(int _u, int _v)
        {
            if (_u != 0)
                iSeparateU = _u;
            if (_v != 0)
                iSeparateV = _v;
        }
        // �\���ʒu�̐ݒ�
        void SetCurrent(int _val);
        // �e�N�X�`���̉��bool�̐ݒ�
        void SetRelease(bool _val = false)
        {
            bRelease = _val;
        }
        // �e�N�X�`���̉��
        void ReleaseTexture()
        {
            m_Texture->Uninit();
            SAFE_NULL(m_Texture);
        }

        // �e�N�X�`���̐ݒ�
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

