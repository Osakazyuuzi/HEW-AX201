#include "WinUtil.h"
#include "Main.h"
#include "Timer.h"

//--- �萔��`
const char* APP_TITLE = "Osakazyuuzi";
const Frame DEFAULT_FPS = 60;

/*
* �G���g���|�C���g
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (FAILED(InitWindow(hInstance, nCmdShow, APP_TITLE, GetAppWidth(), GetAppHeight())))
	{
		Error("window initialize faield");
	}
	Timer::Init(DEFAULT_FPS);
	Init();


	//--- �E�B���h�E�̊Ǘ�
	AppState state = APP_STATE_MESSAGE;
	while (state != APP_STATE_QUIT)
	{
		state = UpdateWindow();
		if (state == APP_STATE_WAIT && Timer::Update())
		{
			Update(Timer::GetDeltaTime());
			Draw();
		}
	}

	// �I����
	Uninit();
	Timer::Uninit();
	UninitWindow();
	return 0;
}
