#include "Com_Collider.h"
#include "../System/CollisionManager.h"

using namespace std;

void Com_Collider::Uninit()
{
	SetNull();
}

void Com_Collider::Push(unsigned int _val)
{
	iArrayNumber = _val;
	CollisionManager::Push(_val, this);
}

void Com_Collider::SetIndex(unsigned int _val)
{
	iIndex = _val;
}

void Com_Collider::SetNull()
{
	CollisionManager::SetNull(this);
}
