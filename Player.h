#ifndef ___PLAYER_H___
#define ___PLAYER_H___
#include "Object3D.h"
#include "MeshRenderer.h"
#include "Rigidbody.h"
#include "PlayerController.h"
#include "AABBCollider.h"

class Player : public Object3D
{
public:
	// コンストラクタ
	Player(std::string name, std::string tag) : Object3D(name, tag) {
		// プレイヤーのモデルをロードする（仮でキューブのオブジェクト）
		GetComponent<MeshRenderer>()->LoadModel("Assets/Model/Object/Cube.fbx", 1.0f);
		// コンポーネントを追加
		AddComponent<PlayerController>();
		AddComponent<Rigidbody>();
		AddComponent<AABBCollider>();
		// 当たり判定の辺の長さをセット
		GetComponent<AABBCollider>()->SetLen({ 1.0f,1.0f,1.0f });
	}
	// デストラクタ
	~Player(){}
};

#endif //!___PLAYER_H___