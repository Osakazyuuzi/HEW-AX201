#ifndef ___ENEMY_CONTROLLER_H___
#define ___ENEMY_CONTROLLER_H__

#include "ComponentBase.h"
#include "ObjectBase.h"

class EnemyController : public ComponentBase
{
private:
	// 移動スピード（位置フレームに進む距離）
	float m_MoveSpeed;
public:
	// コンストラクタ
	EnemyController()
		: m_MoveSpeed(0.05f)
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