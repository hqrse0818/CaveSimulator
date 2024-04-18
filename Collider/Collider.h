#pragma once
#include <SimpleMath.h>
class Collider
{
};

class AABB
	: public Collider
{
public:
	// 中心位置
	DirectX::SimpleMath::Vector3 Center =
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	// サイズ
	DirectX::SimpleMath::Vector3 Size =
		DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);
public:
	void SetCenter(float _x, float _y, float _z)
	{
		Center.x = _x;
		Center.y = _y;
		Center.z = _z;
	}

	void SetSize(float _x, float _y, float _z)
	{
		Size.x = _x;
		Size.y = _y;
		Size.z = _z;
	}

	DirectX::SimpleMath::Vector3 getmax()
	{
		return Center + Size / 2;
	}

	DirectX::SimpleMath::Vector3 getmin()
	{
		return Center - Size / 2;
	}
};

class Box2D
	: public Collider
{
public:
	// 中心位置
	DirectX::SimpleMath::Vector2 Center =
		DirectX::SimpleMath::Vector2(0.0f, 0.0f);
	// サイズ
	DirectX::SimpleMath::Vector2 Size =
		DirectX::SimpleMath::Vector2(1.0f, 1.0f);

public:
	void SetCenter(float _x, float _y)
	{
		Center.x = _x;
		Center.y = _y;
	}

	void SetSize(float _x, float _y)
	{
		Size.x = _x;
		Size.y = _y;
	}

	DirectX::SimpleMath::Vector2 getmax()
	{
		return Center + Size / 2;
	}

	DirectX::SimpleMath::Vector2 getmin()
	{
		return Center - Size / 2;
	}
};

class Sphere
	: public Collider 
{
public:
	//中心位置
	DirectX::SimpleMath::Vector3 Center =
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	// 半径
	float Radius = 1.0f;

public:
	void SetCenter(float _x, float _y, float _z)
	{
		Center.x = _x;
		Center.y = _y;
		Center.z = _z;
	}

	void SetRadius(float _r)
	{
		Radius = _r;
	}
};

class Circle
	: public Collider
{
public:
	// 中心位置
	DirectX::SimpleMath::Vector2 Center =
		DirectX::SimpleMath::Vector2(0.0f, 0.0f);
	// 半径
	float Radius = 1.0f;

public:
	void SetCenter(float _x, float _y)
	{
		Center.x = _x;
		Center.y = _y;
	}

	void SetRadius(float _r)
	{
		Radius = _r;
	}
};

class Cylinder
	: public Collider
{
public:
	// 中心位置
	DirectX::SimpleMath::Vector3 Center =
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	// 高さ
	float Height = 1.0f;
	// 半径
	float Radius = 1.0f;

public:
	void SetCenter(float _x, float _y, float _z)
	{
		Center.x = _x;
		Center.y = _y;
		Center.z = _z;
	}

	void SetHeight(float _h)
	{
		Height = _h;
	}

	void SetRadius(float _r)
	{
		Radius = _r;
	}

	DirectX::SimpleMath::Vector3 getmax()
	{
		DirectX::SimpleMath::Vector3 ret;
		ret.x = Center.x + Radius;
		ret.y = Center.y + Height / 2;
		ret.z = Center.z + Radius;

		return ret;
	}

	DirectX::SimpleMath::Vector3 getmin()
	{
		DirectX::SimpleMath::Vector3 ret;
		ret.x = Center.x - Radius;
		ret.y = Center.y - Height / 2;
		ret.z = Center.z - Radius;

		return ret;
	}
};

class Point3D
	: public Collider
{
public:
	DirectX::SimpleMath::Vector3 Center =
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

public:
	void SetCenter(float _x, float _y, float _z)
	{
		Center.x = _x;
		Center.y = _y;
		Center.z = _z;
	}
};

class Point2D
	: public Collider
{
public:
	// 中心位置
	DirectX::SimpleMath::Vector2 Center =
		DirectX::SimpleMath::Vector2(0.0f, 0.0f);

public:
	void SetCenter(float _x, float _y)
	{
		Center.x = _x;
		Center.y = _y;
	}
};
