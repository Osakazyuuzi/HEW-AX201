#include "CameraObjBase.h"
#include "ObjectManager.h"
#include <memory>

// メインカメラに変更
void CameraObjBase::ChangeToMainCamera()
{
	// MainCameraのタグがついたカメラオブジェクトを探し、
	// タグ名をMainCameraからUntaggedに変更する
	std::shared_ptr<CameraObjBase> pCameraObj;
	pCameraObj = std::reinterpret_pointer_cast<CameraObjBase>(ObjectManager::FindObjectWithTag(TagName::MainCamera));
	pCameraObj->SetTag(TagName::Untagged);

	// 自分のタグをMainCameraにする
	this->m_tag = TagName::MainCamera;
}