#ifndef ___SCENEMANAGER_H___
#define ___SCENEMANAGER_H___

#include "SceneBase.h"
#include "SceneName.h"

class SceneManager
{
public:
	// �R���X�g���N�^
	SceneManager(){}
	// �f�X�g���N�^
	~SceneManager(){}
	
	// �V�[���X�V
	static void LoadUpdate();

	// �V�[�����[�h�ݒ�
	static void LoadScene(SceneName::SceneName scene);

private:
	static bool m_bLoad;
	static SceneName::SceneName m_loadScene;
};


#endif //!___SCENEMANAGER_H___