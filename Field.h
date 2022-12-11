#ifndef ___FIELD_H___
#define ___FIELD_H___
#include "Object3D.h"
#include "AABBCollider.h"

class Field : public Object3D
{
public:
	// コンストラクタ
	Field(std::string name, std::string tag) : Object3D(name, tag) {
		// プレイヤーのモデルをロードする（仮でキューブのオブジェクト）
		GetComponent<MeshRenderer>()->LoadModel("Assets/Model/DemoCube/DemoCube.fbx", 1.0f);
		// コンポーネントを追加
		AddComponent<AABBCollider>();
		// オブジェクトの辺の長さをセット
		GetComponent<Transform>()->SetScale({ 20.0f, 1.0f, 20.0f });
		// 当たり判定の辺の長さをセット
		GetComponent<AABBCollider>()->SetLen({ 20.0f,1.0f,20.0f });
	}
	// デストラクタ
	~Field() {}
};

#endif //!___FIELD_H___
