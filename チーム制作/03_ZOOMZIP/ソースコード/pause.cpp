//==========================================================
//
//ポーズ描画処理
//Author 奥定伊吹
//
//==========================================================
#include "pause.h"
#include "input.h"
#include "fade.h"
#include "game.h"

//マクロ定義
#define NUM_PAUSE	(4)										//ポーズ画面テクスチャの種類
#define BUTTON_PAUSE	(3)									//ポーズボタンの数
#define MAX_PAUSE	(5)										//ポーズ画面に描画するポリゴン数
#define PAUSE_BGDATA		"data\\TEXTURE\\pausebg000.png"	//ポーズ背景テクスチャ
#define PAUSE_CONTINUEDATA	"data\\TEXTURE\\pause003.png"	//ゲーム画面に戻るテクスチャ
#define PAUSE_RETRYDATA		"data\\TEXTURE\\pause004.png"	//ゲームをやり直すテクスチャ
#define PAUSE_QUITDATA		"data\\TEXTURE\\pause005.png"	//タイトルに戻るテクスチャ
#define PAUSE_BUTTONWIDTH	(120.0f)						//ポーズ画面ボタン幅
#define PAUSE_BUTTONHEIGHT	(40.0f)							//ポーズ画面ボタン高さ
#define PAUSE_BUTTONX		(640.0f)						//ポーズ画面X座標
#define PAUSE_BUTTONY		(280.0f)						//ポーズ画面Y座標
#define PAUSE_BUTTONSPACE	(90.0f)							//ポーズ画面縦スペース
#define PAUSE_MOVECOLA		(-0.02f)						//毎フレーム変わるボタンの透明度
#define PAUSE_BGX			(640.0f)						//背景x座標
#define PAUSE_BGY			(360.0f)						//背景y座標
#define PAUSE_BGSIZE		(200.0f)						//サイズ

//ポーズ画面必要物列挙型
typedef enum
{
	PAUSE_FADE = 0,		//背後のポーズ
	PAUSE_BG,			//背後の背景
	PAUSE_CONTINUE,		//ゲーム画面へ戻る
	PAUSE_RETRY,		//ゲームをやり直す
	PAUSE_QUIT,			//タイトル画面へ戻る
	PAUSE_MAX
}PAUSE;

//プロトタイプ宣言
void SetPauseScreen(int nCntpause);

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTexturePause[NUM_PAUSE] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;			//頂点バッファへのポインタ
int g_pauseMenu;									//ポーズメニュー
D3DXCOLOR g_ButtonColor[BUTTON_PAUSE];					//ボタンごとのカラー
bool g_bButtonPress;									//ポーズ画面で決定したかどうか
bool g_bButtonPause;									//ポーズ画面で選択を変更したかどうか
float g_bButtonCola;									//透明度を設定する

//ポーズの初期化処理
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//ポーズメニュー選択を初期化
	g_pauseMenu = PAUSEMENU_CONTINUE;
	g_bButtonPress = false;				//ボタン認識を初期化
	g_bButtonPause = false;				//変更状態を初期化
	g_bButtonCola = PAUSE_MOVECOLA;

	//テクスチャの読み込み
	for (int nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		switch (nCntPause)
		{
		case 0:
			D3DXCreateTextureFromFile(pDevice, PAUSE_BGDATA, &g_pTexturePause[nCntPause]);
			break;
		case 1:
			D3DXCreateTextureFromFile(pDevice, PAUSE_CONTINUEDATA, &g_pTexturePause[nCntPause]);
			break;
		case 2:
			D3DXCreateTextureFromFile(pDevice, PAUSE_RETRYDATA, &g_pTexturePause[nCntPause]);
			break;
		case 3:
			D3DXCreateTextureFromFile(pDevice, PAUSE_QUITDATA, &g_pTexturePause[nCntPause]);
			break;
		}
	}

	//色の設定
	for (int nCntPause = 0; nCntPause < BUTTON_PAUSE; nCntPause++)
	{
		g_ButtonColor[nCntPause] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	//初期カラーを設定
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_PAUSE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	//各ポリゴンの初期化
	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		SetPauseScreen(nCntPause);
	}
}

//ポーズ画面に配置するポリゴンの初期化
void SetPauseScreen(int nCntPause)
{
	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	pVtx += nCntPause * 4;	//ポインタを進める

	switch (nCntPause)
	{
	case PAUSE_FADE:	//ポーズ
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		break;
	case PAUSE_BG:
		pVtx[0].pos = D3DXVECTOR3(PAUSE_BGX - PAUSE_BGSIZE, PAUSE_BGY - PAUSE_BGSIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(PAUSE_BGX + PAUSE_BGSIZE, PAUSE_BGY - PAUSE_BGSIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(PAUSE_BGX - PAUSE_BGSIZE, PAUSE_BGY + PAUSE_BGSIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(PAUSE_BGX + PAUSE_BGSIZE, PAUSE_BGY + PAUSE_BGSIZE, 0.0f);
		break;
	default:
		pVtx[0].pos = D3DXVECTOR3(PAUSE_BUTTONX - PAUSE_BUTTONWIDTH, PAUSE_BUTTONY - PAUSE_BUTTONHEIGHT + ((nCntPause - PAUSE_CONTINUE) * PAUSE_BUTTONSPACE), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(PAUSE_BUTTONX + PAUSE_BUTTONWIDTH, PAUSE_BUTTONY - PAUSE_BUTTONHEIGHT + ((nCntPause - PAUSE_CONTINUE) * PAUSE_BUTTONSPACE), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(PAUSE_BUTTONX - PAUSE_BUTTONWIDTH, PAUSE_BUTTONY + PAUSE_BUTTONHEIGHT + ((nCntPause - PAUSE_CONTINUE) * PAUSE_BUTTONSPACE), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(PAUSE_BUTTONX + PAUSE_BUTTONWIDTH, PAUSE_BUTTONY + PAUSE_BUTTONHEIGHT + ((nCntPause - PAUSE_CONTINUE) * PAUSE_BUTTONSPACE), 0.0f);
		break;
	}

	if (nCntPause == PAUSE_FADE)
	{
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	}
	else if (nCntPause == PAUSE_BG)
	{
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
	}
	else
	{
		pVtx[0].col = g_ButtonColor[nCntPause - PAUSE_CONTINUE];
		pVtx[1].col = g_ButtonColor[nCntPause - PAUSE_CONTINUE];
		pVtx[2].col = g_ButtonColor[nCntPause - PAUSE_CONTINUE];
		pVtx[3].col = g_ButtonColor[nCntPause - PAUSE_CONTINUE];
	}

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}

//ポーズの終了処理
void UninitPause(void)
{
	//テクスチャの廃棄
	for (int nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}
	//頂点バッファの廃棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//ポーズの更新処理
void UpdatePause(void)
{
	if (g_bButtonPress == true)
	{//押された状態の場合
		return;
	}

	if (g_bButtonPause == false)
	{
		for (int nCntPause = 0; nCntPause < BUTTON_PAUSE; nCntPause++)
		{
			if (g_pauseMenu == nCntPause)
			{
				g_ButtonColor[nCntPause] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				g_ButtonColor[nCntPause] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			}
		}
		g_bButtonPause = true;
	}

	g_ButtonColor[g_pauseMenu].a += g_bButtonCola;
	if (g_ButtonColor[g_pauseMenu].a >= 1.0f || g_ButtonColor[g_pauseMenu].a <= 0.0f)
	{
		g_bButtonCola *= -1.0f;
	}

	if (GetKeyboardRepeat(DIK_W) || GetKeyboardTrigger(DIK_W) || GetGamepadTrigger(BUTTON_UP, 0) == true)
	{//上移動キーが入力された

		g_pauseMenu = (g_pauseMenu - 1 + BUTTON_PAUSE) % BUTTON_PAUSE ;
		g_bButtonPause = false;
		g_bButtonCola = PAUSE_MOVECOLA;
	}
	else if (GetKeyboardRepeat(DIK_S) || GetKeyboardTrigger(DIK_S) || GetGamepadTrigger(BUTTON_DOWN, 0) == true)
	{//下移動キーが入力された
		
		g_pauseMenu = (g_pauseMenu + 1) % BUTTON_PAUSE;
		g_bButtonPause = false;
		g_bButtonCola = PAUSE_MOVECOLA;
	}
	else if (GetKeyboardTrigger(DIK_RETURN) || GetGamepadTrigger(BUTTON_A, 0) == true)
	{//決定キーが押された
		switch (g_pauseMenu)
		{
		case PAUSEMENU_CONTINUE:
			SetEnablePause(true);
			break;
		case PAUSEMENU_RETRY:
			SetFadeMode(FADEMODE_NORMAL);
			SetFade(MODE_GAME);
			break;
		case PAUSEMENU_QUIT:
			SetFadeMode(FADEMODE_NORMAL);
			SetFade(MODE_TITLE);
			break;
		}
		if (g_pauseMenu != PAUSEMENU_CONTINUE)
		{
			g_bButtonPress = true;		//押された状態に変更
		}
	}

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * PAUSE_CONTINUE;	//ポインタをボタンまで進めておく

	for (int nCntPause = 0; nCntPause < BUTTON_PAUSE; nCntPause++)
	{
		pVtx[0].col = g_ButtonColor[nCntPause];
		pVtx[1].col = g_ButtonColor[nCntPause];
		pVtx[2].col = g_ButtonColor[nCntPause];
		pVtx[3].col = g_ButtonColor[nCntPause];

		pVtx += 4;	//ポインタをボタンまで進めておく
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}

//ポーズの描画処理
void DrawPause(void)
{

	PDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffPause,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		if (nCntPause == PAUSE_FADE)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, NULL);
		}
		else
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePause[nCntPause - 1]);
		}

		//ポーズの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCntPause * 4,
			2	//頂点情報構造体のサイズ
		);
	}
}