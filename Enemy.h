#ifndef ___ENEMY_H___
#define ___ENEMY_H___
#include "Object3D.h"
#include "MeshRenderer.h"
#include "Rigidbody.h"
#include "AABBCollider.h"
#include "EnemyController.h"

class Enemy : public Object3D
{
public:
	// �R���X�g���N�^
	Enemy(std::string name, std::string tag) : Object3D(name, tag) {
		// �v���C���[�̃��f�������[�h����i���ŃL���[�u�̃I�u�W�F�N�g�j
		GetComponent<MeshRenderer>()->LoadModel("Assets/Model/Object/Cube.fbx", 1.0f);
		// �R���|�[�l���g��ǉ�
		AddComponent<EnemyController>();
		AddComponent<Rigidbody>();
		AddComponent<AABBCollider>();
		// �����蔻��̕ӂ̒������Z�b�g
		GetComponent<AABBCollider>()->SetLen({ 1.0f,1.0f,1.0f });
	}
	// �f�X�g���N�^
	~Enemy() {}
};

#endif //!___ENEMY_H___