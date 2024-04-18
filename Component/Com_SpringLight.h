#pragma once
#include "Component.h"
#include "../Format/Light.h"

// �o�l�̓��������郉�C�g

constexpr float AccelaratePermissionVal = 0.1f;

class Com_SpringLight :
    public Component
{
private:    // �ϐ�
    enum class SpringState
    {
        Start, Do, End
    };
    PointLight* p_mLight;
    // �����ʒu(���R�������Z�����)
    DirectX::SimpleMath::Vector3 mInitPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
    // �x�N�g��
    DirectX::SimpleMath::Vector3 mVelocity{};
    // �d��
    DirectX::SimpleMath::Vector3 mGravity{};
    // ���\���ʒu
    DirectX::SimpleMath::Vector3 mAxis{};
    // �o�l�̃X�e�[�g
    SpringState mState = SpringState::Start;

    // �o�l�̎��R��
    float mSpringLength = 30.0f;
    // ���������̂̎���
    float mMass = 10.0f;
    // �o�l�萔
    float mSpringCoefficient = 5.0f;
    // ��R�W��
    float mAtt = 3.0f;

public: // �ŗL�֐�
    Com_SpringLight();

    // �����ʒu�ݒ�
    void SetInitPos(float _x, float _y, float _z)
    {
        mInitPos.x = _x;
        mInitPos.y = _y;
        mInitPos.z = _z;
    }
    // �d�͂̃Z�b�g
    void SetGravity(float _x, float _y, float _z)
    {
        mGravity.x = _x;
        mGravity.y = _y;
        mGravity.z = _z;
    }
    // �|�W�V�������ʒu�̃Z�b�g
    void SetAxis(float _x, float _y, float _z)
    {
        mAxis.x = _x;
        mAxis.y = _y;
        mAxis.z = _z;
    }
    // ���C�g�̃J���[�̃Z�b�g
    void SetColor(float _r, float _g, float _b)
    {
        p_mLight->Diffuse.x = _r;
        p_mLight->Diffuse.y = _g;
        p_mLight->Diffuse.z = _b;
    }
    // ���C�g�͈̔͂̃Z�b�g
    void SetRange(float _val)
    {
        p_mLight->Position.w = _val;
    }
    // ���C�g�̋����̃Z�b�g
    void SetLightPower(float _val)
    {
        p_mLight->Diffuse.w = _val;
    }
    // �o�l�̎��R���̃Z�b�g
    void SetSpringLength(float _val)
    {
        mSpringLength = _val;
    }
    // ���̂̏d���̃Z�b�g
    void SetMass(float _val)
    {
        mMass = _val;
    }
    // �o�l�萔�̃Z�b�g
    void SetSpringCoefficient(float _val)
    {
        mSpringCoefficient = _val;
    }
    //�@��R�W���̃Z�b�g
    void SetAtt(float _val)
    {
        mAtt = _val;
    }

protected:
    void SpringStart();
    void SpringUpdate();

public: // �p���֐�
    void Init()override;
    void Update()override;
};

