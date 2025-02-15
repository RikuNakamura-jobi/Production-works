//===========================
//
//ダイレクトX.mainファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "input.h"
#include "player.h"
#include "block.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "menu.h"
#include "stageselect.h"
#include "tutorial.h"
#include "sound.h"

//マクロ定義
#define CLASS_NAME "WindowClass"												//ウィンドウクラスの名前
#define WINDOW_NAME "EARTH RECAPTURE FORCE"										//ウィンドウの名前

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);
void DrawOpe(void);

//グローバル変数宣言
LPDIRECT3D9 g_pD3D = NULL;						//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;			//Direct3Dデバイスへのポインタ
LPD3DXFONT g_pFont = NULL;						//フォントへのポインタ
int g_nCountFPS = 0;							//FPSカウント
bool g_bDispDebug = false;						//デバッグ表示のON/OFF
MODE g_mode = MODE_TITLE;						//現在のモード

//===========================
//メイン関数
//===========================
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hinstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),										//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,												//ウィンドウのスタイル
		WindowProc,												//ウィンドウプロシージャ
		0,														//0にする(通常は使用しない)
		0,														//0にする(通常は使用しない)
		hinstance,												//インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),						//タスクバーアイコン
		LoadCursor(NULL, IDC_ARROW),							//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),								//クライアント領域の背景色
		NULL,													//メニューバー
		CLASS_NAME,												//ウィンドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION),						//ファイルのアイコン
	};

	HWND hWnd;													//ウィンドウハンドル(識別子)
	MSG msg;													//メッセージを格納する変数
	DWORD dwCurrentTime;										//現在時刻
	DWORD dwExecLastTime;										//最後に処理した時刻
	DWORD dwFrameCount;											//フレームカウント
	DWORD dwFPSLastTime;										//最後にFPSを計測した時刻
	
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウを作成
	hWnd = CreateWindowEx
	(0,															//拡張ウィンドウスタイル
		CLASS_NAME,												//ウィンドウクラスの名前
		WINDOW_NAME,											//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,									//ウィンドウスタイル
		CW_USEDEFAULT,											//ウィンドウの左上X座標
		CW_USEDEFAULT,											//ウィンドウの左上Y座標
		(rect.right - rect.left),								//ウィンドウの幅
		(rect.bottom - rect.top),								//ウィンドウの高さ
		NULL,													//新ウィンドウのハンドル
		NULL,													//メニューハンドルまたは子ウィンドウID
		hinstance,												//インスタンスハンドル
		NULL);													//ウィンドウ作成データ

	//初期化処理
	if (FAILED(Init(hinstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = DEFAULT_NUMBER;								//初期化
	dwExecLastTime = timeGetTime();								//現在時刻を取得
	dwFrameCount = DEFAULT_NUMBER;
	dwFPSLastTime = timeGetTime();

	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);									//ウィンドウの表示状態を設定
	UpdateWindow(hWnd);											//クライアント領域を更新

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);									//仮想メッセージを文字メッセージに変換
				DispatchMessage(&msg);									//ウィンドウプロシージャへメッセージを送出
			}
		}
		else
		{//DirectXの描画処理
			dwCurrentTime = timeGetTime();								//現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;							//FPSを測定した時刻を保存
				dwFrameCount = DEFAULT_NUMBER;							//フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime;							//処理の開始を保存

				//更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++;											//フレームカウントを加算
			}
		}
	}

	//終了処理
	Uninit();

	//分解能を戻す
	timeEndPeriod(1);

	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//===========================
//ウィンドウプロシージャ
//===========================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;													//返り値を格納

	switch (uMsg)
	{
	case WM_DESTROY:											//ウィンドウ破棄のメッセージ
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:											//キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:												//ボタン押下のメッセージ
		nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//===========================
//初期化処理
//===========================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;					//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;			//プレゼンテーションパラメータ

	//3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現存のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;									//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;									//ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3dpp.BackBufferFormat;						//バックバッファの形式
	d3dpp.BackBufferCount = 1;												//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;								//ダブルバッファの切換(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;									//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;								//デプスバッファとして16bit使う
	d3dpp.Windowed = bWindow;												//ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;				//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;				//インターバル

	//Direct3Dデバイスの生成
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice))
	{
		if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice))
		{
			if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプルステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャーステージステート
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//サウンドの初期化処理
	InitSound(hWnd);

	//デバッグ表示用フォントの作成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//フェードの設定
	InitFade(g_mode);

	return S_OK;
}

//===========================
//終了処理
//===========================
void Uninit(void)
{
	//タイトル画面の終了処理
	UninitTitle();

	//タイトル画面の終了処理
	UninitMenu();

	//タイトル画面の終了処理
	UninitTutorial();

	//タイトル画面の終了処理
	UninitStageselect();

	//ゲーム画面の終了処理
	UninitGame();

	//リザルト画面の終了処理
	UninitResult();

	//フェードの終了処理
	UninitFade();

	//サウンドの終了処理
	UninitSound();

	//キーボードの終了処理
	UninitKeyboard();

	//デバッグ表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//===========================
//更新処理
//===========================
void Update(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	switch (g_mode)
	{
	case MODE_TITLE:
		//ゲーム画面の更新処理
		UpdateTitle();
		break;
	case MODE_MENU:
		//リザルト画面の更新処理
		UpdateMenu();
		break;
	case MODE_TUTORIAL:
		//リザルト画面の更新処理
		UpdateTutorial();
		break;
	case MODE_SELECT:
		//リザルト画面の更新処理
		UpdateStageselect();
		break;
	case MODE_GAME:
		//ゲーム画面の更新処理
		UpdateGame();
		break;
	case MODE_RESULT:
		//リザルト画面の更新処理
		UpdateResult();
		break;
	}

	//フェードの更新処理
	UpdateFade();

	if (GetKeyboardTrigger(DIK_F1) == true)
	{
		g_bDispDebug = g_bDispDebug ^ 1;
	}
}

//===========================
//描画処理
//===========================
void Draw(void)
{
	//画面クリア(バックバッファとZバッファのクリア)
	g_pD3DDevice->Clear(0,NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功したとき
		switch (g_mode)
		{
		case MODE_TITLE:
			//ゲーム画面の描画処理
			DrawTitle();
			break;
		case MODE_MENU:
			//リザルト画面の描画処理
			DrawMenu();
			break;
		case MODE_TUTORIAL:
			//リザルト画面の描画処理
			DrawTutorial();
			break;
		case MODE_SELECT:
			//リザルト画面の描画処理
			DrawStageselect();
			break;
		case MODE_GAME:
			//ゲーム画面の描画処理
			DrawGame();
			break;
		case MODE_RESULT:
			//リザルト画面の描画処理
			DrawResult();
			break;
		}

		if (g_bDispDebug == false && g_mode == MODE_GAME)
		{
#ifdef _DEBUG

			//デバッグ表示
			DrawFPS();

#endif // _DEBUG

#ifndef _DEBUG

			//操作の表示
			DrawOpe();

#endif // _DEBUG
		}

		//描画終了
		g_pD3DDevice->EndScene();
	}

	//フェードの描画処理
	DrawFade();

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//===========================
//デバッグ表示
//===========================
void DrawFPS(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];
	Player *player = GetPlayer();

	//文字列に代入
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//テキストに描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================
//操作表示
//===========================
void DrawOpe(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	//文字列に代入
	wsprintf(&aStr[0], "移動\n上:W\n下:S\n左:A\n右:D\n\n射撃(連射):space\n射撃(単発):B\n射撃(ホーミング):V\n\nポーズ:P\n\n終了:escape", g_nCountFPS);

	//テキストに描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 0));
}

//===========================
//デバイスの取得
//===========================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//===========================
//モードの設定
//===========================
void SetMode(MODE mode)
{
	//現在の画面の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:
		//タイトル画面の終了処理
		UninitTitle();
		break;
	case MODE_MENU:
		//リザルト画面の終了処理
		UninitMenu();
		break;
	case MODE_TUTORIAL:
		//リザルト画面の終了処理
		UninitTutorial();
		break;
	case MODE_SELECT:
		//リザルト画面の終了処理
		UninitStageselect();
		break;
	case MODE_GAME:
		//ゲーム画面の終了処理
		UninitGame();
		break;
	case MODE_RESULT:
		//リザルト画面の終了処理
		UninitResult();
		break;
	}

	//新しい画面の初期化処理
	switch (mode)
	{
	case MODE_TITLE:
		//タイトル画面の初期化処理
		InitTitle();
		break;
	case MODE_MENU:
		//リザルト画面の初期化処理
		InitMenu();
		break;
	case MODE_TUTORIAL:
		//リザルト画面の初期化処理
		InitTutorial();
		break;
	case MODE_SELECT:
		//リザルト画面の初期化処理
		InitStageselect();
		break;
	case MODE_GAME:
		//ゲーム画面の初期化処理
		InitGame();
		break;
	case MODE_RESULT:
		//リザルト画面の初期化処理
		InitResult();
		break;
	}

	g_mode = mode;
}

//===========================
//モードの取得処理
//===========================
MODE GetMode(void)
{
	return g_mode;
}