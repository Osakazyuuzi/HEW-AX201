#ifndef ___COMPONENT_H___
#define ___COMPONENT_H___

#include <memory>

class ObjectBase;
class ComponentBase
{
	friend class ObjectBase;
private:
	// 親オブジェクト
	ObjectBase* m_Owner;
public:
	// コンストラクタ
	ComponentBase();
	// デストラクタ
	virtual ~ComponentBase() {}

	virtual void Start(){}
	// 前更新
	virtual void FixedUpdate() {}
	// 更新
	virtual void Update() {}
	// 後更新
	virtual void LateUpdate() {}

	virtual void End(){}

	// 当たり判定
	virtual void OnCollisionEnter(ObjectBase* object){}
	virtual void OnCollisionStay(ObjectBase* object){}
	virtual void OnCollisionExit(ObjectBase* object){}

	// 格納オブジェクトを取得
	ObjectBase* GetOwner();

	// 格納オブジェクトをセット
	void SetOwner(ObjectBase* Owner);
};

#endif //!___COMPONENT_H___