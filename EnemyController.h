#ifndef ___ENEMY_CONTROLLER_H___
#define ___ENEMY_CONTROLLER_H__

#include "ComponentBase.h"
#include "ObjectBase.h"

class EnemyController : public ComponentBase
{
private:
	// 移動スピード（位置フレームに進む距離）
	float m_MoveSpeed;
	// ターゲットまでのベクトル
	DirectX::XMFLOAT3 m_TargetVector;
	// ターゲットの場所をランダムで変える時間(ミリ秒)
	float m_ChangeTargetTime;
	float m_tic; // フレームカウント
	float m_TargetRotY;	// ターゲットまでのベクトルのY角度
	float m_lateRotY;	// 遅れてついてくるY角度
public:
	// コンストラクタ
	EnemyController()
		: m_MoveSpeed(0.05f)
		, m_ChangeTargetTime(50.0f)
		, m_tic(m_ChangeTargetTime)
		, m_TargetRotY(0.0f)
		, m_lateRotY(0.0f)
	{}
	// デストラクタ
	~EnemyController() {}

	// 更新
	void Update()override;

	// 当たり判定
	void OnCollisionEnter(ObjectBase* object);
	void OnCollisionStay(ObjectBase* object);
	void OnCollisionExit(ObjectBase* object);

	//--- セット関数
	void SetMoveSpeed(float speed) { m_MoveSpeed = speed; }
};

#endif //!___ENEMY_CONTROLLER_H___
