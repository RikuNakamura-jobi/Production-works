//============================
//
//ランキング処理(ranking.h)
//Author:早川 友貴
//
//============================
#include "result.h"
#include "main.h"
#include "game.h"
#include "sound.h"
#include "input.h"
#include "fade.h"
#include "time.h"
#include "ranking.h"
#include <stdio.h>
#include <stdlib.h>

//マクロ
#define HI_DATA (5)		//回す数
#define ZERO_DATA  (0)  //何もないデータ
#define MAX_DATA   (5)  //最大数
#define ONE_DATA  (1)  //1つ足す時に使う
#define MAXONE_DATA (6) //最大数より1大きいデータ
#define X_DATA  (500.0f)  //x軸のデータ
#define Y_DATA  (160.0f)  //y軸のデータ
#define SPACE_DATA (120.0f)  //スコアの間隔のデータ
#define NEWRANK_DATA (50.0f)  //新記録のY幅
#define NUM_RANKING  (2)	//テクスチャの数
#define CNT_DATA	 (300)	//遷移までの時間を数えるデータ

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureRanking[NUM_RANKING] = {};		//テクスチャ(2枚分)へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;  //頂点バッファへのポインタ
int g_RankingTime;		 //スコアを格納する
FILE *pnFile;
int g_aData[MAX_DATA];  //データの数
int g_Rank;			//プレイヤーのスコアランキングを覚える
bool g_Ranking;
int g_TranceCnt;
bool g_Keychack;

//======================
//ランキング画面の初期化処理
//======================
void InitRanking(void)
{
	int nCnt;

	int nTemp;				//比較に使用

	int nCount1, nCount2;  //要素1,要素2

	LPDIRECT3DDEVICE9 pDevice;

	//覚えるランキングを初期化
	g_Rank = -1;

	//ランキングが更新されたか更新
	g_Ranking = false;

	//自動で遷移
	g_TranceCnt = 0;

	//キーチェックを初期化
	g_Keychack = false;

	//スコアの数値をランキングで受け取る処理
	g_RankingTime = GetTime();

	////タイムの初期化
	InitTime();

	//ロード処理
	LoadData();

	//タイムの描画
	for (nCnt = 0; nCnt < HI_DATA; nCnt++)
	{
		//タイムの座標を設定
		SetPosTime(D3DXVECTOR3(X_DATA, Y_DATA + (nCnt * SPACE_DATA), 0.0f), (100.0f), (1000.0f));

		SetTime(g_aData[nCnt]);

		//タイムの描画
		DrawTime();
	}

	//ソートの降順処理
	if (g_aData[HI_DATA - 1] < g_RankingTime)
	{
		g_aData[HI_DATA - 1] = g_RankingTime;
	}

	//ソートの降順処理
	for (nCount1 = ZERO_DATA; nCount1 < HI_DATA - 1; nCount1++)
	{
		for (nCount2 = nCount1 + ONE_DATA; nCount2 < MAX_DATA; nCount2++)
		{
			if (g_aData[nCount1] < g_aData[nCount2])
			{
				//1度別の変数にデータを確保してから上書きする
				nTemp = g_aData[nCount1];
				g_aData[nCount1] = g_aData[nCount2];
				g_aData[nCount2] = nTemp;
			}
		}
	}

	for (nCount1 = ZERO_DATA; nCount1 < HI_DATA; nCount1++)
	{
		if (g_RankingTime == g_aData[nCount1])
		{
			g_Rank = nCount1;

			g_Ranking = true;

			break;
		}
	}

	//セーブデータ処理
	SaveData();

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ranking.png",
		&g_apTextureRanking[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Yourtime.png",
		&g_apTextureRanking[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_RANKING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	VERTEX_2D *pVtx;    //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (nCnt = 0; nCnt < NUM_RANKING; nCnt++)
	{
		if (nCnt == 0)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);
		}

		if (nCnt == 1)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(1000.0f, Y_DATA - (NEWRANK_DATA) + (g_Rank * SPACE_DATA), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1200.0f, Y_DATA - (NEWRANK_DATA) + (g_Rank * SPACE_DATA), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(1000.0f, Y_DATA + (NEWRANK_DATA) + (g_Rank * SPACE_DATA), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1200.0f, Y_DATA + (NEWRANK_DATA) + (g_Rank * SPACE_DATA), 0.0f);
		}
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM005);
}

//============================
//ランキングの終了処理
//============================
void UninitRanking(void)
{
	int nCnt;

	//サウンドの停止
	StopSound();

	//スコアの終了
	UninitTime();

	for (nCnt = 0; nCnt < NUM_RANKING; nCnt++)
	{
		if (g_apTextureRanking[nCnt] != NULL)
		{
			g_apTextureRanking[nCnt]->Release();
			g_apTextureRanking[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

//===============================
//ランキング画面の更新処理
//===============================
void UpdateRanking(void)
{
	/*int nCnt;*/

	FADE pFade;
	pFade = GetFade();

	g_TranceCnt++;

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetPadButtonTrigger(0) == true || g_TranceCnt >= CNT_DATA)
	{//決定(ENTERキー)が押された
		if (g_Keychack == false)
		{
			//モード設定(ゲーム画面に移行)

			//サウンドの再生
			//PlaySound(SOUND_LABEL_SE_ENTER);

			g_Keychack = true;

			SetFade(MODE_TITLE);
		}
	}

}

//=================================
//ランキング画面の描画処理
//=================================
void DrawRanking(void)
{
	int nCnt;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCnt = 0; nCnt < NUM_RANKING; nCnt++)
	{
		if (nCnt == 0 && g_Ranking == false)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureRanking[nCnt]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2); //プリミティブの種類
			break;
		}
		else
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureRanking[nCnt]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2); //プリミティブの種類
		}
	}

	//タイムの描画
	for (nCnt = 0; nCnt < HI_DATA; nCnt++)
	{
		//スコアの座標を設定
		SetPosTime(D3DXVECTOR3(X_DATA, Y_DATA + (nCnt * SPACE_DATA), 0.0f), (50.0f), (50.0f));

		SetTime(g_aData[nCnt]);

		//スコアの描画
		DrawTime();

		SetTime(0);
	}
}

//================================
//セーブデータ処理
//================================
void SaveData(void)
{
	int nCount;

	//ファイルを開く
	pnFile = fopen("data\\TEXTURE\\time.txt", "w");

	if (pnFile != NULL)
	{//ファイルが開けた場合
	 //文字列をファイルに書き込む
		for (nCount = ZERO_DATA; nCount < MAX_DATA; nCount++)
		{
			fprintf(pnFile, "%d\n", g_aData[nCount]);
		}

		//ファイルを閉じる
		fclose(pnFile);
	}

	else
	{//ファイルが読み込めなかった場合
		g_RankingTime = 0;                 //ファイルを読み込めなかったことを表示
	}
}

//================================
//ロード処理
//================================
void LoadData(void)
{
	int nCount;

	//ファイルを開く
	pnFile = fopen("data\\TEXTURE\\time.txt", "r");

	if (pnFile != NULL)
	{//ファイルが開けた場合

		for (nCount = ZERO_DATA; nCount < HI_DATA; nCount++)
		{
			fscanf(pnFile, "%d", &g_aData[nCount]);
		}

		fclose(pnFile);
	}

	else
	{//ファイルが読み込めなかった場合
		printf("***ファイルを読み込めませんでした***\n\n");                  //ファイルを読み込めなかったことを表示
	}
}

//=================================================
//ランキングが更新されたかどうかをリセットする処理
//=================================================
void ResetData(void)
{
	//ランキングが更新されたか更新
	g_Ranking = false;
}


