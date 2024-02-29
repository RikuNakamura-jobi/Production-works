//===========================
//
//ダイレクトX.lifeファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "life.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;								//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;							//頂点情報を格納
D3DXVECTOR3 g_posLife;													//位置
int g_nLife;															//体力の値

//===========================
//体力の初期化処理
//===========================
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\life001.png",
		&g_pTextureLife);

	g_posLife = D3DXVECTOR3(1000.0f, 170.0f, 0.0f);
	g_nLife = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_posLife.x;
	pVtx[0].pos.y = g_posLife.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posLife.x + 65.0f;
	pVtx[1].pos.y = g_posLife.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posLife.x;
	pVtx[2].pos.y = g_posLife.y + 65.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posLife.x + 65.0f;
	pVtx[3].pos.y = g_posLife.y + 65.0f;
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


	//頂点バッファをアンロックする
	g_pVtxBuffLife->Unlock();
}

//===========================
//体力の終了処理
//===========================
void UninitLife(void)
{
	//テクスチャの破棄
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//===========================
//体力の更新処理
//===========================
void UpdateLife(void)
{
	
}

//===========================
//体力の描画処理
//===========================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffLife,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureLife);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0 , 2);
}

//===========================
//体力の設定処理
//===========================
void SetLife(int nLife)
{
	g_nLife = nLife;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_posLife.x;
	pVtx[0].pos.y = g_posLife.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posLife.x + 13.0f * g_nLife;
	pVtx[1].pos.y = g_posLife.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posLife.x;
	pVtx[2].pos.y = g_posLife.y + 65.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posLife.x + 13.0f * g_nLife;
	pVtx[3].pos.y = g_posLife.y + 65.0f;
	pVtx[3].pos.z = 0.0f;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.2f * g_nLife, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.2f * g_nLife, 1.0f);

	g_pVtxBuffLife->Unlock();
}

//===========================
//体力の追加処理
//===========================
void AddLife(int nValue)
{
	g_nLife -= nValue;

	if (g_nLife > 5)
	{
		g_nLife = 5;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_posLife.x;
	pVtx[0].pos.y = g_posLife.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posLife.x + 13.0f * g_nLife;
	pVtx[1].pos.y = g_posLife.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posLife.x;
	pVtx[2].pos.y = g_posLife.y + 65.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posLife.x + 13.0f * g_nLife;
	pVtx[3].pos.y = g_posLife.y + 65.0f;
	pVtx[3].pos.z = 0.0f;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.2f * g_nLife, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.2f * g_nLife, 1.0f);

	g_pVtxBuffLife->Unlock();
}

//===========================
//体力の取得処理
//===========================
int GetLife(void)
{
	return g_nLife;
}