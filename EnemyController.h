#ifndef ___ENEMY_CONTROLLER_H___
#define ___ENEMY_CONTROLLER_H__

#include "ComponentBase.h"
#include "ObjectBase.h"

class EnemyController : public ComponentBase
{
private:
	// �ړ��X�s�[�h�i�ʒu�t���[���ɐi�ދ����j
	float m_MoveSpeed;
public:
	// �R���X�g���N�^
	EnemyController()
		: m_MoveSpeed(0.05f)
	{}
	// �f�X�g���N�^
	~EnemyController() {}

	// �X�V
	void Update()override;

	// �����蔻��
	void OnCollisionEnter(ObjectBase* object);
	void OnCollisionStay(ObjectBase* object);
	void OnCollisionExit(ObjectBase* object);

	//--- �Z�b�g�֐�
	void SetMoveSpeed(float speed) { m_MoveSpeed = speed; }
};

#endif //!___ENEMY_CONTROLLER_H___