//===========================
//
//ダイレクトX.resultファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "result.h"
#include "sound.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "score.h"
#include "fade.h"
#include "bullet.h"

//マクロ定義
#define NUM_RESULT (3)												//リザルト画面の数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureResult[NUM_RESULT] = {};				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffResult = NULL;						//頂点情報を格納
int g_nCounterTime;													//リザルト画面の継続時間

//===========================
//リザルト画面の初期化処理
//===========================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	Player *pPlayer = GetPlayer();
	int nCntResult;
	int nBulletScore = GetBulletScore();

	InitScore();

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Result.png",
		&g_apTextureResult[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Ranking〇.png",
		&g_apTextureResult[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Ranking×.png",
		&g_apTextureResult[2]);

	g_nCounterTime = 900;
	nBulletScore = nBulletScore % 10 / 1;

	SetScore(nBulletScore);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_RESULT,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffResult,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
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
	g_VtxBuffResult->Unlock();

	if (pPlayer->state == PLAYERSTATE_DEATH)
	{
		//サウンドの再生
		PlaySound(SOUND_LABEL_BGM014);
	}
	else
	{
		//サウンドの再生
		PlaySound(SOUND_LABEL_BGM012);
	}
}

//===========================
//リザルト画面の終了処理
//===========================
void UninitResult(void)
{
	int nCntResult;

	//サウンドの停止
	StopSound();

	for (nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{
		//テクスチャの破棄
		if (g_apTextureResult[nCntResult] != NULL)
		{
			g_apTextureResult[nCntResult]->Release();
			g_apTextureResult[nCntResult] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_VtxBuffResult != NULL)
	{
		g_VtxBuffResult->Release();
		g_VtxBuffResult = NULL;
	}

	UninitScore();
}

//===========================
//リザルト画面の更新処理
//===========================
void UpdateResult(void)
{
	FADE fade = GetFade();

	UpdateScore();

	g_nCounterTime--;

	if ((GetKeyboardTrigger(DIK_RETURN) == true && fade == FADE_NONE) || (g_nCounterTime <= 0 && fade == FADE_NONE))
	{
		//モード設定
		SetFade(MODE_TITLE);
	}
}

//===========================
//リザルト画面の描画処理
//===========================
void DrawResult(void)
{
	Player *pPlayer = GetPlayer();
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_VtxBuffResult,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < NUM_RESULT; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureResult[0]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}

	DrawScore();
}