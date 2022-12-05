#include "PlayerController.h"
#include "Input.h"
#include "Transform.h"
#include "ObjectBase.h"
#include "Rigidbody.h"
#include "AABBCollider.h"
#include "XInput.h"
#include "Primitive.h"
#include "Vector3.h"
#include "Float3.h"
#include "CameraDebug.h"
#include "ObjectManager.h"
#include "Arrow.h"
#include <math.h>
#include "CameraPlayer.h"

void PlayerController::Update()
{
	// �J������Player�J�����ł͂Ȃ����͍X�V���Ȃ�
	if (!ObjectManager::FindObjectWithTag(TagName::MainCamera)->GetComponent<CameraPlayer>()) return;

	//--- �ړ��i�J�����̌����ɉ����Ĉړ����������߂�j
	// ���C���J�����̍��W�ƒ����_���擾����
	DirectX::XMFLOAT3 camPos = ObjectManager::FindObjectWithTag(TagName::MainCamera)->GetComponent<Transform>()->GetPosition();
	DirectX::XMFLOAT3 camLook = ObjectManager::FindObjectWithTag(TagName::MainCamera)->GetComponent<CameraPlayer>()->GetLookPoint();
	// Y���ł̃v���C���[�̈ړ��͗v��Ȃ����߁A0.0f�ɐݒ�
	camPos.y = 0.0f;
	camLook.y = 0.0f;
	// XMVECTOR�ɕϊ�
	DirectX::XMVECTOR vCamPos = DirectX::XMLoadFloat3(&camPos);
	DirectX::XMVECTOR vCamLook = DirectX::XMLoadFloat3(&camLook);
	// ���W���璍���_�֌����x�N�g�����Z�o(����)
	DirectX::XMVECTOR vFront;
	vFront = DirectX::XMVectorSubtract(vCamLook, vCamPos);
	vFront = DirectX::XMVector3Normalize(vFront);
	// ���ʕ����ɑ΂��āAY����90����]�������������̃x�N�g�����Z�o
	DirectX::XMMATRIX matRotSide = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(90.0f));
	DirectX::XMVECTOR vSide = DirectX::XMVector3TransformCoord(vFront, matRotSide);

	DirectX::XMVECTOR vMove = DirectX::XMVectorZero();
	// �΂߈ړ�����
	if (IsKeyPress('W')) vMove = DirectX::XMVectorAdd(vMove, vFront);
	if (IsKeyPress('S')) vMove = DirectX::XMVectorAdd(vMove, DirectX::XMVectorScale(vFront, -1.0f));
	if (IsKeyPress('A')) vMove = DirectX::XMVectorAdd(vMove, DirectX::XMVectorScale(vSide, -1.0f));
	if (IsKeyPress('D')) vMove = DirectX::XMVectorAdd(vMove, vSide);
	// �΂߈ړ��̂Ƃ��Ɉړ��ʂ������Ȃ��Ă��܂����߁A���K������
	vMove = DirectX::XMVector3Normalize(vMove);
	vMove = DirectX::XMVectorScale(vMove, 0.1f);

	DirectX::XMFLOAT3 move;
	DirectX::XMStoreFloat3(&move, vMove);
	GetOwner()->GetComponent<Transform>()->SetPosition({
		GetOwner()->GetComponent<Transform>()->GetPosition().x + move.x,
		GetOwner()->GetComponent<Transform>()->GetPosition().y + move.y,
		GetOwner()->GetComponent<Transform>()->GetPosition().z + move.z
		});
	// �ړ������ꍇ�A�ړ����������ɉ�]����
	if (move.x != 0.0f || move.y != 0.0f || move.z != 0.0f) {
		float radY = 0.0f;
		// Z�����ւ̃x�N�g��(���f���̐��ʕ����̃x�N�g��)
		DirectX::XMFLOAT3 zVector = { 0.0f, 0.0f, 1.0f };
		// ���ςƃx�N�g���̒������g����cos�Ƃ����߂�
		DirectX::XMStoreFloat(&radY, DirectX::XMVector3Dot(DirectX::XMVector3Normalize(vMove), DirectX::XMLoadFloat3(&zVector)));
		// ���ς���p�x�����߂�
		radY = ::acos(radY);
		// ���W�A���p���炨�Ȃ��݂̊p�x�ɕύX
		radY = DirectX::XMConvertToDegrees(radY);
		// ��]���E��]������]���𔻕ʂ��邽�߂ɁA�O�ςŋ��߂�
		// ���߂��O�ς�Y�������v���X�������獶���B
		// ���߂��O�ς�Y�������}�C�i�X��������E���B
		DirectX::XMFLOAT3 rotateDirection;
		DirectX::XMStoreFloat3(&rotateDirection, DirectX::XMVector3Cross(DirectX::XMVector3Normalize(vMove), DirectX::XMLoadFloat3(&zVector)));
		if (rotateDirection.y > 0) radY = 180.0f + (180.0f - radY);
		// �Z�o�����p�x��K�p����
		GetOwner()->GetComponent<Transform>()->SetAngle({ 0.0f, radY, 0.0f });
	}

	//--- ����
	if (IsKeyRelease(VK_SPACE)) {
		// �ύX�p�|�C���^
		std::shared_ptr<Transform> trans;
		std::shared_ptr<Rigidbody> rb;

		//--- �I�u�W�F�N�g�쐬
		//   �^�@�FArrow
		//  ���O �FArrow
		// �^�O���FArrow
		ObjectManager::CreateObject<Arrow>("Arrow", TagName::Arrow);
		// "Arrow"�Ƃ������O�̃I�u�W�F�N�g��Transform���擾
		trans = ObjectManager::FindObjectWithName("Arrow")->GetComponent<Transform>();
		// ���W�������̃I�u�W�F�N�g�{�����I�u�W�F�N�g�̖@���i�����P�j�̈ʒu�ɐݒ�
		trans->SetPosition({
			GetOwner()->GetComponent<Transform>()->GetPosition().x + GetOwner()->GetComponent<Transform>()->GetVectorForword().x,
			GetOwner()->GetComponent<Transform>()->GetPosition().y + GetOwner()->GetComponent<Transform>()->GetVectorForword().y,
			GetOwner()->GetComponent<Transform>()->GetPosition().z + GetOwner()->GetComponent<Transform>()->GetVectorForword().z
			});
		// �T�C�Y��ݒ�
		trans->SetScale({ 0.3f, 0.3f, 0.3f });
		// �p�x�������̃I�u�W�F�N�g�̊p�x�ɐݒ�
		trans->SetAngle({
			GetOwner()->GetComponent<Transform>()->GetAngle().x,
			GetOwner()->GetComponent<Transform>()->GetAngle().y + 180.0f,// ��̃��f���Ɩ���ˏo���郂�f���̐��ʂ��Ⴄ�ꍇ�A�����Ő��l��������B
			GetOwner()->GetComponent<Transform>()->GetAngle().z
			});
		// "Arrow"�Ƃ������O�̃I�u�W�F�N�g��Rigidbody���擾
		rb = ObjectManager::FindObjectWithName("Arrow")->GetComponent<Rigidbody>();
		// �����x���I�u�W�F�N�g�̐��ʕ����ɐݒ�
		rb->SetAccele(GetOwner()->GetComponent<Transform>()->GetVectorForword());
	}

	//--- ���W�␳
	// ��������
	if (GetOwner()->GetComponent<Rigidbody>()) {
		// y���W��-9�ȉ�
		if (GetOwner()->GetComponent<Transform>()->GetPosition().y < -9.0f) {
			// ���W��␳
			GetOwner()->GetComponent<Transform>()->SetPosition({
				GetOwner()->GetComponent<Transform>()->GetPosition().x,
				0.0f,
				GetOwner()->GetComponent<Transform>()->GetPosition().z
				});
			// �����x��␳
			GetOwner()->GetComponent<Rigidbody>()->SetAccele({ 0.0f, 0.0f, 0.0f });
		}
	}
}

void PlayerController::OnCollisionEnter(ObjectBase* object)
{
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
	}
}

void PlayerController::OnCollisionStay(ObjectBase* object)
{
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
}

void PlayerController::OnCollisionExit(ObjectBase* object)
{

}