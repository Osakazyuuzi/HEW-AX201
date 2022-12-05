#include "ObjectManager.h"
#include "SceneManager.h"
#include "ColliderManager.h"

std::vector<std::shared_ptr<ObjectBase>> ObjectManager::m_AppendObjectPool;
std::vector<std::shared_ptr<ObjectBase>> ObjectManager::m_ObjectPool;
std::vector<std::shared_ptr<ObjectBase>> ObjectManager::m_DestroyObjectPool;

// オブジェクトの更新
void ObjectManager::Update() {

	SceneManager::LoadUpdate();

	for (auto it = m_AppendObjectPool.begin(); it != m_AppendObjectPool.end(); ++it) {
		for (auto CompIt = (*it)->m_ComponentPool.begin(); CompIt != (*it)->m_ComponentPool.end(); ++CompIt) {
			(*CompIt)->Start();
		}
		m_ObjectPool.push_back((*it));
	}
	m_AppendObjectPool.clear();

	ColliderManager::Update();

	for (auto it = m_ObjectPool.begin(); it != m_ObjectPool.end(); it++) {
		if (!(*it)->GetDestroy())
			(*it)->Update();
	}

	Delete();
}