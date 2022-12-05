#include"Transform.h"
#include"ObjectBase.h"

#include "ObjectManager.h"
#include <memory>

Transform::Transform()
	: m_position(0.0f, 0.0f, 0.0f)
	, m_angle(0.0f, 0.0f, 0.0f)
	, m_scale(1.0f, 1.0f, 1.0f)
	, m_isRoot(true)
{

}

Transform::~Transform()
{

}

void Transform::Update()
{
}

void Transform::SetPosition(const DirectX::XMFLOAT3& pos)
{
	m_position = pos;
}

void Transform::SetAngle(const DirectX::XMFLOAT3& angle)
{
	m_angle = angle;
}

void Transform::SetScale(const DirectX::XMFLOAT3& scale)
{
	m_scale = scale;
}

DirectX::XMFLOAT3 Transform::GetPosition()
{
	if (!m_isRoot)
	{
		return {
			m_position.x + GetOwner()->GetParent()->GetComponent<Transform>()->GetPosition().x,
			m_position.y + GetOwner()->GetParent()->GetComponent<Transform>()->GetPosition().y,
			m_position.z + GetOwner()->GetParent()->GetComponent<Transform>()->GetPosition().z
		};
	}
	return m_position;
}

DirectX::XMFLOAT3 Transform::GetAngle()
{
	if (!m_isRoot)
	{
		return {
			m_angle.x + GetOwner()->GetParent()->GetComponent<Transform>()->GetAngle().x,
			m_angle.y + GetOwner()->GetParent()->GetComponent<Transform>()->GetAngle().y,
			m_angle.z + GetOwner()->GetParent()->GetComponent<Transform>()->GetAngle().z
		};
	}
	return m_angle;
}

DirectX::XMFLOAT3 Transform::GetScale()
{
	if (!m_isRoot)
	{
		return {
			m_scale.x + GetOwner()->GetParent()->GetComponent<Transform>()->GetScale().x,
			m_scale.y + GetOwner()->GetParent()->GetComponent<Transform>()->GetScale().y,
			m_scale.z + GetOwner()->GetParent()->GetComponent<Transform>()->GetScale().z
		};
	}
	return m_scale;
}

DirectX::XMFLOAT3 Transform::GetLocalPosition()
{
	return m_position;
}

DirectX::XMFLOAT3 Transform::GetLocalAngle()
{
	return m_angle;
}

DirectX::XMFLOAT3 Transform::GetLocalScale()
{
	return m_scale;
}

void Transform::MovePosition(const DirectX::XMFLOAT3& vec)
{
	m_position.x += vec.x;
	m_position.y += vec.y;
	m_position.z += vec.z;
}

void Transform::RotateAngle(const DirectX::XMFLOAT3& angle)
{
	m_angle.x += angle.x;
	m_angle.y += angle.y;
	m_angle.z += angle.z;
}

void Transform::EnlargeScale(const DirectX::XMFLOAT3& scale)
{
	m_scale.x += scale.x;
	m_scale.y += scale.y;
	m_scale.z += scale.z;
}

void Transform::EnlargeScaleN(float n)
{
	m_scale.x *= n;
	m_scale.y *= n;
	m_scale.z *= n;
}

void Transform::IsRoot(bool root)
{
	if (m_isRoot == root) return;

	// 子オブジェクトに変更
	if (root == false) {
		m_position.x = 0.0f;
		m_position.y = 0.0f;
		m_position.z = 0.0f;
	}

	m_isRoot = root;
}