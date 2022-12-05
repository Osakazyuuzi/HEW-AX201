#ifndef ___SCENEMANAGER_H___
#define ___SCENEMANAGER_H___

#include "SceneBase.h"
#include "SceneName.h"

class SceneManager
{
public:
	// コンストラクタ
	SceneManager(){}
	// デストラクタ
	~SceneManager(){}
	
	// シーン更新
	static void LoadUpdate();

	// シーンロード設定
	static void LoadScene(SceneName::SceneName scene);

private:
	static bool m_bLoad;
	static SceneName::SceneName m_loadScene;
};


#endif //!___SCENEMANAGER_H___