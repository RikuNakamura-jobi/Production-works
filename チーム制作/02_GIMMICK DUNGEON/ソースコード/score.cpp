//==========================================
//
//  スコア表示(score.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "score.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "point.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_SCORE (999999) //スコアの最大値
#define MIN_SCORE (000000) //スコアの最小値
#define SCORE_NUM (5) //スコアの桁数
#define SCORE_WIDTH (50.0f) //スコアの横幅
#define SCORE_HEIGHT (75.0f) //スコアの高さ
#define SCORE_FPA (40) //スコアのフレーム % 加算度
#define SCORE_FPA_BRC (1000) //スコアの加算度分岐
#define SCORE_SOUND_BRC (1000) //スコアの効果音分岐

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;
int g_nScore; //現在のスコア
int g_nScoreOld; //前回のスコア
D3DXVECTOR3 g_posScore;	//座標

//==========================================
//  初期化処理
//==========================================
void InitScore()
{
	//変数の初期化
	g_nScore = 0;
	g_nScoreOld = 0;
	g_posScore = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * SCORE_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data/TEXTURE/number001.png",
		&g_pTextureScore
	);

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < SCORE_NUM; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x - (SCORE_WIDTH * (SCORE_NUM - nCnt)), 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x - (SCORE_WIDTH * (SCORE_NUM - (nCnt + 1))), 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x - (SCORE_WIDTH * (SCORE_NUM - nCnt)), SCORE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x - (SCORE_WIDTH * (SCORE_NUM - (nCnt + 1))), SCORE_HEIGHT, 0.0f);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//rhwの設定
			pVtx[nCnt].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		//頂点データを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitScore()
{
	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
}

//==========================================
//  更新処理
//==========================================
void UpdateScore()
{
	int nFpa = SCORE_FPA;	//加算数

	if (GetGameState() == GAMESTATE_RESULT)
	{
		nFpa *= 2;
	}

	if (g_nScoreOld < g_nScore)
	{//プラス方向
		
		if (g_nScore - g_nScoreOld >= SCORE_FPA_BRC)
		{//マクロ以上離れていたら
			DiffAddScore(nFpa + 1);
		}
		else
		{
			DiffAddScore(nFpa);
		}
	}
	else if (g_nScoreOld > g_nScore)
	{//マイナス方向

		if (g_nScoreOld - g_nScore >= SCORE_FPA_BRC)
		{//マクロ以上離れていたら
			DiffAddScore(-nFpa - 1);
		}
		else
		{
			DiffAddScore(-nFpa);
		}
	}

#ifdef _DEBUG

	//デバッグ操作
	if (GetKeyboardTrigger(DIK_F7))
	{//スコア加算
		AddScore(200);
	}
	if (GetKeyboardTrigger(DIK_F6))
	{//減算
		AddScore(-200);
	}

#endif
}

//==========================================
//  描画処理
//==========================================
void DrawScore()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < SCORE_NUM; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureScore);

		//描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
	}
}

//==========================================
//  設定処理
//==========================================
void SetScore(int nScore)
{
	//ローカル変数宣言
	int aTex[SCORE_NUM];
	int nCntScore;

	//スコアの取得
	if (nScore > MAX_SCORE)
	{
		g_nScoreOld = MAX_SCORE;
	}
	else if (nScore < MIN_SCORE)
	{
		g_nScoreOld = MIN_SCORE;
	}
	else
	{
		g_nScoreOld = nScore;
	}

	//計算用変数
	int nCalc = g_nScoreOld;

	//テクスチャ座標の計測
	for (int nCntScore = SCORE_NUM - 1; nCntScore >= 0; nCntScore--)
	{
		aTex[nCntScore] = nCalc % 10;
		nCalc /= 10;
	}

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//設定処理
	for (nCntScore = 0; nCntScore < SCORE_NUM; nCntScore++)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTex[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((aTex[nCntScore] * 0.1f) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTex[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((aTex[nCntScore] * 0.1f) + 0.1f, 1.0f);

		pVtx += 4; //頂点データのポインタを4つ進める
	}

	//頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();
}

//==========================================
//  加算処理
//==========================================
void DiffAddScore(int nValue)
{
	if (nValue > 0 && g_nScoreOld + nValue > g_nScore)
	{//結果予定がプラス
		nValue = (g_nScore - g_nScoreOld);
	}
	else if (nValue < 0 && g_nScoreOld + nValue < g_nScore)
	{//結果予定がマイナス
		nValue = (g_nScoreOld - g_nScore);
	}

	//スコアの加算
	SetScore(g_nScoreOld + nValue);
}

//==========================================
//  加算分受け取り処理
//==========================================
void AddScore(int nValue)
{
	//スコアの加算
	if (g_nScore + nValue > MAX_SCORE)
	{//上限
		g_nScore = MAX_SCORE;
	}
	else if (g_nScore + nValue < MIN_SCORE)
	{//下限
		g_nScore = MIN_SCORE;
	}
	else
	{
		g_nScore += nValue;

		if (nValue >= SCORE_SOUND_BRC)
		{
			PlaySound(SOUND_LABEL_SE_ADDSCORE_001);
		}
		else if (nValue >= 0)
		{
			PlaySound(SOUND_LABEL_SE_ADDSCORE_000);
		}
	}
}

//==========================================
//  スコアの座標・倍率設定処理
//==========================================
void SetAdjScore(D3DXVECTOR3 pos, float fMgn)
{
	g_posScore = pos;

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < SCORE_NUM; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x - ((SCORE_WIDTH * fMgn) * (SCORE_NUM - nCnt)), g_posScore.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x - ((SCORE_WIDTH * fMgn) * (SCORE_NUM - (nCnt + 1))), g_posScore.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x - ((SCORE_WIDTH * fMgn) * (SCORE_NUM - nCnt)), g_posScore.y + SCORE_HEIGHT * fMgn, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x - ((SCORE_WIDTH * fMgn) * (SCORE_NUM - (nCnt + 1))),	g_posScore.y + SCORE_HEIGHT * fMgn, 0.0f);

		//頂点データを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();
}

//==========================================
//  スコア取得処理
//==========================================
int GetScore()
{
	return g_nScore;
}

//==========================================
//  スコア取得処理
//==========================================
void DebugScore()
{
	LPD3DXFONT pFont = GetFont();
	RECT rect = { 0,580,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "加減算<H/J>\n今のスコア; %d\n前のスコア: %d",g_nScore,g_nScoreOld);

	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 200, 200, 255));
}
