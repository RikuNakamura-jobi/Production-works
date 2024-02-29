//==========================================================
//
//入力デバイス処理 [input.h]
//Author 奥定伊吹
//
//==========================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include "Xinput.h"

#pragma comment(lib, "xinput.lib")	//入力処理

//==========================================================
//列挙型の定義
//==========================================================
//ゲームパッドボタン
typedef enum
{
	BUTTON_UP = 0,				//上ボタン
	BUTTON_DOWN,				//下ボタン
	BUTTON_LEFT,				//左ボタン
	BUTTON_RIGHT,				//右ボタン
	BUTTON_START,				//start
	BUTTON_BACK,				//back
	BUTTON_LEFTSTCPUSH,			//左スティック押し込み
	BUTTON_RIGHTSTCPUSH,		//右スティック押し込み
	BUTTON_LEFTBUTTON,			//LB(L)ボタン
	BUTTON_RIGHTBUTTON,			//RB(R)ボタン
	BUTTON_11,					//Lトリガー
	BUTTON_12,					//Rトリガー
	BUTTON_A,					//Aボタン
	BUTTON_B,					//Bボタン
	BUTTON_X,					//Xボタン
	BUTTON_Y,					//Yボタン
	BUTTON_LEFT_Y,				//左(Y)
	BUTTON_LEFT_X,				//左(X)
	BUTTON_RIGHT_Y,				//右(Y)
	BUTTON_RIGHT_X,				//右(X)
	BUTTON_MAX
}JOYKEY;

//ゲームパッドスティック
typedef enum
{
	STICK_PLUS = 0,				//値がプラス
	STICK_MINUS,				//値がマイナス
	STICK_MAX

}Stick;

//マウス
typedef enum
{
	MOUSE_LBUTTON = 0,	//左クリック
	MOUSE_RBUTTON,		//右クリック
	MOUSE_WHEEL,		//右クリック
	MOUSE_MAX
}MOUSE;


//==========================================================
//プロトタイプ宣言
//==========================================================
//デバイス
HRESULT InitDevice(HINSTANCE hInstance, HWND hWnd);
void UninitDevice(void);
void UpdateDevice(void);

//キーボード
HRESULT InitKeyboard(HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey);

//ゲームパッド
HRESULT InitGamepad(void);
void UninitGamepad(void);
void UpdateGamepad(void);
bool GetGamepadPress(JOYKEY Key, int nPlayer);
bool GetGamepadRepeat(JOYKEY Key, int nPlayer);
bool GetGamepadRelease(JOYKEY Key, int nPlayer);
bool GetGamepadTrigger(JOYKEY Key, int nPlayer);
bool GetGamepadStickPress(int nPlayer, JOYKEY Key, float DeadZone, Stick PlusStick);
BYTE GetGamepadRightTriggerPress(int nPlayer);

//マウス
HRESULT InitMouse(HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);
bool GetMousePress(int nKey);
bool GetMouseTrigger(int nKey);
bool GetMouseRelease(int nKey);
bool GetMouseRepeat(int nKey);
D3DXVECTOR3 GetMouseCousorMove();

#endif