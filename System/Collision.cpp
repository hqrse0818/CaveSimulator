#include "Collision.h"
#include "../Component/Com_Collider.h"
#include "../Component/Com_BoxCollider.h"
#include "../Component/Com_CylinderCollider.h"
#include "../Component/Com_SphereCollider.h"

using namespace DirectX::SimpleMath;
using namespace std;

// Box2に対してBox1がどれだけめり込んでいるか取得する
DirectX::SimpleMath::Vector3 CollisionHelper::GetAABB2AABBDepthVector3(const AABB& Box1, const AABB& Box2)
{
	Vector3 ret = Vector3(0.0f,0.0f,0.0f);

	Vector3 myMin = Box1.Center - Box1.Size / 2;
	Vector3 myMax = Box1.Center + Box1.Size / 2;
	Vector3 _oMin = Box2.Center - Box2.Size / 2;
	Vector3 _oMax = Box2.Center + Box2.Size / 2;

	// Box1が左側にある
	if (myMin.x < _oMin.x && myMax.x > _oMin.x)
	{
		ret.x = _oMin.x - myMax.x;
	}
	// Box1が右側にある
	if (myMax.x > _oMax.x && myMin.x < _oMax.x)
	{
		ret.x = _oMax.x - myMin.x;
	}
	// Box1が下側にある
	if (myMin.y < _oMin.y && myMax.y > _oMin.y && myMax.y < _oMax.y)
	{
		ret.y = _oMin.y - myMax.y;
	}
	// Box1が上側にある
	if (myMax.y > _oMax.y && myMin.y < _oMax.y && myMin.y > _oMin.y)
	{
		ret.y = _oMax.y - myMin.y;
	}
	// 間にあるもしくはBox1の底辺がBox2よりも低く、Box1の上辺がBox2の上辺よりも高い
	if (myMax.y < _oMax.y && myMin.y > _oMin.y || myMax.y > _oMax.y && myMin.y < _oMin.y)
	{
		//ret.y = 0.0f;
	}
	// Box1が手前側にある
	if (myMin.z < _oMin.z && myMax.z > _oMin.z)
	{
		ret.z = _oMin.z - myMax.z;
	}
	// Box1が奥側にある
	if (myMax.z > _oMax.z && myMin.z < _oMax.z)
	{
		ret.z = _oMax.z - myMin.z;
	}
	return ret;
}

float CollisionHelper::GetAABB2AABBDepthX(const AABB& Box1, const AABB& Box2)
{
	float Box1Min = Box1.Center.x - Box1.Size.x / 2;
	float Box1Max = Box1.Center.x + Box1.Size.x / 2;
	float Box2Min = Box2.Center.x - Box2.Size.x / 2;
	float Box2Max = Box2.Center.x + Box2.Size.x / 2;

	float ret = 0.0f;

	if (Box1Min < Box2Min && Box1Max > Box2Min)
	{
		ret = Box2Min - Box1Max;
	}
	else if (Box1Max > Box2Max && Box1Min < Box2Max)
	{
		ret = Box2Max - Box1Min;
	}

	return ret;
}

float CollisionHelper::GetAABB2AABBDepthY(const AABB& Box1, const AABB& Box2)
{
	float Box1Min = Box1.Center.y - Box1.Size.y / 2;
	float Box1Max = Box1.Center.y + Box1.Size.y / 2;
	float Box2Min = Box2.Center.y - Box2.Size.y / 2;
	float Box2Max = Box2.Center.y + Box2.Size.y / 2;

	float ret = 0.0f;

	if (Box1Min < Box2Min && Box1Max > Box2Min)
	{
		ret = Box2Min - Box1Max;
	}
	else if (Box1Max > Box2Max && Box1Min < Box2Max)
	{
		ret = Box2Max - Box1Min;
	}

	return ret;
}

float CollisionHelper::GetAABB2AABBDepthZ(const AABB& Box1, const AABB& Box2)
{
	float Box1Min = Box1.Center.z - Box1.Size.z / 2;
	float Box1Max = Box1.Center.z+ Box1.Size.z / 2;
	float Box2Min = Box2.Center.z - Box2.Size.z / 2;
	float Box2Max = Box2.Center.z + Box2.Size.z / 2;

	float ret = 0.0f;

	if (Box1Min < Box2Min && Box1Max > Box2Min)
	{
		ret = Box2Min - Box1Max;
	}
	else if (Box1Max > Box2Max && Box1Min < Box2Max)
	{
		ret = Box2Max - Box1Min;
	}

	return ret;
}

// クラスAABB同士
bool CollisionChecker::CheckCollision(AABB& _box1, AABB& _box2)
{
	Vector3 myMin = _box1.Center - _box1.Size / 2;
	Vector3 myMax = _box1.Center + _box1.Size / 2;
	Vector3 _oMin = _box2.Center - _box2.Size / 2;
	Vector3 _oMax = _box2.Center + _box2.Size / 2;

	if (myMin.x > _oMax.x)return false;
	if (myMax.x < _oMin.x)return false;
	if (myMin.y > _oMax.y)return false;
	if (myMax.y < _oMin.y)return false;
	if (myMin.z > _oMax.z)return false;
	if (myMax.z < _oMin.z)return false;

	return true;
}

// クラスAABBとスフィア
bool CollisionChecker::CheckCollision(AABB& _box, Sphere& _sphere)
{
	// 球体の中心点とボックスの中心点の距離を計算
	float distanceX = Math::Abs(_sphere.Center.x - _box.Center.x);
	float distanceY = Math::Abs(_sphere.Center.y - _box.Center.y);
	float distanceZ = Math::Abs(_sphere.Center.z - _box.Center.z);

	// 球体の中心点からボックスの最も近い点までの距離を計算
	float closeX = (distanceX - _box.Size.x / 2);
	float closeY = (distanceY - _box.Size.y / 2);
	float closeZ = (distanceZ - _box.Size.z / 2);

	if (closeX < 0.0f)
		closeX = 0.0f;
	if (closeY < 0.0f)
		closeY = 0.0f;
	if (closeZ < 0.0f)
		closeZ = 0.0f;

	// 2乗同士で計算
	float distanceRoot = Math::GetPower(closeX, 2) + Math::GetPower(closeY, 2) + Math::GetPower(closeZ, 2);
	float radiusRoot = Math::GetPower(_sphere.Radius, 2);

	return distanceRoot <= radiusRoot;
}

bool CollisionChecker::CheckCollision(Sphere& _sphere, AABB& _box)
{
	return CheckCollision(_box, _sphere);
}

bool CollisionChecker::CheckCollision(AABB& _box, Cylinder& _cylinder)
{
	return false;
}

bool CollisionChecker::CheckCollision(Cylinder& _cylinder, AABB& _box)
{
	return false;
}

bool CollisionChecker::CheckCollision(Cylinder& _cylinder1, Cylinder& _cylinder2)
{
	return false;
}

bool CollisionChecker::CheckCollision(Cylinder& _cylinder, Sphere& _sphere)
{
	return false;
}

bool CollisionChecker::CheckCollision(Sphere& _sphere, Cylinder& _cylinder)
{
	return false;
}

// クラススフィア同士
bool CollisionChecker::CheckCollision(Sphere& _sphere1, Sphere& _sphere2)
{
	// 距離を取得
	float distance = 0;

	Vector3 Sub = _sphere2.Center - _sphere1.Center;
	distance += Math::GetPower(Sub.x, 2);
	distance += Math::GetPower(Sub.y, 2);
	distance += Math::GetPower(Sub.z, 2);

	float rr = _sphere1.Radius + _sphere2.Radius;
	rr = Math::GetPower(rr, 2);

	if (rr > distance)
	{
		return true;
	}

	return false;
}

// クラス2DBox同士
bool CollisionChecker::CheckCollision(Box2D& _box1, Box2D& _box2)
{
	Vector2 bo1min = _box1.Center - _box1.Size;
	Vector2 bo1max = _box1.Center + _box1.Size;
	Vector2 bo2min = _box2.Center - _box2.Size;
	Vector2 bo2max = _box2.Center + _box2.Size;

	if (bo1min.x > bo2max.x)return false;
	if (bo1max.x < bo2min.x)return false;
	if (bo1min.y > bo2max.y)return false;
	if (bo1max.y < bo2min.y)return false;

	return true;
}

// クラス2DBoxとサークル
bool CollisionChecker::CheckCollision(Box2D& _box, Circle& _circle)
{
	Point2D point;
	point.Center.x = _circle.Center.x;
	point.Center.y = _circle.Center.y;

	Box2D NewBox{};
	NewBox.Center = _box.Center;
	NewBox.SetSize(_box.Size.x / 2 + _circle.Radius, _box.Size.y / 2 + _circle.Radius);

	return CheckCollision(point, NewBox);
}

// クラスサークルとBox2D
bool CollisionChecker::CheckCollision(Circle& _circle, Box2D& _box)
{
	return CheckCollision(_box, _circle);
}

// クラスサークル同士
bool CollisionChecker::CheckCollision(Circle& _circle1, Circle& _circle2)
{
	float distance = Math::GetDistance(_circle1.Center, _circle2.Center);

	if (distance < _circle1.Radius + _circle2.Radius)
	{
		return true;
	}
	return false;
}

// クラスボックス2Dとポイント
bool CollisionChecker::CheckCollision(Box2D& _box, Point2D& _point)
{
	Vector2 max = _box.getmax();
	Vector2 min = _box.getmin();

	if (_point.Center.x < min.x)return false;
	if (_point.Center.x > max.x)return false;
	if (_point.Center.y < min.y)return false;
	if (_point.Center.y > max.y)return false;

	return true;
}

// クラスポイントとボックス2D
bool CollisionChecker::CheckCollision(Point2D& _point, Box2D& _box)
{
	return CheckCollision(_box, _point);
}

bool CollisionChecker::CheckCollision(Com_Collider* _col1, Com_Collider* _col2)
{
	switch (_col1->mType)
	{
	case ColColliderForm::Box:
	{
		AABB* ab1 = static_cast<Com_BoxCollider*>(_col1)->GetCollider();
		switch (_col2->mType)
		{
		case ColColliderForm::Box:
		{
			AABB* ab2 = static_cast<Com_BoxCollider*>(_col2)->GetCollider();
			return CheckCollision(*ab1, *ab2);
		}
			break;
		case ColColliderForm::Sphere:
		{
			Sphere* sp = static_cast<Com_SphereCollider*>(_col2)->GetCollider();
			return CheckCollision(*ab1, *sp);
		}
			break;
		case ColColliderForm::Cylinder:
		{
			Cylinder* cy = static_cast<Com_CylinderCollider*>(_col2)->GetCollider();
			return CheckCollision(*ab1, *cy);
		}
			break;
		default:
			return false;
			break;
		}
	}
	break;
	case ColColliderForm::Sphere:
	{
		Sphere* sp1 = static_cast<Com_SphereCollider*>(_col1)->GetCollider();
		switch (_col2->mType)
		{
		case ColColliderForm::Box:
		{
			AABB* ab2 = static_cast<Com_BoxCollider*>(_col2)->GetCollider();
			return CheckCollision(*sp1, *ab2);
		}
			break;
		case ColColliderForm::Sphere:
		{
			Sphere* sp = static_cast<Com_SphereCollider*>(_col2)->GetCollider();
			return CheckCollision(*sp1, *sp);
		}
			break;
		case ColColliderForm::Cylinder:
		{
			Cylinder* cy = static_cast<Com_CylinderCollider*>(_col2)->GetCollider();
			return CheckCollision(*sp1, *cy);
		}
			break;
		default:
			return false;
			break;
		}
	}
	break;
	case ColColliderForm::Cylinder:
	{
		Cylinder* cy1 = static_cast<Com_CylinderCollider*>(_col1)->GetCollider();
		switch (_col2->mType)
		{
		case ColColliderForm::Box:
		{
			AABB* ab2 = static_cast<Com_BoxCollider*>(_col2)->GetCollider();
			return CheckCollision(*cy1, *ab2);
		}
			break;
		case ColColliderForm::Sphere:
		{
			Sphere* sp = static_cast<Com_SphereCollider*>(_col2)->GetCollider();
			return CheckCollision(*cy1, *sp);
		}
			break;
		case ColColliderForm::Cylinder:
		{
			Cylinder* cy = static_cast<Com_CylinderCollider*>(_col2)->GetCollider();
			return CheckCollision(*cy1, *cy);
		}
			break;
		default:
			return false;
			break;
		}
	}
		break;
	case ColColliderForm::Point3D:
	{
		return false;
		break;
	}
		break;
	case ColColliderForm::Box2D:
	{
		Box2D* bo1 = static_cast<Com_BoxCollider2D*>(_col1)->GetCollider();
		switch (_col2->mType)
		{
		case ColColliderForm::Box2D:
		{
			Box2D* bo2 = static_cast<Com_BoxCollider2D*>(_col2)->GetCollider();
			return CheckCollision(*bo1, *bo2);
		}
			break;
		case ColColliderForm::Circle:
		{
			Circle* ci = static_cast<Com_CircleCollider*>(_col2)->GetCollider();
			return CheckCollision(*bo1, *ci);
		}
			break;
		default:
			return false;
			break;
		}
	}
		break;
	case ColColliderForm::Circle:
	{
		Circle* ci1 = static_cast<Com_CircleCollider*>(_col1)->GetCollider();
		switch (_col2->mType)
		{
		case ColColliderForm::Box2D:
		{
			Box2D* bo2 = static_cast<Com_BoxCollider2D*>(_col2)->GetCollider();
			return CheckCollision(*ci1, *bo2);
		}
			break;
		case ColColliderForm::Circle:
		{
			Circle* ci = static_cast<Com_CircleCollider*>(_col2)->GetCollider();
			return CheckCollision(*ci1, *ci);
		}
			break;
		default:
			return false;
			break;
		}
	}
		break;
	case ColColliderForm::Point2D:
		return false;
		break;
	default:
		return false;
		break;
	}
}
