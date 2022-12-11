#include "EnemyController.h"
#include "Transform.h"
#include "ObjectBase.h"
#include "Primitive.h"
#include "Vector3.h"
#include "Float3.h"
#include "ObjectManager.h"
#include "Arrow.h"

void EnemyController::Update()
{
	//--- Playerを追いかける処理
	DirectX::XMFLOAT3 rotateDirection;
	m_tic++;
	if (m_tic >= m_ChangeTargetTime) {
		m_tic = rand() % (int)m_ChangeTargetTime;
		// Playerタグのオブジェクトの座標を取得
		DirectX::XMFLOAT3 PlayerPos = ObjectManager::FindObjectWithTag(TagName::Player)->GetComponent<Transform>()->GetPosition();
		// ターゲットとなるプレイヤーの座標をランダムでずらす
		PlayerPos.x += rand() % 8 - 4;
		PlayerPos.z += rand() % 8 - 4;

		// ターゲットまでのベクトルを算出
		DirectX::XMVECTOR vTargetVector =
			DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&PlayerPos), DirectX::XMLoadFloat3(&GetOwner()->GetComponent<Transform>()->GetPosition()));
		// ベクトルを正規化
		vTargetVector = DirectX::XMVector3Normalize(vTargetVector);
		// 移動スピードを掛ける
		vTargetVector = DirectX::XMVectorScale(vTargetVector, m_MoveSpeed);
		// Float3に変換する
		DirectX::XMStoreFloat3(&m_TargetVector, vTargetVector);

		// 移動した場合、移動した方向に回転する
		if (m_TargetVector.x != 0.0f || m_TargetVector.y != 0.0f || m_TargetVector.z != 0.0f) {
			m_TargetRotY = 0.0f;
			// Z方向へのベクトル(モデルの正面方向のベクトル)
			DirectX::XMFLOAT3 zVector = { 0.0f, 0.0f, 1.0f };
			// 内積とベクトルの長さを使ってcosθを求める
			DirectX::XMStoreFloat(&m_TargetRotY, DirectX::XMVector3Dot(DirectX::XMVector3Normalize(vTargetVector), DirectX::XMLoadFloat3(&zVector)));
			// 内積から角度を求める
			m_TargetRotY = ::acos(m_TargetRotY);
			// ラジアン角からおなじみの角度に変更
			m_TargetRotY = DirectX::XMConvertToDegrees(m_TargetRotY);
			// 回転が右回転か左回転かを判別するために、外積で求める
			// 求めた外積のY成分がプラスだったら左回り。
			// 求めた外積のY成分がマイナスだったら右回り。
			DirectX::XMStoreFloat3(&rotateDirection, DirectX::XMVector3Cross(DirectX::XMVector3Normalize(vTargetVector), DirectX::XMLoadFloat3(&zVector)));
			if (rotateDirection.y > 0) m_TargetRotY = 180.0f + (180.0f - m_TargetRotY);
		}
	}

	// 座標に適用する
	GetOwner()->GetComponent<Transform>()->SetPosition({
		GetOwner()->GetComponent<Transform>()->GetPosition().x + m_TargetVector.x,
		GetOwner()->GetComponent<Transform>()->GetPosition().y,
		GetOwner()->GetComponent<Transform>()->GetPosition().z + m_TargetVector.z
		});

	// 二つの間の角度が180°超えていた場合、逆回転の方が速いため、補正
	if (abs(m_lateRotY - m_TargetRotY) >= 180.0f) 
		if (m_TargetRotY < 0.0f) m_TargetRotY = (int)m_TargetRotY + 360;
		else if (m_TargetRotY >= 0.0f) m_TargetRotY = (int)m_TargetRotY - 360;
	// 180°を超えたもしくは、-180°以下になったときに範囲内に補正する
	if (m_lateRotY < -180)  m_lateRotY = ((int)(fabs(m_lateRotY) + 180) % 360 - 180) * -1.0f;
	if ( 180 <= m_lateRotY) m_lateRotY = (int)(m_lateRotY + 180) % 360 - 180;
	// 遅れてついていく場合の角度を計算し適応する
	m_lateRotY = (m_TargetRotY - m_lateRotY) * 0.05f + m_lateRotY;
	GetOwner()->GetComponent<Transform>()->SetAngle({ 0.0f, m_lateRotY,0.0f });
}

void EnemyController::OnCollisionEnter(ObjectBase* object)
{
	// フィールドと当たったときの処理
	if (object->GetTag() == TagName::Field || 
		object->GetTag() == TagName::Enemy) {
		//--- 壁(Plane)にめり込んだOBBを戻す処理
		// 当たったオブジェクトのどの面と当たったときにアクションを起こすか。
		// --- 面の法線を求める
		Float3 Normal = Primitive::Vector3_up; // 上の面に設定
		DirectX::XMVECTOR planeN = DirectX::XMLoadFloat3(&ConvertToDirectXFloat3(Normal));
		DirectX::XMVector3Normalize(planeN);
		// ブロックの角度から回転行列を計算
		DirectX::XMMATRIX rotation;
		rotation =
			DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(object->GetComponent<Transform>()->GetAngle().y)) *
			DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(object->GetComponent<Transform>()->GetAngle().x)) *
			DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(object->GetComponent<Transform>()->GetAngle().z));
		// 法線の向きをブロックの回転行列で変える
		DirectX::XMVector3TransformCoord(planeN, rotation);
		// --- 面上の座標を求める
		DirectX::XMVECTOR Pos;
		if (Normal == Primitive::Vector3_up || Normal == Primitive::Vector3_down) {
			// 高さを法線にかけたものが面上の座標となる
			Pos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&object->GetComponent<Transform>()->GetPosition()),
				DirectX::XMVectorScale(planeN, object->GetComponent<AABBCollider>()->GetPrimitive().lenY() / 2));
		}
		else if (Normal == Primitive::Vector3_forward || Normal == Primitive::Vector3_back) {
			// 奥行を法線にかけたものが面上の座標となる
			Pos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&object->GetComponent<Transform>()->GetPosition()),
				DirectX::XMVectorScale(planeN, object->GetComponent<AABBCollider>()->GetPrimitive().lenZ() / 2));
		}
		else if (Normal == Primitive::Vector3_right || Normal == Primitive::Vector3_left) {
			// 幅を法線にかけたものが面上の座標となる
			Pos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&object->GetComponent<Transform>()->GetPosition()),
				DirectX::XMVectorScale(planeN, object->GetComponent<AABBCollider>()->GetPrimitive().lenX() / 2));
		}
		// --- 平面の法線に対するOBBの射影線の長さを算出
		float r = 0.0f;		// 近接距離
		float Addr;
		DirectX::XMStoreFloat(&Addr, DirectX::XMVector3Dot(
			DirectX::XMVectorScale(DirectX::XMLoadFloat3(&this->GetOwner()->GetComponent<Transform>()->GetVectorForword()),
				this->GetOwner()->GetComponent<AABBCollider>()->GetPrimitive().lenZ() / 2)
			, planeN));
		r += fabs(Addr);
		DirectX::XMStoreFloat(&Addr, DirectX::XMVector3Dot(
			DirectX::XMVectorScale(DirectX::XMLoadFloat3(&this->GetOwner()->GetComponent<Transform>()->GetVectorUp()),
				this->GetOwner()->GetComponent<AABBCollider>()->GetPrimitive().lenY() / 2)
			, planeN));
		r += fabs(Addr);
		DirectX::XMStoreFloat(&Addr, DirectX::XMVector3Dot(
			DirectX::XMVectorScale(DirectX::XMLoadFloat3(&this->GetOwner()->GetComponent<Transform>()->GetVectorRight()),
				this->GetOwner()->GetComponent<AABBCollider>()->GetPrimitive().lenX() / 2)
			, planeN));
		r += fabs(Addr);

		// --- 戻し距離を算出
		float s;
		DirectX::XMStoreFloat(&s, DirectX::XMVector3Dot(
			DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&this->GetOwner()->GetComponent<Transform>()->GetPosition()), Pos), planeN));
		if (s > 0)	s = r - fabs(s);
		else		s = r + fabs(s);

		// --- めり込んだ位置から平面の法線方向に戻し距離だけオフセットする
		DirectX::XMFLOAT3 offsetPos;
		DirectX::XMStoreFloat3(&offsetPos, DirectX::XMVectorScale(planeN, s));
		this->GetOwner()->GetComponent<Transform>()->SetPosition({
			this->GetOwner()->GetComponent<Transform>()->GetPosition().x + offsetPos.x,
			this->GetOwner()->GetComponent<Transform>()->GetPosition().y + offsetPos.y,
			this->GetOwner()->GetComponent<Transform>()->GetPosition().z + offsetPos.z
			});
	}
}

void EnemyController::OnCollisionStay(ObjectBase* object)
{
	// フィールドと当たったときの処理
	if (object->GetTag() == TagName::Field) {
		//--- 壁(Plane)にめり込んだOBBを戻す処理
		// 当たったオブジェクトのどの面と当たったときにアクションを起こすか。
		// --- 面の法線を求める
		Float3 Normal = Primitive::Vector3_up; // 上の面に設定
		DirectX::XMVECTOR planeN = DirectX::XMLoadFloat3(&ConvertToDirectXFloat3(Normal));
		DirectX::XMVector3Normalize(planeN);
		// ブロックの角度から回転行列を計算
		DirectX::XMMATRIX rotation;
		rotation =
			DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(object->GetComponent<Transform>()->GetAngle().y)) *
			DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(object->GetComponent<Transform>()->GetAngle().x)) *
			DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(object->GetComponent<Transform>()->GetAngle().z));
		// 法線の向きをブロックの回転行列で変える
		DirectX::XMVector3TransformCoord(planeN, rotation);
		// --- 面上の座標を求める
		DirectX::XMVECTOR Pos;
		if (Normal == Primitive::Vector3_up || Normal == Primitive::Vector3_down) {
			// 高さを法線にかけたものが面上の座標となる
			Pos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&object->GetComponent<Transform>()->GetPosition()),
				DirectX::XMVectorScale(planeN, object->GetComponent<AABBCollider>()->GetPrimitive().lenY() / 2));
		}
		else if (Normal == Primitive::Vector3_forward || Normal == Primitive::Vector3_back) {
			// 奥行を法線にかけたものが面上の座標となる
			Pos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&object->GetComponent<Transform>()->GetPosition()),
				DirectX::XMVectorScale(planeN, object->GetComponent<AABBCollider>()->GetPrimitive().lenZ() / 2));
		}
		else if (Normal == Primitive::Vector3_right || Normal == Primitive::Vector3_left) {
			// 幅を法線にかけたものが面上の座標となる
			Pos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&object->GetComponent<Transform>()->GetPosition()),
				DirectX::XMVectorScale(planeN, object->GetComponent<AABBCollider>()->GetPrimitive().lenX() / 2));
		}
		// --- 平面の法線に対するOBBの射影線の長さを算出
		float r = 0.0f;		// 近接距離
		float Addr;
		DirectX::XMStoreFloat(&Addr, DirectX::XMVector3Dot(
			DirectX::XMVectorScale(DirectX::XMLoadFloat3(&this->GetOwner()->GetComponent<Transform>()->GetVectorForword()),
				this->GetOwner()->GetComponent<AABBCollider>()->GetPrimitive().lenZ() / 2)
			, planeN));
		r += fabs(Addr);
		DirectX::XMStoreFloat(&Addr, DirectX::XMVector3Dot(
			DirectX::XMVectorScale(DirectX::XMLoadFloat3(&this->GetOwner()->GetComponent<Transform>()->GetVectorUp()),
				this->GetOwner()->GetComponent<AABBCollider>()->GetPrimitive().lenY() / 2)
			, planeN));
		r += fabs(Addr);
		DirectX::XMStoreFloat(&Addr, DirectX::XMVector3Dot(
			DirectX::XMVectorScale(DirectX::XMLoadFloat3(&this->GetOwner()->GetComponent<Transform>()->GetVectorRight()),
				this->GetOwner()->GetComponent<AABBCollider>()->GetPrimitive().lenX() / 2)
			, planeN));
		r += fabs(Addr);

		// --- 戻し距離を算出
		float s;
		DirectX::XMStoreFloat(&s, DirectX::XMVector3Dot(
			DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&this->GetOwner()->GetComponent<Transform>()->GetPosition()), Pos), planeN));
		if (s > 0)	s = r - fabs(s);
		else		s = r + fabs(s);

		// --- めり込んだ位置から平面の法線方向に戻し距離だけオフセットする
		DirectX::XMFLOAT3 offsetPos;
		DirectX::XMStoreFloat3(&offsetPos, DirectX::XMVectorScale(planeN, s));
		this->GetOwner()->GetComponent<Transform>()->SetPosition({
			this->GetOwner()->GetComponent<Transform>()->GetPosition().x + offsetPos.x,
			this->GetOwner()->GetComponent<Transform>()->GetPosition().y + offsetPos.y,
			this->GetOwner()->GetComponent<Transform>()->GetPosition().z + offsetPos.z
			});
		GetOwner()->GetComponent<Rigidbody>()->SetAccele({ 0.0f, 0.0f, 0.0f });
	}

	// 矢と当たったときの処理
	if (object->GetTag() == TagName::Arrow)
	{
		// 自分を削除
		ObjectManager::RemoveObject(GetOwner()->GetThisPtr());
	}
}

void EnemyController::OnCollisionExit(ObjectBase* object)
{

}
