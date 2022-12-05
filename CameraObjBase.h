#ifndef ___CAMERA_BASE_H___
#define ___CAMERA_BASE_H___

#include "ObjectBase.h"
#include <string>
//#include "Camera.h"
#include "CameraDebug.h"

class CameraObjBase : public ObjectBase
{
public:
	// コンストラクタ
	CameraObjBase(std::string name, std::string tag) : ObjectBase(name, tag)
	{

	}
	// デストラクタ
	virtual ~CameraObjBase() {}

	//--- カメラタグ管理用
	void SetTag(std::string tag) { m_tag = tag; }	// タグアクセサー
	void ChangeToMainCamera();	// メインカメラに変更
};

#endif //!___CAMERA_BASE_H___