#include "XInput.h"


// �R���g���[���[�̐ڑ���
unsigned int XInput::m_dwJoyCount;
// �R���g���[���[�̏�ԁi�Y�����̓R���g���[���[�ԍ��j
JOYINFOEX XInput::m_joyState[XInput::mc_NUM_JOY_MAX];
// �R���g���[���[�̃{�^���̏��
DWORD XInput::m_dwJoyButtonTrigger[XInput::mc_NUM_JOY_MAX];	// �g���K���
DWORD XInput::m_dwJoyButtonRelease[XInput::mc_NUM_JOY_MAX];	// �����[�X���
// �R���g���[���[�̕����{�^�����
DWORD XInput::m_dwJoyPOVTrigger[XInput::mc_NUM_JOY_MAX];	// �g���K���
DWORD XInput::m_dwJoyPOVRelease[XInput::mc_NUM_JOY_MAX];	// �����[�X���