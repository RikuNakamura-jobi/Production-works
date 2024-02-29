//===========================
//
//�_�C���N�gX.input�t�@�C��
//Author:�����@��
//
//===========================
#include "input.h"
#include "dinput.h"

//�}�N����`
#define NUM_KEY_MAX (256)			//�L�[�̍ő吔

typedef struct
{
	LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;			//���̓f�o�C�X�ւ̃|�C���^
}Keyboard;

typedef struct
{
	LPDIRECTINPUTDEVICE8 pDevMouse = NULL;				//���̓f�o�C�X�ւ̃|�C���^
	DIMOUSESTATE2 MouseState;							//�S���͏��
	DIMOUSESTATE2 MouseStateTrigger;					//�S���͏��
}Mouse;

typedef struct
{
	LPDIRECTINPUTDEVICE8 pDevPad = NULL;			//���̓f�o�C�X�ւ̃|�C���^
	DIJOYSTATE2 PadState;							//�S���͏��
	DIJOYSTATE2 PadStateTrigger;					//�S���͏��
	DIJOYSTATE2 PadStateRelease;					//�S���͏��
	DIJOYSTATE2 PadStateRepeat;						//�S���͏��
	int nCountRepeat;
}Pad;

//�O���[�o���ϐ��錾
LPDIRECTINPUT8 g_pInput = NULL;						//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;			//���̓f�o�C�X�ւ̃|�C���^
BYTE g_aKeyState[NUM_KEY_MAX];						//�L�[�{�[�h�̃v���X���
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];				//�L�[�{�[�h�̃g���K�[���
BYTE g_aKeyStateRelease[NUM_KEY_MAX];				//�L�[�{�[�h�̃����[�X���
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];				//�L�[�{�[�h�̃��s�[�g���
Mouse g_mouse;										//�}�E�X�\����
Pad g_pad;
int g_nCountRepeat;									//���s�[�g�Ǘ��J�E���^�[

//===========================
//�f�o�C�X�̏���������
//===========================
HRESULT InitDevice(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	if (FAILED(InitKeyboard(hWnd)))
	{
		return E_FAIL;
	}

	if (FAILED(InitMouse(hWnd)))
	{
		return E_FAIL;
	}

	if (FAILED(InitPad(hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===========================
//�f�o�C�X�̏I������
//===========================
void UninitDevice(void)
{
	UninitKeyboard();

	UninitMouse();

	UninitPad();

	//�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//===========================
//�f�o�C�X�̍X�V����
//===========================
void UpdateDevice(void)
{
	UpdateKeyboard();

	UpdateMouse();

	UpdatePad();
}

//===========================
//�L�[�{�[�h�̏���������
//===========================
HRESULT InitKeyboard(HWND hWnd)
{
	//���̓f�o�C�X�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//===========================
//�L�[�{�[�h�̏I������
//===========================
void UninitKeyboard(void)
{
	//�L�[�{�[�h�̔j��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
}

//===========================
//�L�[�{�[�h�̍X�V����
//===========================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];				//�L�[�{�[�h�̓��͏��
	int nCntKey;

	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];

			g_aKeyState[nCntKey] = aKeyState[nCntKey];				//�L�[�{�[�h�̃v���X�����擾

			g_nCountRepeat++;
			if ((g_nCountRepeat % SHOT_INTERVAL_PLAYER) == 0)
			{
				g_nCountRepeat = (g_nCountRepeat + 1) % SHOT_INTERVAL_PLAYER;
				g_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];			//�����[�X����ۑ�
			}
			else
			{
				g_aKeyStateRepeat[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];	//���s�[�g�Ƀ����[�X�̏��ۑ�
			}
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();								//�L�[�{�[�h�̃A�N�Z�X�����擾
	}
}

//===========================
//�p�b�h�̏���������
//===========================
HRESULT InitPad(HWND hWnd)
{
	if (FAILED(g_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))

	//���̓f�o�C�X�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_Joystick, &g_pad.pDevPad, NULL)))
	{
		return E_FAIL;
	}

	if (g_pad.pDevPad == NULL)
	{
		return S_OK;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pad.pDevPad->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(g_pad.pDevPad->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pad.pDevPad->EnumObjects(EnumAxesCallback,
		NULL, DIDFT_ALL)))

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pad.pDevPad->Acquire();

	return S_OK;
}

//===========================
//�p�b�h�̏I������
//===========================
void UninitPad(void)
{
	//�L�[�{�[�h�̔j��
	if (g_pad.pDevPad != NULL)
	{
		g_pad.pDevPad->Unacquire();
		g_pad.pDevPad->Release();
		g_pad.pDevPad = NULL;
	}
}

//===========================
//�p�b�h�̍X�V����
//===========================
void UpdatePad(void)
{
	DIJOYSTATE2 PadState = { 0 };				//�p�b�h�̓��͏��

	if (g_pad.pDevPad != NULL)
	{
		g_pad.pDevPad->Poll();
		if (SUCCEEDED(g_pad.pDevPad->GetDeviceState(sizeof(PadState), &PadState)))
		{
			for (int nCntPad = 0; nCntPad < 16; nCntPad++)
			{
				g_pad.PadStateTrigger.rgbButtons[nCntPad] = (g_pad.PadState.rgbButtons[nCntPad] ^ PadState.rgbButtons[nCntPad]) & PadState.rgbButtons[nCntPad];

				g_pad.PadStateRelease.rgbButtons[nCntPad] = (g_pad.PadState.rgbButtons[nCntPad] ^ PadState.rgbButtons[nCntPad]) & g_pad.PadState.rgbButtons[nCntPad];

				g_pad.nCountRepeat++;
				if ((g_pad.nCountRepeat % SHOT_INTERVAL_PLAYER) == 0)
				{
					g_pad.nCountRepeat = (g_pad.nCountRepeat + 1) % SHOT_INTERVAL_PLAYER;
					g_pad.PadStateRepeat.rgbButtons[nCntPad] = PadState.rgbButtons[nCntPad];			//�����[�X����ۑ�
				}
				else
				{
					g_pad.PadStateRepeat.rgbButtons[nCntPad] = (g_pad.PadState.rgbButtons[nCntPad] ^ PadState.rgbButtons[nCntPad]) & g_pad.PadState.rgbButtons[nCntPad];	//���s�[�g�Ƀ����[�X�̏��ۑ�
				}
			}

			//���͏��̕ۑ�
			g_pad.PadState = PadState;
		}
		else
		{
			g_pad.pDevPad->Acquire();								//�L�[�{�[�h�̃A�N�Z�X�����擾
		}
	}
}

//===========================
//�R�[���o�b�N
//===========================
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	HRESULT hr;

	hr = g_pInput->CreateDevice(pdidInstance->guidInstance, &g_pad.pDevPad, NULL);

	if (FAILED(hr)) return DIENUM_CONTINUE;

	return DIENUM_STOP;
}

BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT     hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = 0 - 1000;
	diprg.lMax = 0 + 1000;
	hr = g_pad.pDevPad->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr)) return DIENUM_STOP;

	return DIENUM_CONTINUE;
}

//===========================
//�}�E�X�̏���������
//===========================
HRESULT InitMouse(HWND hWnd)
{
	//���̓f�o�C�X�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_mouse.pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_mouse.pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(g_mouse.pDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;

	if (FAILED(g_mouse.pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_mouse.pDevMouse->Acquire();

	return S_OK;
}

//===========================
//�}�E�X�̏I������
//===========================
void UninitMouse(void)
{
	//�L�[�{�[�h�̔j��
	if (g_mouse.pDevMouse != NULL)
	{
		g_mouse.pDevMouse->Unacquire();
		g_mouse.pDevMouse->Release();
		g_mouse.pDevMouse = NULL;
	}
}

//===========================
//�}�E�X�̍X�V����
//===========================
void UpdateMouse(void)
{
	DIMOUSESTATE2 mouse;

	if (SUCCEEDED(g_mouse.pDevMouse->GetDeviceState(sizeof(mouse), &mouse)))
	{
		for (int nCntKey = 0; nCntKey < 5; nCntKey++)
		{
			g_mouse.MouseStateTrigger.rgbButtons[nCntKey] = (g_mouse.MouseState.rgbButtons[nCntKey] ^ mouse.rgbButtons[nCntKey]) & mouse.rgbButtons[nCntKey];
		}

		//���͏��̕ۑ�
		g_mouse.MouseState = mouse;
	}
	else
	{
		g_mouse.pDevMouse->Acquire();								//�L�[�{�[�h�̃A�N�Z�X�����擾
	}
}

//===========================
//�v���X���擾����
//===========================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//===========================
//�g���K�[���擾����
//===========================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//===========================
//�����[�X���擾����
//===========================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//===========================
//���s�[�g���擾����
//===========================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//===========================
//�}�E�X�{�^���v���X���擾����
//===========================
bool GetMousePress(int nKey)
{
	return (g_mouse.MouseState.rgbButtons[nKey] & 0x80) ? true : false;
}

//===========================
//�}�E�X�g���K�[���擾����
//===========================
bool GetMouseTrigger(int nKey)
{
	return (g_mouse.MouseStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//===========================
//�p�b�h�{�^���v���X���擾����
//===========================
bool GetPadButtonPress(int nKey)
{
	return (g_pad.PadState.rgbButtons[nKey] & 0x80) ? true : false;
}

//===========================
//�p�b�h�{�^���g���K�[���擾����
//===========================
bool GetPadButtonTrigger(int nKey)
{
	return (g_pad.PadStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//===========================
//�p�b�h�{�^�������[�X���擾����
//===========================
bool GetPadButtonRelease(int nKey)
{
	return (g_pad.PadStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
}

//===========================
//�p�b�h�{�^�����s�[�g���擾����
//===========================
bool GetPadButtonRepeat(int nKey)
{
	return (g_pad.PadStateRepeat.rgbButtons[nKey] & 0x80) ? true : false;
}

//===========================
//�p�b�h���E�X�e�B�b�N�v���X���擾����
//===========================
int GetPadLStickLRPress(void)
{
	return g_pad.PadState.lX;
}

//===========================
//�p�b�h�㉺�X�e�B�b�N�v���X���擾����
//===========================
int GetPadLStickUDPress(void)
{
	return g_pad.PadState.lY;
}

//===========================
//�p�b�h���E�X�e�B�b�N�v���X���擾����
//===========================
int GetPadRStickLRPress(void)
{
	return g_pad.PadState.lRx;
}

//===========================
//�p�b�h�㉺�X�e�B�b�N�v���X���擾����
//===========================
int GetPadRStickUDPress(void)
{
	return g_pad.PadState.lRy;
}

//===========================
//�}�E�X�̃f�o�b�O�\������
//===========================
void GetMouseDebug(void)
{
	char aStr[256];
	
	LPD3DXFONT pFont = GetFont();

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		RECT rect = { 0, nCnt * 40 + 20, SCREEN_WIDTH, SCREEN_HEIGHT };

		if (nCnt < 5)
		{
			wsprintf(&aStr[0], "�}�E�X�̏��: %d\n�}�E�X�̏��: %d", g_mouse.MouseState.rgbButtons[nCnt], g_mouse.MouseStateTrigger.rgbButtons[nCnt]);
		}

		pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
}