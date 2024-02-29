//===========================
//
//ダイレクトX.remainファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "remain.h"

//マクロ定義
#define NUM_PLACE_REMAIN (3)											//残機の最大数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureRemain = NULL;								//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRemain = NULL;						//頂点情報を格納
D3DXVECTOR3 g_posRemain;												//位置
int g_nRemain;															//残機の値

//===========================
//残機の初期化処理
//===========================
void InitRemain(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntRemain;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Player002.png",
		&g_pTextureRemain);

	g_posRemain = D3DXVECTOR3(1000.0f, 240.0f, 0.0f);
	g_nRemain = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE_REMAIN,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffRemain,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRemain->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRemain = 0; nCntRemain < NUM_PLACE_REMAIN; nCntRemain++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_posRemain.x + (80.0f * nCntRemain);
		pVtx[0].pos.y = g_posRemain.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posRemain.x + (80.0f * nCntRemain) + 80.0f;
		pVtx[1].pos.y = g_posRemain.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posRemain.x + (80.0f * nCntRemain);
		pVtx[2].pos.y = g_posRemain.y + 80.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posRemain.x + (80.0f * nCntRemain) + 80.0f;
		pVtx[3].pos.y = g_posRemain.y + 80.0f;
		pVtx[3].pos.z = 0.0f;

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

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;										//頂点座標のポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRemain->Unlock();
}

//===========================
//残機の終了処理
//===========================
void UninitRemain(void)
{
	//テクスチャの破棄
	if (g_pTextureRemain != NULL)
	{
		g_pTextureRemain->Release();
		g_pTextureRemain = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRemain != NULL)
	{
		g_pVtxBuffRemain->Release();
		g_pVtxBuffRemain = NULL;
	}
}

//===========================
//残機の更新処理
//===========================
void UpdateRemain(void)
{
	
}

//===========================
//残機の描画処理
//===========================
void DrawRemain(void)
{
	int nCntRemain;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffRemain,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRemain);

	for (nCntRemain = 0; nCntRemain < NUM_PLACE_REMAIN; nCntRemain++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRemain, 2);
	}
}

//===========================
//残機の設定処理
//===========================
void SetRemain(int nRemain)
{
	int nCntRemain;

	g_nRemain = nRemain;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRemain->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRemain = 0; nCntRemain < NUM_PLACE_REMAIN; nCntRemain++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_posRemain.x + (80.0f * nCntRemain);
		pVtx[0].pos.y = g_posRemain.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posRemain.x + (80.0f * nCntRemain) + 80.0f;
		pVtx[1].pos.y = g_posRemain.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posRemain.x + (80.0f * nCntRemain);
		pVtx[2].pos.y = g_posRemain.y + 80.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posRemain.x + (80.0f * nCntRemain) + 80.0f;
		pVtx[3].pos.y = g_posRemain.y + 80.0f;
		pVtx[3].pos.z = 0.0f;

		pVtx += 4;
	}

	g_pVtxBuffRemain->Unlock();
}

//===========================
//残機の追加処理
//===========================
void AddRemain(int nValue)
{
	int nCntRemain;

	g_nRemain -= nValue;

	if (g_nRemain > 3)
	{
		g_nRemain = 3;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRemain->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRemain = 0; nCntRemain < NUM_PLACE_REMAIN; nCntRemain++)
	{
		if (nCntRemain < g_nRemain)
		{
			//頂点座標の設定
			pVtx[0].pos.x = g_posRemain.x + (80.0f * nCntRemain);
			pVtx[0].pos.y = g_posRemain.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_posRemain.x + (80.0f * nCntRemain) + 80.0f;
			pVtx[1].pos.y = g_posRemain.y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_posRemain.x + (80.0f * nCntRemain);
			pVtx[2].pos.y = g_posRemain.y + 80.0f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_posRemain.x + (80.0f * nCntRemain) + 80.0f;
			pVtx[3].pos.y = g_posRemain.y + 80.0f;
			pVtx[3].pos.z = 0.0f;
		}
		else
		{
			//頂点座標の設定
			pVtx[0].pos.x = g_posRemain.x;
			pVtx[0].pos.y = g_posRemain.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_posRemain.x;
			pVtx[1].pos.y = g_posRemain.y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_posRemain.x;
			pVtx[2].pos.y = g_posRemain.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_posRemain.x;
			pVtx[3].pos.y = g_posRemain.y;
			pVtx[3].pos.z = 0.0f;
		}
		

		pVtx += 4;
	}

	g_pVtxBuffRemain->Unlock();
}

//===========================
//残機の取得処理
//===========================
int GetRemain(void)
{
	return g_nRemain;
}