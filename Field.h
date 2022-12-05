#ifndef ___FIELD_H___
#define ___FIELD_H___
#include "Object3D.h"
#include "AABBCollider.h"

class Field : public Object3D
{
public:
	// �R���X�g���N�^
	Field(std::string name, std::string tag) : Object3D(name, tag) {
		// �v���C���[�̃��f�������[�h����i���ŃL���[�u�̃I�u�W�F�N�g�j
		GetComponent<MeshRenderer>()->LoadModel("Assets/Model/Object/Cube.fbx", 1.0f);
		// �R���|�[�l���g��ǉ�
		AddComponent<AABBCollider>();
		// �I�u�W�F�N�g�̕ӂ̒������Z�b�g
		GetComponent<Transform>()->SetScale({ 20.0f, 1.0f, 20.0f });
		// �����蔻��̕ӂ̒������Z�b�g
		GetComponent<AABBCollider>()->SetLen({ 20.0f,1.0f,20.0f });
	}
	// �f�X�g���N�^
	~Field() {}
};

#endif //!___FIELD_H___