#include "PlayerController.h"
#include "Input.h"
#include "Transform.h"
#include "ObjectBase.h"
#include "Rigidbody.h"
#include "AABBCollider.h"
#include "XInput.h"
#include "Primitive.h"
#include "Vector3.h"
#include "Float3.h"
#include "CameraDebug.h"
#include "ObjectManager.h"
#include "Arrow.h"
#include <math.h>
#include "CameraPlayer.h"

void PlayerController::Update()
{
	// カメラがPlayerカメラではない時は更新しない
	if (!ObjectManager::FindObjectWithTag(TagName::MainCamera)->GetComponent<CameraPlayer>()) return;

	//--- 移動（カメラの向きに応じて移動方向を決める）
	// メインカメラの座標と注視点を取得する
	DirectX::XMFLOAT3 camPos = ObjectManager::FindObjectWithTag(TagName::MainCamera)->GetComponent<Transform>()->GetPosition();
	DirectX::XMFLOAT3 camLook = ObjectManager::FindObjectWithTag(TagName::MainCamera)->GetComponent<CameraPlayer>()->GetLookPoint();
	// Y軸でのプレイヤーの移動は要らないため、0.0fに設定
	camPos.y = 0.0f;
	camLook.y = 0.0f;
	// XMVECTORに変換
	DirectX::XMVECTOR vCamPos = DirectX::XMLoadFloat3(&camPos);
	DirectX::XMVECTOR vCamLook = DirectX::XMLoadFloat3(&camLook);
	// 座標から注視点へ向くベクトルを算出(正面)
	DirectX::XMVECTOR vFront;
	vFront = DirectX::XMVectorSubtract(vCamLook, vCamPos);
	vFront = DirectX::XMVector3Normalize(vFront);
	// 正面方向に対して、Y軸を90°回転させた横向きのベクトルを算出
	DirectX::XMMATRIX matRotSide = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(90.0f));
	DirectX::XMVECTOR vSide = DirectX::XMVector3TransformCoord(vFront, matRotSide);

	DirectX::XMVECTOR vMove = DirectX::XMVectorZero();
	// 斜め移動も可
	if (IsKeyPress('W')) vMove = DirectX::XMVectorAdd(vMove, vFront);
	if (IsKeyPress('S')) vMove = DirectX::XMVectorAdd(vMove, DirectX::XMVectorScale(vFront, -1.0f));
	if (IsKeyPress('A')) vMove = DirectX::XMVectorAdd(vMove, DirectX::XMVectorScale(vSide, -1.0f));
	if (IsKeyPress('D')) vMove = DirectX::XMVectorAdd(vMove, vSide);
	// 斜め移動のときに移動量が多くなってしまうため、正規化する
	vMove = DirectX::XMVector3Normalize(vMove);
	vMove = DirectX::XMVectorScale(vMove, 0.1f);

	DirectX::XMFLOAT3 move;
	DirectX::XMStoreFloat3(&move, vMove);
	GetOwner()->GetComponent<Transform>()->SetPosition({
		GetOwner()->GetComponent<Transform>()->GetPosition().x + move.x,
		GetOwner()->GetComponent<Transform>()->GetPosition().y + move.y,
		GetOwner()->GetComponent<Transform>()->GetPosition().z + move.z
		});
	// 移動した場合、移動した方向に回転する
	if (move.x != 0.0f || move.y != 0.0f || move.z != 0.0f) {
		float radY = 0.0f;
		// Z方向へのベクトル(モデルの正面方向のベクトル)
		DirectX::XMFLOAT3 zVector = { 0.0f, 0.0f, 1.0f };
		// 内積とベクトルの長さを使ってcosθを求める
		DirectX::XMStoreFloat(&radY, DirectX::XMVector3Dot(DirectX::XMVector3Normalize(vMove), DirectX::XMLoadFloat3(&zVector)));
		// 内積から角度を求める
		radY = ::acos(radY);
		// ラジアン角からおなじみの角度に変更
		radY = DirectX::XMConvertToDegrees(radY);
		// 回転が右回転か左回転かを判別するために、外積で求める
		// 求めた外積のY成分がプラスだったら左回り。
		// 求めた外積のY成分がマイナスだったら右回り。
		DirectX::XMFLOAT3 rotateDirection;
		DirectX::XMStoreFloat3(&rotateDirection, DirectX::XMVector3Cross(DirectX::XMVector3Normalize(vMove), DirectX::XMLoadFloat3(&zVector)));
		if (rotateDirection.y > 0) radY = 180.0f + (180.0f - radY);
		// 算出した角度を適用する
		GetOwner()->GetComponent<Transform>()->SetAngle({ 0.0f, radY, 0.0f });
	}

	//--- 発射
	if (IsKeyTrigger(VK_SPACE)) {
		m_tic = 0.0f;	// 押し始めたら、0.0fに初期化

		// 変更用ポインタ
		std::shared_ptr<Transform> trans;
		std::shared_ptr<Rigidbody> rb;

		//--- オブジェクト作成
		//   型　：Arrow
		//  名前 ：Arrow
		// タグ名：Arrow
		m_haveArrow = ObjectManager::CreateObject<Arrow>("Arrow", TagName::Arrow);
		// 今持っているArrowのTransformを取得
		trans = m_haveArrow->GetComponent<Transform>();
		// 座標を自分のオブジェクト＋自分オブジェクトの法線（長さ１）横の位置に設定
		// 要約：右前
		trans->SetPosition({
			GetOwner()->GetComponent<Transform>()->GetPosition().x + 
				GetOwner()->GetComponent<Transform>()->GetVectorRight().x +
				GetOwner()->GetComponent<Transform>()->GetVectorForword().x,
			GetOwner()->GetComponent<Transform>()->GetPosition().y + 
				GetOwner()->GetComponent<Transform>()->GetVectorRight().y +
				GetOwner()->GetComponent<Transform>()->GetVectorForword().y,
			GetOwner()->GetComponent<Transform>()->GetPosition().z + 
				GetOwner()->GetComponent<Transform>()->GetVectorRight().z +
				GetOwner()->GetComponent<Transform>()->GetVectorForword().z
			});
		// 角度を自分のオブジェクトの角度に設定
		trans->SetAngle({
			GetOwner()->GetComponent<Transform>()->GetAngle().x,
			GetOwner()->GetComponent<Transform>()->GetAngle().y + 0.0f,// 矢のモデルと矢を射出するモデルの正面が違う場合、ここで数値調整する。
			GetOwner()->GetComponent<Transform>()->GetAngle().z
			});

	}
	if (IsKeyPress(VK_SPACE)) {
		m_tic++;	// 押している間、カウントする
		// 変更用ポインタ
		std::shared_ptr<Transform> trans;
		std::shared_ptr<Rigidbody> rb;
		// 今持っているArrowのTransformを取得
		trans = m_haveArrow->GetComponent<Transform>();
		// 今持っているArrowのRigidbodyを取得
		rb = m_haveArrow->GetComponent<Rigidbody>();
		// チャージ時間の割合を求める
		float ChargePer = m_tic > m_ChargeTime ? m_ChargeTime/m_ChargeTime : m_tic/m_ChargeTime;
		// 割合を逆にする
		ChargePer = 1 - ChargePer;
		// 座標を自分のオブジェクト＋自分オブジェクトの法線（長さ１）横の位置に設定
		trans->SetPosition({
			GetOwner()->GetComponent<Transform>()->GetPosition().x +
				GetOwner()->GetComponent<Transform>()->GetVectorRight().x +
				GetOwner()->GetComponent<Transform>()->GetVectorForword().x * ChargePer,
			GetOwner()->GetComponent<Transform>()->GetPosition().y +
				GetOwner()->GetComponent<Transform>()->GetVectorRight().y +
				GetOwner()->GetComponent<Transform>()->GetVectorForword().y * ChargePer,
			GetOwner()->GetComponent<Transform>()->GetPosition().z +
				GetOwner()->GetComponent<Transform>()->GetVectorRight().z +
				GetOwner()->GetComponent<Transform>()->GetVectorForword().z * ChargePer
			});
		// 角度を自分のオブジェクトの角度に設定
		trans->SetAngle({
			GetOwner()->GetComponent<Transform>()->GetAngle().x,
			GetOwner()->GetComponent<Transform>()->GetAngle().y + 0.0f,// 矢のモデルと矢を射出するモデルの正面が違う場合、ここで数値調整する。
			GetOwner()->GetComponent<Transform>()->GetAngle().z
			});
		// チャージタイム以上に長押ししていた場合
		if (m_tic > m_ChargeTime) {
			// サイズを設定
			trans->SetScale({ 0.6f, 0.6f, 0.6f });
		}
		// 通常の場合
		else {
			// サイズを設定
			trans->SetScale({ 0.3f, 0.3f, 0.3f });
		}

		// 溜め中なので加速度を0.0fに設定
		rb->SetAccele({ 0.0f, 0.0f, 0.0f});
	}
	if (IsKeyRelease(VK_SPACE)) {
		// 変更用ポインタ
		std::shared_ptr<Rigidbody> rb;
		// 今持っているArrowのRigidbodyを取得
		rb = m_haveArrow->GetComponent<Rigidbody>();
		// チャージタイム以上に長押ししていた場合
		if (m_tic > m_ChargeTime) {
			// 加速度をオブジェクトの正面方向に設定
			rb->SetAccele({
				GetOwner()->GetComponent<Transform>()->GetVectorForword().x * 0.6f,
				GetOwner()->GetComponent<Transform>()->GetVectorForword().y * 0.6f,
				GetOwner()->GetComponent<Transform>()->GetVectorForword().z * 0.6f
				});
		}
		// 通常の場合
		else {
			// 加速度をオブジェクトの正面方向に設定
			rb->SetAccele({
				GetOwner()->GetComponent<Transform>()->GetVectorForword().x * 0.3f,
				GetOwner()->GetComponent<Transform>()->GetVectorForword().y * 0.3f,
				GetOwner()->GetComponent<Transform>()->GetVectorForword().z * 0.3f
				});
		}
	}

	//--- 座標補正
	// 落下判定
	if (GetOwner()->GetComponent<Rigidbody>()) {
		// y座標が-9以下
		if (GetOwner()->GetComponent<Transform>()->GetPosition().y < -9.0f) {
			// 座標を補正
			GetOwner()->GetComponent<Transform>()->SetPosition({
				GetOwner()->GetComponent<Transform>()->GetPosition().x,
				0.0f,
				GetOwner()->GetComponent<Transform>()->GetPosition().z
				});
			// 加速度を補正
			GetOwner()->GetComponent<Rigidbody>()->SetAccele({ 0.0f, 0.0f, 0.0f });
		}
	}
}

void PlayerController::OnCollisionEnter(ObjectBase* object)
{
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
	}
}

void PlayerController::OnCollisionStay(ObjectBase* object)
{
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
}

void PlayerController::OnCollisionExit(ObjectBase* object)
{

}
