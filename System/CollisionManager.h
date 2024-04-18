#pragma once
#include "../Component/Com_Collider.h"
#include "Collision.h"

#include <vector>
#include <array>
// �����蔻��p�}�l�[�W���[
// �Ή�array
// 0 Movable3D, 1 Static3D, 2 Movable2D, 3 Static2D
constexpr int mNumColliderArray = 4;

class CollisionManager
{
private:
	// ���肷��I�u�W�F�N�g���X�g
	static std::array<std::vector<Com_Collider*>, mNumColliderArray> mColList;
	// �R���|�[�l���g�ɕt������Array�̃C���f�b�N�X
	static std::vector<unsigned int> mNum;
	// �`�F�b�J�[�I�u�W�F�N�g�̃��X�g
	static std::vector<CollisionChecker*> mCheckers;

	static bool bReferenceSameObject;
public:
	// ������
	static void Init();
	// ���
	static void Uninit();

	// ����̎��s
	static void ExecuteCollision();
	// �I�u�W�F�N�g�̓o�^
	static void Push(int _layer, Com_Collider* _collider);
	// ���X�g�擾
	static std::vector<Com_Collider*>& GetList(unsigned int _layer)
	{
		return mColList[_layer];
	}

	// �w�肵���R���|�[�l���g��null��
	static void SetNull(Com_Collider* _com);

	// �Q�Ƃ�����Ă��邩�̃Z�b�g
	static void SetReferenceSameObj(bool _val);

private:
	static void Collision3D();
	static void Collision2D();
};

