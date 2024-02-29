//==========================================
//
//ランキングの処理(ranking.cpp)
//Author Kai Takada
//
//==========================================
#include "main.h"
#include "ranking.h"
#include "ranking_rank.h"
#include "ranking_bg.h"
#include "input.h"
#include "fade.h"
#include "score.h"
#include "sound.h"
#include "player.h"
#include <stdio.h>

//========================
//マクロ定義
//========================
#define NUM_DIGITS (5)		//表示する桁数
#define RANKING_FILE "data\\SAVE\\RANKING\\ranking.bin"		//ランキングに出すスコアのファイル
#define RANKING_FONTFILE "data\\Texture\\number001.png"		//数字フォントファイル
#define RANKING_ALPHAINTERVAL (25)		//アルファ値の更新頻度
#define RANKING_FRAME (2700)		//ランキング維持フレーム
#define RANK_SLD_OK (20.0f)		//スライド完了誤差
#define RANK_SLIDE_PER (0.05f)		//スライド慣性割合
#define RANK_RGBA_R (160)		//更新スコアのr色
#define RANK_RGBA_B (0)		//更新スコアのb色

//========================
//プロトタイプ宣言
//========================
void MoveRanking(void);

//========================
//ランキング構造体
//========================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posDest;	//予定位置
	int nScore = 0;		//スコア
	bool bArvl;		//動き終わったか
}Ranking;

//========================
//デバッグ用出力スコア配列
//========================
const int c_aDebugScore[NUM_RANKING] =
{
	12400,
	8600,
	7100,
	5200,
	1800
};

//========================
//グローバル変数宣言
//========================
LPDIRECT3DTEXTURE9 g_apTextureRanking = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;		//頂点バッファへのポインタ
Ranking g_aRanking[NUM_RANKING];		//ランキング情報
int g_nGetScore1;		//スコアの保存
int g_nRankUpdate = -1;		//更新ランクNo.
int g_nTimerRanking;		//ランキング画面表示タイマー
bool g_bFadeRanking;		//無限フェード防止
bool g_bMove;		//スコア移動完了
int g_nFadeCounterRank;			//フェードカウンター
int g_nRankR, g_nRankB;			//対象スコアのアルファ値
int nRankAlphaCnter;		//アルファ値のカウンター
int g_nNewScoreCnt;		//更新したスコアの順位の保存

//===========================
//ランキングの初期化処理
//===========================
void InitRanking(void)
{
	//その他ランキング系の初期化
	InitRankingRank();
	InitRankingBg();

	g_bMove = true;		//ランキングスコア移動が終わってない
	g_nFadeCounterRank = 0;		//フェードカウンターの初期化
	nRankAlphaCnter = 0;		//アルファカウンターの初期化
	g_nRankR = 255;			//r値の初期化
	g_nRankB = 255;			//b値の初期化
	D3DXVECTOR3 *pRankpos = GetRankPos();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		RANKING_FONTFILE,							//テクスチャのファイル名
		&g_apTextureRanking);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_RANKING * NUM_DIGITS,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	g_bFadeRanking = false;		//初期化

	VERTEX_2D*pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntY = 0; nCntY < NUM_RANKING; nCntY++)
	{
		g_aRanking[nCntY].bArvl = false;

		for (int nCntX = 0; nCntX < NUM_DIGITS; nCntX++)
		{
			g_aRanking[nCntY].posDest = D3DXVECTOR3((SCREEN_WIDTH * 0.4f) + (nCntX * RANK_WIDTH * 2),
				pRankpos[nCntY].y,
				0.0f);

			g_aRanking[nCntY].pos = g_aRanking[nCntY].posDest;
			g_aRanking[nCntY].pos.x = SCREEN_WIDTH + RANK_WIDTH;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aRanking[nCntY].pos.x - RANK_WIDTH, g_aRanking[nCntY].pos.y - RANK_HEIGHT, 0.0f);	//(x,y,z)
			pVtx[1].pos = D3DXVECTOR3(g_aRanking[nCntY].pos.x + RANK_WIDTH, g_aRanking[nCntY].pos.y - RANK_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aRanking[nCntY].pos.x - RANK_WIDTH, g_aRanking[nCntY].pos.y + RANK_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aRanking[nCntY].pos.x + RANK_WIDTH, g_aRanking[nCntY].pos.y + RANK_HEIGHT, 0.0f);

			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = pVtx[0].col;
			pVtx[2].col = pVtx[0].col;
			pVtx[3].col = pVtx[0].col;

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			if (g_nRankUpdate != -1)
			{//ランキングが更新された

			}
			pVtx += 4;
		}
		g_aRanking[nCntY].posDest.x = (SCREEN_WIDTH * 0.5f);
	}
	//頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();

	TexRanking();

	int aRankScore[NUM_RANKING];

#ifdef _DEBUG
	for (int nCntSave = 0; nCntSave < NUM_RANKING; nCntSave++)
	{
		aRankScore[nCntSave] = c_aDebugScore[nCntSave];
	}
#else
	for (int nCntSave = 0; nCntSave < NUM_RANKING; nCntSave++)
	{
		aRankScore[nCntSave] = g_aRanking[nCntSave].nScore;
	}
#endif

	FILE *pFile;									//ファイルポイントを宣言	

	pFile = fopen(RANKING_FILE, "wb");

	if (pFile != NULL)
	{//外部ファイルにランキング情報(score)を保存
		fwrite(&aRankScore[0], sizeof(int), NUM_RANKING, pFile);

		fclose(pFile);
	}

	//サウンド再生 =
	PlaySound(SOUND_LABEL_BGM003);
}

//===========================
//ランキングの終了処理
//===========================
void UninitRanking(void)
{
	//サウンドの停止
	StopSound();

	//リセット
	g_nRankUpdate = -1;

	if (g_apTextureRanking != NULL)
	{
		g_apTextureRanking->Release();
		g_apTextureRanking = NULL;					//テクスチャ破棄
	}

	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;					//バッファの破棄
	}

	//その他ランキング系の終了
	UninitRankingRank();
	UninitRankingBg();
}

//===========================
//ランキングの更新処理
//===========================
void UpdateRanking(void)
{
	D3DXVECTOR3 Rankpos;

	VERTEX_2D*pVtx;

	if (g_nRankUpdate != -1)
	{//ランキングが更新されたら
		if (nRankAlphaCnter % RANKING_ALPHAINTERVAL == 0)
		{//0.5秒消したら
			g_nRankR == 255 ? (g_nRankR = RANK_RGBA_R) : (g_nRankR = 255);
			g_nRankB = g_nRankB ? RANK_RGBA_B : 255;
			nRankAlphaCnter = 0;
		}

		nRankAlphaCnter++;
	}

	//スコアポリゴン移動処理
	MoveRanking();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	//位置更新
	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{
		Rankpos = g_aRanking[nCntRank].pos;
		
		for (int nCntAlpha = 0; nCntAlpha < NUM_DIGITS; nCntAlpha++)
		{
			Rankpos.x += RANK_WIDTH * 2;

			if (nCntRank == g_nNewScoreCnt)
			{
				//頂点カラーの設定
				for (int nCntCol = 0;nCntCol < 4;nCntCol++)
				{
					pVtx[nCntCol].col = D3DCOLOR_RGBA(g_nRankR, 255, g_nRankB, 255);
				}
			}

			pVtx[0].pos = D3DXVECTOR3(Rankpos.x - RANK_WIDTH, Rankpos.y - RANK_HEIGHT + (RANK_HEI * nCntRank), 0.0f);	//(x,y,z)
			pVtx[1].pos = D3DXVECTOR3(Rankpos.x + RANK_WIDTH, Rankpos.y - RANK_HEIGHT + (RANK_HEI * nCntRank), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(Rankpos.x - RANK_WIDTH, Rankpos.y + RANK_HEIGHT + (RANK_HEI * nCntRank), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(Rankpos.x + RANK_WIDTH, Rankpos.y + RANK_HEIGHT + (RANK_HEI * nCntRank), 0.0f);

			pVtx += 4;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();

	//スコア移動の完了
	if (GetFade() == FADE_NONE)
	{
		//モード設定
		if ((GetKeyboardTrigger(DIK_RETURN) ||
			GetKeyboardTrigger(DIK_SPACE) ||
			g_nFadeCounterRank >= RANKING_FRAME ||
			GetJoyPadButtonTrigger(KEY_A,0) ||
			GetJoyPadButtonTrigger(KEY_B, 0) ||
			GetJoyPadButtonTrigger(KEY_START, 0))
			&& g_bFadeRanking == false
			&& g_bMove == false)
		{//ENTERキーを押したときor指定フレーム数以上更新したとき
			SetFade(MODE_TITLE);
			g_bFadeRanking = true;
		}
		else if ((GetKeyboardTrigger(DIK_RETURN)||
			GetKeyboardTrigger(DIK_SPACE) ||
			GetJoyPadButtonTrigger(KEY_A, 0) ||
			GetJoyPadButtonTrigger(KEY_B, 0) ||
			GetJoyPadButtonTrigger(KEY_START, 0))
			&& g_bMove == true)
		{
			g_bMove = false;
		}

		if (g_bMove == false)
		{
			if (GetPlayer()->bEaster == true)
			{
				g_nFadeCounterRank++;
			}
			else
			{
				g_nFadeCounterRank += 3;
			}
		}
	}

	//その他ランキング系の更新
	UpdateRankingRank();
	UpdateRankingBg();
}

//===========================
//ランキングの描画処理
//===========================
void DrawRanking(void)
{
	//その他ランキング系の描画
	DrawRankingBg();
	DrawRankingRank();

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureRanking);
	
	for (int nCntDraw = 0; nCntDraw < NUM_DIGITS * NUM_RANKING; nCntDraw++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
			nCntDraw * 4,									//描画する最初のインデックス
			2);												//描画するプリミティブ数
	}
}

//==========================
//ランキングの設定処理(sort)
//==========================
void SetRanking(int nScore)
{
	g_nGetScore1 = nScore;

	if (g_nGetScore1 > g_aRanking[NUM_RANKING - 1].nScore)
	{
		g_aRanking[NUM_RANKING - 1].nScore = g_nGetScore1;

		for (int nCntMax = 0; nCntMax < NUM_RANKING; nCntMax++)
		{
			int nHidata = nCntMax;

			for (int nCntSort = nCntMax + 1; nCntSort < NUM_RANKING; nCntSort++)
			{
				if (g_aRanking[nHidata].nScore < g_aRanking[nCntSort].nScore)
				{
					nHidata = nCntSort;
				}
			}

			if (nHidata != nCntMax)
			{//選択した場所が変わっていたら
				int nTemp = g_aRanking[nCntMax].nScore;
				g_aRanking[nCntMax].nScore = g_aRanking[nHidata].nScore;
				g_aRanking[nHidata].nScore = nTemp;
			}
		}

		for (int nCntComp = 0; nCntComp < NUM_RANKING; nCntComp++)
		{
			if (g_aRanking[nCntComp].nScore == g_nGetScore1)
			{
				g_nRankUpdate = 0;
				g_nNewScoreCnt = nCntComp;
			}
		}
	}
}

//==========================
//ランキングのリセット処理(load)
//==========================
void ResetRanking(void)
{
	D3DXVECTOR3 *pRankpos = GetRankPos();
	int aRankScore[5] = {0,0,0,0,0};

	FILE *pFile;									//ファイルポイントを宣言

	pFile = fopen(RANKING_FILE, "rb");

	if (pFile != NULL)
	{//ファイルが開けたら
		fread(&aRankScore[0], sizeof(int), 5, pFile);
		
		for (int nCnt = 0; nCnt < NUM_RANKING; nCnt++)
		{
			g_aRanking[nCnt].nScore = aRankScore[nCnt];
		}

		fclose(pFile);
	}
	else
	{//開けなかったら
		for (int nCnt = 0;nCnt < NUM_RANKING;nCnt++)
		{
			g_aRanking[nCnt].nScore = 0;
		}
	}
	
	for (int nCnt = 0; nCnt < NUM_RANKING; nCnt++)
	{
		g_aRanking[nCnt].pos = D3DXVECTOR3(SCREEN_WIDTH, pRankpos[nCnt].y, 0.0f);
	}
}

//==========================
//ランキングのテクスチャの設定処理
//==========================
void TexRanking(void)
{
	int aTexU[NUM_DIGITS];				//各桁の数字を格納
	int nCalc = 0;						//計算用
	VERTEX_2D*pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{
		//テクスチャ座標の計測
		nCalc = g_aRanking[nCntRank].nScore;

		for (int nCntScore = NUM_DIGITS - 1; nCntScore >= 0; nCntScore--)
		{
			aTexU[nCntScore] = nCalc % 10;
			nCalc /= 10;
		}

		for (int nCntPlace = 0; nCntPlace < NUM_DIGITS; nCntPlace++)
		{
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (aTexU[nCntPlace] * 0.1f), 0.0f);					//(x,y)
			pVtx[1].tex = D3DXVECTOR2(0.1f + (aTexU[nCntPlace] * 0.1f), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (aTexU[nCntPlace] * 0.1f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (aTexU[nCntPlace] * 0.1f), 1.0f);

			pVtx += 4;		//頂点データのポインタを4つ文進める
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();
}

//==========================
//ランキングの移動処理
//==========================
void MoveRanking(void)
{
	float fDiff = 0.0f;

	for (int nCntArvl = NUM_RANKING - 1; nCntArvl >= 0; nCntArvl--)
	{
		if (g_bMove == false)
		{//移動完了時、固定値を代入する
			g_aRanking[nCntArvl].bArvl = true;
			g_aRanking[nCntArvl].pos.x = g_aRanking[nCntArvl].posDest.x + RANK_SLD_OK;
		}

		if (g_aRanking[nCntArvl].bArvl == false)
		{//まだであれば
			//位置の補正
			fDiff = g_aRanking[nCntArvl].pos.x - g_aRanking[nCntArvl].posDest.x;

			if (fDiff <= RANK_SLD_OK)
			{//一定値まで言っていたら完了に
				g_aRanking[nCntArvl].bArvl = true;
			}
			else
			{//慣性で移動＆終了
				g_aRanking[nCntArvl].pos.x -= fDiff * RANK_SLIDE_PER;
				break;
			}
		}
	}

	if (g_aRanking[0].bArvl
		&& g_aRanking[1].bArvl
		&& g_aRanking[2].bArvl
		&& g_aRanking[3].bArvl
		&& g_aRanking[4].bArvl == true)
	{//全部移動完了
		g_bMove = false;
	}
}
