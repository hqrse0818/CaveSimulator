#pragma once
#include "../Component/Com_Collider.h"
#include "Collision.h"

#include <vector>
#include <array>
// 当たり判定用マネージャー
// 対応array
// 0 Movable3D, 1 Static3D, 2 Movable2D, 3 Static2D
constexpr int mNumColliderArray = 4;

class CollisionManager
{
private:
	// 判定するオブジェクトリスト
	static std::array<std::vector<Com_Collider*>, mNumColliderArray> mColList;
	// コンポーネントに付加するArrayのインデックス
	static std::vector<unsigned int> mNum;
	// チェッカーオブジェクトのリスト
	static std::vector<CollisionChecker*> mCheckers;

	static bool bReferenceSameObject;
public:
	// 初期化
	static void Init();
	// 解放
	static void Uninit();

	// 判定の実行
	static void ExecuteCollision();
	// オブジェクトの登録
	static void Push(int _layer, Com_Collider* _collider);
	// リスト取得
	static std::vector<Com_Collider*>& GetList(unsigned int _layer)
	{
		return mColList[_layer];
	}

	// 指定したコンポーネントをnull化
	static void SetNull(Com_Collider* _com);

	// 参照が被っているかのセット
	static void SetReferenceSameObj(bool _val);

private:
	static void Collision3D();
	static void Collision2D();
};

