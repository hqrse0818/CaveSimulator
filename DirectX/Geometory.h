#pragma once
#include "Shader.h"
#include "../Format.h"
#include <SimpleMath.h>

struct LINE
{
	VERTEX_GEOMETORY vx[2];
};

class Geometory
{
private:
	static ID3D11Buffer* pBoxBuffer;
	static ID3D11Buffer* pLineBuffer;
	static ID3D11Buffer* pSphereBuffer;

	static VertexShader* pVS;
	static PixelShader* pPS;

public:
	static void Create();
	static void Uninit();

	static void DrawBox(
		DirectX::SimpleMath::Vector3 _pos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
		DirectX::SimpleMath::Vector3 _size =DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f),
		DirectX::SimpleMath::Vector3 _rot = DirectX::SimpleMath::Vector3(0.0f,0.0f,0.0f),
		DirectX::SimpleMath::Color _color = DirectX::SimpleMath::Color(1.0f, 0.0f, 0.0f, 1.0f)
	);
	static void DrawLine(
		DirectX::SimpleMath::Vector3 _PosS = DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f),
		DirectX::SimpleMath::Color _ColS = DirectX::SimpleMath::Color(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::SimpleMath::Vector3 _PosE = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f),
		DirectX::SimpleMath::Color _ColE = DirectX::SimpleMath::Color(1.0f, 0.0f, 0.0f, 1.0f)
	);
	static void DrawSphere(DirectX::SimpleMath::Vector3 _Pos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), float _radius = 1.0f, DirectX::SimpleMath::Color _color = DirectX::SimpleMath::Color(1.0f,0.0f, 0.0f, 1.0f));

	static void SetVertex(const char* _name);
	static void SetPixel(const char* _name);
};

