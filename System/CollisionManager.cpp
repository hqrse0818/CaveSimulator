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
	// 2Dと3D別スレッドで行っている
	// 参照が被る場合はシングルスレッドに切り替える
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
	// インデックスの登録
	_collider->SetIndex(mNum[_layer]);
	// インデックスを加算
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
			// 親の取得
			GameObject* pareMe = Colme->p_mObject;
			// アクティブ状態なら続行
			if (pareMe->GetActive())
			{
				for (auto other = std::next(me); other != mColList[0].end(); other++)
				{
					Com_Collider* Colother = *other;
					GameObject* pareOther = Colother->p_mObject;
					// アクティブ状態且つ所持オブジェクトが違うなら続行
					if (pareOther->GetActive() && pareMe != pareOther)
					{
						bool ret = false;
						ret = mCheckers[0]->CheckCollision(Colme, Colother);
						if (ret)
						{
							// 衝突リストに登録
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
			// 親の取得
			GameObject* pareMe = Colme->p_mObject;
			// アクティブ状態なら続行
			if (pareMe->GetActive())
			{
				for (auto other = mColList[1].begin(); other != mColList[1].end(); other++)
				{
					Com_Collider* Colother = *other;
					GameObject* pareOther = Colother->p_mObject;
					// アクティブ状態且つ所持オブジェクトが違うなら続行
					if (pareOther->GetActive() && pareMe != pareOther)
					{
						bool ret = false;
						ret = mCheckers[0]->CheckCollision(Colme, Colother);
						if (ret)
						{
							// 衝突リストに登録
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
			// 親の取得
			GameObject* pareMe = Colme->p_mObject;
			// アクティブ状態なら続行
			if (pareMe->GetActive())
			{
				for (auto other = std::next(me); other != mColList[2].end(); other++)
				{
					Com_Collider* Colother = *other;
					GameObject* pareOther = Colother->p_mObject;
					// アクティブ状態且つ所持オブジェクトが違うなら続行
					if (pareOther->GetActive() && pareMe != pareOther)
					{
						bool ret = false;
						ret = mCheckers[1]->CheckCollision(Colme, Colother);
						if (ret)
						{
							// 衝突リストに登録
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
			// 親の取得
			GameObject* pareMe = Colme->p_mObject;
			// アクティブ状態なら続行
			if (pareMe->GetActive())
			{
				for (auto other = mColList[3].begin(); other != mColList[3].end(); other++)
				{
					Com_Collider* Colother = *other;
					GameObject* pareOther = Colother->p_mObject;
					// アクティブ状態且つ所持オブジェクトが違うなら続行
					if (pareOther->GetActive() && pareMe != pareOther)
					{
						bool ret = false;
						ret = mCheckers[1]->CheckCollision(Colme, Colother);
						if (ret)
						{
							// 衝突リストに登録
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
