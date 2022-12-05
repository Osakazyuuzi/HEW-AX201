#include "WinUtil.h"

//--- �v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//--- �O���[�o���ϐ�
HWND g_hWnd;
DirectX::XMFLOAT2 g_CursorPos;
float g_MouseWheelfwKeys;

HRESULT InitWindow(HINSTANCE hInstance, int CmdShow, LPCSTR appName, LONG width, LONG height)
{

	//--- �ϐ��錾
	WNDCLASSEX wcex;

	//--- ������
	g_CursorPos.x = 0.0f;
	g_CursorPos.y = 0.0f;
	g_MouseWheelfwKeys = 0;

	// �E�B���h�N���X���̐ݒ�
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.hInstance = hInstance;
	wcex.lpszClassName = "Class Name";
	wcex.lpfnWndProc = WndProc;
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	// �E�B���h�E�N���X���̓o�^
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Failed to RegisterClassEx", "Error", MB_OK);
		return 0;
	}

	// �E�B���h�E�̍쐬
	RECT rect = { 0, 0, width, height };
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	DWORD exStyle = WS_EX_OVERLAPPEDWINDOW;
	AdjustWindowRectEx(&rect, style, false, exStyle);
	g_hWnd = CreateWindowEx(
		exStyle, wcex.lpszClassName,
		appName, style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		HWND_DESKTOP,
		NULL, hInstance, NULL
	);

	// �E�B���h�E�̕\��
	ShowWindow(g_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(g_hWnd);

	return S_OK;
}

void UninitWindow()
{
}

AppState UpdateWindow()
{
	MSG message;
	// �������b�Z�[�W�Ȃ�
	if (!PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE))
	{
		return APP_STATE_WAIT;
	}

	// ���b�Z�[�W�̏I��
	if (!GetMessage(&message, NULL, 0, 0))
	{
		return APP_STATE_QUIT;
	}

	// ���b�Z�[�W�̏���
	TranslateMessage(&message);
	DispatchMessage(&message);
	return APP_STATE_MESSAGE;
}

HWND GetHWND()
{
	return g_hWnd;
}

void Error(const char* mes)
{
	MessageBox(g_hWnd, mes, "Error", MB_OK | MB_ICONERROR);
	DestroyWindow(g_hWnd);
}

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// �}�E�X�z�C�[��
	case 0x020a:
	{
		g_MouseWheelfwKeys = HIWORD(wParam);
	}
	break;
	// �J�[�\���̈ړ�
	case WM_MOUSEMOVE:
	{
		WORD _x = LOWORD(lParam);
		WORD _y = HIWORD(lParam);
		// �����₷���悤��float�ɕϊ�
		g_CursorPos.x = _x;
		g_CursorPos.y = _y;
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

DirectX::XMFLOAT2 GetCursorPos()
{
	return g_CursorPos;
}

float GetMouseWheel()
{
	float wheelmove = g_MouseWheelfwKeys / 12000;
	if (g_MouseWheelfwKeys > 1000)
		return -0.1f;
	if (g_MouseWheelfwKeys > 0)
		return 0.1f;
	return 0.0f;
}

void InitMouseWheel()
{
	g_MouseWheelfwKeys = 0.0f;
}