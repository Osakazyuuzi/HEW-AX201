#ifndef ___CAMERA_PLAYER_H___
#define ___CAMERA_PLAYER_H___

#include "Camera.h"

class CameraPlayer : public Camera
{
public:
	// コンストラクタ
	CameraPlayer()
		: m_radius(10.0f)
	{}
	// デストラクタ
	~CameraPlayer(){}

	void Update() override;

	//--- セット関数
	void SetRadXZ(float rad) { m_radXZ = rad; }
	void SetRadY(float rad) { m_radY = rad; }

private:
	float m_radXZ, m_radY;		// 回り込み角度
	float m_radius;				// カメラと注視点の距離
};

#endif //!___CAMERA_PLAYER_H___