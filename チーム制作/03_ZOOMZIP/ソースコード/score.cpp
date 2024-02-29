//==========================================================
//
//スコア処理 [score.h]
//Author Ibuki Okusada
//
//==========================================================
#include "main.h"    //作成したmain.hをインクルードする
#include "score.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "enemy.h"
#include <math.h>

//==========================================================
//マクロ定義
//==========================================================
#define SCORE_X			(SCREEN_WIDTH * 0.45f)	//ロゴX座標
#define SCORE_Y			(SCREEN_HEIGHT * 0.92f)	//ロゴY座標
#define SCORE_SPACEX	(SCREEN_WIDTH * 0.05f)	//ロゴスペース
#define SCORE_WIDTH		(SCREEN_WIDTH * 0.08f)	//ロゴ幅
#define SCORE_HEIGHT	(SCREEN_HEIGHT * 0.14f)	//ロゴ高さ
#define SCORE_FADECNT	(120)					//遷移カウント
#define SCORE_ROTATECNT	(80)					//回転カウンター
#define SCORE_ROTATESPD	(D3DX_PI * 0.1f)		//回転速度
#define MAX_LENGTH		(500.0f)				//最大長さ
#define SCORE_LENGSPD	(4.0f)					//長さ増加速度
#define SCORE_TEXFILENAME	"data\\TEXTURE\\number000.png"	//
#define NUM_SCORE		(3)						//スコア桁数

//==========================================================
//構造体の定義
//==========================================================

//==========================================================
//グローバル変数
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureScore = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;	//頂点バッファへのポインタ
Score g_aScore[NUM_SCORE];	//ロゴの情報配列
int g_nScore = 0;			//スコア

//==========================================================
//プロトタイプ宣言
//==========================================================
void SetpVtxScore(void);

//==========================================================
//テクスチャファイル名
//==========================================================

//==========================================================
//チームロゴの初期化処理
//==========================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	Score *pScore = GetScore();	//ロゴのポインタ

	//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			SCORE_TEXFILENAME,
			&g_pTextureScore);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	g_nScore = 0;

	//情報初期化
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++, pScore++)
	{
		pScore->pos = D3DXVECTOR3(SCORE_X + nCnt * SCORE_SPACEX, SCORE_Y, 0.0f);
		pScore->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pScore->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pScore->fHeight = SCORE_HEIGHT;
		pScore->fWidth = SCORE_WIDTH;
		pScore->fAngle = 0.0f;

		//対角線の長さを算出する
		pScore->fLength = sqrtf(SCORE_WIDTH * SCORE_WIDTH + SCORE_HEIGHT * SCORE_HEIGHT) * 0.5f;

		//対角線の角度を算出する
		pScore->fAngle = atan2f(SCORE_WIDTH, SCORE_HEIGHT);
	}

	//頂点情報設定
	SetpVtxScore();
}

//==========================================================
//チームロゴの終了処理
//==========================================================
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

//==========================================================
//チームロゴの更新処理
//==========================================================
void UpdateScore(void)
{
	
}

//==========================================================
//チームロゴの描画処理
//==========================================================
void DrawScore(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		if (g_aScore[nCnt].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureScore);

			//ポリゴンの描画(開始頂点移動)
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
}

//==========================================================
//スコア取得
//==========================================================
Score *GetScore(void)
{
	return &g_aScore[0];
}

//==========================================================
//チームロゴの描画処理
//==========================================================
void SetpVtxScore(void)
{
	Score *pScore = GetScore();	//ロゴのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++, pScore++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = pScore->pos.x + sinf(pScore->rot.z + (-D3DX_PI + pScore->fAngle)) * pScore->fLength;
		pVtx[0].pos.y = pScore->pos.y + cosf(pScore->rot.z + (-D3DX_PI + pScore->fAngle)) * pScore->fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = pScore->pos.x + sinf(pScore->rot.z + (D3DX_PI - pScore->fAngle)) * pScore->fLength;
		pVtx[1].pos.y = pScore->pos.y + cosf(pScore->rot.z + (D3DX_PI - pScore->fAngle)) * pScore->fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = pScore->pos.x + sinf(pScore->rot.z + -pScore->fAngle) * pScore->fLength;
		pVtx[2].pos.y = pScore->pos.y + cosf(pScore->rot.z + -pScore->fAngle) * pScore->fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = pScore->pos.x + sinf(pScore->rot.z + pScore->fAngle) * pScore->fLength;
		pVtx[3].pos.y = pScore->pos.y + cosf(pScore->rot.z + pScore->fAngle) * pScore->fLength;
		pVtx[3].pos.z = 0.0f;

		//頂点カラーの設定
		pVtx[0].col = pScore->col;
		pVtx[1].col = pScore->col;
		pVtx[2].col = pScore->col;
		pVtx[3].col = pScore->col;

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
	g_pVtxBuffScore->Unlock();
}

//==========================================================
//スコア更新処理
//==========================================================
void AddScore(int nAddScore)
{
	int aTexU[NUM_SCORE] = {};

	g_nScore += nAddScore;
	PlaySound(SOUND_LABEL_SCORE);
	SetNearMiss();

	//タイムを各配列に格納
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		aTexU[nCnt] = g_nScore % (int)pow(10, (NUM_SCORE - nCnt)) / (int)pow(10, (NUM_SCORE - nCnt) - 1);

		if (g_nScore * 10 >= (int)pow(10, (NUM_SCORE - nCnt)))
		{//スコアが桁数より上の場合
			g_aScore[nCnt].bUse = true;
		}
		else
		{
			g_aScore[nCnt].bUse = false;
		}
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffScore->Unlock();
}

//==========================================================
//現在のスコア
//==========================================================
int GetNowScore(void)
{
	return g_nScore;
}