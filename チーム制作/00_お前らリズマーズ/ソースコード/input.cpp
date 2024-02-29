//===========================
//
//ダイレクトX.inputファイル
//Author:中村　陸
//
//===========================
#include "input.h"

//マクロ定義
#define NUM_KEY_MAX (256)			//キーの最大数

//グローバル変数宣言
LPDIRECTINPUT8 g_pInput = NULL;						//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;			//入力デバイスへのポインタ
BYTE g_aKeyState[NUM_KEY_MAX];						//キーボードのプレス情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];				//キーボードのトリガー情報
BYTE g_aKeyStateRelease[NUM_KEY_MAX];				//キーボードのリリース情報
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];				//キーボードのリピート情報
int g_nCountRepeat;									//リピート管理カウンター

//===========================
//初期化処理
//===========================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

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
//終了処理
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

	//オブジェクトの破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//===========================
//更新処理
//===========================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];				//キーボードの入力情報
	int nCntKey;

	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for ( nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
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
		g_pDevKeyboard -> Acquire();								//キーボードのアクセス権を取得
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