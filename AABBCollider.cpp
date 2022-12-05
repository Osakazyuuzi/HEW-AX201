#include "AABBCollider.h"
#include "Primitive.h"
#include "Float3.h"
#include "ObjectBase.h"
#include "Transform.h"
#include "ColliderManager.h"

using namespace Primitive;


void AABBCollider::LateUpdate()
{
	// 自動設定なら
	if (m_bPointAuto) {
		m_Primitive.p = ConvertToFloat3(GetOwner()->GetComponent<Transform>()->GetPosition());
//		m_Primitive.hl.x = GetOwner()->GetComponent<Transform>()->GetScale().x / 2;
//		m_Primitive.hl.y = GetOwner()->GetComponent<Transform>()->GetScale().y / 2;
//		m_Primitive.hl.z = GetOwner()->GetComponent<Transform>()->GetScale().z / 2;
	}

	ColliderBase::LateUpdate();
}

// 適切な処理を呼び出す
void AABBCollider::CallTouchOperation(ColliderBase* collider)
{
	TouchingTheAABB(collider);
}

//--- AABBとの当たり判定
void AABBCollider::TouchingTheAABB(ColliderBase* _AABB)
{
	//--- OBBとOBBでの当たり判定処理
   // 各方向ベクトルの確保
   // （N***:標準化方向ベクトル）
	// メモ
	// 0-X-Right	-e1
	// 1-Y-Up		-e2
	// 2-Z-Forword	-e3
	Float3 NAe1 = Primitive::ConvertToFloat3(GetOwner()->GetComponent<Transform>()->GetVectorRight()), Ae1 = NAe1 * this->GetPrimitive().lenX()/2;
	Float3 NAe2 = Primitive::ConvertToFloat3(GetOwner()->GetComponent<Transform>()->GetVectorUp()), Ae2 = NAe2 * this->GetPrimitive().lenY()/2;
	Float3 NAe3 = Primitive::ConvertToFloat3(GetOwner()->GetComponent<Transform>()->GetVectorForword()), Ae3 = NAe3 * this->GetPrimitive().lenZ()/2;
	Float3 NBe1 = Primitive::ConvertToFloat3(_AABB->GetOwner()->GetComponent<Transform>()->GetVectorRight()), Be1 = NBe1 * _AABB->GetPrimitive().lenX()/2;
	Float3 NBe2 = Primitive::ConvertToFloat3(_AABB->GetOwner()->GetComponent<Transform>()->GetVectorUp()), Be2 = NBe2 * _AABB->GetPrimitive().lenY()/2;
	Float3 NBe3 = Primitive::ConvertToFloat3(_AABB->GetOwner()->GetComponent<Transform>()->GetVectorForword()), Be3 = NBe3 * _AABB->GetPrimitive().lenZ()/2;
	Float3 Interval = this->GetPrimitive().p - _AABB->GetPrimitive().p;

	// 分離軸 : Ae1
	float rA = Ae1.magnitude();
	float rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	float L = fabs(Interval.dot(NAe1));
	if (L > rA + rB)
		return; // 衝突していない

   // 分離軸 : Ae2
	rA = Ae2.magnitude();
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = fabs(Interval.dot(NAe2));
	if (L > rA + rB)
		return;

	// 分離軸 : Ae3
	rA = Ae3.magnitude();
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = fabs(Interval.dot(NAe3));
	if (L > rA + rB)
		return;

	// 分離軸 : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = Be1.magnitude();
	L = fabs(Interval.dot(NBe1));
	if (L > rA + rB)
		return;

	// 分離軸 : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = Be2.magnitude();
	L = fabs(Interval.dot(NBe2));
	if (L > rA + rB)
		return;

	// 分離軸 : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = Be3.magnitude();
	L = fabs(Interval.dot(NBe3));
	if (L > rA + rB)
		return;

	// 分離軸 : C11
	Float3 Cross;
	Cross = NAe1.cross(NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Interval.dot(Cross));
	if (L > rA + rB)
		return;

	// 分離軸 : C12
	Cross = NAe1.cross(NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Interval.dot(Cross));
	if (L > rA + rB)
		return;

	// 分離軸 : C13
	Cross = NAe1.cross(NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Interval.dot(Cross));
	if (L > rA + rB)
		return;

	// 分離軸 : C21
	Cross = NAe2.cross(NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Interval.dot(Cross));
	if (L > rA + rB)
		return;

	// 分離軸 : C22
	Cross = NAe2.cross(NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Interval.dot(Cross));
	if (L > rA + rB)
		return;

	// 分離軸 : C23
	Cross = NAe2.cross(NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Interval.dot(Cross));
	if (L > rA + rB)
		return;

	// 分離軸 : C31
	Cross = NAe3.cross(NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Interval.dot(Cross));
	if (L > rA + rB)
		return;

	// 分離軸 : C32
	Cross = NAe3.cross(NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Interval.dot(Cross));
	if (L > rA + rB)
		return;

	// 分離軸 : C33
	Cross = NAe3.cross(NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Interval.dot(Cross));
	if (L > rA + rB)
		return;

	// 分離平面が存在しないので「衝突している」
	CheckTouchCollider(_AABB);
}

//--- Sphereとの当たり判定
void AABBCollider::TouchingTheSphere(ColliderBase* _Sphere)
{

}