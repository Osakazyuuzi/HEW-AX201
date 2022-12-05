#include "XInput.h"


// コントローラーの接続数
unsigned int XInput::m_dwJoyCount;
// コントローラーの状態（添え字はコントローラー番号）
JOYINFOEX XInput::m_joyState[XInput::mc_NUM_JOY_MAX];
// コントローラーのボタンの情報
DWORD XInput::m_dwJoyButtonTrigger[XInput::mc_NUM_JOY_MAX];	// トリガ情報
DWORD XInput::m_dwJoyButtonRelease[XInput::mc_NUM_JOY_MAX];	// リリース情報
// コントローラーの方向ボタン情報
DWORD XInput::m_dwJoyPOVTrigger[XInput::mc_NUM_JOY_MAX];	// トリガ情報
DWORD XInput::m_dwJoyPOVRelease[XInput::mc_NUM_JOY_MAX];	// リリース情報