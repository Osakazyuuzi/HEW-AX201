#include "ColliderBase.h"
#include "ObjectBase.h"
#include "ColliderManager.h"

void ColliderBase::LateUpdate()
{
	ColliderManager::SetCollider(this);
}

void ColliderBase::CheckTouchCollider(ColliderBase* collider)
{
	// 前フレームに触れていたかチェックする
	ColliderBase::ColliderPool::iterator it;
	for (it = m_TouchingPool.begin(); it != m_TouchingPool.end(); it++)
		if ((*it) == collider) break;

	// 上記で引っかからなかったら
	if (it == m_TouchingPool.end())
	{
		// CollisionEnter呼び出し
		GetOwner()->CallCollisionEnter(collider->GetOwner());
		collider->GetOwner()->CallCollisionEnter(GetOwner());
		// 接触リストに追加
		m_TouchingPool.push_back(collider);
		collider->m_TouchingPool.push_back(this);
	}

	// CollisionStay呼び出し
	GetOwner()->CallCollisionStay(collider->GetOwner());
	collider->GetOwner()->CallCollisionStay(GetOwner());

	// 接触リストに追加
	m_justTouchedPool.push_back(collider);
	collider->m_justTouchedPool.push_back(this);
}

void ColliderBase::CheckExitCollider()
{
	// 元から触れているコライダー
	for (auto It = m_TouchingPool.begin(); It != m_TouchingPool.end(); /*++It*/)
	{
		// 今触れたコライダー
		ColliderPool::iterator justIt;
		for (justIt = m_justTouchedPool.begin(); justIt != m_justTouchedPool.end(); ++justIt)
		{
			// 元も今も触れていたら抜ける
			if ((*It) == (*justIt)) break;
		}
		// 上記で引っかからなかったら
		//  元は触れてて今は触れていない
		if (justIt == m_justTouchedPool.end())
		{
			// CollisionExit呼び出し
			GetOwner()->CallCollisionExit((*It)->GetOwner());
			(*It)->GetOwner()->CallCollisionExit(GetOwner());
				
			// 接触リストから除外
			(*It)->m_TouchingPool.remove(this);
			It = m_TouchingPool.erase(It);
		}
		else
		{
			++It;
		}
	}
	// 今フレームの接触リストを削除
	m_justTouchedPool.clear();
}

// 分離軸に投影された軸成分から投影線分長を算出
float ColliderBase::LenSegOnSeparateAxis(Float3 *Sep, Float3 *e1, Float3 *e2, Float3 *e3)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	Sep->normalize();
	float r1 = fabs(Sep->dot(*e1));
	float r2 = fabs(Sep->dot(*e2));
	float r3 = e3 ? fabs(Sep->dot(*e3)) : 0;
	return r1 + r2 + r3;
}