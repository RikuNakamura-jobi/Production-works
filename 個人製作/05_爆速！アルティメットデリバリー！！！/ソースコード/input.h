//===========================
//
//ダイレクトX.inputヘッダー
//Author:中村　陸
//
//===========================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//マクロ定義---------------------------
#define NUM_KEY_MAX (256)			//キーの最大数

//列挙型定義---------------------------

//クラス定義---------------------------

//インプットクラス
class CInput
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CInput();				//デフォルト
	virtual ~CInput();

	//メンバ関数
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

	virtual bool GetPress(int nKey) = 0;
	virtual bool GetTrigger(int nKey) = 0;
	virtual bool GetRelease(int nKey) = 0;
	virtual bool GetRepeat(int nKey) = 0;
	virtual bool GetAll(void) = 0;

	virtual bool GetButtonPress(int nKey) { return false; }
	virtual bool GetButtonTrigger(int nKey) { return false; }
	virtual bool GetButtonRelease(int nKey) { return false; }
	virtual bool GetButtonRepeat(int nKey) { return false; }
	virtual int GetButtonCrossPress(void) { return 0; }
	virtual int GetLStickLRPress(void) { return 0; }
	virtual int GetLStickUDPress(void) { return 0; }
	virtual int GetRStickLRPress(void) { return 0; }
	virtual int GetRStickUDPress(void) { return 0; }
	
	LPDIRECTINPUTDEVICE8 GetInputDevice(void) { return m_pDevice; }

	//静的メンバ関数
	static LPDIRECTINPUT8 GetInput(void) { return m_pInput; }

protected:			//子ならアクセス可能(使わない)

	//メンバ変数
	LPDIRECTINPUTDEVICE8 m_pDevice;			//入力デバイスへのポインタ

	//静的メンバ変数
	

private:			//外部からアクセス不可能

	//メンバ変数

	//静的メンバ変数
	static LPDIRECTINPUT8 m_pInput;						//DirectInputオブジェクトへのポインタ
};

//キーボード
class CInputKeyboard : public CInput
{
public:				//外部からアクセス可能

					//コンストラクタ・デストラクタ
	CInputKeyboard();				//デフォルト
	~CInputKeyboard();

	//メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);
	bool GetAll(void);

	//静的メンバ関数

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ変数
	BYTE m_aKeyState[NUM_KEY_MAX];						//キーボードのプレス情報
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];				//キーボードのトリガー情報
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];				//キーボードのリリース情報
	BYTE m_aKeyStateRepeat[NUM_KEY_MAX];				//キーボードのリピート情報
	int m_nCountRepeat;

	//静的メンバ変数

};

//マウス
class CInputMouse : public CInput
{
public:				//外部からアクセス可能

					//コンストラクタ・デストラクタ
	CInputMouse();				//デフォルト
	~CInputMouse();

	//メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);
	bool GetAll(void);

	int GetRStickLRPress(void);
	int GetRStickUDPress(void);

	//静的メンバ関数

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ変数
	DIMOUSESTATE2 m_State;							//全入力情報
	DIMOUSESTATE2 m_StateTrigger;					//全入力情報
	DIMOUSESTATE2 m_StateRelease;					//全入力情報
	DIMOUSESTATE2 m_StateRepeat;					//全入力情報
	int m_nCountRepeat;

	//静的メンバ変数

};

//パッド
class CInputPad : public CInput
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CInputPad();				//デフォルト
	~CInputPad();

	//メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetButtonPress(int nKey);
	bool GetButtonTrigger(int nKey);
	bool GetButtonRelease(int nKey);
	bool GetButtonRepeat(int nKey);
	int GetButtonCrossPress(void);
	int GetLStickLRPress(void);
	int GetLStickUDPress(void);
	int GetRStickLRPress(void);
	int GetRStickUDPress(void);

	bool GetPress(int nKey) { return false; }
	bool GetTrigger(int nKey) { return false; }
	bool GetRelease(int nKey) { return false; }
	bool GetRepeat(int nKey) { return false; }
	bool GetAll(void);

	//静的メンバ関数

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//静的メンバ関数

	//メンバ変数
	DIJOYSTATE2 m_PadState;							//全入力情報
	DIJOYSTATE2 m_PadStateTrigger;					//全入力情報
	DIJOYSTATE2 m_PadStateRelease;					//全入力情報
	DIJOYSTATE2 m_PadStateRepeat;					//全入力情報
	int m_nCountRepeat;

	//静的メンバ変数

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_INPUT_H_