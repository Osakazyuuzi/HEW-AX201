#include "CameraPlayer.h"
#include "Transform.h"
#include "ObjectManager.h"

void CameraPlayer::Update()
{
	// Playerタグのついたオブジェクトの座標を取得し、注視点にする。
	m_LookPoint = ObjectManager::FindObjectWithTag(TagName::Player)->GetComponent<Transform>()->GetPosition();

	// 角度と距離、注視点からカメラの位置を計算
	GetOwner()->GetComponent<Transform>()->SetPosition({
	cosf(m_radY) * sinf(m_radXZ) * m_radius + m_LookPoint.x,
	sinf(m_radY) * m_radius + m_LookPoint.y,
	cosf(m_radY) * cosf(m_radXZ) * m_radius + m_LookPoint.z
		});
}