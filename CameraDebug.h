#ifndef ___CAMERADEBUG_H___
#define ___CAMERADEBUG_H___

#include "Camera.h"

class CameraDebug : public Camera
{
public:
	// コンストラクタ
	CameraDebug()
		: m_CursorPos({ 0.0f, 0.0f })
		, m_ZoomSensitivity(6.0f)
		, m_RotateSensitivity(0.3f)
		, m_radXZ(0.0f), m_radY(0.0f), m_radius(10.0f)
	{}
	// デストラクタ
	~CameraDebug(){}

	void Update() override;

private:
	// 画面操作用カーソル座標
	DirectX::XMFLOAT2 m_CursorPos;
	float m_ZoomSensitivity;	// ズーム感度
	float m_RotateSensitivity;	// 回転感度
	float m_radXZ, m_radY;		// 回り込み角度
	float m_radius;				// カメラと注視点の距離
};

#endif //!___CAMERADEBUG_H___