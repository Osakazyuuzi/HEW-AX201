#ifndef ___ENEMY_H___
#define ___ENEMY_H___
#include "Object3D.h"
#include "MeshRenderer.h"
#include "Rigidbody.h"
#include "AABBCollider.h"
#include "EnemyController.h"

class Enemy : public Object3D
{
public:
	// コンストラクタ
	Enemy(std::string name, std::string tag) : Object3D(name, tag) {
		// プレイヤーのモデルをロードする（仮でキューブのオブジェクト）
		GetComponent<MeshRenderer>()->LoadModel("Assets/Model/Object/Cube.fbx", 1.0f);
		// コンポーネントを追加
		AddComponent<EnemyController>();
		AddComponent<Rigidbody>();
		AddComponent<AABBCollider>();
		// 当たり判定の辺の長さをセット
		GetComponent<AABBCollider>()->SetLen({ 1.0f,1.0f,1.0f });
	}
	// デストラクタ
	~Enemy() {}
};

#endif //!___ENEMY_H___