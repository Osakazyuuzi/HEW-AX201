#include "SceneGame01.h"
#include "MeshRenderer.h"
#include "SpriteRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "ObjectManager.h"
#include "Ui.h"

SceneGame01::SceneGame01()
{
	//--- �ύX�p�|�C���^
	std::shared_ptr<MeshRenderer> pMeshRenderer;	// 3D�I�u�W�F�N�g�p�����_���[
	std::shared_ptr<SpriteRenderer> pSpriteRenderer;// 2D�I�u�W�F�N�g�p�����_���[
	std::shared_ptr<Transform> pTransform;			// �ʒu���
	std::shared_ptr<Rigidbody> pRigidbody;			// ��������

	//--- UI�쐬�i��j
	// UI.1 ���F
	ObjectManager::CreateObject<UI>("UI.1");
	pSpriteRenderer = ObjectManager::FindObjectWithName("UI.1")->GetComponent<SpriteRenderer>();
	pTransform = ObjectManager::FindObjectWithName("UI.1")->GetComponent<Transform>();
	pSpriteRenderer->LoadTexture("Assets/Texture/TEST.jpg");
	pSpriteRenderer->SetSize(50, 50);
	ObjectManager::FindObjectWithName("UI.1")->SetLayerNum(2);
	pTransform->SetPosition({ 0.0f, 0.0f, 0.0f });
	// UI.2 ���Ɛ�
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