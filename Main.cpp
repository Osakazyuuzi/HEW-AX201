#include "Main.h"
#include "DirectX.h"
#include "WinUtil.h"
#include "DirectXTex/Texture.h"
#include "Input.h"
#include "Game3D.h"
#include "XInput.h"

//--- 定数定義
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

//-- グローバル変数
Game3D* g_pGame;

unsigned int GetAppWidth()
{
	return SCREEN_WIDTH;
}
unsigned int GetAppHeight()
{
	return SCREEN_HEIGHT;
}

void Init()
{
	if (FAILED(InitDX(GetHWND(), SCREEN_WIDTH, SCREEN_HEIGHT, false)))
	{
		Error("directx initialize failed.");
	}
	if (FAILED(InitTexture(GetDevice())))
	{
		Error("texture initialize failed.");
	}
	if (FAILED(InitInput()))
	{
		Error("input initialize failed.");
	}
	if (FAILED(XInput::InitJoystick()))
	{
		Error("Xinput initialize failed.");
	}
	g_pGame = new Game3D();
}
void Uninit()
{
	delete g_pGame;
	UninitInput();
	UninitTexture();
	UninitDX();
}
void Update(float deltaTime)
{
	if (FAILED(XInput::UpdateJoystick()))
	{
		Error("Xinput update failed.");
	}
	UpdateInput();
	g_pGame->Update();
	g_pGame->LateUpdate();
}
void Draw()
{
	BeginDrawDX3D();
	g_pGame->Draw();
	EndDrawDX();
}
