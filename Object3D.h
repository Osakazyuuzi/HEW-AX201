#pragma once

#include "ObjectBase.h"
#include <string>
#include "Rigidbody.h"
#include "MeshRenderer.h"

class Object3D : public ObjectBase
{
public:
	// �R���X�g���N�^
	Object3D(std::string name, std::string tag) : ObjectBase(name, tag)
	{
		// �f�t�H���g�R���|�[�l���g��ǉ�
		this->AddComponent<MeshRenderer>();	// 3D�I�u�W�F�N�g�p�i�\�ʓI�ȏ��j
	}
	// �f�X�g���N�^
	virtual ~Object3D(){}

	// �`��
	void Draw() override {
		this->GetComponent<MeshRenderer>()->Draw();
	}
};