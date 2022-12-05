#ifndef ___CAMERA_H___
#define ___CAMERA_H___

// 注意：このヘッダはComponentです。カメラそのものではありません。

#include <DirectXMath.h>
#include"ComponentBase.h"

class Camera : public ComponentBase
{
public:
	// コンストラクタ
	Camera()
		: m_LookPoint{0.0f, 0.0f, 0.0f}
		, m_upVector{0.0f, 1.0f, 0.0f}
		, m_fovy(60.0f), m_aspect(16.0f / 9.0f), m_near(0.3f), m_far(1000.0f)
	{}
	// デストラクタ
	virtual ~Camera(){}

	// 更新関数
	virtual void Update(){}

	//--- ゲット関数
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();
	DirectX::XMFLOAT3 GetLookPoint() { return m_LookPoint; }

protected:
	//--- ビュー行列に必要な情報
	// 注視点
	DirectX::XMFLOAT3 m_LookPoint;
	// アップベクトル
	DirectX::XMFLOAT3 m_upVector;

	//--- プロジェクション行列に必要な情報
	float m_fovy, m_aspect, m_near, m_far;
};

#endif //!___CAMERA_H___