//==========================================================
//
//���̓f�o�C�X���� [input.cpp]
//Author ����ɐ�
//
//==========================================================
#include "input.h"

//==========================================================
//�}�N����`
//==========================================================
#define NUM_KEY_MAX	(256)	//�L�[�̍ő吔
#define PLAYER_MAX	(4)		//�v���C���[�̍ő吔
#define REPEAT_TIME (15)	//���s�[�g�^�C�}�[

//==========================================================
//�O���[�o���ϐ�
//==========================================================
//�p�b�h
XINPUT_STATE g_JoyKeyState[PLAYER_MAX];			//�p�b�h�̃{�^��
XINPUT_STATE g_JoyKeyStateTrigger[PLAYER_MAX];	//�p�b�h�̃{�^���g���K�[
XINPUT_STATE g_JoyKeyStaterRelease[PLAYER_MAX];	//�p�b�h�̃{�^�����s�[�g
XINPUT_STATE g_JoyKeyStateRepeat[PLAYER_MAX];	//�p�b�h�̃{�^�����s�[�g
int g_aJoyKeyRepeatCnt[NUM_KEY_MAX];			//�p�b�h�̃��s�[�g�J�E���g

//�L�[�{�[�h
LPDIRECTINPUT8 g_pInputKeyBoard = NULL;	//DirectInput�I�u�W�F�N�g(�L�[�{�[�h)�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard;	//���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
LPCDIDEVICEINSTANCE g_lpdddi;			//�f�o�C�X(�L�[�{�[�h)�̏������\���̂̃|�C���^
LPVOID g_pvRefGamePad;					//�Q�[���p�b�h��
BYTE g_aKeyState[NUM_KEY_MAX];			//�L�[�{�[�h�̃v���X���
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];	//�L�[�{�[�h�̃g���K�[���
BYTE g_aKeyStateRelease[NUM_KEY_MAX];	//�L�[�{�[�h�̃����[�X���
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];	//�L�[�{�[�h�̃��s�[�g���
int g_aKeyRepeatCnt[NUM_KEY_MAX];		//�L�[�{�[�h�̃��s�[�g�J�E���g

										//�}�E�X
LPDIRECTINPUTDEVICE8 g_pDevMouse;		//���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
DIMOUSESTATE2 g_MouseState;				//�}�E�X�̃v���X���
DIMOUSESTATE2 g_MouseStateTrigger;		//�}�E�X�̃g���K�[���
DIMOUSESTATE2 g_MouseStateRelease;		//�}�E�X�̃����[�X���
DIMOUSESTATE2 g_MouseStateRepeat;		//�}�E�X�̃��s�[�g���
POINT g_MousePoint;						//�}�E�X�̃J�[�\���|�C���^�[

//==========================================================
//�f�o�C�X�̏���������
//==========================================================
HRESULT InitDevice(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInputKeyBoard, NULL)))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hWnd)))
	{
		return E_FAIL;
	}

	//�}�E�X�̏�����
	InitMouse(hWnd);

	//�Q�[���p�b�h�̏�����
	InitGamepad();

	return S_OK;
}

//==========================================================
//�f�o�C�X�̏I������
//==========================================================
void UninitDevice(void)
{
	//�L�[�{�[�h�̏I������
	UninitKeyboard();

	//�}�E�X�̏I������
	UninitMouse();

	//�Q�[���p�b�h�̏I������
	UninitGamepad();
}

//==========================================================
//�f�o�C�X�̍X�V����
//==========================================================
void UpdateDevice(void)
{
	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	//�}�E�X�̍X�V����
	UpdateMouse();

	//�Q�[���p�b�h�̍X�V����
	UpdateGamepad();
}

//==========================================================
//�L�[�{�[�h�̏���������
//==========================================================
HRESULT InitKeyboard(HWND hWnd)
{

	//���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(g_pInputKeyBoard->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����擾
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//==========================================================
//�L�[�{�[�h�̏I������
//==========================================================
void UninitKeyboard(void)
{
	//���̓f�o�C�X(�L�[�{�[�h)�̔p��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//���̓f�o�C�X(�L�[�{�[�h)�̔p��
	if (g_pInputKeyBoard != NULL)
	{
		g_pInputKeyBoard->Release();
		g_pInputKeyBoard = NULL;
	}
}

//==========================================================
//�L�[�{�[�h�̍X�V����
//==========================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	//�L�[�{�[�h�̓��͏��
	int nCntKey = 0;
	static int nRepeatCnt[NUM_KEY_MAX] = {};

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];	//�L�[�{�[�h����̃g���K�[����ۑ�
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];	//�L�[�{�[�h����̃����[�X����ۑ�
			g_aKeyState[nCntKey] = aKeyState[nCntKey];	//�L�[�{�[�h����̃v���X����ۑ�
			g_aKeyStateRepeat[nCntKey] = g_aKeyStateTrigger[nCntKey];

			//���s�[�g�擾
			if (GetKeyboardPress(nCntKey) == true)
			{//�v���X�œ��͂���Ă���Ƃ�
				nRepeatCnt[nCntKey]++;

				if (nRepeatCnt[nCntKey] >= REPEAT_TIME)
				{//���s�[�g�J�E���^�[���^�C�}�[�𒴂����Ƃ�
					nRepeatCnt[nCntKey] = 0;
					g_aKeyStateRepeat[nCntKey] = g_aKeyState[nCntKey];
				}
				else
				{
					g_aKeyStateRepeat[nCntKey] = g_aKeyStateRelease[nCntKey];
				}
			}

			if (GetKeyboardRelease(nCntKey) == true)
			{//�����[�X���͂��ꂽ�Ƃ�
				nRepeatCnt[nCntKey] = 0;
			}
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}

}

//==========================================================
//�L�[�{�[�h�̃v���X�����擾
//==========================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//==========================================================
//�L�[�{�[�h�̃g���K�[�����擾
//==========================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==========================================================
//�L�[�{�[�h�̃����[�X�����擾
//==========================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//==========================================================
//�L�[�{�[�h�̃��s�[�g�����擾
//==========================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//==========================================================
//�Q�[���p�b�h�̏���������
//==========================================================
HRESULT InitGamepad(void)
{
	//Xinput�̃X�e�[�g��ݒ�
	XInputEnable(true);
	int nCnt;

	for (nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//�������[���N���A
		memset(&g_JoyKeyState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&g_JoyKeyStateTrigger[nCnt], 0, sizeof(XINPUT_STATE));
	}

	return S_OK;
}

//==========================================================
//�Q�[���p�b�h�̏I������
//==========================================================
void UninitGamepad(void)
{
	//Xinput�X�e�[�g��ݒ�
	XInputEnable(false);
}

//==========================================================
//�X�V����
//==========================================================
void UpdateGamepad(void)
{
	XINPUT_STATE aGamepadState[PLAYER_MAX];	//�Q�[���p�b�h�̓��͏��
	static int nRepeatJoyKeyCnt[PLAYER_MAX] = {};
	int nCntPad = 0;

	for (nCntPad = 0; nCntPad < PLAYER_MAX; nCntPad++)
	{
		//���̓f�o�C�X����f�[�^���擾
		if (XInputGetState(nCntPad, &aGamepadState[nCntPad]) == ERROR_SUCCESS)
		{
			g_JoyKeyStateTrigger[nCntPad].Gamepad.wButtons =
				~g_JoyKeyState[nCntPad].Gamepad.wButtons
				& aGamepadState[nCntPad].Gamepad.wButtons;	//�g���K�[(�{�^��)

			g_JoyKeyStaterRelease[nCntPad].Gamepad.wButtons =
				(g_JoyKeyState[nCntPad].Gamepad.wButtons
					^ aGamepadState[nCntPad].Gamepad.wButtons)
				& g_JoyKeyState[nCntPad].Gamepad.wButtons;	//�����[�X(�{�^��)

			g_JoyKeyState[nCntPad] = aGamepadState[nCntPad];	//�v���X

																//���s�[�g�擾


		}
	}
}

//==========================================================
//�Q�[���p�b�h�̃v���X���擾(�{�^��)
//==========================================================
bool GetGamepadPress(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyState[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//==========================================================
//�Q�[���p�b�h�̃g���K�[���擾(�{�^��)
//==========================================================
bool GetGamepadTrigger(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//==========================================================
//�Q�[���p�b�h�̃��s�[�g�擾
//==========================================================
bool GetGamepadRepeat(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyStateRepeat[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//==========================================================
//�Q�[���p�b�h�̃����[�X�擾
//==========================================================
bool GetGamepadRelease(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyStaterRelease[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//==========================================================
//�Q�[���p�b�h��L�g���K�[
//==========================================================
BYTE GetGamepadLeftTriggerPress(int nPlayer)
{
	return (g_JoyKeyState[nPlayer].Gamepad.bLeftTrigger);
}

//==========================================================
//�Q�[���p�b�h��R�g���K�[
//==========================================================
BYTE GetGamepadRightTriggerPress(int nPlayer)
{
	return (g_JoyKeyState[nPlayer].Gamepad.bRightTrigger);
}

//==========================================================
//�Q�[���p�b�h�̒l
//==========================================================
bool GetGamepadStickPress(int nPlayer, JOYKEY Key, float DeadZone, Stick PlusStick)
{
	float fAnswer = 0.0f;

	switch (Key)
	{
	case BUTTON_LEFT_X:
		fAnswer = (float)g_JoyKeyState[nPlayer].Gamepad.sThumbLX / SHRT_MAX;
		break;
	case BUTTON_LEFT_Y:
		fAnswer = (float)g_JoyKeyState[nPlayer].Gamepad.sThumbLY / SHRT_MAX;
		break;
	case BUTTON_RIGHT_X:
		fAnswer = (float)g_JoyKeyState[nPlayer].Gamepad.sThumbRX / SHRT_MAX;
		break;
	case BUTTON_RIGHT_Y:
		fAnswer = (float)g_JoyKeyState[nPlayer].Gamepad.sThumbRY / SHRT_MAX;
		break;
	default:
		break;
	}

	if ((fAnswer < 0.0f && PlusStick == STICK_MINUS && fAnswer < -DeadZone) || (fAnswer > 0.0f && PlusStick == STICK_PLUS && fAnswer > DeadZone))
	{
		return true;
	}


	return false;
}

//==========================================================
//�}�E�X�̏�����
//==========================================================
HRESULT InitMouse(HWND hWnd)
{
	//���̓f�o�C�X�̐���
	if (FAILED(g_pInputKeyBoard->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g�̐���
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	//���Βl���[�h�Őݒ�

	if (FAILED(g_pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}

	//�}�E�X�ւ̃A�N�Z�X�����l��
	g_pDevMouse->Acquire();

	return S_OK;
}

//==========================================================
//�}�E�X�̏I��
//==========================================================
void UninitMouse(void)
{
	//���̓f�o�C�X�̔p��
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire();
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}
}

//==========================================================
//�}�E�X�̍X�V
//==========================================================
void UpdateMouse(void)
{
	DIMOUSESTATE2 MouseState;	//�}�E�X�̓��͏��

								//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(MouseState), (LPVOID)&MouseState)))
	{
		g_MouseState = MouseState;	//�v���X���擾

									//�}�E�X�̌��݂̃|�C���^���擾����
		GetCursorPos(&g_MousePoint);

		//�X�N���[�����W���N���C�A���g���W�ɕϊ�����
	}
	else
	{
		g_pDevMouse->Acquire(); //�}�E�X�ւ̃A�N�Z�X�����l��
	}
}

//==========================================================
//�}�E�X�̃v���X���擾
//==========================================================
bool GetMousePress(int nKey)
{
	return (g_MouseState.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================================
//�}�E�X�̃g���K�[���擾
//==========================================================
bool GetMouseTrigger(int nKey)
{
	return (g_MouseStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================================
//�}�E�X�̃����[�X���擾
//==========================================================
bool GetMouseRelease(int nKey)
{
	return (g_MouseStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================================
//�}�E�X�̃��s�[�g���擾
//==========================================================
bool GetMouseRepeat(int nKey)
{
	return (g_MouseStateRepeat.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================================
//�}�E�X�̃J�[�\���ړ��ʎ擾
//==========================================================
D3DXVECTOR3 GetMouseCousorMove(void)
{
	return D3DXVECTOR3((float)g_MouseState.lX, (float)g_MouseState.lY, (float)g_MouseState.lZ);
}