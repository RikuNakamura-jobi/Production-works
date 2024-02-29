//===========================
//
//ダイレクトX.inputファイル
//Author:中村　陸
//
//===========================
#include "input.h"
#include "dinput.h"

//マクロ定義
#define NUM_KEY_MAX (256)			//キーの最大数

typedef struct
{
	LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;			//入力デバイスへのポインタ
}Keyboard;

typedef struct
{
	LPDIRECTINPUTDEVICE8 pDevMouse = NULL;				//入力デバイスへのポインタ
	DIMOUSESTATE2 MouseState;							//全入力情報
	DIMOUSESTATE2 MouseStateTrigger;					//全入力情報
}Mouse;

typedef struct
{
	LPDIRECTINPUTDEVICE8 pDevPad = NULL;			//入力デバイスへのポインタ
	DIJOYSTATE2 PadState;							//全入力情報
	DIJOYSTATE2 PadStateTrigger;					//全入力情報
	DIJOYSTATE2 PadStateRelease;					//全入力情報
	DIJOYSTATE2 PadStateRepeat;						//全入力情報
	int nCountRepeat;
}Pad;

//グローバル変数宣言
LPDIRECTINPUT8 g_pInput = NULL;						//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;			//入力デバイスへのポインタ
BYTE g_aKeyState[NUM_KEY_MAX];						//キーボードのプレス情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];				//キーボードのトリガー情報
BYTE g_aKeyStateRelease[NUM_KEY_MAX];				//キーボードのリリース情報
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];				//キーボードのリピート情報
Mouse g_mouse;										//マウス構造体
Pad g_pad;
int g_nCountRepeat;									//リピート管理カウンター

//===========================
//デバイスの初期化処理
//===========================
HRESULT InitDevice(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
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
//デバイスの終了処理
//===========================
void UninitDevice(void)
{
	UninitKeyboard();

	UninitMouse();

	UninitPad();

	//オブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//===========================
//デバイスの更新処理
//===========================
void UpdateDevice(void)
{
	UpdateKeyboard();

	UpdateMouse();

	UpdatePad();
}

//===========================
//キーボードの初期化処理
//===========================
HRESULT InitKeyboard(HWND hWnd)
{
	//入力デバイスの生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//===========================
//キーボードの終了処理
//===========================
void UninitKeyboard(void)
{
	//キーボードの破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
}

//===========================
//キーボードの更新処理
//===========================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];				//キーボードの入力情報
	int nCntKey;

	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];

			g_aKeyState[nCntKey] = aKeyState[nCntKey];				//キーボードのプレス情報を取得

			g_nCountRepeat++;
			if ((g_nCountRepeat % SHOT_INTERVAL_PLAYER) == 0)
			{
				g_nCountRepeat = (g_nCountRepeat + 1) % SHOT_INTERVAL_PLAYER;
				g_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];			//リリース情報を保存
			}
			else
			{
				g_aKeyStateRepeat[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];	//リピートにリリースの情報保存
			}
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();								//キーボードのアクセス権を取得
	}
}

//===========================
//パッドの初期化処理
//===========================
HRESULT InitPad(HWND hWnd)
{
	if (FAILED(g_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))

	//入力デバイスの生成
	if (FAILED(g_pInput->CreateDevice(GUID_Joystick, &g_pad.pDevPad, NULL)))
	{
		return E_FAIL;
	}

	if (g_pad.pDevPad == NULL)
	{
		return S_OK;
	}

	//データフォーマットを設定
	if (FAILED(g_pad.pDevPad->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pad.pDevPad->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	if (FAILED(g_pad.pDevPad->EnumObjects(EnumAxesCallback,
		NULL, DIDFT_ALL)))

	//キーボードへのアクセス権を獲得
	g_pad.pDevPad->Acquire();

	return S_OK;
}

//===========================
//パッドの終了処理
//===========================
void UninitPad(void)
{
	//キーボードの破棄
	if (g_pad.pDevPad != NULL)
	{
		g_pad.pDevPad->Unacquire();
		g_pad.pDevPad->Release();
		g_pad.pDevPad = NULL;
	}
}

//===========================
//パッドの更新処理
//===========================
void UpdatePad(void)
{
	DIJOYSTATE2 PadState = { 0 };				//パッドの入力情報

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
					g_pad.PadStateRepeat.rgbButtons[nCntPad] = PadState.rgbButtons[nCntPad];			//リリース情報を保存
				}
				else
				{
					g_pad.PadStateRepeat.rgbButtons[nCntPad] = (g_pad.PadState.rgbButtons[nCntPad] ^ PadState.rgbButtons[nCntPad]) & g_pad.PadState.rgbButtons[nCntPad];	//リピートにリリースの情報保存
				}
			}

			//入力情報の保存
			g_pad.PadState = PadState;
		}
		else
		{
			g_pad.pDevPad->Acquire();								//キーボードのアクセス権を取得
		}
	}
}

//===========================
//コールバック
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
//マウスの初期化処理
//===========================
HRESULT InitMouse(HWND hWnd)
{
	//入力デバイスの生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_mouse.pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_mouse.pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_mouse.pDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//デバイスの設定
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

	//キーボードへのアクセス権を獲得
	g_mouse.pDevMouse->Acquire();

	return S_OK;
}

//===========================
//マウスの終了処理
//===========================
void UninitMouse(void)
{
	//キーボードの破棄
	if (g_mouse.pDevMouse != NULL)
	{
		g_mouse.pDevMouse->Unacquire();
		g_mouse.pDevMouse->Release();
		g_mouse.pDevMouse = NULL;
	}
}

//===========================
//マウスの更新処理
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

		//入力情報の保存
		g_mouse.MouseState = mouse;
	}
	else
	{
		g_mouse.pDevMouse->Acquire();								//キーボードのアクセス権を取得
	}
}

//===========================
//プレス情報取得処理
//===========================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//===========================
//トリガー情報取得処理
//===========================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//===========================
//リリース情報取得処理
//===========================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//===========================
//リピート情報取得処理
//===========================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//===========================
//マウスボタンプレス情報取得処理
//===========================
bool GetMousePress(int nKey)
{
	return (g_mouse.MouseState.rgbButtons[nKey] & 0x80) ? true : false;
}

//===========================
//マウストリガー情報取得処理
//===========================
bool GetMouseTrigger(int nKey)
{
	return (g_mouse.MouseStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//===========================
//パッドボタンプレス情報取得処理
//===========================
bool GetPadButtonPress(int nKey)
{
	return (g_pad.PadState.rgbButtons[nKey] & 0x80) ? true : false;
}

//===========================
//パッドボタントリガー情報取得処理
//===========================
bool GetPadButtonTrigger(int nKey)
{
	return (g_pad.PadStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//===========================
//パッドボタンリリース情報取得処理
//===========================
bool GetPadButtonRelease(int nKey)
{
	return (g_pad.PadStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
}

//===========================
//パッドボタンリピート情報取得処理
//===========================
bool GetPadButtonRepeat(int nKey)
{
	return (g_pad.PadStateRepeat.rgbButtons[nKey] & 0x80) ? true : false;
}

//===========================
//パッド左右スティックプレス情報取得処理
//===========================
int GetPadLStickLRPress(void)
{
	return g_pad.PadState.lX;
}

//===========================
//パッド上下スティックプレス情報取得処理
//===========================
int GetPadLStickUDPress(void)
{
	return g_pad.PadState.lY;
}

//===========================
//パッド左右スティックプレス情報取得処理
//===========================
int GetPadRStickLRPress(void)
{
	return g_pad.PadState.lRx;
}

//===========================
//パッド上下スティックプレス情報取得処理
//===========================
int GetPadRStickUDPress(void)
{
	return g_pad.PadState.lRy;
}

//===========================
//マウスのデバッグ表示処理
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
			wsprintf(&aStr[0], "マウスの情報: %d\nマウスの情報: %d", g_mouse.MouseState.rgbButtons[nCnt], g_mouse.MouseStateTrigger.rgbButtons[nCnt]);
		}

		pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
}