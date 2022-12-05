#ifndef ___UI_H___
#define ___UI_H___

#include "ObjectBase.h"
#include "SpriteRenderer.h"
#include "DirectX.h"

class UI : public ObjectBase
{
public:
	// �R���X�g���N�^
	UI(std::string name, std::string tag) : ObjectBase(name, tag) {
		// �f�t�H���g�R���|�[�l���g��ǉ�
		this->AddComponent<SpriteRenderer>();	// 2D�p�\�ʏ��

		// �S�Ă̐ݒ肪�I�������Ăяo��
		this->Start();
	}
	// �f�X�g���N�^
	~UI(){}

	// �`��
	void Draw() override {
		this->GetComponent<SpriteRenderer>()->Draw();
	}
};

#endif //!___UI_H___