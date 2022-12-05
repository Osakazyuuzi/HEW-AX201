#include "Rigidbody.h"
#include "ObjectBase.h"
#include "Transform.h"
#include "ObjectManager.h"

void Rigidbody::FixedUpdate()
{
	// 加速度に重力を適用
	m_Accele.y -= m_Mass * mc_gravity;
}

void Rigidbody::LateUpdate()
{
	// オブジェクトの位置に加速度を適用
	GetOwner()->GetComponent<Transform>()->SetPosition(
		{ GetOwner()->GetComponent<Transform>()->GetPosition().x + m_Accele.x,
		GetOwner()->GetComponent<Transform>()->GetPosition().y + m_Accele.y,
		GetOwner()->GetComponent<Transform>()->GetPosition().z + m_Accele.z }
	);

	// 削除補正
	if (GetOwner()->GetComponent<Transform>()->GetPosition().y < -10.0f)
		if (!GetOwner()->GetDestroy())
			ObjectManager::RemoveObject(GetOwner()->GetThisPtr());
}