//============================
//
//タイム処理(time.cpp)
//Author:早川 友貴
//
//============================
//#include "bullet.h"
//#include "enemy.h"
#include "sound.h"
#include "time.h"
#include "fade.h"
#include "game.h"
#include"player.h"
#include"result.h"

//マクロ定義
#define NUM_PLACE (3)		//タイムの桁数
#define MAX_TIME (128)
#define POS_DATA  (200.0f)  //タイムの幅を決める

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;  //テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL; //頂点バッファへのポインタ
D3DXVECTOR3 g_posTime;  //タイムの位置
int g_Timelog;			//タイムの秒を測る
int g_nTime;			//タイムの値
bool g_bfade;
float g_fWindth;		//幅の値
float g_fHeight;		//高さの値

//=============================
//  タイムの初期化
//=============================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTime;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\time.png",
		&g_pTextureTime);

	//タイムの情報を初期化
	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		g_posTime = D3DXVECTOR3(60.0f, 40.0f, 0.0f);  //位置を初期化

		g_Timelog = 0;      //タイムの秒

		g_nTime = 180;		//タイムの値

		g_bfade = false;	//フェードに使う変数

		g_fWindth = (0.0f);	  //幅の値
		g_fHeight = (0.0f);   //高さの値
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	VERTEX_2D *pVtx;    //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posTime.x - 40.0f + (nCntTime * 100.0f), g_posTime.y - 40.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posTime.x + 40.0f + (nCntTime * 100.0f), g_posTime.y - 40.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posTime.x - 40.0f + (nCntTime * 100.0f), g_posTime.y + 40.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posTime.x + 40.0f + (nCntTime * 100.0f), g_posTime.y + 40.0f, 0.0f);

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

		if (nCntTime == 0)
		{
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.2f, 1.0f);
		}

		if (nCntTime == 1)
		{
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.8f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.9f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.8f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.9f, 1.0f);
		}

		if (nCntTime == 2)
		{
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		pVtx += 4;		//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//=====================
//タイムの終了処理
//=====================
void UninitTime(void)
{
	//テクスチャの破棄
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//=====================
//タイムの更新処理
//=====================
void UpdateTime(void)
{
	FADE pFade;
	pFade = GetFade();
	Player *pPlayer = GetPlayer();

	LPDIRECT3DDEVICE9 pDevice;
	int aTexU[NUM_PLACE];  //各桁の数字を格納
	int nCntTime;

	g_Timelog++;

	if (g_Timelog % 60 == 0)
	{
		//デバイスの取得
		pDevice = GetDevice();

		if (pFade == FADE_NONE)
		{
			g_nTime --;
		}

		aTexU[0] = g_nTime % 1000 / 100;
		aTexU[1] = g_nTime % 100 / 10;
		aTexU[2] = g_nTime % 10 / 1;

		VERTEX_2D *pVtx;    //頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
		{
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (aTexU[nCntTime] * 0.1f), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (aTexU[nCntTime] * 0.1f), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (aTexU[nCntTime] * 0.1f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (aTexU[nCntTime] * 0.1f), 1.0f);

			pVtx += 4;		//頂点データのポインタを4つ分進める
		}
		//頂点バッファをアンロックする
		g_pVtxBuffTime->Unlock();
	}

	if (pPlayer->nLife == 0)
	{
		g_nTime = 1;
	}

	//タイムの上限
	else if (g_nTime <= 0)
	{//0になったら
		g_nTime = 1;
	}

	//ゲームオーバー画面への移行
	if (g_bfade == false && g_nTime <= 1)
	{
		g_bfade = true;

		SetResult(RESULT_OVER);

		//ゲーム状態の設定
		SetGameState(GAMESTATE_END, 120);
	}
}

//======================
//タイムの描画処理
//======================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	int nCntTime;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime);

	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
	}
}

//===============================
//タイムの設定処理
//===============================
void SetTime(int nTime)
{
	LPDIRECT3DDEVICE9 pDevice;
	int aTexU[NUM_PLACE];  //各桁の数字を格納
	int nCntTime;
	g_nTime = nTime;

	//デバイスの取得
	pDevice = GetDevice();

	aTexU[0] = g_nTime % 1000 / 100;
	aTexU[1] = g_nTime % 100 / 10;
	aTexU[2] = g_nTime % 10 / 1;

	VERTEX_2D *pVtx;    //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aTexU[nCntTime] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aTexU[nCntTime] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aTexU[nCntTime] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aTexU[nCntTime] * 0.1f), 1.0f);

		pVtx += 4;		//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//==================================
//タイムの位置設定
//==================================
void SetPosTime(D3DXVECTOR3 pos, float fWindth, float fHeight)
{
	int nCntTime;

	//タイムの設定をを初期化
	g_posTime = pos;

	g_fWindth = fWindth;

	g_fHeight = fHeight;

	VERTEX_2D *pVtx;    //頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posTime.x + (nCntTime * POS_DATA) - g_fWindth, g_posTime.y - g_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posTime.x + (nCntTime * POS_DATA) + g_fWindth, g_posTime.y - g_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posTime.x + (nCntTime * POS_DATA) - g_fWindth, g_posTime.y + g_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posTime.x + (nCntTime * POS_DATA) + g_fWindth, g_posTime.y + g_fHeight , 0.0f);

		pVtx += 4;		//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//======================
//タイムを明け渡す処理
//======================
int GetTime(void)
{
	return g_nTime;
}
//=============================
//時間をリセットする処理
//=============================
void ResetTime(void)
{
	g_nTime = 1;
}