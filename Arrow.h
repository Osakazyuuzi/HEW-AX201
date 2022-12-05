#ifndef ___ARROW_H___
#define ___ARROW_H___
#include "Object3D.h"
#include "MeshRenderer.h"
#include "Rigidbody.h"
#include "AABBCollider.h"
#include "ArrowController.h"

class Arrow : public Object3D
{
public:
	// �R���X�g���N�^
	Arrow(std::string name, std::string tag) : Object3D(name, tag) {
		// ��̃��f�������[�h����
		GetComponent<MeshRenderer>()->LoadModel("Assets/Model/Object/Arrow.fbx", 1.0f);
		// �R���|�[�l���g��ǉ�
		AddComponent<Rigidbody>();
		AddComponent<AABBCollider>();
		AddComponent<ArrowController>();
		// �����蔻��̑傫���𒲐��i���f���ǂݍ��ݎ��_�j
		GetComponent<AABBCollider>()->SetLen({ 0.6f, 0.6f, 3.0f });
	}
	// �f�X�g���N�^
	~Arrow() {}
};

#endif //!___ARROW_H___