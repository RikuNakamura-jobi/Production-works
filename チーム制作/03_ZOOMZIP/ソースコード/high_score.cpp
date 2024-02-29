//==========================================================
//
//ハイスコア処理 [high_score.h]
//Author Ibuki Okusada
//
//==========================================================
#include "main.h"    //作成したmain.hをインクルードする
#include "high_score.h"
#include "fade.h"
#include "input.h"
#include <math.h>
#include "score.h"
#include <stdio.h>
#include "player.h"
#include "game.h"
#include "sound.h"

//==========================================================
//マクロ定義
//==========================================================
#define SCORE_X			(SCREEN_WIDTH * 0.4f)	//ロゴX座標
#define SCORE_Y			(SCREEN_HEIGHT * 0.4f)	//ロゴY座標
#define SCORE_SPACEX	(SCREEN_WIDTH * 0.08f)	//ロゴスペース
#define SCORE_WIDTH		(SCREEN_WIDTH * 0.15f)	//ロゴ幅
#define SCORE_HEIGHT	(SCREEN_HEIGHT * 0.25f)	//ロゴ高さ
#define SCORE_FADECNT	(120)					//遷移カウント
#define SCORE_ROTATECNT	(80)					//回転カウンター
#define SCORE_ROTATESPD	(D3DX_PI * 0.1f)		//回転速度
#define MAX_LENGTH		(500.0f)				//最大長さ
#define SCORE_LENGSPD	(4.0f)					//長さ増加速度
#define SCORE_TEXFILENAME		"data\\TEXTURE\\number000.png"	//
#define SCOREFONT_TEXFILENAME	"data\\TEXTURE\\highscore000.png"	//
#define SCOREINPUT_TEXFILENAME	"data\\TEXTURE\\highscore001.png"	//
#define NUM_SCORE		(3)						//スコア桁数
#define HIGHSCORE_FILE	"data\\FILE\\highscore.bin"	//
#define SCOREFONT_X			(SCREEN_WIDTH * 0.5f)	//ロゴX座標
#define SCOREFONT_Y			(SCREEN_HEIGHT * 0.15f)	//ロゴY座標
#define SCOREFONT_WIDTH		(SCREEN_WIDTH * 0.15f)	//ロゴ幅
#define SCOREFONT_HEIGHT	(SCREEN_HEIGHT * 0.1f)	//ロゴ高さ
#define SCOREINPUT_X		(SCREEN_WIDTH * 0.85f)	//ロゴX座標
#define SCOREINPUT_Y		(SCREEN_HEIGHT * 0.85f)	//ロゴY座標
#define SCOREINPUT_WIDTH	(SCREEN_WIDTH * 0.15f)	//ロゴ幅
#define SCOREINPUT_HEIGHT	(SCREEN_HEIGHT * 0.13f)	//ロゴ高さ

//==========================================================
//構造体の定義
//==========================================================

//==========================================================
//グローバル変数
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureHighScore = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHighScore = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureHighScoreFont = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHighScoreFont = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureHighScoreInput = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHighScoreInput = NULL;	//頂点バッファへのポインタ
HighScore g_aHighScore[NUM_SCORE];	//ロゴの情報配列
int g_nHighScore = 0;			//スコア
bool g_bNewRecord = false;		//更新したかどうか

//==========================================================
//プロトタイプ宣言
//==========================================================
void SetpVtxHighScore(void);
void SaveRankingScore(void);
void LoadRankingScore(void);
void SetpVtxFont(void);
void SetpVtxInput(void);

//==========================================================
//テクスチャファイル名
//==========================================================

//==========================================================
//ハイスコアの初期化処理
//==========================================================
void InitHighScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	HighScore *pHighScore = GetHighScore();	//ロゴのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		SCORE_TEXFILENAME,
		&g_pTextureHighScore);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHighScore,
		NULL);

	g_nHighScore = 0;
	g_bNewRecord = false;
	

	//情報初期化
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++, pHighScore++)
	{
		pHighScore->pos = D3DXVECTOR3(SCORE_X + nCnt * SCORE_SPACEX, SCORE_Y, 0.0f);
		pHighScore->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pHighScore->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pHighScore->fHeight = SCORE_HEIGHT;
		pHighScore->fWidth = SCORE_WIDTH;
		pHighScore->fAngle = 0.0f;

		//対角線の長さを算出する
		pHighScore->fLength = sqrtf(SCORE_WIDTH * SCORE_WIDTH + SCORE_HEIGHT * SCORE_HEIGHT) * 0.5f;

		//対角線の角度を算出する
		pHighScore->fAngle = atan2f(SCORE_WIDTH, SCORE_HEIGHT);
	}

	//頂点情報設定
	SetpVtxHighScore();

	LoadRankingScore();

	AddHighScore(0);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		SCOREFONT_TEXFILENAME,
		&g_pTextureHighScoreFont);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHighScoreFont,
		NULL);

	//頂点設定
	SetpVtxFont();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		SCOREINPUT_TEXFILENAME,
		&g_pTextureHighScoreInput);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHighScoreInput,
		NULL);

	//
	SetpVtxInput();
}

//==========================================================
//ハイスコアの終了処理
//==========================================================
void UninitHighScore(void)
{
	//テクスチャの破棄
	if (g_pTextureHighScore != NULL)
	{
		g_pTextureHighScore->Release();
		g_pTextureHighScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffHighScore != NULL)
	{
		g_pVtxBuffHighScore->Release();
		g_pVtxBuffHighScore = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureHighScoreFont != NULL)
	{
		g_pTextureHighScoreFont->Release();
		g_pTextureHighScoreFont = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffHighScoreFont != NULL)
	{
		g_pVtxBuffHighScoreFont->Release();
		g_pVtxBuffHighScoreFont = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureHighScoreInput != NULL)
	{
		g_pTextureHighScoreInput->Release();
		g_pTextureHighScoreInput = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffHighScoreInput != NULL)
	{
		g_pVtxBuffHighScoreInput->Release();
		g_pVtxBuffHighScoreInput = NULL;
	}
}

//==========================================================
//ハイスコアの更新処理
//==========================================================
void UpdateHighScore(void)
{
	if (GetNowScore() > g_nHighScore)
	{//ハイスコアを超えた場合
		AddHighScore(GetNowScore() - g_nHighScore);
	}


	if (GetGameState() == GAMESTATE_END)
	{
		//データの保存
		if (g_bNewRecord == false)
		{
			SaveRankingScore();
			g_bNewRecord = true;
		}
		if (GetKeyboardTrigger(DIK_RETURN) || GetGamepadTrigger(BUTTON_B, 0))
		{
			SetFade(MODE_TEAMLOGO);
			PlaySound(SOUND_LABEL_SECURSOR);
		}
		else if (GetKeyboardTrigger(DIK_SPACE) || GetGamepadTrigger(BUTTON_A, 0))
		{
			SetFade(MODE_GAME);
			PlaySound(SOUND_LABEL_SEBUTTON);
		}
	}
	else
	{
		if (GetKeyboardTrigger(DIK_RETURN) || GetGamepadTrigger(BUTTON_B, 0))
		{
			SetGameState(GAMESTATE_END, 0);
		}
	}
}

//==========================================================
//ハイスコアの描画処理
//==========================================================
void DrawHighScore(void)
{

	if (GetPlayer()->bUse == false)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffHighScore, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
		{
			if (g_aHighScore[nCnt].bUse == true)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureHighScore);

				//ポリゴンの描画(開始頂点移動)
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
			}
		}

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffHighScoreFont, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureHighScoreFont);

		//ポリゴンの描画(開始頂点移動)
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffHighScoreInput, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureHighScoreInput);

		//ポリゴンの描画(開始頂点移動)
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//==========================================================
//スコア取得
//==========================================================
HighScore *GetHighScore(void)
{
	return &g_aHighScore[0];
}

//==========================================================
//ハイスコアの描画処理
//==========================================================
void SetpVtxHighScore(void)
{
	HighScore *pHighScore = GetHighScore();	//ロゴのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffHighScore->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++, pHighScore++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = pHighScore->pos.x + sinf(pHighScore->rot.z + (-D3DX_PI + pHighScore->fAngle)) * pHighScore->fLength;
		pVtx[0].pos.y = pHighScore->pos.y + cosf(pHighScore->rot.z + (-D3DX_PI + pHighScore->fAngle)) * pHighScore->fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = pHighScore->pos.x + sinf(pHighScore->rot.z + (D3DX_PI - pHighScore->fAngle)) * pHighScore->fLength;
		pVtx[1].pos.y = pHighScore->pos.y + cosf(pHighScore->rot.z + (D3DX_PI - pHighScore->fAngle)) * pHighScore->fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = pHighScore->pos.x + sinf(pHighScore->rot.z + -pHighScore->fAngle) * pHighScore->fLength;
		pVtx[2].pos.y = pHighScore->pos.y + cosf(pHighScore->rot.z + -pHighScore->fAngle) * pHighScore->fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = pHighScore->pos.x + sinf(pHighScore->rot.z + pHighScore->fAngle) * pHighScore->fLength;
		pVtx[3].pos.y = pHighScore->pos.y + cosf(pHighScore->rot.z + pHighScore->fAngle) * pHighScore->fLength;
		pVtx[3].pos.z = 0.0f;

		//頂点カラーの設定
		pVtx[0].col = pHighScore->col;
		pVtx[1].col = pHighScore->col;
		pVtx[2].col = pHighScore->col;
		pVtx[3].col = pHighScore->col;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffHighScore->Unlock();
}

//==========================================================
//スコア更新処理
//==========================================================
void AddHighScore(int nAddHighScore)
{
	int aTexU[NUM_SCORE] = {};

	g_nHighScore += nAddHighScore;

	//タイムを各配列に格納
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		aTexU[nCnt] = g_nHighScore % (int)pow(10, (NUM_SCORE - nCnt)) / (int)pow(10, (NUM_SCORE - nCnt) - 1);

		if (g_nHighScore * 10 >= (int)pow(10, (NUM_SCORE - nCnt)) || nCnt == NUM_SCORE - 1)
		{//スコアが桁数より上の場合
			g_aHighScore[nCnt].bUse = true;
		}
		else
		{
			g_aHighScore[nCnt].bUse = false;
		}
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffHighScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_SCORE; nCntTime++)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * aTexU[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * aTexU[nCntTime] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aTexU[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * aTexU[nCntTime] + 0.1f, 1.0f);

		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffHighScore->Unlock();
}

//==========================================================
//現在のスコア
//==========================================================
int GetNowHighScore(void)
{
	return g_nHighScore;
}

//========================================
//ランキングの保存処理
//========================================
void SaveRankingScore(void)
{
	FILE *pFile;	//ファイルへのポインタ

	pFile = fopen(HIGHSCORE_FILE, "wb");

	if (pFile != NULL)
	{//ファイルが開けた場合

	 //データを保存
		fwrite(&g_nHighScore, sizeof(int), 1, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合

	}
}

//========================================
//ランキングの読み込み処理
//========================================
void LoadRankingScore(void)
{
	FILE *pFile;	//ファイルへのポインタ

	pFile = fopen(HIGHSCORE_FILE, "rb");

	if (pFile != NULL)
	{//ファイルが開けた場合

		//データを読み込む
		fread(&g_nHighScore, sizeof(int), 1, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
	 //固定の値を代入
		g_nHighScore = 0;
	}
}

//========================================
//ハイスコア文字の頂点設定
//========================================
void SetpVtxFont(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffHighScoreFont->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);
	//頂点座標の設定
	pVtx[0].pos.x = SCOREFONT_X - SCOREFONT_WIDTH;
	pVtx[0].pos.y = SCOREFONT_Y - SCOREFONT_HEIGHT;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = SCOREFONT_X + SCOREFONT_WIDTH;
	pVtx[1].pos.y = SCOREFONT_Y - SCOREFONT_HEIGHT;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = SCOREFONT_X - SCOREFONT_WIDTH;
	pVtx[2].pos.y = SCOREFONT_Y + SCOREFONT_HEIGHT;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = SCOREFONT_X + SCOREFONT_WIDTH;
	pVtx[3].pos.y = SCOREFONT_Y + SCOREFONT_HEIGHT;
	pVtx[3].pos.z = 0.0f;

	//頂点カラーの設定
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//rhwの設定
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffHighScoreFont->Unlock();
}

void SetpVtxInput(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffHighScoreInput->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);
	//頂点座標の設定
	pVtx[0].pos.x = SCOREINPUT_X - SCOREINPUT_WIDTH;
	pVtx[0].pos.y = SCOREINPUT_Y - SCOREINPUT_HEIGHT;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = SCOREINPUT_X + SCOREINPUT_WIDTH;
	pVtx[1].pos.y = SCOREINPUT_Y - SCOREINPUT_HEIGHT;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = SCOREINPUT_X - SCOREINPUT_WIDTH;
	pVtx[2].pos.y = SCOREINPUT_Y + SCOREINPUT_HEIGHT;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = SCOREINPUT_X + SCOREINPUT_WIDTH;
	pVtx[3].pos.y = SCOREINPUT_Y + SCOREINPUT_HEIGHT;
	pVtx[3].pos.z = 0.0f;

	//頂点カラーの設定
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//rhwの設定
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffHighScoreInput->Unlock();
}