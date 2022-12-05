#ifndef ___PLAYER_CONTROLLER_H___
#define ___PLAYER_CONTROLLER_H__

#include "ComponentBase.h"
#include "ObjectBase.h"

class PlayerController : public ComponentBase
{
private:
public:
	// コンストラクタ
	PlayerController() {}
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