#ifndef ___ARROW_CONTROLLER_H___
#define ___ARROW_CONTROLLER_H__

#include "ComponentBase.h"
#include "ObjectBase.h"

class ArrowController : public ComponentBase
{
private:
public:
	// コンストラクタ
	ArrowController() {}
	// デストラクタ
	~ArrowController() {}

	// 更新
	void Update()override;


	// 当たり判定
	void OnCollisionEnter(ObjectBase* object);
	void OnCollisionStay(ObjectBase* object);
	void OnCollisionExit(ObjectBase* object);
};

#endif //!___ARROW_CONTROLLER_H___