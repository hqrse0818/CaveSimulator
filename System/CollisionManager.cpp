#include "CollisionManager.h"
#include "../GameObject/GameObject.h"

using namespace std;

std::vector<unsigned int> CollisionManager::mNum{};
std::array<std::vector<Com_Collider*>,mNumColliderArray> CollisionManager::mColList{};
std::vector<CollisionChecker*> CollisionManager::mCheckers{};
bool CollisionManager::bReferenceSameObject = false;

void CollisionManager::Init()
{
	mNum.resize(4);
	mNum[0] = 0;
	mNum[1] = 0;
	mNum[2] = 0;
	mNum[3] = 0;
	mCheckers.resize(2);
	mCheckers[0] = new CollisionChecker();
	mCheckers[1] = new CollisionChecker();
}

void CollisionManager::Uninit()
{
	for (auto list : mColList)
	{
		for (auto l : list)
		{
			if (l != nullptr)
			{
				l->Uninit();
				delete l;
				l = nullptr;
			}
		}
		list.clear();
	}

	for (auto check : mCheckers)
	{
		delete check;
	}
	mCheckers.clear();
}

void CollisionManager::ExecuteCollision()
{
	// 2D��3D�ʃX���b�h�ōs���Ă���
	// �Q�Ƃ����ꍇ�̓V���O���X���b�h�ɐ؂�ւ���
	if (!bReferenceSameObject)
	{
		thread th1(Collision3D);
		thread th2(Collision2D);
		th1.join();
		th2.join();
	}
	else
	{
		Collision3D();
		Collision2D();
	}
}

void CollisionManager::Push(int _layer, Com_Collider* _collider)
{
	mColList[_layer].emplace_back(_collider);
	// �C���f�b�N�X�̓o�^
	_collider->SetIndex(mNum[_layer]);
	// �C���f�b�N�X�����Z
	mNum[_layer]++;
}

void CollisionManager::SetNull(Com_Collider* _com)
{
	mColList[_com->iArrayNumber][_com->iIndex] = nullptr;
}

void CollisionManager::Collision3D()
{
	for (auto me = mColList[0].begin(); me != mColList[0].end(); me++)
	{
		if (*me != nullptr)
		{
			Com_Collider* Colme = *me;
			// �e�̎擾
			GameObject* pareMe = Colme->p_mObject;
			// �A�N�e�B�u��ԂȂ瑱�s
			if (pareMe->GetActive())
			{
				for (auto other = std::next(me); other != mColList[0].end(); other++)
				{
					Com_Collider* Colother = *other;
					GameObject* pareOther = Colother->p_mObject;
					// �A�N�e�B�u��Ԋ������I�u�W�F�N�g���Ⴄ�Ȃ瑱�s
					if (pareOther->GetActive() && pareMe != pareOther)
					{
						bool ret = false;
						ret = mCheckers[0]->CheckCollision(Colme, Colother);
						if (ret)
						{
							// �Փ˃��X�g�ɓo�^
							pareMe->mCollisionvector.emplace_back(pareOther);
							pareOther->mCollisionvector.emplace_back(pareMe);
						}
						bool check = false;
						for (auto& n : pareMe->mLastCollisionvector)
						{
							if (n == pareOther)
							{
								if (ret)
								{
									pareMe->OnCollisionStay(pareOther);
									pareOther->OnCollisionStay(pareMe);
									check = true;
									break;
								}
								else
								{
									pareMe->OnCollisionExit(pareOther);
									pareOther->OnCollisionExit(pareMe);
								}
							}
						}
						if (!check && ret)
						{
							pareMe->OnCollisionEnter(pareOther);
							pareOther->OnCollisionEnter(pareMe);
						}
					}
				}
			}
		}
	}

	for (auto me = mColList[0].begin(); me != mColList[0].end(); me++)
	{
		if (*me != nullptr)
		{
			Com_Collider* Colme = *me;
			// �e�̎擾
			GameObject* pareMe = Colme->p_mObject;
			// �A�N�e�B�u��ԂȂ瑱�s
			if (pareMe->GetActive())
			{
				for (auto other = mColList[1].begin(); other != mColList[1].end(); other++)
				{
					Com_Collider* Colother = *other;
					GameObject* pareOther = Colother->p_mObject;
					// �A�N�e�B�u��Ԋ������I�u�W�F�N�g���Ⴄ�Ȃ瑱�s
					if (pareOther->GetActive() && pareMe != pareOther)
					{
						bool ret = false;
						ret = mCheckers[0]->CheckCollision(Colme, Colother);
						if (ret)
						{
							// �Փ˃��X�g�ɓo�^
							pareMe->mCollisionvector.emplace_back(pareOther);
							pareOther->mCollisionvector.emplace_back(pareMe);
						}
						bool check = false;
						for (auto& n : pareMe->mLastCollisionvector)
						{
							if (n == pareOther)
							{
								if (ret)
								{
									pareMe->OnCollisionStay(pareOther);
									pareOther->OnCollisionStay(pareMe);
									check = true;
									break;
								}
								else
								{
									pareMe->OnCollisionExit(pareOther);
									pareOther->OnCollisionExit(pareMe);
								}
							}
						}
						if (!check && ret)
						{
							pareMe->OnCollisionEnter(pareOther);
							pareOther->OnCollisionEnter(pareMe);
						}
					}
				}
			}
		}
	}
}

void CollisionManager::Collision2D()
{
	for (auto me = mColList[2].begin(); me != mColList[2].end(); me++)
	{
		if (*me != nullptr)
		{
			Com_Collider* Colme = *me;
			// �e�̎擾
			GameObject* pareMe = Colme->p_mObject;
			// �A�N�e�B�u��ԂȂ瑱�s
			if (pareMe->GetActive())
			{
				for (auto other = std::next(me); other != mColList[2].end(); other++)
				{
					Com_Collider* Colother = *other;
					GameObject* pareOther = Colother->p_mObject;
					// �A�N�e�B�u��Ԋ������I�u�W�F�N�g���Ⴄ�Ȃ瑱�s
					if (pareOther->GetActive() && pareMe != pareOther)
					{
						bool ret = false;
						ret = mCheckers[1]->CheckCollision(Colme, Colother);
						if (ret)
						{
							// �Փ˃��X�g�ɓo�^
							pareMe->mCollisionvector.emplace_back(pareOther);
							pareOther->mCollisionvector.emplace_back(pareMe);
						}
						bool check = false;
						for (auto& n : pareMe->mLastCollisionvector)
						{
							if (n == pareOther)
							{
								if (ret)
								{
									pareMe->OnCollisionStay(pareOther);
									pareOther->OnCollisionStay(pareMe);
									check = true;
									break;
								}
								else
								{
									pareMe->OnCollisionExit(pareOther);
									pareOther->OnCollisionExit(pareMe);
								}
							}
						}
						if (!check && ret)
						{
							pareMe->OnCollisionEnter(pareOther);
							pareOther->OnCollisionEnter(pareMe);
						}
					}
				}
			}
		}
	}

	for (auto me = mColList[2].begin(); me != mColList[2].end(); me++)
	{
		if (*me != nullptr)
		{
			Com_Collider* Colme = *me;
			// �e�̎擾
			GameObject* pareMe = Colme->p_mObject;
			// �A�N�e�B�u��ԂȂ瑱�s
			if (pareMe->GetActive())
			{
				for (auto other = mColList[3].begin(); other != mColList[3].end(); other++)
				{
					Com_Collider* Colother = *other;
					GameObject* pareOther = Colother->p_mObject;
					// �A�N�e�B�u��Ԋ������I�u�W�F�N�g���Ⴄ�Ȃ瑱�s
					if (pareOther->GetActive() && pareMe != pareOther)
					{
						bool ret = false;
						ret = mCheckers[1]->CheckCollision(Colme, Colother);
						if (ret)
						{
							// �Փ˃��X�g�ɓo�^
							pareMe->mCollisionvector.emplace_back(pareOther);
							pareOther->mCollisionvector.emplace_back(pareMe);
						}
						bool check = false;
						for (auto& n : pareMe->mLastCollisionvector)
						{
							if (n == pareOther)
							{
								if (ret)
								{
									pareMe->OnCollisionStay(pareOther);
									pareOther->OnCollisionStay(pareMe);
									check = true;
									break;
								}
								else
								{
									pareMe->OnCollisionExit(pareOther);
									pareOther->OnCollisionExit(pareMe);
								}
							}
						}
						if (!check && ret)
						{
							pareMe->OnCollisionEnter(pareOther);
							pareOther->OnCollisionEnter(pareMe);
						}
					}
				}
			}
		}
	}
}

void CollisionManager::SetReferenceSameObj(bool _val)
{
	bReferenceSameObject = _val;
}
