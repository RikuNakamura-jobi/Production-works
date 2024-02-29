//========================================
//
//2Dシューティングゲーム(ポーズ処理)[pause.cpp]
//Author:佐久間優香
//
//========================================
#include "main.h"
#include "input.h"
#include "pause.h"
#include "game.h"
#include "fade.h"
#include "option.h"
#include "pause_frame.h"
#include "sound.h"

//==========================================
//  ポリゴン状態列挙型定義  kanazaki
//==========================================
typedef enum
{
	POLYGONSTATE_NONE = 0,
	POLYGONSTATE_GRAW,
	POLYGONSTATE_DECLINE,
	POLYGONSTATE_MAX
}POLYGONSTATE;

//==========================================
//  マクロ定義
//  tomoya kanazaki
//==========================================
#define PASUE_BASE_POS (D3DXVECTOR3(640.0f, 325.0f, 0.0f)) //ポリゴンの基準位置
#define PAUSE_SIZE (D3DXVECTOR3(190.0f, 45.0f, 0.0f)) //ポリゴンの基準サイズ
#define PAUSE_SPACE (130.0f) //ポリゴン同士の間隔
#define PAUSE_SELECT_WIDTH (PAUSE_SIZE.x * 1.1f) //選択ポリゴンの幅
#define PAUSE_SELECT_HEIGHT (PAUSE_SIZE.y * 1.1f) //選択ポリゴンの高さ
#define PAUSE_NONE_WIDTH (PAUSE_SIZE.x * 0.9f) //非選択ポリゴンの幅
#define PAUSE_NONE_HEIGHT (PAUSE_SIZE.y * 0.9f) //非選択ポリゴンの高さ
#define PAUSE_BITWEEN (0.01f) //ポリゴンの拡縮率
#define PAUSE_GRAW_WIDTH (PAUSE_SELECT_WIDTH * 1.1f) //決定項目の幅
#define PAUSE_GRAW_HEIGHT (PAUSE_SELECT_HEIGHT * 1.1f) //決定項目の高さ
#define PAUSE_GRAW (0.02f) //ポリゴンの成長率

//==========================================
//  ポーズ構造体定義  kanazaki
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //中心座標
	float fWidth; //ポリゴンの幅
	float fHeight; //ポリゴンの高さ
	D3DXCOLOR col; //頂点カラー
	POLYGONSTATE graw;
}PAUSE;

//ポーズのファイル名を保持
const char *c_apFilenamePause[] =
{
	"data\\TEXTURE\\continue000.png",
	"data\\TEXTURE\\restart000.png",
	"data\\TEXTURE\\title003.png",
};

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTexturePause[PAUSE_MENU_MAX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;			//頂点バッファへのポインタ
PAUSE_MENU g_PauseMenu;									//ポーズメニュー
PAUSE g_Pause[PAUSE_MENU_MAX];		//ポーズ構造体
int aData[PAUSE_MENU_MAX];

//========================================
//ポーズの初期化処理
//========================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	VERTEX_2D *pVtx;									//頂点情報へのポインタ
	int nCntPause;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < PAUSE_MENU_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile
		(
			pDevice,
			c_apFilenamePause[nCntTex],
			&g_apTexturePause[nCntTex]
		);
	}

	//変数の初期化 kanazaki
	g_PauseMenu = PAUSE_MENU_CONTENUE;
	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		//ポリゴン座標を設定
		g_Pause[nCntPause].pos = PASUE_BASE_POS;
		g_Pause[nCntPause].pos.y += PAUSE_SPACE * nCntPause;

		//選択/非選択の個別設定
		if (nCntPause == g_PauseMenu) //選択
		{
			//ポリゴンを大きいサイズに設定
			g_Pause[nCntPause].fWidth = PAUSE_SELECT_WIDTH;
			g_Pause[nCntPause].fHeight = PAUSE_SELECT_HEIGHT;

			//不透明に設定
			g_Pause[nCntPause].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else //非選択
		{
			//ポリゴンを小さいサイズに設定
			g_Pause[nCntPause].fWidth = PAUSE_NONE_WIDTH;
			g_Pause[nCntPause].fHeight = PAUSE_NONE_HEIGHT;

			//半透明に設定
			g_Pause[nCntPause].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		}
		g_Pause[nCntPause].graw = POLYGONSTATE_NONE;

		//sakuma
		aData[nCntPause] = 0;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_MENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Pause[nCntPause].pos.x - g_Pause[nCntPause].fWidth, g_Pause[nCntPause].pos.y - g_Pause[nCntPause].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Pause[nCntPause].pos.x + g_Pause[nCntPause].fWidth, g_Pause[nCntPause].pos.y - g_Pause[nCntPause].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Pause[nCntPause].pos.x - g_Pause[nCntPause].fWidth, g_Pause[nCntPause].pos.y + g_Pause[nCntPause].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Pause[nCntPause].pos.x + g_Pause[nCntPause].fWidth, g_Pause[nCntPause].pos.y + g_Pause[nCntPause].fHeight, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_Pause[nCntPause].col;
		pVtx[1].col = g_Pause[nCntPause].col;
		pVtx[2].col = g_Pause[nCntPause].col;
		pVtx[3].col = g_Pause[nCntPause].col;

		//テクスチャの座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;								//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

	//オプション機能
	InitOption();

	//ポーズの枠の初期化処理
	InitPauseFrame();
}

//========================================
//ポーズの終了処理
//========================================
void UninitPause(void)
{
	int nCntPause;

	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		//テクスチャの破棄
		if (g_apTexturePause[nCntPause] != NULL)
		{
			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}

	}
	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

	//オプション機能
	UninitOption();

	//ポーズの枠の終了処理
	UninitPauseFrame();
}

//========================================
//ポーズの更新処理
//========================================
void UpdatePause(void)
{
	VERTEX_2D *pVtx;									//頂点情報へのポインタ
	int nCntPause;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		if (g_Pause[nCntPause].graw == POLYGONSTATE_NONE)
		{
			if (nCntPause == g_PauseMenu) //選択
			{
				//ポリゴンを大きいサイズに設定
				if (g_Pause[nCntPause].fWidth < PAUSE_SELECT_WIDTH)
				{
					g_Pause[nCntPause].fWidth += g_Pause[nCntPause].fWidth * PAUSE_BITWEEN;
				}
				if (g_Pause[nCntPause].fHeight < PAUSE_SELECT_HEIGHT)
				{
					g_Pause[nCntPause].fHeight += g_Pause[nCntPause].fHeight * PAUSE_BITWEEN;
				}

				//不透明に設定
				g_Pause[nCntPause].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else //非選択
			{
				//ポリゴンを小さいサイズに設定
				if (g_Pause[nCntPause].fWidth > PAUSE_NONE_WIDTH)
				{
					g_Pause[nCntPause].fWidth -= g_Pause[nCntPause].fWidth * PAUSE_BITWEEN;
				}
				if (g_Pause[nCntPause].fHeight > PAUSE_NONE_HEIGHT)
				{
					g_Pause[nCntPause].fHeight -= g_Pause[nCntPause].fHeight * PAUSE_BITWEEN;
				}

				//半透明に設定
				g_Pause[nCntPause].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
			}
		}

		if (GetKeyboardTrigger(DIK_P) == true)
		{
			g_PauseMenu = PAUSE_MENU_CONTENUE;
			aData[nCntPause] = 0;
		}

		if (GetFade() == FADE_NONE)
		{
			//キーが押されたとき
			if (GetKeyboardTrigger(DIK_W) || GetStickTriggerL(STICK_UP, 0) || GetJoyPadCrossTrigger(CROSS_UP, 0))
			{//Wキーが押された

				if (aData[nCntPause] >= 1)
				{
					aData[nCntPause]--;
				}
				else if (aData[nCntPause] == 0)
				{
					aData[nCntPause] = 2;
				}

				PlaySound(SOUND_LABEL_SE_BUTTON_003);
			}
			if (GetKeyboardTrigger(DIK_S) || GetStickTriggerL(STICK_DOWN, 0) || GetJoyPadCrossTrigger(CROSS_DOWN, 0))
			{//Sキーが押された

				if (aData[nCntPause] == 2)
				{
					aData[nCntPause] = 0;
				}
				else if (aData[nCntPause] >= 0)
				{
					aData[nCntPause]++;
				}
				PlaySound(SOUND_LABEL_SE_BUTTON_003);
			}
		}

		switch (aData[nCntPause])
		{
		case PAUSE_MENU_CONTENUE:
			g_PauseMenu = PAUSE_MENU_CONTENUE;
			break;

		case PAUSE_MENU_RETRY:
			g_PauseMenu = PAUSE_MENU_RETRY;
			break;

		case PAUSE_MENU_QUIT:
			g_PauseMenu = PAUSE_MENU_QUIT;
			break;
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Pause[nCntPause].pos.x - g_Pause[nCntPause].fWidth, g_Pause[nCntPause].pos.y - g_Pause[nCntPause].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Pause[nCntPause].pos.x + g_Pause[nCntPause].fWidth, g_Pause[nCntPause].pos.y - g_Pause[nCntPause].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Pause[nCntPause].pos.x - g_Pause[nCntPause].fWidth, g_Pause[nCntPause].pos.y + g_Pause[nCntPause].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Pause[nCntPause].pos.x + g_Pause[nCntPause].fWidth, g_Pause[nCntPause].pos.y + g_Pause[nCntPause].fHeight, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = g_Pause[nCntPause].col;
		pVtx[1].col = g_Pause[nCntPause].col;
		pVtx[2].col = g_Pause[nCntPause].col;
		pVtx[3].col = g_Pause[nCntPause].col;

		pVtx += 4;								//頂点データのポインタを4つ分進める
	}

	//決定された項目を拡大
	if (g_Pause[g_PauseMenu].graw == POLYGONSTATE_GRAW)
	{
		if (g_Pause[g_PauseMenu].fWidth < PAUSE_GRAW_WIDTH && g_Pause[g_PauseMenu].fHeight < PAUSE_GRAW_HEIGHT)
		{
			g_Pause[g_PauseMenu].fWidth += g_Pause[g_PauseMenu].fWidth * PAUSE_GRAW;
			g_Pause[g_PauseMenu].fHeight += g_Pause[g_PauseMenu].fHeight * PAUSE_GRAW;
		}
		else
		{
			g_Pause[g_PauseMenu].graw = POLYGONSTATE_DECLINE;
		}
	}

	//決定された項目を縮小
	if (g_Pause[g_PauseMenu].graw == POLYGONSTATE_DECLINE)
	{
		if (g_Pause[g_PauseMenu].fWidth > PAUSE_SELECT_WIDTH)
		{
			g_Pause[g_PauseMenu].fWidth -= g_Pause[g_PauseMenu].fWidth * PAUSE_GRAW;
		}
		if (g_Pause[g_PauseMenu].fHeight > PAUSE_SELECT_HEIGHT)
		{
			g_Pause[g_PauseMenu].fHeight -= g_Pause[g_PauseMenu].fHeight * PAUSE_GRAW;
		}
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	if (GetFade() == FADE_NONE)
	{//フェードしていないとき
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) || GetJoyPadButtonTrigger(KEY_A, 0) || GetMouseTrigger(MOUSEBUTTON_LEFT))
		{//決定キー(ENTERキー)が押されたとき

			PlaySound(SOUND_LABEL_SE_BUTTON_001);

			switch (g_PauseMenu)
			{
			case PAUSE_MENU_CONTENUE:
				SetEnablePause(false);
				break;

			case PAUSE_MENU_RETRY:
				if (GetMode() == MODE_GAME)
				{
					SetFade(MODE_GAME);
				}
				if (GetMode() == MODE_TUTORIAL)
				{
					SetFade(MODE_TUTORIAL);
				}
				break;

			case PAUSE_MENU_QUIT:
				// モード設定(リザルト画面に移行)
				SetFade(MODE_TITLE);
				break;
			}

			//ポリゴンの拡縮を設定
			if (g_PauseMenu != PAUSE_MENU_CONTENUE)
			{
				g_Pause[g_PauseMenu].graw = POLYGONSTATE_GRAW;
			}
		}
	}

	//オプション機能
	UpdateOption();

	//ポーズの枠の更新処理
	UpdatePauseFrame();

}

//========================================
//ポーズの描画処理
//========================================
void DrawPause(void)
{
	//ポーズの枠の描画処理
	DrawPauseFrame();

	int nCntPause;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTexturePause[nCntPause]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}

	//オプション機能
	DrawOption();
}
