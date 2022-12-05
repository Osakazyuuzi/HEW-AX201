#include "EnemyController.h"
#include "Transform.h"
#include "ObjectBase.h"
#include "Primitive.h"
#include "Vector3.h"
#include "Float3.h"
#include "ObjectManager.h"
#include "Arrow.h"

void EnemyController::Update()
{
	//--- Player��ǂ������鏈��
	// Player�^�O�̃I�u�W�F�N�g�̍��W���擾
	DirectX::XMFLOAT3 PlayerPos = ObjectManager::FindObjectWithTag(TagName::Player)->GetComponent<Transform>()->GetPosition();
	// �^�[�Q�b�g�܂ł̃x�N�g�����Z�o
	DirectX::XMVECTOR vTargetVector = 
		DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&PlayerPos), DirectX::XMLoadFloat3(&GetOwner()->GetComponent<Transform>()->GetPosition()));
	// �x�N�g���𐳋K��
	vTargetVector = DirectX::XMVector3Normalize(vTargetVector);
	// �ړ��X�s�[�h���|����
	vTargetVector = DirectX::XMVectorScale(vTargetVector, m_MoveSpeed);
	// Float3�ɕϊ�����
	DirectX::XMFLOAT3 TargetVector;
	DirectX::XMStoreFloat3(&TargetVector, vTargetVector);
	// ���W�ɓK�p����
	GetOwner()->GetComponent<Transform>()->SetPosition({
		GetOwner()->GetComponent<Transform>()->GetPosition().x + TargetVector.x,
		GetOwner()->GetComponent<Transform>()->GetPosition().y,
		GetOwner()->GetComponent<Transform>()->GetPosition().z + TargetVector.z
		});

	// �ړ������ꍇ�A�ړ����������ɉ�]����
	if (TargetVector.x != 0.0f || TargetVector.y != 0.0f || TargetVector.z != 0.0f) {
		float radY = 0.0f;
		// Z�����ւ̃x�N�g��(���f���̐��ʕ����̃x�N�g��)
		DirectX::XMFLOAT3 zVector = { 0.0f, 0.0f, 1.0f };
		// ���ςƃx�N�g���̒������g����cos�Ƃ����߂�
		DirectX::XMStoreFloat(&radY, DirectX::XMVector3Dot(DirectX::XMVector3Normalize(vTargetVector), DirectX::XMLoadFloat3(&zVector)));
		// ���ς���p�x�����߂�
		radY = ::acos(radY);
		// ���W�A���p���炨�Ȃ��݂̊p�x�ɕύX
		radY = DirectX::XMConvertToDegrees(radY);
		// ��]���E��]������]���𔻕ʂ��邽�߂ɁA�O�ςŋ��߂�
		// ���߂��O�ς�Y�������v���X�������獶���B
		// ���߂��O�ς�Y�������}�C�i�X��������E���B
		DirectX::XMFLOAT3 rotateDirection;
		DirectX::XMStoreFloat3(&rotateDirection, DirectX::XMVector3Cross(DirectX::XMVector3Normalize(vTargetVector), DirectX::XMLoadFloat3(&zVector)));
		if (rotateDirection.y > 0) radY = 180.0f + (180.0f - radY);
		// �Z�o�����p�x��K�p����
		GetOwner()->GetComponent<Transform>()->SetAngle({ 0.0f, radY, 0.0f });
	}
}

void EnemyController::OnCollisionEnter(ObjectBase* object)
{
	// �t�B�[���h�Ɠ��������Ƃ��̏���
	if (object->GetTag() == TagName::Field || 
		object->GetTag() == TagName::Enemy) {
		//--- ��(Plane)�ɂ߂荞��OBB��߂�����
		// ���������I�u�W�F�N�g�̂ǂ̖ʂƓ��������Ƃ��ɃA�N�V�������N�������B
		// --- �ʂ̖@�������߂�
		Float3 Normal = Primitive::Vector3_up; // ��̖ʂɐݒ�
		DirectX::XMVECTOR planeN = DirectX::XMLoadFloat3(&ConvertToDirectXFloat3(Normal));
		DirectX::XMVector3Normalize(planeN);
		// �u���b�N�̊p�x�����]�s����v�Z
		DirectX::XMMATRIX rotation;
		rotation =
			DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(object->GetComponent<Transform>()->GetAngle().y)) *
			DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(object->GetComponent<Transform>()->GetAngle().x)) *
			DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(object->GetComponent<Transform>()->GetAngle().z));
		// �@���̌������u���b�N�̉�]�s��ŕς���
		DirectX::XMVector3TransformCoord(planeN, rotation);
		// --- �ʏ�̍��W�����߂�
		DirectX::XMVECTOR Pos;
		if (Normal == Primitive::Vector3_up || Normal == Primitive::Vector3_down) {
			// ������@���ɂ��������̂��ʏ�̍��W�ƂȂ�
			Pos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&object->GetComponent<Transform>()->GetPosition()),
				DirectX::XMVectorScale(planeN, object->GetComponent<AABBCollider>()->GetPrimitive().lenY() / 2));
		}
		else if (Normal == Primitive::Vector3_forward || Normal == Primitive::Vector3_back) {
			// ���s��@���ɂ��������̂��ʏ�̍��W�ƂȂ�
			Pos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&object->GetComponent<Transform>()->GetPosition()),
				DirectX::XMVectorScale(planeN, object->GetComponent<AABBCollider>()->GetPrimitive().lenZ() / 2));
		}
		else if (Normal == Primitive::Vector3_right || Normal == Primitive::Vector3_left) {
			// ����@���ɂ��������̂��ʏ�̍��W�ƂȂ�
			Pos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&object->GetComponent<Transform>()->GetPosition()),
				DirectX::XMVectorScale(planeN, object->GetComponent<AABBCollider>()->GetPrimitive().lenX() / 2));
		}
		// --- ���ʂ̖@���ɑ΂���OBB�̎ˉe���̒������Z�o
		float r = 0.0f;		// �ߐڋ���
		float Addr;
		DirectX::XMStoreFloat(&Addr, DirectX::XMVector3Dot(
			DirectX::XMVectorScale(DirectX::XMLoadFloat3(&this->GetOwner()->GetComponent<Transform>()->GetVectorForword()),
				this->GetOwner()->GetComponent<AABBCollider>()->GetPrimitive().lenZ() / 2)
			, planeN));
		r += fabs(Addr);
		DirectX::XMStoreFloat(&Addr, DirectX::XMVector3Dot(
			DirectX::XMVectorScale(DirectX::XMLoadFloat3(&this->GetOwner()->GetComponent<Transform>()->GetVectorUp()),
				this->GetOwner()->GetComponent<AABBCollider>()->GetPrimitive().lenY() / 2)
			, planeN));
		r += fabs(Addr);
		DirectX::XMStoreFloat(&Addr, DirectX::XMVector3Dot(
			DirectX::XMVectorScale(DirectX::XMLoadFloat3(&this->GetOwner()->GetComponent<Transform>()->GetVectorRight()),
				this->GetOwner()->GetComponent<AABBCollider>()->GetPrimitive().lenX() / 2)
			, planeN));
		r += fabs(Addr);

		// --- �߂��������Z�o
		float s;
		DirectX::XMStoreFloat(&s, DirectX::XMVector3Dot(
			DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&this->GetOwner()->GetComponent<Transform>()->GetPosition()), Pos), planeN));
		if (s > 0)	s = r - fabs(s);
		else		s = r + fabs(s);

		// --- �߂荞�񂾈ʒu���畽�ʂ̖@�������ɖ߂����������I�t�Z�b�g����
		DirectX::XMFLOAT3 offsetPos;
		DirectX::XMStoreFloat3(&offsetPos, DirectX::XMVectorScale(planeN, s));
		this->GetOwner()->GetComponent<Transform>()->SetPosition({
			this->GetOwner()->GetComponent<Transform>()->GetPosition().x + offsetPos.x,
			this->GetOwner()->GetComponent<Transform>()->GetPosition().y + offsetPos.y,
			this->GetOwner()->GetComponent<Transform>()->GetPosition().z + offsetPos.z
			});
	}
}

void EnemyController::OnCollisionStay(ObjectBase* object)
{
	// �t�B�[���h�Ɠ��������Ƃ��̏���
	if (object->GetTag() == TagName::Field) {
		//--- ��(Plane)�ɂ߂荞��OBB��߂�����
		// ���������I�u�W�F�N�g�̂ǂ̖ʂƓ��������Ƃ��ɃA�N�V�������N�������B
		// --- �ʂ̖@�������߂�
		Float3 Normal = Primitive::Vector3_up; // ��̖ʂɐݒ�
		DirectX::XMVECTOR planeN = DirectX::XMLoadFloat3(&ConvertToDirectXFloat3(Normal));
		DirectX::XMVector3Normalize(planeN);
		// �u���b�N�̊p�x�����]�s����v�Z
		DirectX::XMMATRIX rotation;
		rotation =
			DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(object->GetComponent<Transform>()->GetAngle().y)) *
			DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(object->GetComponent<Transform>()->GetAngle().x)) *
			DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(object->GetComponent<Transform>()->GetAngle().z));
		// �@���̌������u���b�N�̉�]�s��ŕς���
		DirectX::XMVector3TransformCoord(planeN, rotation);
		// --- �ʏ�̍��W�����߂�
		DirectX::XMVECTOR Pos;
		if (Normal == Primitive::Vector3_up || Normal == Primitive::Vector3_down) {
			// ������@���ɂ��������̂��ʏ�̍��W�ƂȂ�
			Pos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&object->GetComponent<Transform>()->GetPosition()),
				DirectX::XMVectorScale(planeN, object->GetComponent<AABBCollider>()->GetPrimitive().lenY() / 2));
		}
		else if (Normal == Primitive::Vector3_forward || Normal == Primitive::Vector3_back) {
			// ���s��@���ɂ��������̂��ʏ�̍��W�ƂȂ�
			Pos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&object->GetComponent<Transform>()->GetPosition()),
				DirectX::XMVectorScale(planeN, object->GetComponent<AABBCollider>()->GetPrimitive().lenZ() / 2));
		}
		else if (Normal == Primitive::Vector3_right || Normal == Primitive::Vector3_left) {
			// ����@���ɂ��������̂��ʏ�̍��W�ƂȂ�
			Pos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&object->GetComponent<Transform>()->GetPosition()),
				DirectX::XMVectorScale(planeN, object->GetComponent<AABBCollider>()->GetPrimitive().lenX() / 2));
		}
		// --- ���ʂ̖@���ɑ΂���OBB�̎ˉe���̒������Z�o
		float r = 0.0f;		// �ߐڋ���
		float Addr;
		DirectX::XMStoreFloat(&Addr, DirectX::XMVector3Dot(
			DirectX::XMVectorScale(DirectX::XMLoadFloat3(&this->GetOwner()->GetComponent<Transform>()->GetVectorForword()),
				this->GetOwner()->GetComponent<AABBCollider>()->GetPrimitive().lenZ() / 2)
			, planeN));
		r += fabs(Addr);
		DirectX::XMStoreFloat(&Addr, DirectX::XMVector3Dot(
			DirectX::XMVectorScale(DirectX::XMLoadFloat3(&this->GetOwner()->GetComponent<Transform>()->GetVectorUp()),
				this->GetOwner()->GetComponent<AABBCollider>()->GetPrimitive().lenY() / 2)
			, planeN));
		r += fabs(Addr);
		DirectX::XMStoreFloat(&Addr, DirectX::XMVector3Dot(
			DirectX::XMVectorScale(DirectX::XMLoadFloat3(&this->GetOwner()->GetComponent<Transform>()->GetVectorRight()),
				this->GetOwner()->GetComponent<AABBCollider>()->GetPrimitive().lenX() / 2)
			, planeN));
		r += fabs(Addr);

		// --- �߂��������Z�o
		float s;
		DirectX::XMStoreFloat(&s, DirectX::XMVector3Dot(
			DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&this->GetOwner()->GetComponent<Transform>()->GetPosition()), Pos), planeN));
		if (s > 0)	s = r - fabs(s);
		else		s = r + fabs(s);

		// --- �߂荞�񂾈ʒu���畽�ʂ̖@�������ɖ߂����������I�t�Z�b�g����
		DirectX::XMFLOAT3 offsetPos;
		DirectX::XMStoreFloat3(&offsetPos, DirectX::XMVectorScale(planeN, s));
		this->GetOwner()->GetComponent<Transform>()->SetPosition({
			this->GetOwner()->GetComponent<Transform>()->GetPosition().x + offsetPos.x,
			this->GetOwner()->GetComponent<Transform>()->GetPosition().y + offsetPos.y,
			this->GetOwner()->GetComponent<Transform>()->GetPosition().z + offsetPos.z
			});
		GetOwner()->GetComponent<Rigidbody>()->SetAccele({ 0.0f, 0.0f, 0.0f });
	}

	// ��Ɠ��������Ƃ��̏���
	if (object->GetTag() == TagName::Arrow)
	{
		// �������폜
		ObjectManager::RemoveObject(GetOwner()->GetThisPtr());
	}
}

void EnemyController::OnCollisionExit(ObjectBase* object)
{

}