//===========================
//
//ダイレクトX.stagestartファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "stagestart.h"
#include "game.h"

//マクロ定義
#define NUM_STAGESTART (4)												//ボス警告の数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureStagestart[NUM_STAGESTART] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffStagestart = NULL;						//頂点情報を格納
float g_aTexVStagestart[NUM_STAGESTART];								//テクスチャ座標の開始位置
float g_fCntStagestart;													//ステージ開始カウント
int g_nTimerStagestart;													//ステージ開始タイマー
bool g_bUseStagestart;													//ステージ開始描画判定

//===========================
//ボス警告の初期化処理
//===========================
void InitStagestart(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntStagestart;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\stagestart000.png",
		&g_apTextureStagestart[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\stagestart001.png",
		&g_apTextureStagestart[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\stagestart002.png",
		&g_apTextureStagestart[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\stagestart003.png",
		&g_apTextureStagestart[3]);

	//テクスチャ座標の開始位置の初期化
	for (nCntStagestart = 0; nCntStagestart < NUM_STAGESTART; nCntStagestart++)
	{
		g_aTexVStagestart[nCntStagestart] = 0.9f;
	}

	g_bUseStagestart = false;
	g_fCntStagestart = 0.0f;
	g_nTimerStagestart = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_STAGESTART,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffStagestart,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffStagestart->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStagestart = 0; nCntStagestart < NUM_STAGESTART; nCntStagestart++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(300.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(980.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(300.0f, 720.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(980.0f, 720.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_VtxBuffStagestart->Unlock();
}

//===========================
//ボス警告の終了処理
//===========================
void UninitStagestart(void)
{
	int nCntStagestart;

	for (nCntStagestart = 0; nCntStagestart < NUM_STAGESTART; nCntStagestart++)
	{
		//テクスチャの破棄
		if (g_apTextureStagestart[nCntStagestart] != NULL)
		{
			g_apTextureStagestart[nCntStagestart]->Release();
			g_apTextureStagestart[nCntStagestart] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_VtxBuffStagestart != NULL)
	{
		g_VtxBuffStagestart->Release();
		g_VtxBuffStagestart = NULL;
	}
}

//===========================
//ボス警告の更新処理
//===========================
void UpdateStagestart(void)
{
	int nCntStagestart;
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	if (g_bUseStagestart == true)
	{
		if (g_nTimerStagestart < 180.0f)
		{
			if (g_fCntStagestart < 1.0f)
			{
				col.a = g_fCntStagestart;
				g_fCntStagestart += 0.05f;
			}
			else if (g_fCntStagestart >= 1.0f)
			{
				col.a = 1.0f;
				g_fCntStagestart = 1.0f;
			}
		}
		else if (g_nTimerStagestart >= 180.0f)
		{
			if (g_fCntStagestart > 0.0f)
			{
				col.a = g_fCntStagestart;
				g_fCntStagestart -= 0.05f;
			}
			else if (g_fCntStagestart <= 0.0f)
			{
				col.a = 0.0f;
				g_fCntStagestart = 0.0f;
			}
		}

		g_nTimerStagestart++;
	}
	
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffStagestart->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStagestart = 0; nCntStagestart < NUM_STAGESTART; nCntStagestart++)
	{
		if (g_bUseStagestart == true)
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
	g_VtxBuffStagestart->Unlock();
}

//===========================
//ボス警告の描画処理
//===========================
void DrawStagestart(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	STAGESTATE stageState = GetStageState();

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_VtxBuffStagestart,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_bUseStagestart == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureStagestart[stageState - 1]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (stageState - 1) * 4, 2);
	}
}

//===========================
//ボス警告の描画処理
//===========================
void SetStagestart(void)
{
	g_bUseStagestart = g_bUseStagestart ? false : true;
}