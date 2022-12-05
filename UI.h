#ifndef ___UI_H___
#define ___UI_H___

#include "ObjectBase.h"
#include "SpriteRenderer.h"
#include "DirectX.h"

class UI : public ObjectBase
{
public:
	// コンストラクタ
	UI(std::string name, std::string tag) : ObjectBase(name, tag) {
		// デフォルトコンポーネントを追加
		this->AddComponent<SpriteRenderer>();	// 2D用表面情報

		// 全ての設定が終わったら呼び出す
		this->Start();
	}
	// デストラクタ
	~UI(){}

	// 描画
	void Draw() override {
		this->GetComponent<SpriteRenderer>()->Draw();
	}
};

#endif //!___UI_H___