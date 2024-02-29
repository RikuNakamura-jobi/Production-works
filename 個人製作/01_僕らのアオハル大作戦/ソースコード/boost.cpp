//===========================
//
//ダイレクトX.Boostファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "boost.h"
#include "player.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureBoost = NULL;								//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoost = NULL;							//頂点情報を格納
D3DXVECTOR3 g_posBoost;													//位置
D3DXCOLOR g_colBoost;
int g_nBoost;															//体力の値

//===========================
//体力の初期化処理
//===========================
void InitBoost(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\boss001.png",
		&g_pTextureBoost);

	g_posBoost = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_colBoost = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	g_nBoost = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoost,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBoost->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_posBoost.x;
	pVtx[0].pos.y = g_posBoost.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posBoost.x + 64.0f;
	pVtx[1].pos.y = g_posBoost.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posBoost.x;
	pVtx[2].pos.y = g_posBoost.y + 5.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posBoost.x + 64.0f;
	pVtx[3].pos.y = g_posBoost.y + 5.0f;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = g_colBoost;
	pVtx[1].col = g_colBoost;
	pVtx[2].col = g_colBoost;
	pVtx[3].col = g_colBoost;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	//頂点バッファをアンロックする
	g_pVtxBuffBoost->Unlock();
}

//===========================
//体力の終了処理
//===========================
void UninitBoost(void)
{
	//テクスチャの破棄
	if (g_pTextureBoost != NULL)
	{
		g_pTextureBoost->Release();
		g_pTextureBoost = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBoost != NULL)
	{
		g_pVtxBuffBoost->Release();
		g_pVtxBuffBoost = NULL;
	}
}

//===========================
//体力の更新処理
//===========================
void UpdateBoost(void)
{
	Player *pPlayer = GetPlayer();

	g_posBoost.x = pPlayer->pos.x - 32.0f;
	g_posBoost.y = pPlayer->pos.y + 32.0f;
	g_posBoost.z = pPlayer->pos.z;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBoost->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_posBoost.x;
	pVtx[0].pos.y = g_posBoost.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posBoost.x + (0.4266666666666667f * g_nBoost);
	pVtx[1].pos.y = g_posBoost.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posBoost.x;
	pVtx[2].pos.y = g_posBoost.y + 5.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posBoost.x + +(0.4266666666666667f * g_nBoost);
	pVtx[3].pos.y = g_posBoost.y + 5.0f;
	pVtx[3].pos.z = 0.0f;

	if (pPlayer->playertype == PLAYERTYPE_3)
	{
		if (g_nBoost >= 150)
		{
			g_colBoost.a -= 0.1f;

			if (g_colBoost.a < 0.0f)
			{
				g_colBoost.a = 0.0f;
			}
		}
		else if (g_nBoost < 150)
		{
			g_colBoost.a = 1.0f;
		}
	}
	else
	{
		g_colBoost.a = 0.0f;
	}

	//頂点カラーの設定
	pVtx[0].col = g_colBoost;
	pVtx[1].col = g_colBoost;
	pVtx[2].col = g_colBoost;
	pVtx[3].col = g_colBoost;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.001f * g_nBoost, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.001f * g_nBoost, 1.0f);

	g_pVtxBuffBoost->Unlock();
}

//===========================
//体力の描画処理
//===========================
void DrawBoost(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffBoost,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBoost);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0 , 2);
}

//===========================
//体力の設定処理
//===========================
void SetBoost(int nBoost)
{
	g_nBoost = nBoost;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBoost->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_posBoost.x;
	pVtx[0].pos.y = g_posBoost.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posBoost.x + (0.4266666666666667f * g_nBoost);
	pVtx[1].pos.y = g_posBoost.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posBoost.x;
	pVtx[2].pos.y = g_posBoost.y + 5.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posBoost.x + +(0.4266666666666667f * g_nBoost);
	pVtx[3].pos.y = g_posBoost.y + 5.0f;
	pVtx[3].pos.z = 0.0f;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.015625f * g_nBoost, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.015625f * g_nBoost, 1.0f);

	g_pVtxBuffBoost->Unlock();
}

//===========================
//体力の追加処理
//===========================
void AddBoost(int nValue)
{
	Player *pPlayer = GetPlayer();
	g_nBoost -= nValue;

	g_posBoost.x = pPlayer->pos.x - 32.0f;
	g_posBoost.y = pPlayer->pos.y + 32.0f;
	g_posBoost.z = pPlayer->pos.z;

	if (g_nBoost >= 150)
	{
		g_nBoost = 150;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBoost->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_posBoost.x;
	pVtx[0].pos.y = g_posBoost.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posBoost.x + (0.4266666666666667f * g_nBoost);
	pVtx[1].pos.y = g_posBoost.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posBoost.x;
	pVtx[2].pos.y = g_posBoost.y + 5.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posBoost.x + +(0.4266666666666667f * g_nBoost);
	pVtx[3].pos.y = g_posBoost.y + 5.0f;
	pVtx[3].pos.z = 0.0f;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.015625f * g_nBoost, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.015625f * g_nBoost, 1.0f);

	g_pVtxBuffBoost->Unlock();
}

//===========================
//体力の取得処理
//===========================
int GetBoost(void)
{
	return g_nBoost;
}