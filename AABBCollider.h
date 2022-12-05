/*
ObjectBase�N���X - �p������p
�p����
�@Object3D�N���X - �p������p
�@�p����
   // �����ɃI�u�W�F�N�g��ǉ����Ă����B
�@UI - 


*/

#ifndef ___AABBCOLLIDER_H___
#define ___AABBCOLLIDER_H___

#include "Primitive.h"
#include "AABB.h"
#include "ColliderBase.h"
#include "Point.h"
#include "Float3.h"
#include "Transform.h"

class AABBCollider : public ColliderBase
{
private:
	// �`��
	Primitive::AABB m_Primitive;
	// Point��Transform���玩���ݒ�
	bool m_bPointAuto;
	// ���[�h���̏�� - ���f���̃R���X�g���N�^���ł̂ݕύX��������
	Transform m_RoadTransData;

public:
	// �R���X�g���N�^
	AABBCollider()
		: ColliderBase()
		, m_bPointAuto(true)
	{}
	// �f�X�g���N�^
	~AABBCollider(){}

	void LateUpdate();

	// �K�؂ȏ������Ăяo��
	void CallTouchOperation(ColliderBase* collider) override;

	//--- AABB�Ƃ̓����蔻��
	void TouchingTheAABB(ColliderBase* _AABB) override;
	//--- Sphere�Ƃ̓����蔻��
	void TouchingTheSphere(ColliderBase* _Sphere) override;

	//--- �Q�b�g�֐�
	Primitive::AABB& GetPrimitive() { return m_Primitive; }

	//--- �Z�b�g�֐�
	void SetLoadPoint(DirectX::XMFLOAT3 point) { m_RoadTransData.SetPosition(point); }
	void SetLoadLen(DirectX::XMFLOAT3 len) { m_RoadTransData.SetScale(len); }
	void SetPoint(Primitive::Float3 point) { m_Primitive.p = point; }
	void SetLen(Primitive::Float3 len) { m_Primitive.hl = len/2; }
	void SetLenX(float len) { m_Primitive.hl.x = len/2; }
	void SetLenY(float len) { m_Primitive.hl.y = len/2; }
	void SetLenZ(float len) { m_Primitive.hl.z = len/2; }
	void SetPointAuto(bool bAuto) { m_bPointAuto = bAuto; }
};

#endif //!___AABBCOLLIDER_H___