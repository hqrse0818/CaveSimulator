#pragma once

// 前方宣言
class GameObject;

class Component
{
public:
	// コンポーネントを所持してる親オブジェクト
	GameObject* p_mObject;

	// コンポーネントが有効かどうか
	bool bEnable;

public:
	// コンストラクタ
	Component()
	{
		p_mObject = nullptr;
		bEnable = true;
	}

	// デストラクタ
	virtual ~Component() {};

	virtual void Init() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void PreDraw() {};
	virtual void Draw() {};
	virtual void ShadowDraw() {};
	virtual void OnCollisionEnter(GameObject* _obj) {};
	virtual void OnCollisionStay(GameObject* _obj) {};
	virtual void OnCollisionExit(GameObject* _obj) {};

	virtual void Uninit() {};
};