#pragma once

#include "ObjectBase.h"
#include <string>
#include "Rigidbody.h"
#include "MeshRenderer.h"

class Object3D : public ObjectBase
{
public:
	// コンストラクタ
	Object3D(std::string name, std::string tag) : ObjectBase(name, tag)
	{
		// デフォルトコンポーネントを追加
		this->AddComponent<MeshRenderer>();	// 3Dオブジェクト用（表面的な情報）
	}
	// デストラクタ
	virtual ~Object3D(){}

	// 描画
	void Draw() override {
		this->GetComponent<MeshRenderer>()->Draw();
	}
};