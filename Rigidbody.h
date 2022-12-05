#ifndef ___RIGIDBODY_H___
#define ___RIGIDBODY_H___

#include "ComponentBase.h"
#include <DirectXMath.h>

// X,Z �̉����x�͖�����

class Rigidbody : public ComponentBase
{
public:
	// �R���X�g���N�^
	Rigidbody()
		: m_Mass(0.03f)
		, m_Accele({0.0f, 0.0f, 0.0f})
	{}
	// �f�X�g���N�^
	virtual ~Rigidbody(){}

	// �X�V
	void FixedUpdate() override;
	void LateUpdate() override;

	// �����x��������
	void AddForce(DirectX::XMFLOAT3 force) {
		m_Accele = {
			m_Accele.x + force.x,
			m_Accele.y + force.y,
			m_Accele.z + force.z
		};
	}

	//--- �Q�b�g�֐�
	float GetMass(void) { return m_Mass; }
	DirectX::XMFLOAT3 GetAccele(void) { return m_Accele; }

	//--- �Z�b�g�֐�
	void SetMass(float mass) { m_Mass = mass; }
	void SetAccele(DirectX::XMFLOAT3 accele) { m_Accele = accele; }

	// �d��
	constexpr static float mc_gravity = 0.1f;

private:
	// ���ʁi�d�ʁj
	float m_Mass;
	// �����x
	DirectX::XMFLOAT3 m_Accele;
};

#endif //!___RIGIDBODY_H___