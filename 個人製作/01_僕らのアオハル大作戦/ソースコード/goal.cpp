//===========================
//
//ダイレクトX.goalファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "goal.h"
#include "player.h"

//マクロ定義
#define MAX_GOAL (256)

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureGoal = NULL;				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffGoal = NULL;			//頂点情報を格納
Goal g_aGoal[MAX_GOAL];								//敵の情報

//テクスチャファイル名
const char *c_apFilenameGoal[1] =
{
	"data\\TEXTURE\\goal000.png"
};

//===========================
//敵の初期化処理
//===========================
void InitGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntGoal;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\goal000.png",
		&g_pTextureGoal);

	for (nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		g_aGoal[nCntGoal].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aGoal[nCntGoal].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aGoal[nCntGoal].fHeight = 80.0f;
		g_aGoal[nCntGoal].fWidth = 80.0f;
		g_aGoal[nCntGoal].bUse = false;					//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GOAL,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffGoal,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_aGoal[nCntGoal].pos.x;
		pVtx[0].pos.y = g_aGoal[nCntGoal].pos.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aGoal[nCntGoal].pos.x + g_aGoal[nCntGoal].fWidth;
		pVtx[1].pos.y = g_aGoal[nCntGoal].pos.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aGoal[nCntGoal].pos.x;
		pVtx[2].pos.y = g_aGoal[nCntGoal].pos.y + g_aGoal[nCntGoal].fHeight;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aGoal[nCntGoal].pos.x + g_aGoal[nCntGoal].fWidth;
		pVtx[3].pos.y = g_aGoal[nCntGoal].pos.y + g_aGoal[nCntGoal].fHeight;
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
	g_VtxBuffGoal->Unlock();
}

//===========================
//敵の終了処理
//===========================
void UninitGoal(void)
{
	//テクスチャの破棄
	if (g_pTextureGoal != NULL)
	{
		g_pTextureGoal->Release();
		g_pTextureGoal = NULL;
	}

	//頂点バッファの破棄
	if (g_VtxBuffGoal != NULL)
	{
		g_VtxBuffGoal->Release();
		g_VtxBuffGoal = NULL;
	}
}

//===========================
//敵の更新処理
//===========================
void UpdateGoal(void)
{
	
}

//===========================
//敵の描画処理
//===========================
void DrawGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntGoal;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_VtxBuffGoal,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		if (g_aGoal[nCntGoal].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureGoal);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGoal * 4, 2);
		}
	}
}

//===========================
//敵の設定処理
//===========================
void SetGoal(D3DXVECTOR3 pos, int nType)
{
	int nCntGoal;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		if (g_aGoal[nCntGoal].bUse == false)
		{
			g_aGoal[nCntGoal].pos = pos;
			g_aGoal[nCntGoal].nType = nType;
			g_aGoal[nCntGoal].bUse = true;

			//頂点座標の設定
			pVtx[0].pos.x = g_aGoal[nCntGoal].pos.x;
			pVtx[0].pos.y = g_aGoal[nCntGoal].pos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aGoal[nCntGoal].pos.x + g_aGoal[nCntGoal].fWidth;
			pVtx[1].pos.y = g_aGoal[nCntGoal].pos.y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aGoal[nCntGoal].pos.x;
			pVtx[2].pos.y = g_aGoal[nCntGoal].pos.y + g_aGoal[nCntGoal].fHeight;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aGoal[nCntGoal].pos.x + g_aGoal[nCntGoal].fWidth;
			pVtx[3].pos.y = g_aGoal[nCntGoal].pos.y + g_aGoal[nCntGoal].fHeight;
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

			break;
		}

		pVtx += 4;
	}

	g_VtxBuffGoal->Unlock();
}

//===========================
//プレイヤーとの当たり判定
//===========================
bool CollisionGoal(D3DXVECTOR3 *pPos)
{
	bool bGoal = false;

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		if (g_aGoal[nCntGoal].bUse == true)
		{
			if (((pPos->x - (g_aGoal[nCntGoal].pos.x + 40.0f)) * (pPos->x - (g_aGoal[nCntGoal].pos.x + 40.0f))) + ((pPos->y - (g_aGoal[nCntGoal].pos.y + 40.0f)) * (pPos->y - (g_aGoal[nCntGoal].pos.y + 40.0f))) < (32.0f * 32.0f))
			{
				bGoal = true;
			}
		}
	}

	return bGoal;
}