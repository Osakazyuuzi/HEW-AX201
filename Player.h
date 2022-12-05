#ifndef ___PLAYER_H___
#define ___PLAYER_H___
#include "Object3D.h"
#include "MeshRenderer.h"
#include "Rigidbody.h"
#include "PlayerController.h"
#include "AABBCollider.h"

class Player : public Object3D
{
public:
	// �R���X�g���N�^
	Player(std::string name, std::string tag) : Object3D(name, tag) {
		// �v���C���[�̃��f�������[�h����i���ŃL���[�u�̃I�u�W�F�N�g�j
		GetComponent<MeshRenderer>()->LoadModel("Assets/Model/Object/Cube.fbx", 1.0f);
		// �R���|�[�l���g��ǉ�
		AddComponent<PlayerController>();
		AddComponent<Rigidbody>();
		AddComponent<AABBCollider>();
		// �����蔻��̕ӂ̒������Z�b�g
		GetComponent<AABBCollider>()->SetLen({ 1.0f,1.0f,1.0f });
	}
	// �f�X�g���N�^
	~Player(){}
};

#endif //!___PLAYER_H___