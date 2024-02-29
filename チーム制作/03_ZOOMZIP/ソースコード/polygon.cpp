//==========================================================
//
//ポリゴン処理 [polygon.h]
//Author Ibuki Okusada
//
//==========================================================
#include "polygon.h"
#include "texture.h"
#include "fileload.h"
#include "texture.h"
#include "player.h"

//==========================================================
//マクロ定義
//==========================================================
#define MAX_POLYGON		(3)

//==========================================================
//グローバル変数
//==========================================================
LPDIRECT3DTEXTURE9 g_pTexturePolygon[MAX_POLYGON] = {};			//テクスチャ1へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;	//頂点バッファへのポインタ
polygon g_aPolygon[MAX_POLYGON];
float g_fPolyTexU = 0.0f;

//==========================================================
//ポリゴンの初期化処理
//==========================================================
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得
	VERTEX_2D *pVtx;				//頂点情報へのポインタ
	g_fPolyTexU = 0.0f;

	//ファイルの初期化
	InitFileLoad();
	GametxtFileLoad();

	//テクスチャの初期化
	InitTexture();

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (GetMode() == MODE_GAME && nCntPolygon < MAX_POLYGON - 1)
		{
			g_pTexturePolygon[nCntPolygon] = *SetTexture(nCntPolygon);
		}
		else
		{
			g_pTexturePolygon[nCntPolygon] = *SetTexture(0);
		}

		g_aPolygon[nCntPolygon].pos = {};
		g_aPolygon[nCntPolygon].rot = {};
		g_aPolygon[nCntPolygon].fWidth = 0.0f;
		g_aPolygon[nCntPolygon].fHeight = 0.0f;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(g_fPolyTexU, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_fPolyTexU + 1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_fPolyTexU, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_fPolyTexU + 1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();
}

//==========================================================
//ポリゴンの終了処理
//==========================================================
void UninitPolygon(void)
{
	//テクスチャの終了処理
	UninitTexture();

	//テクスチャの破棄
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (g_pTexturePolygon[nCntPolygon] != NULL)
		{
			g_pTexturePolygon[nCntPolygon]->Release();
			g_pTexturePolygon[nCntPolygon] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//==========================================================
//ポリゴンの更新処理
//==========================================================
void UpdatePolygon(void)
{
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	switch (GetMode())
	{
	case MODE_TITLE:	//タイトル画面
		g_fPolyTexU += 0.0003f;

		if (g_fPolyTexU > 1.0f)
		{
			g_fPolyTexU = 0.0f;
		}
		break;
	case MODE_TUTORIAL:	//チュートリアル画面
		break;
	case MODE_RESULT:	//リザルト画面
		g_fPolyTexU += 0.0003f;

		if (g_fPolyTexU > 1.0f)
		{
			g_fPolyTexU = 0.0f;
		}
		break;
	case MODE_RANKING:	//ランキング画面
		break;
	default:	//ゲーム画面
		if (GetPlayer()->move.x < 0.0f)
		{
			g_fPolyTexU += 0.0003f;
		}
		else
		{
			g_fPolyTexU += 0.0001f;
		}

		if (g_fPolyTexU > 1.0f)
		{
			g_fPolyTexU = 0.0f;
		}

		break;
	}

	if (MODE_TUTORIAL != GetMode())
	{
		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
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
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(g_fPolyTexU, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_fPolyTexU + 0.5f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_fPolyTexU, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_fPolyTexU + 0.5f, 1.0f);

			pVtx += 4;
		}

		//頂点バッファをアンロックする
		g_pVtxBuffPolygon->Unlock();
	}
}

//==========================================================
//ポリゴンの描画処理
//==========================================================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (GetMode() == MODE_GAME && nCntPolygon >= MAX_POLYGON - 1)
		{
			break;
		}

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygon[nCntPolygon]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
			4 * nCntPolygon,								//プリミティブ（ポリゴンの数）
			2);												//描画するプリミティブ数
	}
}