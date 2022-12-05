#include "ComponentBase.h"
#include "ObjectBase.h"

ComponentBase::ComponentBase()
	: m_Owner(nullptr)
{
}

// 格納オブジェクトを取得
ObjectBase* ComponentBase::GetOwner() { return m_Owner; }

// 格納オブジェクトをセット
void ComponentBase::SetOwner(ObjectBase* Owner) { m_Owner = Owner; }