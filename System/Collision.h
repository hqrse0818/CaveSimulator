#pragma once
#include "../Format.h"
#include "CustomMath.h"
#include "../Collider/Collider.h"

class Com_Collider;

// �����蔻�菈��

namespace CollisionHelper
{
	// Box1��Box2�ɑ΂��Ăǂꂾ���߂荞��ł��邩�擾Vector3
	DirectX::SimpleMath::Vector3 GetAABB2AABBDepthVector3(const AABB& Box1, const AABB& Box2);

	// Box1��x������Box2�ɑ΂��Ăǂꂾ���߂荞��ł��邩�擾
	float GetAABB2AABBDepthX(const AABB& Box1, const AABB& Box2);

	// Box1��y������Box2�ɑ΂��Ăǂꂾ���߂荞��ł��邩�擾
	float GetAABB2AABBDepthY(const AABB& Box1, const AABB& Box2);

	// Box1��z������Box2�ɑ΂��Ăǂꂾ���߂荞��ł��邩�擾
	float GetAABB2AABBDepthZ(const AABB& Box1, const AABB& Box2);
}

class CollisionChecker
{
public:
	// AABB���m
	bool CheckCollision(AABB& _box1, AABB& _box2);
	// AABB�ƃX�t�B�A
	bool CheckCollision(AABB& _box, Sphere& _sphere);
	// �X�t�B�A��AABB
	bool CheckCollision(Sphere& _sphere, AABB& _box);
	// AABB�ƃV�����_�[
	bool CheckCollision(AABB& _box, Cylinder& _cylinder);
	// �V�����_�[��AABB
	bool CheckCollision(Cylinder& _cylinder, AABB& _box);
	// �V�����_�[���m
	bool CheckCollision(Cylinder& _cylinder1, Cylinder& _cylinder2);
	// �V�����_�[�ƃX�t�B�A
	bool CheckCollision(Cylinder& _cylinder, Sphere& _sphere);
	// �X�t�B�A�ƃV�����_�[
	bool CheckCollision(Sphere& _sphere, Cylinder& _cylinder);
	// �X�t�B�A���m
	bool CheckCollision(Sphere& _sphere1, Sphere& _sphere2);
	// 2DBox���m
	bool CheckCollision(Box2D& _box1, Box2D& _box2);
	// 2DBox��Circle
	bool CheckCollision(Box2D& _box, Circle& _circle);
	// Circle��Box2D
	bool CheckCollision(Circle& _circle, Box2D& _box);
	// Cirecle���m
	bool CheckCollision(Circle& _circle1, Circle& _circle2);
	// Box2D�ƃ|�C���g
	bool CheckCollision(Box2D& _box, Point2D& _point);
	// �|�C���g��Box2D
	bool CheckCollision(Point2D& _point, Box2D& _box);
	// �e���v���[�g
	bool CheckCollision(Com_Collider* _col1, Com_Collider* _col2);
};