// 加藤先生からいただいたヒントから作られたプログラムです。
// 加藤先生に感謝とお礼の気持ちを忘れないでください。
// 本当にありがたいことなので、、、

// XBOXコントローラーのプログラム(XInput)で
// 困っている人がいたら教えても大丈夫です。

// ※配布はしないでください。

#ifndef ___XINPUT_H___
#define ___XINPUT_H___

#include <Windows.h>
#include <math.h>

#define JOYPOVUP			JOY_POVFORWARD
#define JOYPOVRIGHTUP		4500
#define JOYPOVRIGHT			JOY_POVRIGHT
#define JOYPOVRIGHTDOWN		13500
#define JOYPOVDOWN			JOY_POVBACKWARD
#define JOYPOVLEFTDOWN		22500
#define JOYPOVLEFT			JOY_POVLEFT
#define JOYPOVLEFTUP		31500

enum BUTTON_TYPE
{
	// ボタンタイプを設定すべき
	A		= 0x0000,
	B		= 0x0001,
	X		= 0x0002,
	Y		= 0x0003,
	L		= 0x0004,
	R		= 0x0005,
	VIEW	= 0x0006,
	MENU	= 0x0007,
	LStick	= 0x0008,
	RStick	= 0x0009,
};

enum CURSOR_BUTTON_TYPE
{
	// 方向ボタン「上」
	POV_UP = JOYPOVUP,
	// 方向ボタン「右」
	POV_RIGHT = JOYPOVRIGHT,
	// 方向ボタン「下」
	POV_DOWN = JOYPOVDOWN,
	// 方向ボタン「左」
	POV_LEFT = JOYPOVLEFT};

class XInput
{
public:
	// ゲームパッドの初期化処理
	static HRESULT InitJoystick()
	{
		GetJoyCount();
		return S_OK;
	}

	// ゲームパッド接続数取得
	static DWORD GetJoyCount()
	{
		JOYINFO ji;
		DWORD dwCount = joyGetNumDevs();

		if (dwCount > mc_NUM_JOY_MAX)
			dwCount = mc_NUM_JOY_MAX;
		for (m_dwJoyCount = 0; m_dwJoyCount < dwCount; ++m_dwJoyCount) {
			if (joyGetPos(m_dwJoyCount, &ji) != JOYERR_NOERROR)
				break;
		}
		return m_dwJoyCount;
	}

	// ゲームパッドの更新処理
	static HRESULT UpdateJoystick()
	{
		HRESULT hr = S_OK;
		DWORD i;

		for (i = 0; i < m_dwJoyCount; ++i) {
			JOYINFOEX& ji = m_joyState[i];
			DWORD dwButtons = ji.dwButtons;
			DWORD dwPOV = ji.dwPOV;

			ZeroMemory(&ji, sizeof(ji));
			ji.dwSize = sizeof(ji);
			ji.dwFlags = JOY_RETURNALL;
			if (joyGetPosEx(i, &ji) != JOYERR_NOERROR) {
				hr = E_FAIL;
				break;
			}
			m_dwJoyButtonTrigger[i] = (m_joyState[i].dwButtons ^ dwButtons) & ~dwButtons;
			m_dwJoyButtonRelease[i] = (m_joyState[i].dwButtons ^ dwButtons) & dwButtons;

			m_dwJoyPOVTrigger[i] = (m_joyState[i].dwPOV ^ dwPOV) & ~dwPOV;
			m_dwJoyPOVRelease[i] = (m_joyState[i].dwPOV ^ dwPOV) & dwPOV;
		}
		for (; i < mc_NUM_JOY_MAX; ++i) {
			JOYINFOEX& ji = m_joyState[i];

			ZeroMemory(&ji, sizeof(ji));
			m_dwJoyButtonTrigger[i] = 0;
			m_dwJoyButtonRelease[i] = 0;
			m_dwJoyPOVTrigger[i] = 0;
			m_dwJoyPOVRelease[i] = 0;
		}
		return hr;
	}

	// ゲームパッド情報取得
	static JOYINFOEX* GetJoyState(DWORD dwJoy)
	{
		if (dwJoy >= mc_NUM_JOY_MAX) return nullptr;
		return &m_joyState[dwJoy];
	}

	// ゲームパッドX軸取得
	static LONG GetJoyX(DWORD dwJoy)
	{
		if (dwJoy >= mc_NUM_JOY_MAX) return 0L;
		return (LONG)m_joyState[dwJoy].dwXpos - 0x08000;
	}

	// ゲームパッドY軸取得
	static LONG GetJoyY(DWORD dwJoy)
	{
		if (dwJoy >= mc_NUM_JOY_MAX) return 0L;
		return (LONG)m_joyState[dwJoy].dwYpos - 0x08000;
	}

	// ゲームパッドZ軸取得
	static LONG GetJoyZ(DWORD dwJoy)
	{
		if (dwJoy >= mc_NUM_JOY_MAX) return 0L;
		return (LONG)m_joyState[dwJoy].dwZpos - 0x08000;
	}

	// ゲームパッド　方向ボタン情報取得
	static bool GetJoyPOVButton(DWORD dwJoy, DWORD nCursorType)
	{
		if (dwJoy >= mc_NUM_JOY_MAX) return false;

		// 斜め方向も入力可
		if (JOYPOVUP == nCursorType) {
			if (JOYPOVLEFTUP == m_joyState[dwJoy].dwPOV || JOYPOVRIGHTUP == m_joyState[dwJoy].dwPOV)
				return true;
		}
		else if (JOYPOVRIGHT == nCursorType) {
			if (JOYPOVRIGHTUP == m_joyState[dwJoy].dwPOV || JOYPOVRIGHTDOWN == m_joyState[dwJoy].dwPOV)
				return true;
		}
		else if (JOYPOVLEFT == nCursorType) {
			if (JOYPOVLEFTUP == m_joyState[dwJoy].dwPOV || JOYPOVLEFTDOWN == m_joyState[dwJoy].dwPOV)
				return true;
		}
		else if (JOYPOVDOWN == nCursorType) {
			if (JOYPOVLEFTDOWN == m_joyState[dwJoy].dwPOV || JOYPOVRIGHTDOWN == m_joyState[dwJoy].dwPOV)
				return true;
		}

		return (m_joyState[dwJoy].dwPOV == nCursorType) ? true : false;
	}

	// ゲームパッド　ボタン情報取得
	static bool GetJoyButton(DWORD dwJoy, DWORD dwBtn)
	{
		if (dwJoy >= mc_NUM_JOY_MAX) return false;
		return (m_joyState[dwJoy].dwButtons & (1 << dwBtn)) ? true : false;
	}

	// ゲームパッド　トリガ情報取得
	static bool GetJoyTrigger(DWORD dwJoy, DWORD dwBtn)
	{
		if (dwJoy >= mc_NUM_JOY_MAX) return false;
		return (m_dwJoyButtonTrigger[dwJoy] & (1 << dwBtn)) ? true : false;
	}

	// ゲームパッド　リリース情報取得
	static bool GetJoyRelease(DWORD dwJoy, DWORD dwBtn)
	{
		if (dwJoy >= mc_NUM_JOY_MAX) return false;
		return (m_dwJoyButtonRelease[dwJoy] & (1 << dwBtn)) ? true : false;
	}

	//--- 定数
	// コントローラーの許容数
	constexpr static int mc_NUM_JOY_MAX = 2;
private:
	// コントローラーの接続数
	static unsigned int m_dwJoyCount;
	// コントローラーの状態（添え字はコントローラー番号）
	static JOYINFOEX m_joyState[mc_NUM_JOY_MAX];
	// コントローラーのボタンの情報
	static DWORD m_dwJoyButtonTrigger[mc_NUM_JOY_MAX];	// トリガ情報
	static DWORD m_dwJoyButtonRelease[mc_NUM_JOY_MAX];	// リリース情報
	// コントローラーの方向ボタン情報
	static DWORD m_dwJoyPOVTrigger[mc_NUM_JOY_MAX];	// トリガ情報
	static DWORD m_dwJoyPOVRelease[mc_NUM_JOY_MAX];	// リリース情報
};

#endif //!___XINPUT_H___