#include "SceneManager.h"
#include "ObjectManager.h"
#include "SceneTitle.h"
#include "SceneGame01.h"

bool SceneManager::m_bLoad = false;
SceneName::SceneName SceneManager::m_loadScene = SceneName::SceneTitle;

void SceneManager::LoadUpdate() {
	if (m_bLoad) {
		m_bLoad = false;
		// オブジェクトを全削除
		ObjectManager::DestroyAllObject();

		// ロードシーンで分岐
		switch (m_loadScene) {
		case SceneName::SceneTitle:
			SceneTitle();
			break;
		case SceneName::SceneGame01:
			SceneGame01();
			break;
		}
	}
}

void SceneManager::LoadScene(SceneName::SceneName scene) {
	m_loadScene = scene;
	m_bLoad = true;
}