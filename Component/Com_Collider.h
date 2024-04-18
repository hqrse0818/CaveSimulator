#pragma once
#include "Component.h"
#include "../Collider/Collider.h"

enum class ColColliderForm
{
    None, Box, Box2D, Sphere, Circle, Cylinder, Point3D, Point2D
};

class Com_Collider : public Component
{
protected:
	bool bHit = false;

public :
    // �ړ��̗L��
	bool bMovable = false;
    // �j��\
    bool bCanDestroy = false;
    // ����������s��Ȃ�
    bool bIsTrigger = false;
    // �o���
    bool bCanStepOn = false;
    // �`
    ColColliderForm mType = ColColliderForm::None;

    unsigned int iArrayNumber;
    unsigned int iIndex;

public:
    virtual void Uninit();

    bool GetHit()
    {
        return bHit;
    }
    bool SetHit(bool _b)
    {
        bHit = _b;
    }

    virtual Collider* GetCollider() 
    {
        return nullptr;
    };

    // �R���C�_�[�o�^
    void Push(unsigned int _val);
    // �o�^��o�^
    void SetIndex(unsigned int _val);
    // ���X�g��null��
    void SetNull();
};