#include "ObjectBase.h"
#include "ObjectManager.h"
#include "Transform.h"
#include "ColliderManager.h"

// コンストラクタ
ObjectBase::ObjectBase()
	: m_name("Object")
	, m_tag(TagName::Untagged)
	, m_isDestroy(false)
	, m_Parent(nullptr)
	, m_LayerNum(0)
{
	// デフォルトコンポーネントを追加
	AddComponent<Transform>();
}

// 更新
void ObjectBase::Update() {
	for (auto it = m_ComponentPool.begin(); it != m_ComponentPool.end(); it++)
		(*it)->Update();
}

// オブジェクトを削除する
void ObjectBase::Destroy() {
	ObjectManager::RemoveObject(static_cast<std::shared_ptr<ObjectBase>>(this));
}

// レイヤー番号による比較
bool operator<(const std::shared_ptr<ObjectBase>& left, const std::shared_ptr<ObjectBase>& right)
{
	return left->GetLayerNum() < right->GetLayerNum();
}
bool operator>(const std::shared_ptr<ObjectBase>& left, const std::shared_ptr<ObjectBase>& right)
{
	return left->GetLayerNum() > right->GetLayerNum();
}