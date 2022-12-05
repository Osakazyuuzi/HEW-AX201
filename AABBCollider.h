/*
ObjectBaseクラス - 継承元専用
継承先
　Object3Dクラス - 継承元専用
　継承先
   // ここにオブジェクトを追加していく。
　UI - 


*/

#ifndef ___AABBCOLLIDER_H___
#define ___AABBCOLLIDER_H___

#include "Primitive.h"
#include "AABB.h"
#include "ColliderBase.h"
#include "Point.h"
#include "Float3.h"
#include "Transform.h"

class AABBCollider : public ColliderBase
{
private:
	// 形状
	Primitive::AABB m_Primitive;
	// PointをTransformから自動設定
	bool m_bPointAuto;
	// ロード時の情報 - モデルのコンストラクタ内でのみ変更を加える
	Transform m_RoadTransData;

public:
	// コンストラクタ
	AABBCollider()
		: ColliderBase()
		, m_bPointAuto(true)
	{}
	// デストラクタ
	~AABBCollider(){}

	void LateUpdate();

	// 適切な処理を呼び出す
	void CallTouchOperation(ColliderBase* collider) override;

	//--- AABBとの当たり判定
	void TouchingTheAABB(ColliderBase* _AABB) override;
	//--- Sphereとの当たり判定
	void TouchingTheSphere(ColliderBase* _Sphere) override;

	//--- ゲット関数
	Primitive::AABB& GetPrimitive() { return m_Primitive; }

	//--- セット関数
	void SetLoadPoint(DirectX::XMFLOAT3 point) { m_RoadTransData.SetPosition(point); }
	void SetLoadLen(DirectX::XMFLOAT3 len) { m_RoadTransData.SetScale(len); }
	void SetPoint(Primitive::Float3 point) { m_Primitive.p = point; }
	void SetLen(Primitive::Float3 len) { m_Primitive.hl = len/2; }
	void SetLenX(float len) { m_Primitive.hl.x = len/2; }
	void SetLenY(float len) { m_Primitive.hl.y = len/2; }
	void SetLenZ(float len) { m_Primitive.hl.z = len/2; }
	void SetPointAuto(bool bAuto) { m_bPointAuto = bAuto; }
};

#endif //!___AABBCOLLIDER_H___