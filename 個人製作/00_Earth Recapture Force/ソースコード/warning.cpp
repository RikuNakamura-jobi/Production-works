//===========================
//
//ダイレクトX.warningファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "warning.h"
#include "game.h"

//マクロ定義
#define NUM_WARNING (4)													//ボス警告の数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureWarning[NUM_WARNING] = {};				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffWarning = NULL;						//頂点情報を格納
float g_aTexVWarning[NUM_WARNING];										//テクスチャ座標の開始位置
float g_fCntWarning;													//警告点滅用変数
bool g_bUseWarning;														//警告描画判定

//===========================
//ボス警告の初期化処理
//===========================
void InitWarning(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntWarning;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\warning.png",
		&g_apTextureWarning[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\warning.png",
		&g_apTextureWarning[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\warning.png",
		&g_apTextureWarning[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\warning.png",
		&g_apTextureWarning[3]);

	//テクスチャ座標の開始位置の初期化
	for (nCntWarning = 0; nCntWarning < NUM_WARNING; nCntWarning++)
	{
		g_aTexVWarning[nCntWarning] = 0.9f;
	}

	g_bUseWarning = false;
	g_fCntWarning = 0.0f;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_WARNING,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffWarning,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffWarning->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWarning = 0; nCntWarning < NUM_WARNING; nCntWarning++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(300.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(980.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(300.0f, 720.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(980.0f, 720.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_VtxBuffWarning->Unlock();
}

//===========================
//ボス警告の終了処理
//===========================
void UninitWarning(void)
{
	int nCntWarning;

	for (nCntWarning = 0; nCntWarning < NUM_WARNING; nCntWarning++)
	{
		//テクスチャの破棄
		if (g_apTextureWarning[nCntWarning] != NULL)
		{
			g_apTextureWarning[nCntWarning]->Release();
			g_apTextureWarning[nCntWarning] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_VtxBuffWarning != NULL)
	{
		g_VtxBuffWarning->Release();
		g_VtxBuffWarning = NULL;
	}
}

//===========================
//ボス警告の更新処理
//===========================
void UpdateWarning(void)
{
	int nCntWarning;
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	col.a = (sinf(g_fCntWarning) + 1.0f) / 2;

	g_fCntWarning += 0.1f;
	
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffWarning->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWarning = 0; nCntWarning < NUM_WARNING; nCntWarning++)
	{
		if (g_bUseWarning == true)
		{
			//頂点カラーの設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_VtxBuffWarning->Unlock();
}

//===========================
//ボス警告の描画処理
//===========================
void DrawWarning(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	STAGESTATE stageState = GetStageState();

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_VtxBuffWarning,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_bUseWarning == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureWarning[stageState - 1]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (stageState - 1) * 4, 2);
	}
}

//===========================
//ボス警告の描画処理
//===========================
void SetWarning(void)
{
	g_bUseWarning = g_bUseWarning ? false : true;
}