//============================
//
//ランキング順位の処理(ranking_rank.cpp)
//
//Author 髙田 佳依
//
//============================
#include "main.h"
#include "ranking.h"
#include "ranking_rank.h"
#include <stdio.h>

//===========================
//マクロ定義
//===========================
#define RANK_W (80)						//順位のサイズ（横）
#define RANK_H (50)						//順位のサイズ（縦）
#define PASS_RANKING_RANK "data\\Texture\\rank000.png"	//テクスチャパス

//===========================
//グローバル変数宣言
//===========================
LPDIRECT3DTEXTURE9 g_pTextureRankingRank = NULL;				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingRank = NULL;			//頂点バッファへのポインタ
D3DXVECTOR3 g_Rankpos[NUM_RANKING];

//===========================
//ランキングの初期化処理
//===========================
void InitRankingRank(void)
{
	for (int nCnt = 0; nCnt < NUM_RANKING; nCnt++)
	{
		g_Rankpos[nCnt] = D3DXVECTOR3(SCREEN_WIDTH * 0.40f, (SCREEN_HEIGHT / 8.0f) *  (nCnt + 2), 0.0f);
	}

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		PASS_RANKING_RANK,							//テクスチャのファイル名
		&g_pTextureRankingRank);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_RANKING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingRank,
		NULL);

	VERTEX_2D*pVtx;
	
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRankingRank->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntInit = 0; nCntInit < NUM_RANKING; nCntInit++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Rankpos[nCntInit].x - RANK_W, g_Rankpos[nCntInit].y - RANK_H + (RANK_HEI * nCntInit), 0.0f);	//(x,y,z)
		pVtx[1].pos = D3DXVECTOR3(g_Rankpos[nCntInit].x + RANK_W, g_Rankpos[nCntInit].y - RANK_H + (RANK_HEI * nCntInit), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Rankpos[nCntInit].x - RANK_W, g_Rankpos[nCntInit].y + RANK_H + (RANK_HEI * nCntInit), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Rankpos[nCntInit].x + RANK_W, g_Rankpos[nCntInit].y + RANK_H + (RANK_HEI * nCntInit), 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(nCntInit * 0.2f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((nCntInit * 0.2f) + 0.2f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nCntInit * 0.2f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((nCntInit * 0.2f) + 0.2f, 1.0f);

		pVtx += 4;
	}
	
	//頂点バッファをアンロックする
	g_pVtxBuffRankingRank->Unlock();
}

//===========================
//ランキングの終了処理
//===========================
void UninitRankingRank(void)
{
	if (g_pTextureRankingRank != NULL)
	{
		g_pTextureRankingRank->Release();
		g_pTextureRankingRank = NULL;					//テクスチャ破棄
	}

	if (g_pVtxBuffRankingRank != NULL)
	{
		g_pVtxBuffRankingRank->Release();
		g_pVtxBuffRankingRank = NULL;					//バッファの破棄
	}
}

//===========================
//ランキングの更新処理
//===========================
void UpdateRankingRank(void)
{

}

//===========================
//ランキングの描画処理
//===========================
void DrawRankingRank(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankingRank, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRankingRank);

	for (int nCntDraw = 0; nCntDraw < NUM_RANKING; nCntDraw++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
			nCntDraw * 4,									//描画する最初のインデックス
			2);												//描画するプリミティブ数
	}
}

//===========================
//ランキングの描画処理
//===========================
D3DXVECTOR3 *GetRankPos()
{
	return &g_Rankpos[0];
}
