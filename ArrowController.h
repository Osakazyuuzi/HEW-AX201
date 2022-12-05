#ifndef ___ARROW_CONTROLLER_H___
#define ___ARROW_CONTROLLER_H__

#include "ComponentBase.h"
#include "ObjectBase.h"

class ArrowController : public ComponentBase
{
private:
public:
	// �R���X�g���N�^
	ArrowController() {}
	// �f�X�g���N�^
	~ArrowController() {}

	// �X�V
	void Update()override;


	// �����蔻��
	void OnCollisionEnter(ObjectBase* object);
	void OnCollisionStay(ObjectBase* object);
	void OnCollisionExit(ObjectBase* object);
};

#endif //!___ARROW_CONTROLLER_H___