#ifndef ___RIGIDBODY_H___
#define ___RIGIDBODY_H___

#include "ComponentBase.h"
#include <DirectXMath.h>

// X,Z の加速度は未実装

class Rigidbody : public ComponentBase
{
public:
	// コンストラクタ
	Rigidbody()
		: m_Mass(0.03f)
		, m_Accele({0.0f, 0.0f, 0.0f})
	{}
	// デストラクタ
	virtual ~Rigidbody(){}

	// 更新
	void FixedUpdate() override;
	void LateUpdate() override;

	// 加速度を加える
	void AddForce(DirectX::XMFLOAT3 force) {
		m_Accele = {
			m_Accele.x + force.x,
			m_Accele.y + force.y,
			m_Accele.z + force.z
		};
	}

	//--- ゲット関数
	float GetMass(void) { return m_Mass; }
	DirectX::XMFLOAT3 GetAccele(void) { return m_Accele; }

	//--- セット関数
	void SetMass(float mass) { m_Mass = mass; }
	void SetAccele(DirectX::XMFLOAT3 accele) { m_Accele = accele; }

	// 重力
	constexpr static float mc_gravity = 0.1f;

private:
	// 質量（重量）
	float m_Mass;
	// 加速度
	DirectX::XMFLOAT3 m_Accele;
};

#endif //!___RIGIDBODY_H___