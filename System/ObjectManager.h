#pragma once
#include "../GameObject/GameObject.h"
#include <string>

// unorderd_mapによるオブジェクト管理配列
// 高速なアクセスが必要なオブジェクトを登録する
class ObjectManager
{
private:
	Objectmap mObjectmap;

public:
	void AddGameObject(GameObject* _Object)
	{
		// 引数のレイヤーにオブジェクトの名前として登録
		
		// 名前が被っている場合の処理
		if (mObjectmap.count(_Object->sObjectName) > 0)
		{
			// 現在のオブジェクト名の末尾にオブジェクトのIDを付与する
			std::string str = std::to_string(_Object->GetID().first);
			_Object->sObjectName = _Object->sObjectName + "_";
			_Object->sObjectName = _Object->sObjectName + str;
			str = std::to_string(_Object->GetID().second);
			_Object->sObjectName = _Object->sObjectName + "_";
			_Object->sObjectName = _Object->sObjectName + str;
			return;
		}
		mObjectmap[_Object->sObjectName] = _Object;
	}

	GameObject* GetGameObject(std::string _name)
	{
		if (mObjectmap.count(_name) > 0)
		{
			return mObjectmap[_name];
		}
		return nullptr;
	}

	GameObject* GetGameObject(const char* _name)
	{
		if (mObjectmap.count(_name) > 0)
		{
			return mObjectmap[_name];
		}
		return nullptr;
	}
};

