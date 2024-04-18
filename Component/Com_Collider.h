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
    // ˆÚ“®‚Ì—L–³
	bool bMovable = false;
    // ”j‰ó‰Â”\
    bool bCanDestroy = false;
    // •¨—”»’è‚ğs‚í‚È‚¢
    bool bIsTrigger = false;
    // “o‚ê‚é
    bool bCanStepOn = false;
    // Œ`
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

    // ƒRƒ‰ƒCƒ_[“o˜^
    void Push(unsigned int _val);
    // “o˜^æ“o˜^
    void SetIndex(unsigned int _val);
    // ƒŠƒXƒg‚Ìnull‰»
    void SetNull();
};