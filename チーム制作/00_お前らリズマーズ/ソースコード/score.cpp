//===========================
//
//ダイレクトX.scoreファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "score.h"

//マクロ定義
#define NUM_PLACE (8)													//スコアの最大数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;								//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;							//頂点情報を格納
D3DXVECTOR3 g_posScore;													//位置
int g_nScore;															//スコアの値

//===========================
//スコアの初期化処理
//===========================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number001.png",
		&g_pTextureScore);

	g_posScore = D3DXVECTOR3(1000.0f, 50.0f, 0.0f);
	g_nScore = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		if (nCntScore == 7)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(500.0f, 260.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(780.0f, 260.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(500.0f, 500.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(780.0f, 500.0f, 0.0f);
		}
		else
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;										//頂点座標のポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//===========================
//スコアの終了処理
//===========================
void UninitScore(void)
{
	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//===========================
//スコアの更新処理
//===========================
void UpdateScore(void)
{
	
}

//===========================
//スコアの描画処理
//===========================
void DrawScore(void)
{
	int nCntScore;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffScore,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntScore, 2);
	}
}

//===========================
//スコアの設定処理
//===========================
void SetScore(int nScore)
{
	int nCntScore, aTexU[NUM_PLACE];

	g_nScore = nScore;
	
	aTexU[0] = g_nScore % 100000000 / 10000000;
	aTexU[1] = g_nScore % 10000000 / 1000000;
	aTexU[2] = g_nScore % 1000000 / 100000;
	aTexU[3] = g_nScore % 100000 / 10000;
	aTexU[4] = g_nScore % 10000 / 1000;
	aTexU[5] = g_nScore % 1000 / 100;
	aTexU[6] = g_nScore % 100 / 10;
	aTexU[7] = g_nScore % 10 / 1;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		if (nCntScore == 7)
		{
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.1f * aTexU[nCntScore], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[nCntScore], 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aTexU[nCntScore], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[nCntScore], 1.0f);
		}

		pVtx += 4;
	}

	g_pVtxBuffScore->Unlock();
}

//===========================
//スコアの追加処理
//===========================
void AddScore(int nValue)
{
	int aTexU[NUM_PLACE], nCntScore;

	g_nScore += nValue;

	if (g_nScore >= 100000000)
	{
		g_nScore = 99999999;
	}

	aTexU[0] = g_nScore % 100000000 / 10000000;
	aTexU[1] = g_nScore % 10000000 / 1000000;
	aTexU[2] = g_nScore % 1000000 / 100000;
	aTexU[3] = g_nScore % 100000 / 10000;
	aTexU[4] = g_nScore % 10000 / 1000;
	aTexU[5] = g_nScore % 1000 / 100;
	aTexU[6] = g_nScore % 100 / 10;
	aTexU[7] = g_nScore % 10 / 1;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * aTexU[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aTexU[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[nCntScore], 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffScore->Unlock();
}

//===========================
//スコアの取得処理
//===========================
int GetScore(void)
{
	return g_nScore;
}