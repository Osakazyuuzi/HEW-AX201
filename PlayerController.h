#ifndef ___PLAYER_CONTROLLER_H___
#define ___PLAYER_CONTROLLER_H__

#include "ComponentBase.h"
#include "ObjectBase.h"

class PlayerController : public ComponentBase
{
private:
public:
	// �R���X�g���N�^
	PlayerController() {}
	// �f�X�g���N�^
	~PlayerController() {}

	// �X�V
	void Update()override;


	// �����蔻��
	void OnCollisionEnter(ObjectBase* object);
	void OnCollisionStay(ObjectBase* object);
	void OnCollisionExit(ObjectBase* object);
};

#endif //!___PLAYER_CONTROLLER_H___