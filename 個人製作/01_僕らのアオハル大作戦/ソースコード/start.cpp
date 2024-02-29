//===========================
//
//ダイレクトX.startファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "start.h"
#include "player.h"

//マクロ定義
#define MAX_START (256)

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureStart = NULL;				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffStart = NULL;			//頂点情報を格納
Start g_aStart[MAX_START];								//敵の情報

//テクスチャファイル名
const char *c_apFilenameStart[1] =
{
	"data\\TEXTURE\\start000.png"
};

//===========================
//敵の初期化処理
//===========================
void InitStart(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntStart;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\start000.png",
		&g_pTextureStart);

	for (nCntStart = 0; nCntStart < MAX_START; nCntStart++)
	{
		g_aStart[nCntStart].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aStart[nCntStart].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aStart[nCntStart].fHeight = 80.0f;
		g_aStart[nCntStart].fWidth = 80.0f;
		g_aStart[nCntStart].bUse = false;					//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_START,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffStart,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStart = 0; nCntStart < MAX_START; nCntStart++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_aStart[nCntStart].pos.x;
		pVtx[0].pos.y = g_aStart[nCntStart].pos.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aStart[nCntStart].pos.x + g_aStart[nCntStart].fWidth;
		pVtx[1].pos.y = g_aStart[nCntStart].pos.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aStart[nCntStart].pos.x;
		pVtx[2].pos.y = g_aStart[nCntStart].pos.y + g_aStart[nCntStart].fHeight;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aStart[nCntStart].pos.x + g_aStart[nCntStart].fWidth;
		pVtx[3].pos.y = g_aStart[nCntStart].pos.y + g_aStart[nCntStart].fHeight;
		pVtx[3].pos.z = 0.0f;

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
	g_VtxBuffStart->Unlock();
}

//===========================
//敵の終了処理
//===========================
void UninitStart(void)
{
	//テクスチャの破棄
	if (g_pTextureStart != NULL)
	{
		g_pTextureStart->Release();
		g_pTextureStart = NULL;
	}

	//頂点バッファの破棄
	if (g_VtxBuffStart != NULL)
	{
		g_VtxBuffStart->Release();
		g_VtxBuffStart = NULL;
	}
}

//===========================
//敵の更新処理
//===========================
void UpdateStart(void)
{

}

//===========================
//敵の描画処理
//===========================
void DrawStart(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntStart;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_VtxBuffStart,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntStart = 0; nCntStart < MAX_START; nCntStart++)
	{
		if (g_aStart[nCntStart].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureStart);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntStart * 4, 2);
		}
	}
}

//===========================
//敵の設定処理
//===========================
void SetStart(D3DXVECTOR3 pos, int nType)
{
	int nCntStart;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStart = 0; nCntStart < MAX_START; nCntStart++)
	{
		if (g_aStart[nCntStart].bUse == false)
		{
			g_aStart[nCntStart].pos = pos;
			g_aStart[nCntStart].nType = nType;
			g_aStart[nCntStart].bUse = true;

			SetPlayer(D3DXVECTOR3 (pos.x + 40.0f, pos.y + 40.0f, 0.0f));

			//頂点座標の設定
			pVtx[0].pos.x = g_aStart[nCntStart].pos.x;
			pVtx[0].pos.y = g_aStart[nCntStart].pos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aStart[nCntStart].pos.x + g_aStart[nCntStart].fWidth;
			pVtx[1].pos.y = g_aStart[nCntStart].pos.y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aStart[nCntStart].pos.x;
			pVtx[2].pos.y = g_aStart[nCntStart].pos.y + g_aStart[nCntStart].fHeight;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aStart[nCntStart].pos.x + g_aStart[nCntStart].fWidth;
			pVtx[3].pos.y = g_aStart[nCntStart].pos.y + g_aStart[nCntStart].fHeight;
			pVtx[3].pos.z = 0.0f;

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

			break;
		}

		pVtx += 4;
	}

	g_VtxBuffStart->Unlock();
}

//===========================
//プレイヤーとの当たり判定
//===========================
bool CollisionStart(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fAngle, float fLength, float fRot, Start **pStart)
{
	bool bLand = false;

	for (int nCntStart = 0; nCntStart < MAX_START; nCntStart++)
	{
		if (g_aStart[nCntStart].bUse == true)
		{
			if ((pPos->x + 24.0f > g_aStart[nCntStart].pos.x && pPos->x - 24.0f < g_aStart[nCntStart].pos.x + g_aStart[nCntStart].fWidth) && (pPos->y + 32.0f > g_aStart[nCntStart].pos.y && pPosOld->y + 32.0f <= g_aStart[nCntStart].pos.y) && pPos->y + 32.0f < g_aStart[nCntStart].pos.y + g_aStart[nCntStart].fHeight)
			{
				bLand = true;
			}
			else if ((pPos->x + 24.0f > g_aStart[nCntStart].pos.x && pPos->x - 24.0f < g_aStart[nCntStart].pos.x + g_aStart[nCntStart].fWidth) && (pPos->y - 32.0f < g_aStart[nCntStart].pos.y + g_aStart[nCntStart].fHeight && pPosOld->y - 32.0f >= g_aStart[nCntStart].pos.y + g_aStart[nCntStart].fHeight) && pPos->y - 32.0f > g_aStart[nCntStart].pos.y)
			{
				bLand = true;
			}
		}
	}

	return bLand;
}