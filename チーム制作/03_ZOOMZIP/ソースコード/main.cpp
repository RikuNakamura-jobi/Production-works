//==========================================================
//
//メイン処理 [main.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "main.h"
#include "stdio.h"
#include "input.h"
#include "debugproc.h"
#include "game.h"
#include "fade.h"
#include "result.h"
#include "ranking.h"
#include "title.h"
#include "tutorial.h"
#include "sound.h"
#include "logo.h"

//マクロ定義
#define WINDOW_NAME	"ZOOMZIP"	//ウインドウの名前
#define PCTIME		(1000/60)		//パソコンの時間
#define MAX_STRING	(512)			//文字の最大数
#define CLASS_NAME	"WindowClass"	//ウインドウクラスの名前

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//グローバル変数
LPDIRECT3D9 g_pD3D = NULL;	//Direct3Dオブジェクトのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	//Direct3Dデバイスへのポインタ
int g_nCountFPS = 0;		//FPSカウンタ
MODE g_mode = MODE_TEAMLOGO;	//モード
int g_bWire = 0;			//ワイヤーフレーム表示のON/OFF

//================================================
//メイン関数
//================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;	//現在時刻
	DWORD dwExecLastTime;	//最後に処理した時刻
	DWORD dwFrameCount;		//フレームカウント
	DWORD dwFPSLastTime;	//最後にFPSを計測した時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						//ウインドウのスタイル
		WindowProc,						//ウインドウプロシージャ
		0,								//0にする(通常は使用しない)
		0,								//0にする(通常は使用しない)
		hInstance,						//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),	//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),		//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),		//クライアント領域の背景色
		NULL,							//メニューバー
		CLASS_NAME,						//ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)	//ファイルのアイコン
	};

	HWND hWnd;	//ウインドウハンドル
	MSG msg;	//メッセージを格納する変数

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント形成を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx
	(
		0,								//拡張ウインドウスタイル
		CLASS_NAME,						//ウインドウクラスの名前
		WINDOW_NAME,					//ウインドウの名前
		WS_OVERLAPPEDWINDOW,			//ウインドウスタイル
		CW_USEDEFAULT,					//ウインドウの左上x座標
		CW_USEDEFAULT,					//ウインドウの左上y座標
		(rect.right - rect.left),		//ウインドウの幅
		(rect.bottom - rect.top),		//ウインドウの高さ
		NULL,							//親ウインドウのハンドル
		NULL,							//メニューハンドルまたは子ウインドウID
		hInstance,						//インスタンスハンドル
		NULL							//ウインドウ作成データ
	);

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, FALSE)))
	{//初期化が失敗した場合
		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;	//初期化
	dwExecLastTime = timeGetTime();	//現在時刻を取得(保存)
	dwFrameCount = 0;	//初期化
	dwFPSLastTime = timeGetTime();	//現在時刻を取得(保存)

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();	//現在時刻を取得
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
			 //FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	//FPSを測定した時刻を保存
				dwFrameCount = 0;	//フレームカウントをクリア
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime;	//処理開始の時刻[現在時刻]を保存

												//更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++;	//フレームカウントを加算
			}
		}
	}

	//終了処理
	Uninit();

	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//================================================
//ウインドウプロシージャ
//================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;					//返り値を格納


	const RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };		//ウインドウの領域(形)

	switch (uMsg)
	{
	case WM_DESTROY:		//ウィンドウ廃棄のメッセージ

							//WM_QUETメッセージを送る
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:		//キー押下のメッセージ

		switch (wParam)
		{
		case VK_ESCAPE:

			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				//ウインドウを廃棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	case WM_COMMAND:	//コマンド発行メッセージ
		break;
	case WM_CLOSE:		//閉じるボタン押下
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{//はいを選択したとき
		 //ウインドウを廃棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{//いいえを選択したとき
			return 0;	//0を返さないと終了してしまう
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	//既定の処理を返す
}

//================================================
//初期化処理
//================================================
LRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメータ

									//DirectX3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						//ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						//バックバッファーの形式
	d3dpp.BackBufferCount = 1;									//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil = TRUE;						//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//インターバル


	//Direct3Dデバイスの作成
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
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

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);


	//テクスチャステージステート
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//デバイスの初期化
	if (FAILED(InitDevice(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//サウンドの初期化
	InitSound(hWnd);

	//デバッグ表示の初期化処理
	InitDebugProc();

	//フェードの初期化
	InitFade(g_mode);

	return S_OK;
}

//================================================
//終了処理
//================================================
void Uninit(void)
{
	//デバイスの終了処理
	UninitDevice();

	//サウンドの終了処理
	UninitSound();

	//デバッグ表示の終了処理
	UninitDebugProc();

	//フェードの終了処理
	UninitFade();

	//Direct3Dデバイスの廃棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの廃棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}

//================================================
//更新処理
//================================================
void Update(void)
{
	//デバイスの更新処理
	UpdateDevice();

	//デバッグ表示の更新処理
	UpdateDebugProc();

	//次のモードの更新
	switch (g_mode)
	{
	case MODE_TITLE:	//タイトル画面
		UpdateTitle();
		break;
	case MODE_TUTORIAL:	//チュートリアル画面
		UpdateTutorial();
		break;
	case MODE_GAME:		//ゲーム画面
		UpdateGame();
		break;
	case MODE_RESULT:	//リザルト画面
		UpdateResult();
		break;
	case MODE_RANKING:	//ランキング画面
		UpdateRanking();
		break;
	case MODE_TEAMLOGO:	//チームロゴ画面
		UpdateLogo();
		break;
	}

	//フェードの更新処理
	UpdateFade();

	if (GetKeyboardTrigger(DIK_F8) == true)
	{//F8キー入力されたとき
		g_bWire = g_bWire ^ 1;

		//描画モードの変更
		switch (g_bWire)
		{
		case 1:
			//ワイヤーフレーム
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			break;
		case 0:
			//通常描画
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			break;
		}
	}
}

//================================================
//描画処理
//================================================
void Draw(void)
{
	//画面クリア
	g_pD3DDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画が成功した場合

		switch (g_mode)
		{
		case MODE_TITLE:	//タイトル画面
			DrawTitle();
			break;
		case MODE_TUTORIAL:	//チュートリアル画面
			DrawTutorial();
			break;
		case MODE_GAME:		//ゲーム画面
			DrawGame();
			break;
		case MODE_RESULT:	//リザルト画面
			DrawResult();
			break;
		case MODE_RANKING:	//ランキング画面
			DrawRanking();
			break;
		case MODE_TEAMLOGO:	//チームロゴ画面
			DrawLogo();
			break;
		}

		//フェードの描画処理
		DrawFade();

		//デバッグ表示の描画処理
		DrawDebugProc();

		//描画終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//================================================
//デバイスの取得
//================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//================================================
//FPSの取得
//================================================
int GetFPS(void)
{
	return g_nCountFPS;
}

//================================================
//モードの設定
//================================================
void SetMode(MODE mode)
{
	//現在のモードの終了
	switch (g_mode)
	{
	case MODE_TITLE:	//タイトル画面
		UninitTitle();
		break;
	case MODE_TUTORIAL:	//チュートリアル画面
		UninitTutorial();
		break;
	case MODE_GAME:		//ゲーム画面
		UninitGame();
		break;
	case MODE_RESULT:	//リザルト画面
		UninitResult();
		break;
	case MODE_RANKING:	//ランキング画面
		UninitRanking();
		break;
	case MODE_TEAMLOGO:	//チームロゴ画面
		UninitLogo();
		break;
	}

	//次のモードに変更
	g_mode = mode;

	//次のモードの初期化
	switch (mode)
	{
	case MODE_TITLE:	//タイトル画面
		InitTitle();
		break;
	case MODE_TUTORIAL:	//チュートリアル画面
		InitTutorial();
		break;
	case MODE_GAME:		//ゲーム画面
		InitGame();
		break;
	case MODE_RESULT:	//リザルト画面
		InitResult();
		break;
	case MODE_RANKING:	//ランキング画面
		InitRanking();
		break;
	case MODE_TEAMLOGO:	//チームロゴ画面
		InitLogo();
		break;
	}
}

//================================================
//モードの取得
//================================================
MODE GetMode(void)
{
	return g_mode;
}