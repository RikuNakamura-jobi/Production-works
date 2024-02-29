//==================================================================================================
//
//point_log.cpp
//Author:髙山桃也
//
//==================================================================================================

#include "main.h"
#include "point_log.h"
#include "point.h"

//==============================
// グローバル変数宣言
//==============================
#define LOG_WIDTH	(500)	//横幅
#define LOG_HEIGHT	(100)	//高さ

//==============================
// グローバル変数宣言
//==============================
LPDIRECT3DTEXTURE9 g_apTexturePointLog[LOGTYPE_MAX] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPointLog = NULL;	//頂点バッファへのポインタ
PointLog g_aPointLog[MAX_POINT_NUM];	//構造体情報
const char *c_apFilenameMessage[LOGTYPE_MAX] =
{//Xファイルパス
	"data\\TEXTURE\\PointMessage\\pointmessage_000.png",
	"data\\TEXTURE\\PointMessage\\pointmessage_001.png",
	"data\\TEXTURE\\PointMessage\\pointmessage_002.png",
	"data\\TEXTURE\\PointMessage\\pointmessage_003.png",
	"data\\TEXTURE\\PointMessage\\pointmessage_004.png",
	"data\\TEXTURE\\PointMessage\\pointmessage_005.png",
	"data\\TEXTURE\\PointMessage\\pointmessage_006.png",
	"data\\TEXTURE\\PointMessage\\pointmessage_007.png",
};

//==================================================================================================
// 初期化処理
//==================================================================================================
void InitPointLog(void)
{
	//変数初期化
	ZeroMemory(&g_aPointLog[0],sizeof(PointLog) * MAX_POINT_NUM);

	//デバイスポインタ設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntTex = 0; nCntTex < LOGTYPE_MAX;nCntTex++)
	{
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameMessage[nCntTex],
			&g_apTexturePointLog[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POINT_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPointLog,
		NULL);

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPointLog->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPoint = 0;nCntPoint < MAX_POINT_NUM;nCntPoint++)
	{
		g_aPointLog[nCntPoint].col = D3DXCOLOR{1.0f,1.0f,1.0f,1.0f};

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

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//ポインタを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffPointLog->Unlock();
}

//==================================================================================================
//終了処理
//==================================================================================================
void UninitPointLog(void)
{
	for (int nCntTex = 0; nCntTex < LOGTYPE_MAX; nCntTex++)
	{
		if (g_apTexturePointLog[nCntTex] != NULL)
		{//テクスチャへのポインタの破棄
			g_apTexturePointLog[nCntTex]->Release();
			g_apTexturePointLog[nCntTex] = NULL;
		}
	}
	
	if (g_pVtxBuffPointLog != NULL)
	{//頂点バッファへのポインタの破棄
		g_pVtxBuffPointLog->Release();
		g_pVtxBuffPointLog = NULL;
	}
}

//==================================================================================================
// 更新処理
//==================================================================================================
void UpdatePointLog(void)
{
	//情報取得
	PointLog *pPointLog = GetPointLog();

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPointLog->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPointLog = 0; nCntPointLog < MAX_POINT_NUM; nCntPointLog++, pPointLog++)
	{
		if (pPointLog->bUse)
		{//使用されている状態

			if (pPointLog->type == LOGTYPE_TOTALSCORE)
			{
				//相対位置の設定
				pPointLog->pos =
				{
					pPointLog->pPoint->pos.x - LOG_WIDTH * pPointLog->fScale * 0.7f,
					pPointLog->pPoint->pos.y,
					0.0f
				};
			}
			else
			{
				//相対位置の設定
				pPointLog->pos =
				{
					pPointLog->pPoint->pos.x - LOG_WIDTH * pPointLog->fScale,
					pPointLog->pPoint->pos.y,
					0.0f
				};
			}

			//色の設定
			pPointLog->col.a = pPointLog->pPoint->col.a;

			//死ぬときは一緒だよ処理
			pPointLog->bUse = pPointLog->pPoint->bUse;

			//頂点情報の更新===========================================

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pPointLog->pos.x, pPointLog->pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pPointLog->pos.x + LOG_WIDTH * pPointLog->fScale, pPointLog->pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pPointLog->pos.x, pPointLog->pos.y + LOG_HEIGHT * pPointLog->fScale, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pPointLog->pos.x + LOG_WIDTH * pPointLog->fScale, pPointLog->pos.y + LOG_HEIGHT * pPointLog->fScale, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = pPointLog->col;
			pVtx[1].col = pPointLog->col;
			pVtx[2].col = pPointLog->col;
			pVtx[3].col = pPointLog->col;

			//頂点情報の更新===========================================
		}

		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffPointLog->Unlock();
}

//==================================================================================================
// 描画処理
//==================================================================================================
void DrawPointLog(void)
{
	//情報取得
	PointLog *pPointLog = GetPointLog();

	//デバイスポインタ設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPointLog, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPointLog = 0;nCntPointLog < MAX_POINT_NUM;nCntPointLog++, pPointLog++)
	{
		if (pPointLog->bUse)
		{//使用されている状態
			//テクスチャ設定
			pDevice->SetTexture(0, g_apTexturePointLog[pPointLog->type]);

			//背景の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntPointLog, 2);
		}
	}
}

//==================================================================================================
// 設定処理
//==================================================================================================
void SetPointLog(Point *pPoint, LOGTYPE type, float fScale)
{
	//情報取得
	PointLog *pPointLog = GetPointLog();

	for (int nCntPointLog = 0;nCntPointLog < MAX_POINT_NUM;nCntPointLog++, pPointLog++)
	{
		if (pPointLog->bUse == false)
		{//使用されていないなら

			//引数受け取り
			pPointLog->pPoint = pPoint;
			pPointLog->type = type;
			pPointLog->fScale = fScale;

			//使用している状態にする
			pPointLog->bUse = true;

			break;
		}
	}
}

//==================================================================================================
// 情報取得
//==================================================================================================
PointLog *GetPointLog(void)
{
	return &g_aPointLog[0];
}