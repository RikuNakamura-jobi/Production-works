//===========================
//
//ダイレクトX.coinファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "coin.h"
#include "player.h"

//マクロ定義
#define MAX_COIN (256)

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureCoin = NULL;				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffCoin = NULL;			//頂点情報を格納
Coin g_aCoin[MAX_COIN];								//敵の情報
int g_nNumCoin;

//テクスチャファイル名
const char *c_apFilenameCoin[1] =
{
	"data\\TEXTURE\\coin000.png"
};

//===========================
//敵の初期化処理
//===========================
void InitCoin(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntCoin;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Coin000.png",
		&g_pTextureCoin);

	for (nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++)
	{
		g_aCoin[nCntCoin].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCntCoin].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCntCoin].fHeight = 80.0f;
		g_aCoin[nCntCoin].fWidth = 80.0f;
		g_aCoin[nCntCoin].bUse = false;					//使用していない状態にする
	}

	g_nNumCoin = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_COIN,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffCoin,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffCoin->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_aCoin[nCntCoin].pos.x;
		pVtx[0].pos.y = g_aCoin[nCntCoin].pos.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aCoin[nCntCoin].pos.x + g_aCoin[nCntCoin].fWidth;
		pVtx[1].pos.y = g_aCoin[nCntCoin].pos.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aCoin[nCntCoin].pos.x;
		pVtx[2].pos.y = g_aCoin[nCntCoin].pos.y + g_aCoin[nCntCoin].fHeight;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aCoin[nCntCoin].pos.x + g_aCoin[nCntCoin].fWidth;
		pVtx[3].pos.y = g_aCoin[nCntCoin].pos.y + g_aCoin[nCntCoin].fHeight;
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
	g_VtxBuffCoin->Unlock();
}

//===========================
//敵の終了処理
//===========================
void UninitCoin(void)
{
	//テクスチャの破棄
	if (g_pTextureCoin != NULL)
	{
		g_pTextureCoin->Release();
		g_pTextureCoin = NULL;
	}

	//頂点バッファの破棄
	if (g_VtxBuffCoin != NULL)
	{
		g_VtxBuffCoin->Release();
		g_VtxBuffCoin = NULL;
	}
}

//===========================
//敵の更新処理
//===========================
void UpdateCoin(void)
{

}

//===========================
//敵の描画処理
//===========================
void DrawCoin(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntCoin;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_VtxBuffCoin,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++)
	{
		if (g_aCoin[nCntCoin].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureCoin);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCoin * 4, 2);
		}
	}
}

//===========================
//敵の設定処理
//===========================
void SetCoin(D3DXVECTOR3 pos, int nType)
{
	int nCntCoin;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffCoin->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++)
	{
		if (g_aCoin[nCntCoin].bUse == false)
		{
			g_aCoin[nCntCoin].pos = pos;
			g_aCoin[nCntCoin].nType = nType;
			g_aCoin[nCntCoin].bUse = true;

			//頂点座標の設定
			pVtx[0].pos.x = g_aCoin[nCntCoin].pos.x;
			pVtx[0].pos.y = g_aCoin[nCntCoin].pos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aCoin[nCntCoin].pos.x + g_aCoin[nCntCoin].fWidth;
			pVtx[1].pos.y = g_aCoin[nCntCoin].pos.y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aCoin[nCntCoin].pos.x;
			pVtx[2].pos.y = g_aCoin[nCntCoin].pos.y + g_aCoin[nCntCoin].fHeight;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aCoin[nCntCoin].pos.x + g_aCoin[nCntCoin].fWidth;
			pVtx[3].pos.y = g_aCoin[nCntCoin].pos.y + g_aCoin[nCntCoin].fHeight;
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

			g_nNumCoin++;
			break;
		}

		pVtx += 4;
	}

	g_VtxBuffCoin->Unlock();
}

//===========================
//プレイヤーとの当たり判定
//===========================
bool CollisionCoin(D3DXVECTOR3 *pPos)
{
	bool bCoin = false;

	for (int nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++)
	{
		if (g_aCoin[nCntCoin].bUse == true)
		{
			if (((pPos->x - (g_aCoin[nCntCoin].pos.x + 40.0f)) * (pPos->x - (g_aCoin[nCntCoin].pos.x + 40.0f))) + ((pPos->y - (g_aCoin[nCntCoin].pos.y + 60.0f)) * (pPos->y - (g_aCoin[nCntCoin].pos.y + 60.0f))) < (64.0f * 64.0f))
			{
				bCoin = true;
				g_aCoin[nCntCoin].bUse = false;
				g_nNumCoin--;
			}
		}
	}

	return bCoin;
}

//===========================
//敵の全滅判定処理
//===========================
int GetNumCoin(void)
{
	return g_nNumCoin;
}