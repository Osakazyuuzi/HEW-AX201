#ifndef ___COLLIDER_H___
#define ___COLLIDER_H___

#include "ComponentBase.h"
#include "Primitive.h"
#include "AABB.h"
#include <list>
#include <memory>

using namespace Primitive;

class ColliderBase : public ComponentBase
{
	using ColliderPool = std::list<ColliderBase*>;
public:
	// コンストラクタ
	ColliderBase(){}
	// デストラクタ
	virtual ~ColliderBase(){}

	// 後更新
	void LateUpdate() override;

	// 適切な処理を呼び出す
	virtual void CallTouchOperation(ColliderBase* collider) = 0;

	//--- 当たり判定処理
	// AABBとの当たり判定
	virtual void TouchingTheAABB(ColliderBase* _AABB) = 0;
	// Sphereとの当たり判定
	virtual void TouchingTheSphere(ColliderBase* _Sphere) = 0;

	virtual void CheckTouchCollider(ColliderBase* collider);
	virtual void CheckExitCollider();

	// 分離軸に投影された軸成分から投影線分長を算出
	float LenSegOnSeparateAxis(Float3 *Sep, Float3 *e1, Float3 *e2, Float3 *e3 = 0);

	//--- ゲット関数
	virtual Primitive::AABB& GetPrimitive() = 0;

protected:
	// 元から触れているコライダーのリスト
	ColliderPool m_TouchingPool;
	// 今触れたコライダーのリスト
	ColliderPool m_justTouchedPool;
};

#endif //!___COLLIDER_H___