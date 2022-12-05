#include "CameraDebug.h"
#include "WinUtil.h"
#include "Input.h"
#include "Transform.h"
#include "ObjectBase.h"

void CameraDebug::Update()
{
	// Altキー
	if (IsKeyPress(VK_MENU))
	{
		// マウスの左ボタン
		if (IsKeyPress(VK_LBUTTON))
		{
			//=========================
			// Altキー＋マウス左ボタンで
			//  注視点を中心に回転する
			//=========================
			// マウスを右に
			if (m_CursorPos.x < GetCursorPos().x)
				m_radXZ -= (m_CursorPos.x - GetCursorPos().x) / 180 * 3.141592f * m_RotateSensitivity;
			// マウスを左に
			if (m_CursorPos.x > GetCursorPos().x)
				m_radXZ += (GetCursorPos().x - m_CursorPos.x) / 180 * 3.141592f * m_RotateSensitivity;
			// マウスを上に
			if (m_CursorPos.y < GetCursorPos().y)
				m_radY -= (m_CursorPos.y - GetCursorPos().y) / 180 * 3.141592f * m_RotateSensitivity;
			// マウスを下に
			if (m_CursorPos.y > GetCursorPos().y)
				m_radY += (GetCursorPos().y - m_CursorPos.y) / 180 * 3.141592f * m_RotateSensitivity;

			//=========================
			// Yの回り込みをした際に、
			//  アップベクトル変換の必要性をチェック
			//=========================
			//??????????????????????
		}
		// マウスの中ボタン
		else if (IsKeyPress(VK_MBUTTON))
		{
			//=========================
			// Altキー＋マウス中ボタンで
			//  注視点とカメラを動かす
			//=========================

			// マウスを右に
			if (m_CursorPos.x < GetCursorPos().x) {
				m_LookPoint.x -= cosf(m_radXZ) * (m_CursorPos.x - GetCursorPos().x) * 0.01f;
				m_LookPoint.z -= -sinf(m_radXZ) * (m_CursorPos.x - GetCursorPos().x) * 0.01f;
			}
			// マウスを左に
			if (m_CursorPos.x > GetCursorPos().x) {
				m_LookPoint.x += cosf(m_radXZ) * (GetCursorPos().x - m_CursorPos.x) * 0.01f;
				m_LookPoint.z += -sinf(m_radXZ) * (GetCursorPos().x - m_CursorPos.x) * 0.01f;
			}
			// マウスを上に
			if (m_CursorPos.y < GetCursorPos().y) {
				m_LookPoint.x -= sinf(m_radY) * -sinf(m_radXZ) * (m_CursorPos.y - GetCursorPos().y) * 0.01f;
				m_LookPoint.y += -cosf(m_radY) * (m_CursorPos.y - GetCursorPos().y) * 0.01f;
				m_LookPoint.z -= sinf(m_radY) * -cosf(m_radXZ) * (m_CursorPos.y - GetCursorPos().y) * 0.01f;
			}
			// マウスを下に
			if (m_CursorPos.y > GetCursorPos().y) {
				m_LookPoint.x -= -sinf(m_radY) * sinf(m_radXZ) * (m_CursorPos.y - GetCursorPos().y) * 0.01f;
				m_LookPoint.y += -cosf(m_radY) * (m_CursorPos.y - GetCursorPos().y) * 0.01f;
				m_LookPoint.z -= -sinf(m_radY) * cosf(m_radXZ) * (m_CursorPos.y - GetCursorPos().y) * 0.01f;
			}
		}
	}

	//=========================
	// マウスホイールで
	//  カメラを正面に進ませる
	//=========================
	// ホイールの回した分だけ注視点とカメラ座標の距離を操作する
	m_radius -= (GetMouseWheel() * m_ZoomSensitivity);
	// マウスホイールを初期化する
	InitMouseWheel();

	// 角度と距離、注視店からカメラの位置を計算
	GetOwner()->GetComponent<Transform>()->SetPosition({
	cosf(m_radY) * sinf(m_radXZ) * m_radius + m_LookPoint.x,
	sinf(m_radY) * m_radius + m_LookPoint.y,
	cosf(m_radY) * cosf(m_radXZ) * m_radius + m_LookPoint.z
	});
	// 現在のカーソル座標で更新
	m_CursorPos = GetCursorPos();
}