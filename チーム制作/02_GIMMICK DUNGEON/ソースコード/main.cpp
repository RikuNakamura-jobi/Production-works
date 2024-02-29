//========================================
//
//2Dシューティングゲーム[main.cpp]
//Author:佐久間優香
//
//========================================
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "fade.h"
#include "team_logo.h"
#include "title.h"
#include "game.h"
#include "tutorial.h"
#include "result.h"
#include "ranking.h"
#include "camera.h"
#include "menu.h"
#include "player.h"
#include "enemy.h"
#include "object.h"
#include "orbit.h"
#include "shadow.h"
#include "sound.h"
#include "fog.h"
#include "option.h"
#include "model.h"
#include "score.h"
#include "effect.h"

//マクロ定義
#define CLASS_NAME				"WindowClass"					//ウインドウクラスの名前
#define WINDOW_NAME				"3Dゲーム"						//ウインドウの名前(キャプションに表示)
#define ID_BUTTON_FINISH		(101)							//終了ボタンのID
#define ID_TIMER				(131)							//タイマーのID
#define TIMER_INTERVAL			(1000/60)						//タイマーの発生間隔(ミリ秒)

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstsnce, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);
void DrawMove(void);
void DrawRot(void);
void DrawPos(void);
void DrawModelPos(void);
void DrawEnemyState(void);

//グローバル変数
LPDIRECT3D9 g_pD3D = NULL;										//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;							//Direc3Dデバイスへのポインタ
LPD3DXFONT g_pFont = NULL;										//フォントへのポインタ
int g_nCountFPS = 0;											//FPSカウント
bool g_bDispDebug;										//デバッグ表示のON/OFF
#ifdef _DEBUG
MODE g_mode = MODE_LOGO;										//現在のモード
#else
MODE g_mode = MODE_LOGO;										//現在のモード
#endif

//========================================
//メイン関数
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wcex
    {
        sizeof(WNDCLASSEX),											//WNDCLASSEXのメモリサイズ
        CS_CLASSDC,													//ウインドウのスタイル
        WindowProc,													//ウインドウのプロシージャ
        0,															//0にする(通常は使用しない)
        0,															//0にする(通常は使用しない)
        hInstance,													//インスタンスハンドル
        LoadIcon(NULL,IDI_APPLICATION),								//タスクバーのアイコン
        LoadCursor(NULL,IDC_ARROW),									//マウスカーソル
        (HBRUSH)(COLOR_WINDOW + 1),									//クライアント領域の背景色
        NULL,														//メニューバー
        CLASS_NAME,													//ウインドクラスの名前
        LoadIcon(NULL,IDI_APPLICATION),								//ファイルのアイコン
    };

    HWND hWnd;														//ウインドウハンドル(識別子)
    MSG msg;														//メッセージを格納する変数
    DWORD dwCurrentTime;											//現在時刻
    DWORD dwExecLastTime;											//最後に処理した時刻
    DWORD dwFrameCount;												//フレームカウント
    DWORD dwFPSLastTime;											//最後にを計測した時刻

    RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };				//画面サイズの構造体

    //ウインドウクラスの登録
    RegisterClassEx(&wcex);

    AdjustWindowRect(&rect, WS_EX_OVERLAPPEDWINDOW, FALSE);

    //ウインドウを生成
    hWnd = CreateWindowEx(0,										//拡張ウインドウスタイル
        CLASS_NAME,													//ウインドウクラスの名前
        WINDOW_NAME,												//ウインドウの名前
        WS_OVERLAPPEDWINDOW,										//ウインドウのスタイル
        CW_USEDEFAULT,												//ウインドウの左上X座標
        CW_USEDEFAULT,												//ウインドウの左上Y座標
        (rect.right - rect.left),									//ウインドウの幅
        (rect.bottom - rect.top),									//ウインドウの高さ
        NULL,														//親ウインドウのハンドル
        NULL,														//メニューハンドルまたは子ウインドウID
        hInstance,													//インスタンスハンドル
        NULL);														//ウインドウ作成データ

    //初期化処理
#ifdef _DEBUG
    if (FAILED(Init(hInstance, hWnd, TRUE)))
    {//初期化処理が失敗した場合
        return -1;
    }
#else
    if (FAILED(Init(hInstance, hWnd, FALSE)))
    {//初期化処理が失敗した場合
        return -1;
    }
#endif //_DEBUG

    //分解能を設定
    timeBeginPeriod(1);
    dwCurrentTime = 0;												//初期化
    dwExecLastTime = timeGetTime();									//現在時刻を取得(保存)

    //ウインドウの表示
    ShowWindow(hWnd, nCmdShow);										//ウインドウの表示状態を設定
    UpdateWindow(hWnd);												//クライアント領域を更新
    dwFrameCount = 0;
    dwFPSLastTime = timeGetTime();

    srand((unsigned int)time(0));//乱数の初期化

    //メッセージループ
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            else
            {
                //メッセージの設定
                TranslateMessage(&msg);								//仮想キーメッセージを文字メッセージへ変換
                DispatchMessage(&msg);								//ウインドウプロシージャへメッセージを送出
            }
        }
        else
        {//DirectXの処理
            dwCurrentTime = timeGetTime();							//現在時刻を取得

            if ((dwCurrentTime - dwFPSLastTime) >= 500)
            {//0.5秒経過
                //FPSを計測
                g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

                dwFPSLastTime = dwCurrentTime;						//FPSを計測したときの時刻を保存
                dwFrameCount = 0;									//フレームカウントをクリア
            }
            if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
            {//60文の1秒経過
                dwExecLastTime = dwCurrentTime;						//処理開始の時刻[現在時刻]を保存

                //更新処理
                Update();

                //描画処理
                Draw();

                dwFrameCount++;										//フレームカウントを加算
            }
        }
    }
    //終了処理
    Uninit();

    //ウインドウクラスの登録を解除
    UnregisterClass(CLASS_NAME, wcex.hInstance);
    return(int)msg.wParam;

}

//========================================
//ウインドウプロシージャ
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int nID;														//返り値を格納

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:												//キー押下のメッセージ
        switch (wParam)
        {
        case VK_ESCAPE:												//[ESC]キーが押された

            //nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);
            //if (nID == IDYES)
            //{

                //ウインドウを破棄する(WM_DESTROYメッセージを送る)
                DestroyWindow(hWnd);
            //}
            break;
        }
        break;

    case WM_COMMAND:												//コマンド発行のメッセージ
        if (LOWORD(wParam) == ID_BUTTON_FINISH)
        {
            nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);
            if (nID == IDYES)
            {

                //ウインドウを破棄する(WM_DESTROYメッセージを送る)
                DestroyWindow(hWnd);
            }
        }
        break;

    case WM_CLOSE:

        //nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);

        //if (nID == IDYES)
        //{
            //ウインドウを破棄する(WM_DESTROYメッセージを送る)
            DestroyWindow(hWnd);
        //}
        //else
        //{
            return 0;												//0を返さないと終了してしまう
        //}

        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);				//既定の処理を返す
}

//========================================
//初期化処理
//========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
    D3DDISPLAYMODE d3ddm;											//ディスプレイモード
    D3DPRESENT_PARAMETERS d3dpp;									//プレゼンテーションパラメータ

    //Direct3Dオブジェクトの生成
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
    ZeroMemory(&d3dpp, sizeof(d3dpp));								//パラメータのゼロクリア

    d3dpp.BackBufferWidth = SCREEN_WIDTH;							//ゲーム画面サイズ(幅)
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//ゲーム画面サイズ(高さ)
    d3dpp.BackBufferFormat = d3ddm.Format;							//バックバッファの形式
    d3dpp.BackBufferCount = 1;										//バックバッファの数
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//ダブルバッファの切り替え(映像信号に同期)
    d3dpp.EnableAutoDepthStencil = TRUE;							//デプスバッファとステンシルバッファを生成
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//デプスバッファとして16bitを使う
    d3dpp.Windowed = bWindow;										//ウインドウモード
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//リフレッシュレート
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//インターバル

    //Direct3Dデバイスの生成
    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp,
        &g_pD3DDevice)))
    {
        if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &d3dpp,
            &g_pD3DDevice)))
        {
            if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,

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

    //テクスチャステージステートの設定
    g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

    //デバッグ表示用フォントの生成
    D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Teminal", &g_pFont);

    //キーボードの初期化処理
    if (FAILED(InitDevice(hInstance, hWnd)))
    {
        return E_FAIL;
    }
	InitOption();

    //サウンドの初期化処理
    InitSound(hWnd);

	//モデル情報の初期化
	InitModel();

	//モデル読み込み
	LoadAllModel();

	//霧の初期化
	InitFog();

    //フェードの設定
    InitFade(g_mode);

    //ランキングの読み込み
    ResetRanking();

#ifdef _DEBUG	//デバッグ表記をするかどうか
	g_bDispDebug = false;
#else
	g_bDispDebug = false;
#endif

    return S_OK;
}

//========================================
//終了処理
//========================================
void Uninit(void)
{
    //分解能を戻す
    timeEndPeriod(1);

	//モデル情報終了
	UninitModel();

	//霧の終了処理
	UninitFog();

	//タイトル画面の終了処理
	UninitTeam_logo();
	
	//タイトル画面の終了処理
	UninitTitle();

    //チュートリアルの終了処理
    UninitTutorial();

    //ゲーム画面の終了処理
    UninitGame();

    //リザルト画面の終了処理
    UninitResult();

    //ランキング画面の終了処理
    UninitRanking();

    //フェードの終了処理
    UninitFade();

    //サウンドの終了処理
    UninitSound();

    //入力デバイスの終了処理
    UninitDevice();
	UninitOption();

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

//========================================
//更新処理
//========================================
void Update(void)
{
    //入力デバイスの更新処理
    UpdateDevice();

    switch (g_mode)
    {
	case MODE_LOGO:
		UpdateTeam_logo();									//ロゴ画面
		break;
	
	case MODE_TITLE:
        UpdateTitle();									//タイトル画面
        break;

    case MODE_TUTORIAL:
        UpdateTutorial();									//チュートリアル画面
        break;

    case MODE_GAME:
        UpdateGame();									//ゲーム画面
        break;

    case MODE_RANKING:
        UpdateRanking();								//ランキング画面
        break;
    }

	if (GetKeyboardTrigger(DIK_F1) == true)
	{//デバッグ表記切り替え
		g_bDispDebug = (g_bDispDebug == false) ? true : false;
	}

	// 音の更新処理
	UpdateSound();

	// 霧の更新処理
	UpdateFog();
    
	// フェードの更新処理
    UpdateFade();
}

//========================================
//描画処理
//========================================
void Draw(void)
{
    //画面クリア(バックバッファとZバッファのクリア)
    g_pD3DDevice->Clear(0, NULL,
        (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
        D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

    //描画開始
    if (SUCCEEDED(g_pD3DDevice->BeginScene()))
    {//描画開始が成功した場合

		//霧の設定
		SetFog();

        switch (g_mode)
        {
		case MODE_LOGO:
			DrawTeam_logo();								//ロゴ画面
			break;
		
		case MODE_TITLE:
            DrawTitle();									//タイトル画面
            break;

        case MODE_TUTORIAL:
            DrawTutorial();									//チュートリアル画面
            break;

        case MODE_GAME:
            DrawGame();										//ゲーム画面
            break;

        case MODE_RANKING:
            DrawRanking();									//ランキング画面
            break;
        }

        //フェードの描画処理
        DrawFade();

		g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		g_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
		g_pD3DDevice->SetRenderState(D3DRS_FOGDENSITY, (DWORD)0.66f);

		if (g_bDispDebug)
		{//デバッグ表記
			//移動量
			DrawMove();

			//FPSの表示
			DrawFPS();

			//向き
			DrawRot();

			//位置
			DrawPos();

			//モデルの位置
			DrawModelPos();

			//敵の状態
			DrawEnemyState();

			//カメラの情報
			DrawDebugCamera(g_pFont);

			//オブジェクト情報
			DebugObject();

			//敵の情報
			DrawEnemyDebug();

			//プレイヤーモーション情報
			DebugPlayerMotion();

			//スコア情報
			DebugScore();

			//エフェクト情報
			DebugEffect();
		}

        //描画終了
        g_pD3DDevice->EndScene();
    }

    //バックバッファとフロントバッファの入れ替え
    g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//========================================
//デバイスの取得
//========================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
    return g_pD3DDevice;
}

//========================================
//FPSの表示
//========================================
void DrawFPS(void)
{
    RECT rect = { 0,160,SCREEN_WIDTH,SCREEN_HEIGHT };
    char aStr[256];

    //文字列に代入
    wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

    //テキストの描画
    g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//========================================
//操作説明
//========================================
void DrawMove(void)
{
    RECT rect1 = { 0,370,SCREEN_WIDTH,SCREEN_HEIGHT };
    char aStr[6][256];
	int nNumString;

	if (GetPhotoMode())
	{
		//文字列に代入
		wsprintf(&aStr[0][0], "水平移動:[WASD]\n");
		wsprintf(&aStr[1][0], "加    速:[LSHIFT]\n");
		wsprintf(&aStr[2][0], "上下移動:[TB]\n");
		wsprintf(&aStr[3][0], "左右回転:[QE]\n");
		wsprintf(&aStr[4][0], "上下移動:[YN]\n");
		wsprintf(&aStr[5][0], "フォトモード切り替え:[RCTRL]\n");
		nNumString = 6;
	}
	else
	{
		//文字列に代入
		wsprintf(&aStr[0][0], "移動:[WASD]");
		wsprintf(&aStr[1][0], "フォトモード切り替え:[RCTRL]\n");
		nNumString = 2;
	}

	for (int nCntText = 0;nCntText < nNumString;nCntText++)
	{
		//テキストの描画
		g_pFont->DrawText(NULL, &aStr[nCntText][0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

		rect1.top += 20;
	}
}

//========================================
//操作説明
//========================================
void DrawRot(void)
{
    Camera *pCamera = GetCamera();
    RECT rect3 = { 0,200,SCREEN_WIDTH,SCREEN_HEIGHT };
    char aStr[256];

    sprintf(&aStr[0], "カメラの角度;%0.3f %0.3f %0.3f", pCamera->rot.x, pCamera->rot.y, pCamera->rot.z);

    //テキストの描画
    g_pFont->DrawText(NULL, &aStr[0], -1, &rect3, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//========================================
//操作説明
//========================================
void DrawPos(void)
{
    Camera *pCamera = GetCamera();
    RECT rect4 = { 0,180,SCREEN_WIDTH,SCREEN_HEIGHT };
    char aStr[256];

    sprintf(&aStr[0], "注視点の位置;%0.3f %0.3f %0.3f", pCamera->posR.x, pCamera->posR.y, pCamera->posR.z);

    //テキストの描画
    g_pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//========================================
//プレイヤーの状態デバッグ表示
//========================================
void DrawModelPos(void)
{
    Player *pPlayer = GetPlayer();
    RECT rect4 = { 0,220,SCREEN_WIDTH,SCREEN_HEIGHT };
    char aStr[256];

    float fAngle = atan2f(pPlayer->move.x, pPlayer->move.z);

    sprintf(&aStr[0], "プレイヤーの体力：[%d] \nプレイヤーの位置：[%.2f,%.2f,%.2f]\nプレイヤーの向き：[%.2f,%.2f,%.2f]\n移動方向[%.2f]",
        pPlayer->nLife,pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z, pPlayer->rot.x, pPlayer->rot.y, pPlayer->rot.z, fAngle);

    //テキストの描画
    g_pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//========================================
//敵の状態デバッグ表示
//========================================
void DrawEnemyState(void)
{
    Enemy *pEnemy = GetEnemy();
    RECT rect4 = { 0,350,SCREEN_WIDTH,SCREEN_HEIGHT };
    char aStr[256];

    sprintf(&aStr[0], "敵の体力：[%d]", pEnemy->nLife);

    //テキストの描画
    g_pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//========================================
//モードの設定
//========================================
void SetMode(MODE mode)
{
    //現在の画面(モード)の終了処理
    switch (g_mode)
    {
	case MODE_LOGO:
		UninitTeam_logo();									//ロゴ画面
		break;
	
	case MODE_TITLE:
        UninitTitle();										//タイトル画面
        break;

    case MODE_TUTORIAL:
        UninitTutorial();									//チュートリアル画面
        break;

    case MODE_GAME:
        UninitGame();										//ゲーム画面
        break;

    case MODE_RANKING:
        UninitRanking();									//ランキング画面
        break;
    }

    //a <-What?!
    g_mode = mode;

    //新しい画面(モード)の初期化処理
    switch (g_mode)
    {
	case MODE_LOGO:
		InitTeam_logo();									//ロゴ画面
		break;
	
	case MODE_TITLE:
        InitTitle();										//タイトル画面
        break;

    case MODE_TUTORIAL:
        InitTutorial();										//チュートリアル画面
        break;

    case MODE_GAME:
        InitGame();											//ゲーム画面
        break;

    case MODE_RANKING:
        InitRanking();											//ランキング画面
        break;
    }
}

//========================================
//モードの取得
//========================================
MODE GetMode(void)
{
    return g_mode;
}

//========================================
//フォントの取得
//========================================
LPD3DXFONT GetFont(void)
{
    return g_pFont;
}