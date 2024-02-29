//==================================================================================================
//
//Time.cpp
//Author:髙山桃也
//
//==================================================================================================

#include "main.h"
#include "time.h"
#include "game.h"
#include "input.h"

//マクロ定義
#define NUM_PLACE				(4)											//スコアの桁数
#define NUM_TEX					(3)											//テクスチャの数

#define TIME_WIDTH				(50)										//横幅
#define TIME_HEIGHT				(50)										//高さ

#define BACK_WIDTH				(300)										//幅
#define BACK_HEIGHT				(100)										//高さ

#define CLOCK_WIDTH				(5)											//幅
#define CLOCK_HEIGHT			(70)										//高さ

#define INITIAL_MINITE			(3)											//初期の時間（分）
#define INITIAL_SECOND			(0)											//初期の時間（秒）

#define TIME_TEX_FILE			"data\\TEXTURE\\number001.png"			//テクスチャ名
#define BACK_TEX_FILE			"data\\TEXTURE\\clock000.png"			//テクスチャ名
#define CLOCK_TEX_FILE			"data\\TEXTURE\\clockhands000.png"		//テクスチャ名

//時計の構造体
typedef struct
{
	D3DXVECTOR3 pos;						//位置
	D3DXVECTOR3 rot;						//向き
	float fAngle;							//対角線の角度
	float fLength;							//対角線の長さ
}Clock;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureTime[NUM_TEX] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;			//頂点バッファへのポインタ
D3DXVECTOR3 g_posTime;									//スコアの位置
Clock g_clock;											//時計の構造体情報
int g_nTime;											//スコアの値
int g_nLastTime;										//前回のスコアの値
int g_nCntSecond;										//一秒をはかるカウンタ
int g_nMinute;											//分の数

//プロトタイプ宣言
void ControlDebug(void);

//==================================================================================================
//初期化処理
//==================================================================================================
void InitTime(void)
{
	//デバイスポインタ設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		TIME_TEX_FILE,
		&g_apTextureTime[0]);

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		BACK_TEX_FILE,
		&g_apTextureTime[1]);

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		CLOCK_TEX_FILE,
		&g_apTextureTime[2]);

	//カウンタの初期化
	g_nCntSecond = 0;
	g_nMinute = INITIAL_MINITE;

	//スコア値の初期化
	g_nTime = 0;
	g_nLastTime = INITIAL_SECOND;

	//スコア位置の初期化
	g_posTime = D3DXVECTOR3(SCREEN_WIDTH * 0.5f,50.0f,0.0f);
	D3DXVECTOR3 posBack = { SCREEN_WIDTH * 0.5f,50.0f,0.0f };

	//時計の初期化
	g_clock.pos = D3DXVECTOR3(posBack.x - 89.0f, posBack.y - 3.0f, 0.0f);
	g_clock.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_clock.fAngle = atan2f(CLOCK_WIDTH,CLOCK_HEIGHT);
	g_clock.fLength = sqrtf(CLOCK_WIDTH * CLOCK_WIDTH + CLOCK_HEIGHT * CLOCK_HEIGHT) * 0.5f;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (NUM_PLACE + NUM_TEX - 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{//全ての初期化

		if (nCntTime >= NUM_PLACE / 2)
		{
			g_posTime.x += TIME_WIDTH / 2;
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posTime.x - TIME_WIDTH * 0.5f + (nCntTime * TIME_WIDTH),
								  g_posTime.y - TIME_HEIGHT * 0.5f, 0.0f);

		pVtx[1].pos = D3DXVECTOR3(g_posTime.x + TIME_WIDTH * 0.5f + (nCntTime * TIME_WIDTH),
								  g_posTime.y - TIME_HEIGHT * 0.5f, 0.0f);

		pVtx[2].pos = D3DXVECTOR3(g_posTime.x - TIME_WIDTH * 0.5f + (nCntTime * TIME_WIDTH),
								  g_posTime.y + TIME_HEIGHT * 0.5f, 0.0f);

		pVtx[3].pos = D3DXVECTOR3(g_posTime.x + TIME_WIDTH * 0.5f + (nCntTime * TIME_WIDTH),
								  g_posTime.y + TIME_HEIGHT * 0.5f, 0.0f);

		if (nCntTime >= NUM_PLACE / 2)
		{
			g_posTime.x -= TIME_WIDTH / 2;
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 125);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 125);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 125);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 125);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}

	//UI背景設定========================================================================
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3
	(
		posBack.x - BACK_WIDTH * 0.5f,
		posBack.y - BACK_HEIGHT * 0.5f,
		0.0f
	);
	pVtx[1].pos = D3DXVECTOR3
	(
		posBack.x + BACK_WIDTH * 0.5f,
		posBack.y - BACK_HEIGHT * 0.5f,
		0.0f
	);
	pVtx[2].pos = D3DXVECTOR3
	(
		posBack.x - BACK_WIDTH * 0.5f,
		posBack.y + BACK_HEIGHT * 0.5f,
		0.0f
	);
	pVtx[3].pos = D3DXVECTOR3
	(
		posBack.x + BACK_WIDTH * 0.5f,
		posBack.y + BACK_HEIGHT * 0.5f,
		0.0f
	);

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

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//針設定========================================================================
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffTime->Unlock();
}

//==================================================================================================
//終了処理
//==================================================================================================
void UninitTime(void)
{
	for (int nCnt = 0; nCnt < NUM_TEX; nCnt++)
	{
		if (g_apTextureTime[nCnt] != NULL)
		{//テクスチャへのポインタ破棄
			g_apTextureTime[nCnt]->Release();
			g_apTextureTime[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffTime != NULL)
	{//頂点バッファへのポインタ破棄
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//==================================================================================================
//更新処理
//==================================================================================================
void UpdateTime(void)
{
#ifdef  _DEBUG
	//デバッグ操作
	ControlDebug();
#endif //  _DEBUG

	//変数宣言
	int aTexU[NUM_PLACE], nCount;
	int aData1, aData2;
	D3DXCOLOR UICol = { 1.0f,1.0f,1.0f,1.0f };

	//ポインタ宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);
	
	if (g_nMinute <= 0 && g_nLastTime <= 0)
	{//時間超過の補正
		g_nMinute = 0;
		g_nLastTime = 0;
	}

	if ((g_nMinute != 0 || g_nLastTime != 0) && GetGameState() == GAMESTATE_NORMAL)
	{//時間切れでない場合のみカウントダウン
		g_nCntSecond++;

		if (g_nCntSecond >= 60)
		{//１秒の計測
			g_nLastTime--;

			g_nCntSecond = 0;
		}

		if (g_nLastTime < 0)
		{//一分の計測
			g_nMinute--;

			g_nLastTime = 59;
		}

		//針を回す
		g_clock.rot.z -= (D3DX_PI * 2) / ((INITIAL_MINITE * 60 + INITIAL_SECOND) * 60);
	}

	if (g_nMinute == 0 && g_nLastTime < 30)
	{//赤警告
		UICol = {1.0f,0.0f,0.0f,1.0f};
	}
	else if (g_nMinute == 0)
	{//黄色警告
		UICol = { 1.0f,0.86f,0.6f,1.0f };
	}

	for (nCount = 0; nCount < NUM_PLACE / 2; nCount++)
	{//スコア表示
	 //各桁割り出し
		aData1 = pow(10.0, NUM_PLACE / 2 - nCount);
		aData2 = pow(10.0, NUM_PLACE / 2 - nCount - 1);

		aTexU[nCount] = g_nMinute % aData1 / aData2;

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 1.0f);
		
		//頂点カラーの設定
		pVtx[0].col = UICol;
		pVtx[1].col = UICol;
		pVtx[2].col = UICol;
		pVtx[3].col = UICol;

		//頂点情報のポインタを４つ分進める
		pVtx += 4;
	}

	for (; nCount < NUM_PLACE; nCount++)
	{//スコア表示
	 //各桁割り出し
		aData1 = pow(10.0, NUM_PLACE - nCount);
		aData2 = pow(10.0, NUM_PLACE - nCount - 1);

		aTexU[nCount] = g_nLastTime % aData1 / aData2;

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 1.0f);

		//頂点カラーの設定
		pVtx[0].col = UICol;
		pVtx[1].col = UICol;
		pVtx[2].col = UICol;
		pVtx[3].col = UICol;

		//頂点情報のポインタを４つ分進める
		pVtx += 4;
	}

	//頂点カラーの設定
	pVtx[0].col = UICol;
	pVtx[1].col = UICol;
	pVtx[2].col = UICol;
	pVtx[3].col = UICol;

	//針までポインタを進める
	pVtx += 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z + g_clock.fAngle) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z + g_clock.fAngle) * g_clock.fLength,
		0.0f
	);
	pVtx[1].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z - g_clock.fAngle) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z - g_clock.fAngle) * g_clock.fLength,
		0.0f
	);
	pVtx[2].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z - g_clock.fAngle + D3DX_PI) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z - g_clock.fAngle + D3DX_PI) * g_clock.fLength,
		0.0f
	);
	pVtx[3].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z + g_clock.fAngle - D3DX_PI) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z + g_clock.fAngle - D3DX_PI) * g_clock.fLength,
		0.0f
	);

	//頂点カラーの設定
	/*pVtx[0].col = UICol;
	pVtx[1].col = UICol;
	pVtx[2].col = UICol;
	pVtx[3].col = UICol;*/

	//頂点バッファをアンロック
	g_pVtxBuffTime->Unlock();
}

//==================================================================================================
//デバッグ操作
//==================================================================================================
void ControlDebug(void)
{
	if (GetKeyboardTrigger(DIK_1))
	{
		g_nLastTime -= 10;
	}
	else if (GetKeyboardTrigger(DIK_2))
	{
		g_nLastTime += 10;
	}
}

//==================================================================================================
//描画処理
//==================================================================================================
void DrawTime(void)
{
	//デバイスポインタ設定
	LPDIRECT3DDEVICE9 pDevice;

	//変数宣言
	int nCntTime;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, g_apTextureTime[0]);
	
	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{//全ての弾をチェックする
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
	}

	//テクスチャ設定
	pDevice->SetTexture(0, g_apTextureTime[1]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);

	//テクスチャ設定
	pDevice->SetTexture(0, g_apTextureTime[2]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntTime + 1) * 4, 2);
}

//==================================================================================================
//設定処理
//==================================================================================================
void SetTime(int nTime)
{
	//変数宣言
	int aTexU[NUM_PLACE],nCount;
	int aData1, aData2;

	//初期スコアを入手
	g_nTime = nTime;

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCount = 0; nCount < NUM_PLACE / 2; nCount++)
	{//スコア表示
	 //各桁割り出し
		aData1 = pow(10.0, NUM_PLACE / 2 - nCount);
		aData2 = pow(10.0, NUM_PLACE / 2 - nCount - 1);

		aTexU[nCount] = g_nMinute % aData1 / aData2;

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 1.0f);

		//頂点情報のポインタを４つ分進める
		pVtx += 4;
	}

	for (; nCount < NUM_PLACE; nCount++)
	{//スコア表示
	 //各桁割り出し
		aData1 = pow(10.0, NUM_PLACE - nCount);
		aData2 = pow(10.0, NUM_PLACE - nCount - 1);

		aTexU[nCount] = g_nLastTime % aData1 / aData2;

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 1.0f);

		//頂点情報のポインタを４つ分進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffTime->Unlock();
}

//==================================================================================================
//スコア加算処理
//==================================================================================================
void AddTime(int nValue)
{
	//加算スコアを入手
	g_nTime += nValue;

	if (g_nTime >= 100000000)
	{//上限に達したら
		g_nTime = 99999999;
	}
}

//==================================================================================================
//タイム取得
//==================================================================================================
int GetTime(void)
{
	//変数宣言
	int nTime = g_nMinute * 60 + g_nLastTime;

	return nTime;
}

//==================================================================================================
//タイムアップ取得
//==================================================================================================
bool GetTimeUp(void)
{
	if (g_nMinute == 0 && g_nLastTime == 0)
	{//時間切れ
		return true;
	}
	else
	{
		return false;
	}
}

//==================================================================================================
//リザルト画面での更新処理
//==================================================================================================
void UpdateTimeResult(void)
{
	//変数宣言
	int aTexU[NUM_PLACE], nCount;
	int aData1, aData2;
	D3DXCOLOR UICol = { 1.0f,1.0f,1.0f,1.0f };

	//ポインタ宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nMinute == 0 && g_nLastTime < 30)
	{//赤警告
		UICol = { 1.0f,0.0f,0.0f,1.0f };
	}
	else if (g_nMinute == 0)
	{//黄色警告
		UICol = { 1.0f,0.86f,0.6f,1.0f };
	}

	for (nCount = 0; nCount < NUM_PLACE / 2; nCount++)
	{//スコア表示
	 //各桁割り出し
		aData1 = pow(10.0f, NUM_PLACE / 2 - nCount);
		aData2 = pow(10.0f, NUM_PLACE / 2 - nCount - 1);

		aTexU[nCount] = g_nMinute % aData1 / aData2;

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 1.0f);

		//頂点カラーの設定
		pVtx[0].col = UICol;
		pVtx[1].col = UICol;
		pVtx[2].col = UICol;
		pVtx[3].col = UICol;

		//頂点情報のポインタを４つ分進める
		pVtx += 4;
	}

	for (; nCount < NUM_PLACE; nCount++)
	{//スコア表示
	 //各桁割り出し
		aData1 = pow(10.0, NUM_PLACE - nCount);
		aData2 = pow(10.0, NUM_PLACE - nCount - 1);

		aTexU[nCount] = g_nLastTime % aData1 / aData2;

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aTexU[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aTexU[nCount] * 0.1f, 1.0f);

		//頂点カラーの設定
		pVtx[0].col = UICol;
		pVtx[1].col = UICol;
		pVtx[2].col = UICol;
		pVtx[3].col = UICol;

		//頂点情報のポインタを４つ分進める
		pVtx += 4;
	}

	//頂点カラーの設定
	pVtx[0].col = UICol;
	pVtx[1].col = UICol;
	pVtx[2].col = UICol;
	pVtx[3].col = UICol;

	//針までポインタを進める
	pVtx += 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z + g_clock.fAngle) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z + g_clock.fAngle) * g_clock.fLength,
		0.0f
	);
	pVtx[1].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z - g_clock.fAngle) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z - g_clock.fAngle) * g_clock.fLength,
		0.0f
	);
	pVtx[2].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z - g_clock.fAngle + D3DX_PI) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z - g_clock.fAngle + D3DX_PI) * g_clock.fLength,
		0.0f
	);
	pVtx[3].pos = D3DXVECTOR3
	(
		g_clock.pos.x + sinf(g_clock.rot.z + g_clock.fAngle - D3DX_PI) * g_clock.fLength,
		g_clock.pos.y + cosf(g_clock.rot.z + g_clock.fAngle - D3DX_PI) * g_clock.fLength,
		0.0f
	);

	//頂点カラーの設定
	/*pVtx[0].col = UICol;
	pVtx[1].col = UICol;
	pVtx[2].col = UICol;
	pVtx[3].col = UICol;*/

	//頂点バッファをアンロック
	g_pVtxBuffTime->Unlock();
}