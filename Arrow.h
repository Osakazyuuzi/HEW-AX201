#ifndef ___ARROW_H___
#define ___ARROW_H___
#include "Object3D.h"
#include "MeshRenderer.h"
#include "Rigidbody.h"
#include "AABBCollider.h"
#include "ArrowController.h"

class Arrow : public Object3D
{
public:
	// コンストラクタ
	Arrow(std::string name, std::string tag) : Object3D(name, tag) {
		// 矢のモデルをロードする
		GetComponent<MeshRenderer>()->LoadModel("Assets/Model/DemoCube/DemoCube.fbx", 1.0f);
		// コンポーネントを追加
		AddComponent<Rigidbody>();
		AddComponent<AABBCollider>();
		AddComponent<ArrowController>();
		// 当たり判定の大きさを調整（モデル読み込み時点）
		GetComponent<AABBCollider>()->SetLen({ 0.6f, 0.6f, 3.0f });
	}
	// デストラクタ
	~Arrow() {}
};

#endif //!___ARROW_H___
