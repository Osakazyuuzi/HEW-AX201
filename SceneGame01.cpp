#include "SceneGame01.h"
#include "MeshRenderer.h"
#include "SpriteRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "ObjectManager.h"
#include "Ui.h"

SceneGame01::SceneGame01()
{
	//--- 変更用ポインタ
	std::shared_ptr<MeshRenderer> pMeshRenderer;	// 3Dオブジェクト用レンダラー
	std::shared_ptr<SpriteRenderer> pSpriteRenderer;// 2Dオブジェクト用レンダラー
	std::shared_ptr<Transform> pTransform;			// 位置情報
	std::shared_ptr<Rigidbody> pRigidbody;			// 物理挙動

	//--- UI作成（例）
	// UI.1 虹色
	ObjectManager::CreateObject<UI>("UI.1");
	pSpriteRenderer = ObjectManager::FindObjectWithName("UI.1")->GetComponent<SpriteRenderer>();
	pTransform = ObjectManager::FindObjectWithName("UI.1")->GetComponent<Transform>();
	pSpriteRenderer->LoadTexture("Assets/Texture/TEST.jpg");
	pSpriteRenderer->SetSize(50, 50);
	ObjectManager::FindObjectWithName("UI.1")->SetLayerNum(2);
	pTransform->SetPosition({ 0.0f, 0.0f, 0.0f });
	// UI.2 白と赤
	ObjectManager::CreateObject<UI>("UI.2");
	pSpriteRenderer = ObjectManager::FindObjectWithName("UI.2")->GetComponent<SpriteRenderer>();
	pTransform = ObjectManager::FindObjectWithName("UI.2")->GetComponent<Transform>();
	pSpriteRenderer->LoadTexture("Assets/Texture/TEST.png");
	pSpriteRenderer->SetSize(50, 50);
	ObjectManager::FindObjectWithName("UI.2")->SetLayerNum(1);
	pTransform->SetPosition({ 25.0f, 25.0f, 0.0f });
}

SceneGame01::~SceneGame01()
{

}