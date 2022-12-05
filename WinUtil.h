#ifndef ___WINUTIL_H___
#define ___WINUTIL_H___

#include <windows.h>
#include <DirectXMath.h>

//--- 定義
enum AppState
{
	APP_STATE_MESSAGE,
	APP_STATE_WAIT,
	APP_STATE_QUIT
};

//--- プロトタイプ宣言
HRESULT InitWindow(HINSTANCE hInstance, int CmdShow, LPCSTR appName, LONG width, LONG height);
void UninitWindow();
AppState UpdateWindow();
HWND GetHWND();
void Error(const char* mes);

DirectX::XMFLOAT2 GetCursorPos();
float GetMouseWheel();
void InitMouseWheel();

#endif //!___WINUTIL_H___