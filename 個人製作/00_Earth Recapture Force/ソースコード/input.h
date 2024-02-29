//===========================
//
//�_�C���N�gX.input�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//===========================
//�v���g�^�C�v�錾
//===========================
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

//�p�b�h
HRESULT InitPad(HWND hWnd);
void UninitPad(void);
void UpdatePad(void);
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext);
bool GetPadButtonPress(int nKey);
bool GetPadButtonTrigger(int nKey);
bool GetPadButtonRelease(int nKey);
bool GetPadButtonRepeat(int nKey);
int GetPadLStickLRPress(void);
int GetPadLStickUDPress(void);
int GetPadRStickLRPress(void);
int GetPadRStickUDPress(void);

//�}�E�X
HRESULT InitMouse(HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);
void GetMouseDebug(void);
bool GetMousePress(int nKey);
bool GetMouseTrigger(int nKey);

#endif // !_INPUT_H_