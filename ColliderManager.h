#ifndef ___COLLIDERMANAGER_H___
#define ___COLLIDERMANAGER_H___

#include "ColliderBase.h"
#include <list>

class ColliderManager
{
public:
	// コンストラクタ
	ColliderManager(){}
	// デストラクタ
	~ColliderManager(){}

	// 更新
	static void Update();
	// 当たり判定を登録
	static void SetCollider(ColliderBase* collider) {
		m_ColliderList.push_back(collider);
	}

private:
	// 当たり判定リスト
	static std::list<ColliderBase*> m_ColliderList;
};

#endif //!___COLLIDERMANAGER_H___