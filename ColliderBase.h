#ifndef ___COLLIDER_H___
#define ___COLLIDER_H___

#include "ComponentBase.h"
#include "Primitive.h"
#include "AABB.h"
#include <list>
#include <memory>

using namespace Primitive;

class ColliderBase : public ComponentBase
{
	using ColliderPool = std::list<ColliderBase*>;
public:
	// �R���X�g���N�^
	ColliderBase(){}
	// �f�X�g���N�^
	virtual ~ColliderBase(){}

	// ��X�V
	void LateUpdate() override;

	// �K�؂ȏ������Ăяo��
	virtual void CallTouchOperation(ColliderBase* collider) = 0;

	//--- �����蔻�菈��
	// AABB�Ƃ̓����蔻��
	virtual void TouchingTheAABB(ColliderBase* _AABB) = 0;
	// Sphere�Ƃ̓����蔻��
	virtual void TouchingTheSphere(ColliderBase* _Sphere) = 0;

	virtual void CheckTouchCollider(ColliderBase* collider);
	virtual void CheckExitCollider();

	// �������ɓ��e���ꂽ���������瓊�e���������Z�o
	float LenSegOnSeparateAxis(Float3 *Sep, Float3 *e1, Float3 *e2, Float3 *e3 = 0);

	//--- �Q�b�g�֐�
	virtual Primitive::AABB& GetPrimitive() = 0;

protected:
	// ������G��Ă���R���C�_�[�̃��X�g
	ColliderPool m_TouchingPool;
	// ���G�ꂽ�R���C�_�[�̃��X�g
	ColliderPool m_justTouchedPool;
};

#endif //!___COLLIDER_H___