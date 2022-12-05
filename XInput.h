// �����搶���炢���������q���g������ꂽ�v���O�����ł��B
// �����搶�Ɋ��ӂƂ���̋C������Y��Ȃ��ł��������B
// �{���ɂ��肪�������ƂȂ̂ŁA�A�A

// XBOX�R���g���[���[�̃v���O����(XInput)��
// �����Ă���l�������狳���Ă����v�ł��B

// ���z�z�͂��Ȃ��ł��������B

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
	// �{�^���^�C�v��ݒ肷�ׂ�
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
	// �����{�^���u��v
	POV_UP = JOYPOVUP,
	// �����{�^���u�E�v
	POV_RIGHT = JOYPOVRIGHT,
	// �����{�^���u���v
	POV_DOWN = JOYPOVDOWN,
	// �����{�^���u���v
	POV_LEFT = JOYPOVLEFT};

class XInput
{
public:
	// �Q�[���p�b�h�̏���������
	static HRESULT InitJoystick()
	{
		GetJoyCount();
		return S_OK;
	}

	// �Q�[���p�b�h�ڑ����擾
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

	// �Q�[���p�b�h�̍X�V����
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

	// �Q�[���p�b�h���擾
	static JOYINFOEX* GetJoyState(DWORD dwJoy)
	{
		if (dwJoy >= mc_NUM_JOY_MAX) return nullptr;
		return &m_joyState[dwJoy];
	}

	// �Q�[���p�b�hX���擾
	static LONG GetJoyX(DWORD dwJoy)
	{
		if (dwJoy >= mc_NUM_JOY_MAX) return 0L;
		return (LONG)m_joyState[dwJoy].dwXpos - 0x08000;
	}

	// �Q�[���p�b�hY���擾
	static LONG GetJoyY(DWORD dwJoy)
	{
		if (dwJoy >= mc_NUM_JOY_MAX) return 0L;
		return (LONG)m_joyState[dwJoy].dwYpos - 0x08000;
	}

	// �Q�[���p�b�hZ���擾
	static LONG GetJoyZ(DWORD dwJoy)
	{
		if (dwJoy >= mc_NUM_JOY_MAX) return 0L;
		return (LONG)m_joyState[dwJoy].dwZpos - 0x08000;
	}

	// �Q�[���p�b�h�@�����{�^�����擾
	static bool GetJoyPOVButton(DWORD dwJoy, DWORD nCursorType)
	{
		if (dwJoy >= mc_NUM_JOY_MAX) return false;

		// �΂ߕ��������͉�
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

	// �Q�[���p�b�h�@�{�^�����擾
	static bool GetJoyButton(DWORD dwJoy, DWORD dwBtn)
	{
		if (dwJoy >= mc_NUM_JOY_MAX) return false;
		return (m_joyState[dwJoy].dwButtons & (1 << dwBtn)) ? true : false;
	}

	// �Q�[���p�b�h�@�g���K���擾
	static bool GetJoyTrigger(DWORD dwJoy, DWORD dwBtn)
	{
		if (dwJoy >= mc_NUM_JOY_MAX) return false;
		return (m_dwJoyButtonTrigger[dwJoy] & (1 << dwBtn)) ? true : false;
	}

	// �Q�[���p�b�h�@�����[�X���擾
	static bool GetJoyRelease(DWORD dwJoy, DWORD dwBtn)
	{
		if (dwJoy >= mc_NUM_JOY_MAX) return false;
		return (m_dwJoyButtonRelease[dwJoy] & (1 << dwBtn)) ? true : false;
	}

	//--- �萔
	// �R���g���[���[�̋��e��
	constexpr static int mc_NUM_JOY_MAX = 2;
private:
	// �R���g���[���[�̐ڑ���
	static unsigned int m_dwJoyCount;
	// �R���g���[���[�̏�ԁi�Y�����̓R���g���[���[�ԍ��j
	static JOYINFOEX m_joyState[mc_NUM_JOY_MAX];
	// �R���g���[���[�̃{�^���̏��
	static DWORD m_dwJoyButtonTrigger[mc_NUM_JOY_MAX];	// �g���K���
	static DWORD m_dwJoyButtonRelease[mc_NUM_JOY_MAX];	// �����[�X���
	// �R���g���[���[�̕����{�^�����
	static DWORD m_dwJoyPOVTrigger[mc_NUM_JOY_MAX];	// �g���K���
	static DWORD m_dwJoyPOVRelease[mc_NUM_JOY_MAX];	// �����[�X���
};

#endif //!___XINPUT_H___