#ifndef ___PLAYER_CONTROLLER_H___
#define ___PLAYER_CONTROLLER_H__

#include "ComponentBase.h"
#include "ObjectBase.h"

class PlayerController : public ComponentBase
{
private:
	//--- 射撃用
	// 長押しによる溜め攻撃を使用可能になる時間
	float m_ChargeTime;
	float m_tic; // フレームカウント
	std::shared_ptr<ObjectBase> m_haveArrow; // 現在プレイヤーが持っている矢のポインタ
public:
	// コンストラクタ
	PlayerController()
		: m_ChargeTime(30.0f) // 60 fps の場合 0.5 秒
	{}
	// デストラクタ
	~PlayerController() {}

	// 更新
	void Update()override;


	// 当たり判定
	void OnCollisionEnter(ObjectBase* object);
	void OnCollisionStay(ObjectBase* object);
	void OnCollisionExit(ObjectBase* object);
};

#endif //!___PLAYER_CONTROLLER_H___
