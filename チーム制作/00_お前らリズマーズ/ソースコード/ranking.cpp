//===========================
//
//ダイレクトX.rankingファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "Ranking.h"
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "score.h"
#include <stdio.h>

//マクロ定義
#define NUM_PLACE_RANKING (8)											//スコアの最大数
#define MAX_RANKING (5)													//スコアのランク数

//ランキング構造体
typedef struct
{
	D3DXVECTOR3 pos;													//位置
	int nScore;															//スコア
}RANKING;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureRankingScore = NULL;						//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingScore = NULL;					//頂点情報を格納
RANKING g_aRanking[MAX_RANKING];										//ランキング情報
D3DXVECTOR3 g_PosRanking;												//ランキングの位置
int g_nRankUpdate = -1;													//ランキングの更新判定
int g_nTimerRanking;													//ランキングのタイトル遷移タイマー

//===========================
//スコアの初期化処理
//===========================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntRanking;
	int nScore = GetScore();

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number001.png",
		&g_pTextureRankingScore);

	g_PosRanking = D3DXVECTOR3(500.0f, 135.0f, 0.0f);
	g_nTimerRanking = 0;

	VERTEX_2D *pVtx;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE_RANKING * MAX_RANKING,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingScore,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		for (int nCntRankingScore = 0; nCntRankingScore < NUM_PLACE_RANKING; nCntRankingScore++)
		{
			//頂点座標の設定
			pVtx[0].pos.x = g_PosRanking.x + (20.0f * nCntRankingScore);
			pVtx[0].pos.y = g_PosRanking.y + (40.0f * nCntRanking) + (44.0f * nCntRanking);
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_PosRanking.x + (20.0f * nCntRankingScore) + 20.0f;
			pVtx[1].pos.y = g_PosRanking.y + (40.0f * nCntRanking) + (44.0f * nCntRanking);
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_PosRanking.x + (20.0f * nCntRankingScore);
			pVtx[2].pos.y = g_PosRanking.y + (40.0f * nCntRanking) + (44.0f * nCntRanking) + 40.0f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_PosRanking.x + (20.0f * nCntRankingScore) + 20.0f;
			pVtx[3].pos.y = g_PosRanking.y + (40.0f * nCntRanking) + (44.0f * nCntRanking) + 40.0f;
			pVtx[3].pos.z = 0.0f;

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
	}

	if (g_nRankUpdate != -1)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRankingScore->Unlock();

	SetRanking(nScore);

	PlaySound(SOUND_LABEL_BGM007);
}

//===========================
//スコアの終了処理
//===========================
void UninitRanking(void)
{
	g_nRankUpdate = -1;

	//サウンドの停止
	StopSound();

	//テクスチャの破棄
	if (g_pTextureRankingScore != NULL)
	{
		g_pTextureRankingScore->Release();
		g_pTextureRankingScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRankingScore != NULL)
	{
		g_pVtxBuffRankingScore->Release();
		g_pVtxBuffRankingScore = NULL;
	}
}

//===========================
//スコアの更新処理
//===========================
void UpdateRanking(void)
{
	FADE fade = GetFade();

	g_nTimerRanking++;

	int aTexU[MAX_RANKING][NUM_PLACE_RANKING];

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		aTexU[nCntRanking][0] = g_aRanking[nCntRanking].nScore % 100000000 / 10000000;
		aTexU[nCntRanking][1] = g_aRanking[nCntRanking].nScore % 10000000 / 1000000;
		aTexU[nCntRanking][2] = g_aRanking[nCntRanking].nScore % 1000000 / 100000;
		aTexU[nCntRanking][3] = g_aRanking[nCntRanking].nScore % 100000 / 10000;
		aTexU[nCntRanking][4] = g_aRanking[nCntRanking].nScore % 10000 / 1000;
		aTexU[nCntRanking][5] = g_aRanking[nCntRanking].nScore % 1000 / 100;
		aTexU[nCntRanking][6] = g_aRanking[nCntRanking].nScore % 100 / 10;
		aTexU[nCntRanking][7] = g_aRanking[nCntRanking].nScore % 10 / 1;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		for (int nCntRankingScore = 0; nCntRankingScore < NUM_PLACE_RANKING; nCntRankingScore++)
		{
			if (g_nRankUpdate != -1)
			{
				if (nCntRanking == g_nRankUpdate)
				{
					//頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				}
			}

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.1f * aTexU[nCntRanking][nCntRankingScore], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[nCntRanking][nCntRankingScore], 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aTexU[nCntRanking][nCntRankingScore], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[nCntRanking][nCntRankingScore], 1.0f);

			pVtx += 4;
		}
	}

	g_pVtxBuffRankingScore->Unlock();

	if (g_nTimerRanking == 1500 || (GetKeyboardTrigger(DIK_RETURN) == true && fade == FADE_NONE))
	{
		SetFade(MODE_TITLE);
	}
}

//===========================
//スコアの描画処理
//===========================
void DrawRanking(void)
{
	int nCntRanking;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//スコア
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffRankingScore,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRankingScore);

	for (nCntRanking = 0; nCntRanking < NUM_PLACE_RANKING * MAX_RANKING; nCntRanking++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRanking, 2);
	}
}

//===========================
//スコアの設定処理
//===========================
void SetRanking(int nRanking)
{
	int nMaxNumber, nTemp;

	//スコアのロード処理
	LoadData();

	if (g_aRanking[MAX_RANKING - 1].nScore <= nRanking)
	{//ランキング入れ替え
		g_aRanking[MAX_RANKING - 1].nScore = nRanking;
	}

	//降順挿入ソート
	for (int nCnt1 = 1; nCnt1 < MAX_RANKING; nCnt1++)
	{
		nMaxNumber = nCnt1;

		while (nMaxNumber > 0 && g_aRanking[nMaxNumber - 1].nScore < g_aRanking[nMaxNumber].nScore)
		{
			nTemp = g_aRanking[nMaxNumber - 1].nScore;
			g_aRanking[nMaxNumber - 1].nScore = g_aRanking[nMaxNumber].nScore;
			g_aRanking[nMaxNumber].nScore = nTemp;

			nMaxNumber--;
		}
	}

	//ランキング更新確認
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		if (g_aRanking[nCnt].nScore == nRanking)
		{
			g_nRankUpdate = nCnt;
			break;
		}
	}

	//スコアのセーブ処理
	SaveData();
}

//===========================
//スコアのリセット処理
//===========================
void ResetRanking(void)
{
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		g_aRanking[nCntRanking].pos.x = 460.0f;
		g_aRanking[nCntRanking].pos.y = 100.0f;
		g_aRanking[nCntRanking].pos.z = 0.0f;

		g_aRanking[nCntRanking].nScore = 0;
	}
}

//===========================
//スコアのロード処理
//===========================
void LoadData(void)
{
	FILE *pFile;

	//ファイルから問題を読み込み
	pFile = fopen("ranking.bin", "rb");

	if (pFile != NULL)
	{//開けた場合

		fread(&g_aRanking[0].nScore, sizeof(RANKING), MAX_RANKING, pFile);
		fclose(pFile);
	}
	else
	{//開けなかった場合

	 //ランキング初期化
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
		{
			g_aRanking[nCnt].nScore = 0;
		}
	}
}

//===========================
//スコアのセーブ処理
//===========================
void SaveData(void)
{
	FILE *pFile;

	//ファイルから問題を読み込み
	pFile = fopen("ranking.bin", "wb");

	if (pFile != NULL)
	{//開けた場合

		fwrite(&g_aRanking[0].nScore, sizeof(RANKING), MAX_RANKING, pFile);
		fclose(pFile);
	}
	else
	{//開けなかった場合

	}
}