#include "ColliderBase.h"
#include "ObjectBase.h"
#include "ColliderManager.h"

void ColliderBase::LateUpdate()
{
	ColliderManager::SetCollider(this);
}

void ColliderBase::CheckTouchCollider(ColliderBase* collider)
{
	// ‘OƒtƒŒ[ƒ€‚ÉG‚ê‚Ä‚¢‚½‚©ƒ`ƒFƒbƒN‚·‚é
	ColliderBase::ColliderPool::iterator it;
	for (it = m_TouchingPool.begin(); it != m_TouchingPool.end(); it++)
		if ((*it) == collider) break;

	// ã‹L‚Åˆø‚Á‚©‚©‚ç‚È‚©‚Á‚½‚ç
	if (it == m_TouchingPool.end())
	{
		// CollisionEnterŒÄ‚Ño‚µ
		GetOwner()->CallCollisionEnter(collider->GetOwner());
		collider->GetOwner()->CallCollisionEnter(GetOwner());
		// ÚGƒŠƒXƒg‚É’Ç‰Á
		m_TouchingPool.push_back(collider);
		collider->m_TouchingPool.push_back(this);
	}

	// CollisionStayŒÄ‚Ño‚µ
	GetOwner()->CallCollisionStay(collider->GetOwner());
	collider->GetOwner()->CallCollisionStay(GetOwner());

	// ÚGƒŠƒXƒg‚É’Ç‰Á
	m_justTouchedPool.push_back(collider);
	collider->m_justTouchedPool.push_back(this);
}

void ColliderBase::CheckExitCollider()
{
	// Œ³‚©‚çG‚ê‚Ä‚¢‚éƒRƒ‰ƒCƒ_[
	for (auto It = m_TouchingPool.begin(); It != m_TouchingPool.end(); /*++It*/)
	{
		// ¡G‚ê‚½ƒRƒ‰ƒCƒ_[
		ColliderPool::iterator justIt;
		for (justIt = m_justTouchedPool.begin(); justIt != m_justTouchedPool.end(); ++justIt)
		{
			// Œ³‚à¡‚àG‚ê‚Ä‚¢‚½‚ç”²‚¯‚é
			if ((*It) == (*justIt)) break;
		}
		// ã‹L‚Åˆø‚Á‚©‚©‚ç‚È‚©‚Á‚½‚ç
		//  Œ³‚ÍG‚ê‚Ä‚Ä¡‚ÍG‚ê‚Ä‚¢‚È‚¢
		if (justIt == m_justTouchedPool.end())
		{
			// CollisionExitŒÄ‚Ño‚µ
			GetOwner()->CallCollisionExit((*It)->GetOwner());
			(*It)->GetOwner()->CallCollisionExit(GetOwner());
				
			// ÚGƒŠƒXƒg‚©‚çœŠO
			(*It)->m_TouchingPool.remove(this);
			It = m_TouchingPool.erase(It);
		}
		else
		{
			++It;
		}
	}
	// ¡ƒtƒŒ[ƒ€‚ÌÚGƒŠƒXƒg‚ğíœ
	m_justTouchedPool.clear();
}

// •ª—£²‚É“Š‰e‚³‚ê‚½²¬•ª‚©‚ç“Š‰eü•ª’·‚ğZo
float ColliderBase::LenSegOnSeparateAxis(Float3 *Sep, Float3 *e1, Float3 *e2, Float3 *e3)
{
	// 3‚Â‚Ì“àÏ‚Ìâ‘Î’l‚Ì˜a‚Å“Š‰eü•ª’·‚ğŒvZ
	// •ª—£²Sep‚Í•W€‰»‚³‚ê‚Ä‚¢‚é‚±‚Æ
	Sep->normalize();
	float r1 = fabs(Sep->dot(*e1));
	float r2 = fabs(Sep->dot(*e2));
	float r3 = e3 ? fabs(Sep->dot(*e3)) : 0;
	return r1 + r2 + r3;
}