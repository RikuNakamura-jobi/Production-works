//===========================
//
//ダイレクトX.bgrankingファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "bgranking.h"

//マクロ定義
#define NUM_BGRANKING (3)														//背景の数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureBgRanking[NUM_BGRANKING] = {};					//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffBgRanking = NULL;								//頂点情報を格納
float g_aTexVRanking[NUM_BGRANKING];											//テクスチャ座標の開始位置

//===========================
//背景の初期化処理
//===========================
void InitBgRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBgRanking;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bgranking000.png",
		&g_apTextureBgRanking[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bgranking000.png",
		&g_apTextureBgRanking[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bgranking000.png",
		&g_apTextureBgRanking[2]);

	//テクスチャ座標の開始位置の初期化
	for (nCntBgRanking = 0; nCntBgRanking < NUM_BGRANKING; nCntBgRanking++)
	{
		g_aTexVRanking[nCntBgRanking] = 0.9f;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BGRANKING,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffBgRanking,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffBgRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBgRanking = 0; nCntBgRanking < NUM_BGRANKING; nCntBgRanking++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
		pVtx[1].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
		pVtx[2].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
		pVtx[3].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_VtxBuffBgRanking->Unlock();
}

//===========================
//背景の終了処理
//===========================
void UninitBgRanking(void)
{
	int nCntBgRanking;

	for (nCntBgRanking = 0; nCntBgRanking < NUM_BGRANKING; nCntBgRanking++)
	{
		//テクスチャの破棄
		if (g_apTextureBgRanking[nCntBgRanking] != NULL)
		{
			g_apTextureBgRanking[nCntBgRanking]->Release();
			g_apTextureBgRanking[nCntBgRanking] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_VtxBuffBgRanking != NULL)
	{
		g_VtxBuffBgRanking->Release();
		g_VtxBuffBgRanking = NULL;
	}
}

//===========================
//背景の更新処理
//===========================
void UpdateBgRanking(void)
{
	int nCntBgRanking;
	
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffBgRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBgRanking = 0; nCntBgRanking < NUM_BGRANKING; nCntBgRanking++)
	{
		//テクスチャ座標の開始位置の更新
		g_aTexVRanking[nCntBgRanking] -= 0.005f * nCntBgRanking + 0.002f;

		if (g_apTextureBgRanking[nCntBgRanking] <= 0)
		{
			g_aTexVRanking[nCntBgRanking] = 0.9f;
		}

		//テクスチャ座標の更新
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_VtxBuffBgRanking->Unlock();
}

//===========================
//背景の描画処理
//===========================
void DrawBgRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBgRanking;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_VtxBuffBgRanking,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBgRanking = 0; nCntBgRanking < NUM_BGRANKING; nCntBgRanking++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureBgRanking[nCntBgRanking]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBgRanking * 4, 2);
	}
}