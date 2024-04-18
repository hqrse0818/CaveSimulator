#pragma once
#include "../Format.h"
#include "CustomMath.h"
#include "../Collider/Collider.h"

class Com_Collider;

// 当たり判定処理

namespace CollisionHelper
{
	// Box1がBox2に対してどれだけめり込んでいるか取得Vector3
	DirectX::SimpleMath::Vector3 GetAABB2AABBDepthVector3(const AABB& Box1, const AABB& Box2);

	// Box1がx方向にBox2に対してどれだけめり込んでいるか取得
	float GetAABB2AABBDepthX(const AABB& Box1, const AABB& Box2);

	// Box1がy方向にBox2に対してどれだけめり込んでいるか取得
	float GetAABB2AABBDepthY(const AABB& Box1, const AABB& Box2);

	// Box1がz方向にBox2に対してどれだけめり込んでいるか取得
	float GetAABB2AABBDepthZ(const AABB& Box1, const AABB& Box2);
}

class CollisionChecker
{
public:
	// AABB同士
	bool CheckCollision(AABB& _box1, AABB& _box2);
	// AABBとスフィア
	bool CheckCollision(AABB& _box, Sphere& _sphere);
	// スフィアとAABB
	bool CheckCollision(Sphere& _sphere, AABB& _box);
	// AABBとシリンダー
	bool CheckCollision(AABB& _box, Cylinder& _cylinder);
	// シリンダーとAABB
	bool CheckCollision(Cylinder& _cylinder, AABB& _box);
	// シリンダー同士
	bool CheckCollision(Cylinder& _cylinder1, Cylinder& _cylinder2);
	// シリンダーとスフィア
	bool CheckCollision(Cylinder& _cylinder, Sphere& _sphere);
	// スフィアとシリンダー
	bool CheckCollision(Sphere& _sphere, Cylinder& _cylinder);
	// スフィア同士
	bool CheckCollision(Sphere& _sphere1, Sphere& _sphere2);
	// 2DBox同士
	bool CheckCollision(Box2D& _box1, Box2D& _box2);
	// 2DBoxとCircle
	bool CheckCollision(Box2D& _box, Circle& _circle);
	// CircleとBox2D
	bool CheckCollision(Circle& _circle, Box2D& _box);
	// Cirecle同士
	bool CheckCollision(Circle& _circle1, Circle& _circle2);
	// Box2Dとポイント
	bool CheckCollision(Box2D& _box, Point2D& _point);
	// ポイントとBox2D
	bool CheckCollision(Point2D& _point, Box2D& _box);
	// テンプレート
	bool CheckCollision(Com_Collider* _col1, Com_Collider* _col2);
};