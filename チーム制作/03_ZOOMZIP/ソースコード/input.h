//==========================================================
//
//���̓f�o�C�X���� [input.h]
//Author ����ɐ�
//
//==========================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include "Xinput.h"

#pragma comment(lib, "xinput.lib")	//���͏���

//==========================================================
//�񋓌^�̒�`
//==========================================================
//�Q�[���p�b�h�{�^��
typedef enum
{
	BUTTON_UP = 0,				//��{�^��
	BUTTON_DOWN,				//���{�^��
	BUTTON_LEFT,				//���{�^��
	BUTTON_RIGHT,				//�E�{�^��
	BUTTON_START,				//start
	BUTTON_BACK,				//back
	BUTTON_LEFTSTCPUSH,			//���X�e�B�b�N��������
	BUTTON_RIGHTSTCPUSH,		//�E�X�e�B�b�N��������
	BUTTON_LEFTBUTTON,			//LB(L)�{�^��
	BUTTON_RIGHTBUTTON,			//RB(R)�{�^��
	BUTTON_11,					//L�g���K�[
	BUTTON_12,					//R�g���K�[
	BUTTON_A,					//A�{�^��
	BUTTON_B,					//B�{�^��
	BUTTON_X,					//X�{�^��
	BUTTON_Y,					//Y�{�^��
	BUTTON_LEFT_Y,				//��(Y)
	BUTTON_LEFT_X,				//��(X)
	BUTTON_RIGHT_Y,				//�E(Y)
	BUTTON_RIGHT_X,				//�E(X)
	BUTTON_MAX
}JOYKEY;

//�Q�[���p�b�h�X�e�B�b�N
typedef enum
{
	STICK_PLUS = 0,				//�l���v���X
	STICK_MINUS,				//�l���}�C�i�X
	STICK_MAX

}Stick;

//�}�E�X
typedef enum
{
	MOUSE_LBUTTON = 0,	//���N���b�N
	MOUSE_RBUTTON,		//�E�N���b�N
	MOUSE_WHEEL,		//�E�N���b�N
	MOUSE_MAX
}MOUSE;


//==========================================================
//�v���g�^�C�v�錾
//==========================================================
//�f�o�C�X
HRESULT InitDevice(HINSTANCE hInstance, HWND hWnd);
void UninitDevice(void);
void UpdateDevice(void);

//�L�[�{�[�h
HRESULT InitKeyboard(HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey);

//�Q�[���p�b�h
HRESULT InitGamepad(void);
void UninitGamepad(void);
void UpdateGamepad(void);
bool GetGamepadPress(JOYKEY Key, int nPlayer);
bool GetGamepadRepeat(JOYKEY Key, int nPlayer);
bool GetGamepadRelease(JOYKEY Key, int nPlayer);
bool GetGamepadTrigger(JOYKEY Key, int nPlayer);
bool GetGamepadStickPress(int nPlayer, JOYKEY Key, float DeadZone, Stick PlusStick);
BYTE GetGamepadRightTriggerPress(int nPlayer);

//�}�E�X
HRESULT InitMouse(HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);
bool GetMousePress(int nKey);
bool GetMouseTrigger(int nKey);
bool GetMouseRelease(int nKey);
bool GetMouseRepeat(int nKey);
D3DXVECTOR3 GetMouseCousorMove();

#endif