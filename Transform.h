#ifndef _____TRANSFORM_H_____
#define _____TRANSFORM_H_____

#include"ComponentBase.h"
#include <DirectXMath.h>

class Transform : public ComponentBase
{
public:
	// �R���X�g���N�^
	Transform();
	// �f�X�g���N�^
	~Transform();

	void Update();

	// �ʒu��vec����������
	void MovePosition(const DirectX::XMFLOAT3& vec);
	// angle������]������
	void RotateAngle(const DirectX::XMFLOAT3& angle);
	// �X�P�[����scale�����傫������
	void EnlargeScale(const DirectX::XMFLOAT3& scale);
	// �X�P�[����n�{����
	void EnlargeScaleN(float n);

	// Transform���q�I�u�W�F�N�g�p�E�e�I�u�W�F�N�g�p�ɐݒ�
	void IsRoot(bool root);

	//--- �Z�b�g�֐�
	void SetPosition(const DirectX::XMFLOAT3& pos);
	void SetAngle(const DirectX::XMFLOAT3& angle);
	void SetScale(const DirectX::XMFLOAT3& scale);

	//--- �Q�b�g�֐�
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetAngle();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMFLOAT3 GetLocalPosition();
	DirectX::XMFLOAT3 GetLocalAngle();
	DirectX::XMFLOAT3 GetLocalScale();
	DirectX::XMFLOAT3 GetVectorForword() {
		DirectX::XMMATRIX rotation; 
		rotation =
			DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_angle.y)) *
			DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_angle.x)) *
			DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_angle.z));
		DirectX::XMFLOAT3 value;
		value.x = 0.0f;
		value.y = 0.0f;
		value.z = 1.0f;
		DirectX::XMVECTOR forword = DirectX::XMLoadFloat3(&value);
		DirectX::XMStoreFloat3(&value, DirectX::XMVector3TransformCoord(forword, rotation));
		return value;
	}
	DirectX::XMFLOAT3 GetVectorRight() {
		DirectX::XMMATRIX rotation;
		rotation =
			DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_angle.y)) *
			DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_angle.x)) *
			DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_angle.z));
		DirectX::XMFLOAT3 value;
		value.x = 1.0f;
		value.y = 0.0f;
		value.z = 0.0f;
		DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&value);
		DirectX::XMStoreFloat3(&value, DirectX::XMVector3TransformCoord(right, rotation));
		return value;
	}
	DirectX::XMFLOAT3 GetVectorUp() {
		DirectX::XMMATRIX rotation;
		rotation =
			DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_angle.y)) *
			DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_angle.x)) *
			DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_angle.z));
		DirectX::XMFLOAT3 value;
		value.x = 0.0f;
		value.y = 1.0f;
		value.z = 0.0f;
		DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&value);
		DirectX::XMStoreFloat3(&value, DirectX::XMVector3TransformCoord(up, rotation));
		return value;
	}

private:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_angle;
	DirectX::XMFLOAT3 m_scale;

	// �K�w�̈�ԏォ�ǂ���
	bool m_isRoot;
	// �Ō��false�ɂȂ��Ă���ύX�����������ǂ���
	//bool m_hasChange;
};

#endif // !_____TRANSFORM_H_____
