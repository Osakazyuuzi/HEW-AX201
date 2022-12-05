#ifndef ___CAMERAOBJ_PLAYER_H___
#define ___CAMERAOBJ_PLAYER_H___
#include "CameraObjBase.h"
#include <string>
#include "CameraPlayer.h"

class CameraObjPlayer : public CameraObjBase
{
public:
	// コンストラクタ
	CameraObjPlayer(std::string name, std::string tag) : CameraObjBase(name, tag)
	{
		// コンポーネント追加
		AddComponent<CameraPlayer>();
	}
	// デストラクタ
	virtual ~CameraObjPlayer(){}
};


#endif //!___CAMERAOBJ_PLAYER_H___