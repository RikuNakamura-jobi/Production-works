//===========================
//
//ダイレクトX.titleファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "title.h"
#include "sound.h"
#include "input.h"
#include "fade.h"

//マクロ定義
#define ENTER_POS (100)											//タイトル画面のENTERの位置

//列挙型
typedef enum
{
	TITLE_BG = 0,
	TITLE_ENTER,
	TITLE_MAX,
}TITLE;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureTitle[TITLE_MAX] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffTitle = NULL;					//頂点情報を格納
int g_nCounterTitleState;										//状態管理カウンター
int nTimerRanking;												//タイトルのランキング遷移タイマー
float g_fCntTitle = 0.0f;										//タイトルの点滅用変数
bool g_TitleEnd;												//終了判定

//===========================
//タイトル画面の初期化処理
//===========================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTitle;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\title_all.png",
		&g_apTextureTitle[TITLE_BG]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\title004.png",
		&g_apTextureTitle[TITLE_ENTER]);

	g_nCounterTitleState = 120;
	g_fCntTitle = 0.0f;
	nTimerRanking = 0;
	g_TitleEnd = false;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLE_MAX,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffTitle,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{
		switch (nCntTitle)
		{
		case TITLE_BG:
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			break;
		
		case TITLE_ENTER:
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, ENTER_POS, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, ENTER_POS, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT + ENTER_POS, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT + ENTER_POS, 0.0f);
			break;
		}
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_VtxBuffTitle->Unlock();

	PlaySound(SOUND_LABEL_BGM010);
}

//===========================
//タイトル画面の終了処理
//===========================
void UninitTitle(void)
{
	int nCntTitle;

	//サウンドの停止
	StopSound();

	for (nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{
		//テクスチャの破棄
		if (g_apTextureTitle[nCntTitle] != NULL)
		{
			g_apTextureTitle[nCntTitle]->Release();
			g_apTextureTitle[nCntTitle] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_VtxBuffTitle != NULL)
	{
		g_VtxBuffTitle->Release();
		g_VtxBuffTitle = NULL;
	}
}

//===========================
//タイトル画面の更新処理
//===========================
void UpdateTitle(void)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	FADE fade = GetFade();
	int nCntTitle;

	nTimerRanking++;

	if (g_TitleEnd == false)
	{
		col.a = (sinf(g_fCntTitle) + 1.0f) / 2;

		g_fCntTitle += 0.05f;
	}
	else
	{
		col.a = (sinf(g_fCntTitle) + 1.0f) / 2;

		g_fCntTitle += 0.5f;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{
		if (nCntTitle == TITLE_ENTER)
		{
			//頂点カラーの設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_VtxBuffTitle->Unlock();

	if (GetKeyboardTrigger(DIK_RETURN) == true && g_TitleEnd == false)
	{
		//サウンドの再生
		PlaySound(SOUND_LABEL_SE_DECISION001);

		g_TitleEnd = true;
	}

	if (g_TitleEnd == true)
	{
		g_nCounterTitleState--;

		if (g_nCounterTitleState <= 0 && fade == FADE_NONE)
		{
			//モード設定
			SetFade(MODE_TUTORIAL);
		}
	}
}

//===========================
//タイトル画面の描画処理
//===========================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTitle;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_VtxBuffTitle,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureTitle[nCntTitle]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
	}
}