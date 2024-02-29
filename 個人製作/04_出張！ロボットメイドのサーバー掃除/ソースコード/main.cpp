//===========================
//
//ダイレクトX.mainファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "manager.h"
#include "debugproc.h"

//マクロ定義
#define CLASS_NAME "WindowClass"												//ウィンドウクラスの名前
#define WINDOW_NAME "出張!ロボットメイドのサーバー掃除"												//ウィンドウの名前

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//グローバル変数宣言

//=====================================
//メイン関数
//=====================================
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hinstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	//メモリリークを出力
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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

	//マネージャーの生成初期化処理
	CManager *pManager = CManager::Get();

	if (pManager != NULL)
	{
#ifdef _DEBUG
		if (FAILED(pManager->Init(hinstance, hWnd, TRUE)))
		{
			return -1;
		}
#else
		if (FAILED(pManager->Init(hinstance, hWnd, FALSE)))
		{
			return -1;
		}
#endif //_DEBUG
	}
	else
	{
		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;								//初期化
	dwExecLastTime = timeGetTime();								//現在時刻を取得
	dwFrameCount = 0;
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
				pManager->SetFPS((dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime));

				dwFPSLastTime = dwCurrentTime;							//FPSを測定した時刻を保存
				dwFrameCount = 0;							//フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime;							//処理の開始を保存
				
				if (pManager != NULL)
				{
					//マネージャーの更新処理
					pManager->Update();

					//マネージャーの描画処理
					pManager->Draw();
				}

				dwFrameCount++;											//フレームカウントを加算
			}
		}
	}

	//マネージャーの破棄
	if (pManager != NULL)
	{
		pManager->Release();
		pManager = NULL;
	}

	//分解能を戻す
	timeEndPeriod(1);

	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=====================================
//ウィンドウプロシージャ
//=====================================
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

#ifdef _DEBUG
			//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
#else
			nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
#endif
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