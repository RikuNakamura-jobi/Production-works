//===========================
//
//ダイレクトX.areaファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "area.h"
#include "player.h"

//マクロ定義
#define NUM_AREA (3)												//背景の数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureArea[NUM_AREA] = {};					//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffArea = NULL;						//頂点情報を格納

//===========================
//背景の初期化処理
//===========================
void InitArea(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	Player *pPlayer = GetPlayer();
	int nCntArea;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\hitarea2 (2).png",
		&g_apTextureArea[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\hitarea2 (2).png",
		&g_apTextureArea[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\hitarea2 (2).png",
		&g_apTextureArea[2]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_AREA,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffArea,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffArea->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntArea = 0; nCntArea < NUM_AREA; nCntArea++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = pPlayer->pos.x - 87.0f;
		pVtx[0].pos.y = pPlayer->pos.y - 17.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pPlayer->pos.x - 25.0f;
		pVtx[1].pos.y = pPlayer->pos.y - 17.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pPlayer->pos.x - 87.0f;
		pVtx[2].pos.y = pPlayer->pos.y + 57.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pPlayer->pos.x - 25.0f;
		pVtx[3].pos.y = pPlayer->pos.y + 57.0f;
		pVtx[3].pos.z = 0.0f;

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
	g_VtxBuffArea->Unlock();
}

//===========================
//背景の終了処理
//===========================
void UninitArea(void)
{
	int nCntArea;

	for (nCntArea = 0; nCntArea < NUM_AREA; nCntArea++)
	{
		//テクスチャの破棄
		if (g_apTextureArea[nCntArea] != NULL)
		{
			g_apTextureArea[nCntArea]->Release();
			g_apTextureArea[nCntArea] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_VtxBuffArea != NULL)
	{
		g_VtxBuffArea->Release();
		g_VtxBuffArea = NULL;
	}
}

//===========================
//背景の更新処理
//===========================
void UpdateArea(void)
{
	
}

//===========================
//背景の描画処理
//===========================
void DrawArea(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntArea;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_VtxBuffArea,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntArea = 0; nCntArea < NUM_AREA; nCntArea++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureArea[nCntArea]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntArea * 4, 2);
	}
}