#include "Rigidbody.h"
#include "ObjectBase.h"
#include "Transform.h"
#include "ObjectManager.h"

void Rigidbody::FixedUpdate()
{
	// �����x�ɏd�͂�K�p
	m_Accele.y -= m_Mass * mc_gravity;
}

void Rigidbody::LateUpdate()
{
	// �I�u�W�F�N�g�̈ʒu�ɉ����x��K�p
	GetOwner()->GetComponent<Transform>()->SetPosition(
		{ GetOwner()->GetComponent<Transform>()->GetPosition().x + m_Accele.x,
		GetOwner()->GetComponent<Transform>()->GetPosition().y + m_Accele.y,
		GetOwner()->GetComponent<Transform>()->GetPosition().z + m_Accele.z }
	);

	// �폜�␳
	if (GetOwner()->GetComponent<Transform>()->GetPosition().y < -10.0f)
		if (!GetOwner()->GetDestroy())
			ObjectManager::RemoveObject(GetOwner()->GetThisPtr());
}