//=====================================
//
//ダイレクトX.inputファイル
//Author:中村　陸
//
//=====================================
#include "input.h"
#include "dinput.h"
#include "manager.h"
#include "debugproc.h"

//マクロ定義

//静的メンバ変数宣言-------------------
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CInput::CInput()
{
	m_pDevice = NULL;
}

CInput::~CInput()
{
}

//=====================================
//インプットの初期化処理
//=====================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		//DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=====================================
//デバイスの終了処理
//=====================================
void CInput::Uninit(void)
{
	//デバイスの破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	//オブジェクトの破棄
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//=====================================
//キーボード
//=====================================
// コンストラクタ・デストラクタ
//=====================================
CInputKeyboard::CInputKeyboard()
{
	memset(&m_aKeyState[0], 0, NUM_KEY_MAX);
	memset(&m_aKeyStateTrigger[0], 0, NUM_KEY_MAX);
	memset(&m_aKeyStateRelease[0], 0, NUM_KEY_MAX);
	memset(&m_aKeyStateRepeat[0], 0, NUM_KEY_MAX);
	m_nCountRepeat = 0;
}

CInputKeyboard::~CInputKeyboard()
{
}

//=====================================
//初期化処理
//=====================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	LPDIRECTINPUT8 pInput = GetInput();						//DirectInputオブジェクトへのポインタ

	//入力デバイスの生成
	if (FAILED(pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//=====================================
//キーボードの終了処理
//=====================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//=====================================
//キーボードの更新処理
//=====================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];				//キーボードの入力情報
	int nCntKey;

	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];

			m_aKeyState[nCntKey] = aKeyState[nCntKey];				//キーボードのプレス情報を取得

			m_nCountRepeat++;
			if ((m_nCountRepeat % 10) == 0)
			{
				m_nCountRepeat = (m_nCountRepeat + 1) % 10;
				m_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];			//リリース情報を保存
			}
			else
			{
				m_aKeyStateRepeat[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];	//リピートにリリースの情報保存
			}
		}
	}
	else
	{
		m_pDevice->Acquire();								//キーボードのアクセス権を取得
	}
}

//=====================================
//プレス情報取得処理
//=====================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//=====================================
//トリガー情報取得処理
//=====================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=====================================
//リリース情報取得処理
//=====================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//=====================================
//リピート情報取得処理
//=====================================
bool CInputKeyboard::GetRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//=====================================
//全情報取得処理
//=====================================
bool CInputKeyboard::GetAll(void)
{
	for (int nCntKey = 0; nCntKey < 256; nCntKey++)
	{
		if ((m_aKeyState[nCntKey] & 0x80) ? true : false)
		{
			return true;
		}
	}

	return false;
}
//キーボード
//=====================================

//=====================================
//パッド
//=====================================
// コンストラクタ・デストラクタ
//=====================================
CInputPad::CInputPad()
{
	m_nCountRepeat = 0;
}

CInputPad::~CInputPad()
{
}

//=====================================
//コールバック
//=====================================
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	HRESULT hr;
	LPDIRECTINPUT8 pInput = CInput::GetInput();						//DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 pDevice = CManager::Get()->GetInputPad()->CInput::GetInputDevice();			//入力デバイスへのポインタ

	hr = pInput->CreateDevice(pdidInstance->guidInstance, &pDevice, NULL);

	if (FAILED(hr)) return DIENUM_CONTINUE;

	return DIENUM_STOP;
}

BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT     hr;
	DIPROPRANGE diprg;
	LPDIRECTINPUTDEVICE8 pDevice = CManager::Get()->GetInputPad()->CInput::GetInputDevice();			//入力デバイスへのポインタ

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = 0 - 1000;
	diprg.lMax = 0 + 1000;
	hr = pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr)) return DIENUM_STOP;

	return DIENUM_CONTINUE;
}

//=====================================
//初期化処理
//=====================================
HRESULT CInputPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	LPDIRECTINPUT8 pInput = GetInput();						//DirectInputオブジェクトへのポインタ

	CInput::Init(hInstance, hWnd);

	if (FAILED(pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	//入力デバイスの生成
	if (FAILED(pInput->CreateDevice(GUID_Joystick, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	if (m_pDevice == NULL)
	{
		return S_OK;
	}

	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	if (FAILED(m_pDevice->EnumObjects(EnumAxesCallback,NULL, DIDFT_ALL)))
		
	//キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//=====================================
//終了処理
//=====================================
void CInputPad::Uninit(void)
{
	CInput::Uninit();
}

//=====================================
//更新処理
//=====================================
void CInputPad::Update(void)
{
	DIJOYSTATE2 PadState = { 0 };				//パッドの入力情報

	if (m_pDevice != NULL)
	{
		m_pDevice->Poll();
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(PadState), &PadState)))
		{
			for (int nCntPad = 0; nCntPad < 16; nCntPad++)
			{
				m_PadStateTrigger.rgbButtons[nCntPad] = (m_PadState.rgbButtons[nCntPad] ^ PadState.rgbButtons[nCntPad]) & PadState.rgbButtons[nCntPad];

				m_PadStateRelease.rgbButtons[nCntPad] = (m_PadState.rgbButtons[nCntPad] ^ PadState.rgbButtons[nCntPad]) & m_PadState.rgbButtons[nCntPad];

				m_nCountRepeat++;
				if ((m_nCountRepeat % 10) == 0)
				{
					m_nCountRepeat = (m_nCountRepeat + 1) % 10;
					m_PadStateRepeat.rgbButtons[nCntPad] = PadState.rgbButtons[nCntPad];			//リリース情報を保存
				}
				else
				{
					m_PadStateRepeat.rgbButtons[nCntPad] = (m_PadState.rgbButtons[nCntPad] ^ PadState.rgbButtons[nCntPad]) & m_PadState.rgbButtons[nCntPad];	//リピートにリリースの情報保存
				}
			}

			//入力情報の保存
			m_PadState = PadState;
		}
		else
		{
			m_pDevice->Acquire();								//キーボードのアクセス権を取得
		}
	}

	int nCheck = 1;
	CManager::Get()->GetDebugProc()->Print("Pad: %d\n", nCheck);

	nCheck = m_PadState.lX;
	CManager::Get()->GetDebugProc()->Print("左スティック横: %d\n", nCheck);

	nCheck = m_PadState.lY;
	CManager::Get()->GetDebugProc()->Print("左スティック縦: %d\n", nCheck);

	nCheck = m_PadState.lZ;
	CManager::Get()->GetDebugProc()->Print("右スティック横: %d\n", nCheck);

	nCheck = m_PadState.lRz;
	CManager::Get()->GetDebugProc()->Print("右スティック縦: %d\n", nCheck);

	for (int nCnt = 0; nCnt < 12; nCnt++)
	{
		nCheck = (m_PadState.rgbButtons[nCnt] & 0x80) ? 1 : 0;
		CManager::Get()->GetDebugProc()->Print("rgbButtons%d: %d\n", nCnt, nCheck);
	}

	nCheck = m_PadState.rgdwPOV[0];
	CManager::Get()->GetDebugProc()->Print("rgdwPOV0: %d\n", nCheck);
}

//=====================================
//パッドボタンプレス情報取得処理
//=====================================
bool CInputPad::GetButtonPress(int nKey)
{
	return (m_PadState.rgbButtons[nKey] & 0x80) ? true : false;
}

//=====================================
//パッドボタントリガー情報取得処理
//=====================================
bool CInputPad::GetButtonTrigger(int nKey)
{
	return (m_PadStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//=====================================
//パッドボタンリリース情報取得処理
//=====================================
bool CInputPad::GetButtonRelease(int nKey)
{
	return (m_PadStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
}

//=====================================
//パッドボタンリピート情報取得処理
//=====================================
bool CInputPad::GetButtonRepeat(int nKey)
{
	return (m_PadStateRepeat.rgbButtons[nKey] & 0x80) ? true : false;
}

//=====================================
//パッド左右スティックプレス情報取得処理
//=====================================
int CInputPad::GetLStickLRPress(void)
{
	return m_PadState.lX;
}

//=====================================
//パッド上下スティックプレス情報取得処理
//=====================================
int CInputPad::GetLStickUDPress(void)
{
	return m_PadState.lY;
}

//=====================================
//パッド左右スティックプレス情報取得処理
//=====================================
int CInputPad::GetRStickLRPress(void)
{
	return m_PadState.lZ;
}

//=====================================
//パッド上下スティックプレス情報取得処理
//=====================================
int CInputPad::GetRStickUDPress(void)
{
	return m_PadState.lRz;
}

//=====================================
//パッド十字ボタンプレス情報取得処理
//=====================================
int CInputPad::GetButtonCrossPress(void)
{
	return m_PadState.rgdwPOV[0];
}

//=====================================
//パッド十字ボタンプレス情報取得処理
//=====================================
bool CInputPad::GetAll(void)
{
	for (int nCntKey = 0; nCntKey < 128; nCntKey++)
	{
		if ((m_PadState.rgbButtons[nCntKey] & 0x80) ? true : false)
		{
			return true;
		}
	}

	if (m_PadState.lX != 0 ||
		m_PadState.lY != 0 ||
		m_PadState.lZ != 0 ||
		m_PadState.lRz != 0 ||
		m_PadState.rgdwPOV[0] != -1)
	{
		return true;
	}

	return false;
}
//パッド
//=====================================

//=====================================
//マウス
//=====================================
// コンストラクタ・デストラクタ
//=====================================
CInputMouse::CInputMouse()
{
	m_nCountRepeat = 0;
}

CInputMouse::~CInputMouse()
{
}

//=====================================
//マウスの初期化処理
//=====================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	LPDIRECTINPUT8 pInput = GetInput();						//DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 pDevice = GetInputDevice();			//入力デバイスへのポインタ

	//入力デバイスの生成
	if (FAILED(pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
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

	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//=====================================
//マウスの終了処理
//=====================================
void CInputMouse::Uninit(void)
{
	LPDIRECTINPUTDEVICE8 pDevice = GetInputDevice();			//入力デバイスへのポインタ

	//キーボードの破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
}

//=====================================
//マウスの更新処理
//=====================================
void CInputMouse::Update(void)
{
	DIMOUSESTATE2 mouse;
	LPDIRECTINPUTDEVICE8 pDevice = GetInputDevice();			//入力デバイスへのポインタ

	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(mouse), &mouse)))
	{
		for (int nCntKey = 0; nCntKey < 5; nCntKey++)
		{
			m_StateTrigger.rgbButtons[nCntKey] = (m_State.rgbButtons[nCntKey] ^ mouse.rgbButtons[nCntKey]) & mouse.rgbButtons[nCntKey];

			m_nCountRepeat++;
			if ((m_nCountRepeat % 10) == 0)
			{
				m_nCountRepeat = (m_nCountRepeat + 1) % 10;
				m_StateRepeat.rgbButtons[nCntKey] = mouse.rgbButtons[nCntKey];			//リリース情報を保存
			}
			else
			{
				m_StateRepeat.rgbButtons[nCntKey] = (m_StateRepeat.rgbButtons[nCntKey] ^ mouse.rgbButtons[nCntKey]) & m_StateRepeat.rgbButtons[nCntKey];	//リピートにリリースの情報保存
			}
		}

		//入力情報の保存
		m_State = mouse;
	}
	else
	{
		m_pDevice->Acquire();								//キーボードのアクセス権を取得
	}
}

//=====================================
//マウスボタンプレス情報取得処理
//=====================================
bool CInputMouse::GetPress(int nKey)
{
	return (m_State.rgbButtons[nKey] & 0x80) ? true : false;
}

//=====================================
//マウストリガー情報取得処理
//=====================================
bool CInputMouse::GetTrigger(int nKey)
{
	return (m_StateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//=====================================
//マウスリピート情報取得処理
//=====================================
bool CInputMouse::GetRepeat(int nKey)
{
	return (m_StateRepeat.rgbButtons[nKey] & 0x80) ? true : false;
}

//=====================================
//マウス横移動情報取得処理
//=====================================
int CInputMouse::GetRStickLRPress(void)
{
	return m_State.lX * 100;
}

//=====================================
//マウス縦移動情報取得処理
//=====================================
int CInputMouse::GetRStickUDPress(void)
{
	return m_State.lY * 100;
}

//=====================================
//マウス全情報取得処理
//=====================================
bool CInputMouse::GetAll(void)
{
	for (int nCntKey = 0; nCntKey < 8; nCntKey++)
	{
		if ((m_State.rgbButtons[nCntKey] & 0x80) ? true : false)
		{
			return true;
		}
	}

	if (m_State.lX != 0 ||
		m_State.lY != 0)
	{
		return true;
	}

	return false;
}
//マウス
//=====================================