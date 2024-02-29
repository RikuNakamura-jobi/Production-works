//==========================================================
//
//入力デバイス処理 [input.cpp]
//Author 奥定伊吹
//
//==========================================================
#include "input.h"

//==========================================================
//マクロ定義
//==========================================================
#define NUM_KEY_MAX	(256)	//キーの最大数
#define PLAYER_MAX	(4)		//プレイヤーの最大数
#define REPEAT_TIME (15)	//リピートタイマー

//==========================================================
//グローバル変数
//==========================================================
//パッド
XINPUT_STATE g_JoyKeyState[PLAYER_MAX];			//パッドのボタン
XINPUT_STATE g_JoyKeyStateTrigger[PLAYER_MAX];	//パッドのボタントリガー
XINPUT_STATE g_JoyKeyStaterRelease[PLAYER_MAX];	//パッドのボタンリピート
XINPUT_STATE g_JoyKeyStateRepeat[PLAYER_MAX];	//パッドのボタンリピート
int g_aJoyKeyRepeatCnt[NUM_KEY_MAX];			//パッドのリピートカウント

//キーボード
LPDIRECTINPUT8 g_pInputKeyBoard = NULL;	//DirectInputオブジェクト(キーボード)へのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard;	//入力デバイス(キーボード)へのポインタ
LPCDIDEVICEINSTANCE g_lpdddi;			//デバイス(キーボード)の情報を持つ構造体のポインタ
LPVOID g_pvRefGamePad;					//ゲームパッドの
BYTE g_aKeyState[NUM_KEY_MAX];			//キーボードのプレス情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];	//キーボードのトリガー情報
BYTE g_aKeyStateRelease[NUM_KEY_MAX];	//キーボードのリリース情報
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];	//キーボードのリピート情報
int g_aKeyRepeatCnt[NUM_KEY_MAX];		//キーボードのリピートカウント

										//マウス
LPDIRECTINPUTDEVICE8 g_pDevMouse;		//入力デバイス(キーボード)へのポインタ
DIMOUSESTATE2 g_MouseState;				//マウスのプレス情報
DIMOUSESTATE2 g_MouseStateTrigger;		//マウスのトリガー情報
DIMOUSESTATE2 g_MouseStateRelease;		//マウスのリリース情報
DIMOUSESTATE2 g_MouseStateRepeat;		//マウスのリピート情報
POINT g_MousePoint;						//マウスのカーソルポインター

//==========================================================
//デバイスの初期化処理
//==========================================================
HRESULT InitDevice(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInputKeyBoard, NULL)))
	{
		return E_FAIL;
	}

	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hWnd)))
	{
		return E_FAIL;
	}

	//マウスの初期化
	InitMouse(hWnd);

	//ゲームパッドの初期化
	InitGamepad();

	return S_OK;
}

//==========================================================
//デバイスの終了処理
//==========================================================
void UninitDevice(void)
{
	//キーボードの終了処理
	UninitKeyboard();

	//マウスの終了処理
	UninitMouse();

	//ゲームパッドの終了処理
	UninitGamepad();
}

//==========================================================
//デバイスの更新処理
//==========================================================
void UpdateDevice(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	//マウスの更新処理
	UpdateMouse();

	//ゲームパッドの更新処理
	UpdateGamepad();
}

//==========================================================
//キーボードの初期化処理
//==========================================================
HRESULT InitKeyboard(HWND hWnd)
{

	//入力デバイス(キーボード)の生成
	if (FAILED(g_pInputKeyBoard->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を取得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//==========================================================
//キーボードの終了処理
//==========================================================
void UninitKeyboard(void)
{
	//入力デバイス(キーボード)の廃棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//入力デバイス(キーボード)の廃棄
	if (g_pInputKeyBoard != NULL)
	{
		g_pInputKeyBoard->Release();
		g_pInputKeyBoard = NULL;
	}
}

//==========================================================
//キーボードの更新処理
//==========================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	//キーボードの入力情報
	int nCntKey = 0;
	static int nRepeatCnt[NUM_KEY_MAX] = {};

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];	//キーボードからのトリガー情報を保存
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];	//キーボードからのリリース情報を保存
			g_aKeyState[nCntKey] = aKeyState[nCntKey];	//キーボードからのプレス情報を保存
			g_aKeyStateRepeat[nCntKey] = g_aKeyStateTrigger[nCntKey];

			//リピート取得
			if (GetKeyboardPress(nCntKey) == true)
			{//プレスで入力されているとき
				nRepeatCnt[nCntKey]++;

				if (nRepeatCnt[nCntKey] >= REPEAT_TIME)
				{//リピートカウンターがタイマーを超えたとき
					nRepeatCnt[nCntKey] = 0;
					g_aKeyStateRepeat[nCntKey] = g_aKeyState[nCntKey];
				}
				else
				{
					g_aKeyStateRepeat[nCntKey] = g_aKeyStateRelease[nCntKey];
				}
			}

			if (GetKeyboardRelease(nCntKey) == true)
			{//リリース入力されたとき
				nRepeatCnt[nCntKey] = 0;
			}
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();	//キーボードへのアクセス権を獲得
	}

}

//==========================================================
//キーボードのプレス情報を取得
//==========================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//==========================================================
//キーボードのトリガー情報を取得
//==========================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==========================================================
//キーボードのリリース情報を取得
//==========================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//==========================================================
//キーボードのリピート情報を取得
//==========================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//==========================================================
//ゲームパッドの初期化処理
//==========================================================
HRESULT InitGamepad(void)
{
	//Xinputのステートを設定
	XInputEnable(true);
	int nCnt;

	for (nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//メモリーをクリア
		memset(&g_JoyKeyState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&g_JoyKeyStateTrigger[nCnt], 0, sizeof(XINPUT_STATE));
	}

	return S_OK;
}

//==========================================================
//ゲームパッドの終了処理
//==========================================================
void UninitGamepad(void)
{
	//Xinputステートを設定
	XInputEnable(false);
}

//==========================================================
//更新処理
//==========================================================
void UpdateGamepad(void)
{
	XINPUT_STATE aGamepadState[PLAYER_MAX];	//ゲームパッドの入力情報
	static int nRepeatJoyKeyCnt[PLAYER_MAX] = {};
	int nCntPad = 0;

	for (nCntPad = 0; nCntPad < PLAYER_MAX; nCntPad++)
	{
		//入力デバイスからデータを取得
		if (XInputGetState(nCntPad, &aGamepadState[nCntPad]) == ERROR_SUCCESS)
		{
			g_JoyKeyStateTrigger[nCntPad].Gamepad.wButtons =
				~g_JoyKeyState[nCntPad].Gamepad.wButtons
				& aGamepadState[nCntPad].Gamepad.wButtons;	//トリガー(ボタン)

			g_JoyKeyStaterRelease[nCntPad].Gamepad.wButtons =
				(g_JoyKeyState[nCntPad].Gamepad.wButtons
					^ aGamepadState[nCntPad].Gamepad.wButtons)
				& g_JoyKeyState[nCntPad].Gamepad.wButtons;	//リリース(ボタン)

			g_JoyKeyState[nCntPad] = aGamepadState[nCntPad];	//プレス

																//リピート取得


		}
	}
}

//==========================================================
//ゲームパッドのプレスを取得(ボタン)
//==========================================================
bool GetGamepadPress(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyState[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//==========================================================
//ゲームパッドのトリガーを取得(ボタン)
//==========================================================
bool GetGamepadTrigger(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//==========================================================
//ゲームパッドのリピート取得
//==========================================================
bool GetGamepadRepeat(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyStateRepeat[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//==========================================================
//ゲームパッドのリリース取得
//==========================================================
bool GetGamepadRelease(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyStaterRelease[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//==========================================================
//ゲームパッドのLトリガー
//==========================================================
BYTE GetGamepadLeftTriggerPress(int nPlayer)
{
	return (g_JoyKeyState[nPlayer].Gamepad.bLeftTrigger);
}

//==========================================================
//ゲームパッドのRトリガー
//==========================================================
BYTE GetGamepadRightTriggerPress(int nPlayer)
{
	return (g_JoyKeyState[nPlayer].Gamepad.bRightTrigger);
}

//==========================================================
//ゲームパッドの値
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
//マウスの初期化
//==========================================================
HRESULT InitMouse(HWND hWnd)
{
	//入力デバイスの生成
	if (FAILED(g_pInputKeyBoard->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットの生成
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	//相対値モードで設定

	if (FAILED(g_pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}

	//マウスへのアクセス権を獲得
	g_pDevMouse->Acquire();

	return S_OK;
}

//==========================================================
//マウスの終了
//==========================================================
void UninitMouse(void)
{
	//入力デバイスの廃棄
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire();
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}
}

//==========================================================
//マウスの更新
//==========================================================
void UpdateMouse(void)
{
	DIMOUSESTATE2 MouseState;	//マウスの入力情報

								//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(MouseState), (LPVOID)&MouseState)))
	{
		g_MouseState = MouseState;	//プレス情報取得

									//マウスの現在のポインタを取得する
		GetCursorPos(&g_MousePoint);

		//スクリーン座標をクライアント座標に変換する
	}
	else
	{
		g_pDevMouse->Acquire(); //マウスへのアクセス権を獲得
	}
}

//==========================================================
//マウスのプレス情報取得
//==========================================================
bool GetMousePress(int nKey)
{
	return (g_MouseState.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================================
//マウスのトリガー情報取得
//==========================================================
bool GetMouseTrigger(int nKey)
{
	return (g_MouseStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================================
//マウスのリリース情報取得
//==========================================================
bool GetMouseRelease(int nKey)
{
	return (g_MouseStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================================
//マウスのリピート情報取得
//==========================================================
bool GetMouseRepeat(int nKey)
{
	return (g_MouseStateRepeat.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================================
//マウスのカーソル移動量取得
//==========================================================
D3DXVECTOR3 GetMouseCousorMove(void)
{
	return D3DXVECTOR3((float)g_MouseState.lX, (float)g_MouseState.lY, (float)g_MouseState.lZ);
}