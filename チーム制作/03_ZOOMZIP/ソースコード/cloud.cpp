//==========================================================
//
//ポリゴン処理 [Cloud.h]
//Author Ibuki Okusada
//
//==========================================================
#include "cloud.h"
#include "texture.h"
#include "fileload.h"
#include "texture.h"
#include "player.h"

//==========================================================
//マクロ定義
//==========================================================
#define MAX_POLYGON		(1)

//==========================================================
//グローバル変数
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureCloud[MAX_POLYGON] = {};			//テクスチャ1へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCloud = NULL;	//頂点バッファへのポインタ
Cloud g_aCloud[MAX_POLYGON];

//==========================================================
//ポリゴンの初期化処理
//==========================================================
void InitCloud(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	for (int nCntCloud = 0; nCntCloud < MAX_POLYGON; nCntCloud++)
	{
		if (GetMode() == MODE_GAME && nCntCloud == MAX_POLYGON - 1)
		{
			g_pTextureCloud[nCntCloud] = *SetTexture(2);
		}

		g_aCloud[nCntCloud].pos = {};
		g_aCloud[nCntCloud].rot = {};
		g_aCloud[nCntCloud].fWidth = 0.0f;
		g_aCloud[nCntCloud].fHeight = 0.0f;
		g_aCloud[nCntCloud].fTexU = 0.0f;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCloud,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCloud->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCloud = 0; nCntCloud < MAX_POLYGON; nCntCloud++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 700.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1280.0f, 700.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 920.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1280.0f, 920.0f, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(g_aCloud[nCntCloud].fTexU, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aCloud[nCntCloud].fTexU + 1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aCloud[nCntCloud].fTexU, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aCloud[nCntCloud].fTexU + 1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffCloud->Unlock();
}

//==========================================================
//ポリゴンの終了処理
//==========================================================
void UninitCloud(void)
{

	//テクスチャの破棄
	for (int nCntCloud = 0; nCntCloud < MAX_POLYGON; nCntCloud++)
	{
		if (g_pTextureCloud[nCntCloud] != NULL)
		{
			g_pTextureCloud[nCntCloud]->Release();
			g_pTextureCloud[nCntCloud] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCloud != NULL)
	{
		g_pVtxBuffCloud->Release();
		g_pVtxBuffCloud = NULL;
	}
}

//==========================================================
//ポリゴンの更新処理
//==========================================================
void UpdateCloud(void)
{
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	switch (GetMode())
	{
	case MODE_TITLE:	//タイトル画面
		g_aCloud[0].fTexU += 0.0003f;

		if (g_aCloud[0].fTexU > 1.0f)
		{
			g_aCloud[0].fTexU = 0.0f;
		}
		break;
	case MODE_TUTORIAL:	//チュートリアル画面
		break;
	case MODE_RESULT:	//リザルト画面
		g_aCloud[0].fTexU += 0.0003f;

		if (g_aCloud[0].fTexU > 1.0f)
		{
			g_aCloud[0].fTexU = 0.0f;
		}
		break;
	case MODE_RANKING:	//ランキング画面
		break;
	default:	//ゲーム画面
		if (GetPlayer()->move.x < 0.0f)
		{
			g_aCloud[0].fTexU += 0.0003f;
		}
		else
		{
			g_aCloud[0].fTexU += 0.0001f;
		}

		if (g_aCloud[0].fTexU > 1.0f)
		{
			g_aCloud[0].fTexU = 0.0f;
		}

		break;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCloud->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCloud = 0; nCntCloud < MAX_POLYGON; nCntCloud++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 680.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1280.0f, 680.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 850.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1280.0f, 850.0f, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(g_aCloud[0].fTexU, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aCloud[0].fTexU + 0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aCloud[0].fTexU, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aCloud[0].fTexU + 0.5f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffCloud->Unlock();
}

//==========================================================
//ポリゴンの描画処理
//==========================================================
void DrawCloud(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得

	for (int nCntCloud = 0; nCntCloud < MAX_POLYGON; nCntCloud++)
	{

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffCloud, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureCloud[nCntCloud]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
			4 * nCntCloud,								//プリミティブ（ポリゴンの数）
			2);												//描画するプリミティブ数
	}
}