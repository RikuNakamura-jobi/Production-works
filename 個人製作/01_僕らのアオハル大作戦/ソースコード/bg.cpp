//===========================
//
//ダイレクトX.bgファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "bg.h"

//マクロ定義
#define NUM_BG (3)												//背景の数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureBg[NUM_BG] = {};					//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffBg = NULL;						//頂点情報を格納
float g_aTexV[NUM_BG];											//テクスチャ座標の開始位置

//===========================
//背景の初期化処理
//===========================
void InitBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBg;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\aoharuBG000.png",
		&g_apTextureBg[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\aoharuBG001.png",
		&g_apTextureBg[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\aoharuBG000.png",
		&g_apTextureBg[2]);

	//テクスチャ座標の開始位置の初期化
	for (nCntBg = 0; nCntBg < NUM_BG; nCntBg++)
	{
		g_aTexV[nCntBg] = 0.9f;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffBg,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBg = 0; nCntBg < NUM_BG; nCntBg++)
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
	g_VtxBuffBg->Unlock();
}

//===========================
//背景の終了処理
//===========================
void UninitBg(void)
{
	int nCntBg;

	for (nCntBg = 0; nCntBg < NUM_BG; nCntBg++)
	{
		//テクスチャの破棄
		if (g_apTextureBg[nCntBg] != NULL)
		{
			g_apTextureBg[nCntBg]->Release();
			g_apTextureBg[nCntBg] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_VtxBuffBg != NULL)
	{
		g_VtxBuffBg->Release();
		g_VtxBuffBg = NULL;
	}
}

//===========================
//背景の更新処理
//===========================
void UpdateBg(void)
{
	int nCntBg;
	
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBg = 0; nCntBg < NUM_BG; nCntBg++)
	{
		//テクスチャ座標の開始位置の更新
		g_aTexV[nCntBg] += 0.0002f * nCntBg + 0.0002f;

		if (g_apTextureBg[nCntBg] <= 0)
		{
			g_aTexV[nCntBg] = 0.9f;
		}

		if (nCntBg == 1)
		{
			//テクスチャ座標の更新
			pVtx[0].tex = D3DXVECTOR2(g_aTexV[nCntBg], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aTexV[nCntBg] + 1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_aTexV[nCntBg], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_aTexV[nCntBg] + 1.0f, 1.0f);
		}
		else
		{
			//テクスチャ座標の更新
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_VtxBuffBg->Unlock();
}

//===========================
//背景の描画処理
//===========================
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBg;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_VtxBuffBg,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBg = 0; nCntBg < NUM_BG; nCntBg++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureBg[nCntBg]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg * 4, 2);
	}
}