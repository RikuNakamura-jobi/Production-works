//==================================================================================================
//
//effect_2D.cpp
//Author:髙山桃也		edit: 髙田佳依
//
//==================================================================================================

#include "main.h"
#include "effect_2D.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_EFFECT2D		(4096)										//最大数
#define LIFE_SPEED		(10)										//寿命の減る速度
#define EFFECT2D_TEX_FILE		"data\\TEXTURE\\effect000.png"		//テクスチャファイル名
#define MOVE_FACT		(0.96f)										//移動減衰係数

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureEffect2D = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect2D = NULL;	//頂点バッファへのポインタ
Effect2D g_aEffect2D[MAX_EFFECT2D];						//エフェクトの情報

//==========================================
//  プロトタイプ宣言
//==========================================
void UpdateEffect2DPos(Effect2D *pEffect2D);
void UpdateEffect2DPolygon(Effect2D *pEffect2D,int nCntEffect2D);
void DebugEffect2D(void);

//==================================================================================================
//初期化処理
//==================================================================================================
void InitEffect2D(void)
{
	//デバイスポインタ設定
	LPDIRECT3DDEVICE9 pDevice;

	//ポインタ宣言
	Effect2D *pEffect2D = GetEffect2D();

	//変数宣言
	int nCntEffect2D;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		EFFECT2D_TEX_FILE,
		&g_pTextureEffect2D);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT2D,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect2D,
		NULL);

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect2D->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT2D; nCntEffect2D++,pEffect2D++)
	{//全ての弾の初期化
	    //頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pEffect2D->fRadius = 0.0f;
		pEffect2D->col = D3DXCOLOR{ 1.0f,1.0f,1.0f,1.0f };
		pEffect2D->pos = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
		pEffect2D->relPos = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
		pEffect2D->move = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
		pEffect2D->nLife = 0;
		pEffect2D->nMaxLife = 0;
		pEffect2D->bUse = false; //エフェクトを使用していない状態にする
		pEffect2D->fGrav = 0.0f;

		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffEffect2D->Unlock();
}

//==================================================================================================
//終了処理
//==================================================================================================
void UninitEffect2D(void)
{
	if (g_pTextureEffect2D != NULL)
	{//テクスチャへのポインタ破棄
		g_pTextureEffect2D->Release();
		g_pTextureEffect2D = NULL;
	}

	if (g_pVtxBuffEffect2D != NULL)
	{//頂点バッファへのポインタ破棄
		g_pVtxBuffEffect2D->Release();
		g_pVtxBuffEffect2D = NULL;
	}
}

//==================================================================================================
//更新処理
//==================================================================================================
void UpdateEffect2D(void)
{
	//変数宣言
	int nCntEffect2D;
	
	//ポインタ宣言
	Effect2D *pEffect2D = GetEffect2D();

	for (nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT2D; nCntEffect2D++, pEffect2D++)
	{//全ての更新

		if (pEffect2D->bUse)
		{//使用されている状態なら

			//位置更新処理
			UpdateEffect2DPos(pEffect2D);
			
			//半径の更新(ポリゴンサイズの更新)
			pEffect2D->fRadius *= ((float)pEffect2D->nLife / (float)pEffect2D->nMaxLife);

			//寿命カウントダウン
			pEffect2D->nLife--;

			if (pEffect2D->fRadius <= 0.3f)
			{
			 //使用していない状態にする
				pEffect2D->bUse = false;
			}

			//ポリゴン更新処理
			UpdateEffect2DPolygon(pEffect2D, nCntEffect2D);
		}
	}
}

//==================================================================================================
//位置更新処理
//==================================================================================================
void UpdateEffect2DPos(Effect2D *pEffect2D)
{
	//移動量減衰
	pEffect2D->move = pEffect2D->move * MOVE_FACT;

	pEffect2D->move.y += pEffect2D->fGrav;

	if (pEffect2D->pPosOwner != NULL)
	{//相対位置での設定
		pEffect2D->relPos.x += pEffect2D->move.x;
		pEffect2D->relPos.y += pEffect2D->move.y;
		pEffect2D->relPos.z += pEffect2D->move.z;

		pEffect2D->pos.x = pEffect2D->pPosOwner->x + pEffect2D->relPos.x;
		pEffect2D->pos.y = pEffect2D->pPosOwner->y + pEffect2D->relPos.y;
		pEffect2D->pos.z = pEffect2D->pPosOwner->z + pEffect2D->relPos.z;
	}
	else
	{//絶対座標更新
		pEffect2D->pos += pEffect2D->move;
	}
}

//==================================================================================================
//ポリゴン更新処理
//==================================================================================================
void UpdateEffect2DPolygon(Effect2D *pEffect2D,int nCntEffect2D)
{
	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect2D->Lock(0, 0, (void**)&pVtx, 0);

	//頂点データのポインタを4つ分進める
	pVtx += 4 * nCntEffect2D;

	//頂点座標の設定
	pVtx[0].pos = { pEffect2D->pos.x - pEffect2D->fRadius, pEffect2D->pos.y - pEffect2D->fRadius, 0.0f };
	pVtx[1].pos = { pEffect2D->pos.x + pEffect2D->fRadius, pEffect2D->pos.y - pEffect2D->fRadius, 0.0f };
	pVtx[2].pos = { pEffect2D->pos.x - pEffect2D->fRadius, pEffect2D->pos.y + pEffect2D->fRadius, 0.0f };
	pVtx[3].pos = { pEffect2D->pos.x + pEffect2D->fRadius, pEffect2D->pos.y + pEffect2D->fRadius, 0.0f };

	//頂点カラーの設定
	pVtx[0].col = pEffect2D->col;
	pVtx[1].col = pEffect2D->col;
	pVtx[2].col = pEffect2D->col;
	pVtx[3].col = pEffect2D->col;

	//頂点バッファをアンロック
	g_pVtxBuffEffect2D->Unlock();
}

//==================================================================================================
//描画処理
//==================================================================================================
void DrawEffect2D(void)
{
	//デバイスポインタ設定
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ポインタ宣言
	Effect2D *pEffect2D = GetEffect2D();

	//アルファテストの有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);

	for (int nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT2D; nCntEffect2D++, pEffect2D++)
	{
		if (pEffect2D->bUse)
		{
			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect2D, 0, sizeof(VERTEX_2D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			//テクスチャ設定
			pDevice->SetTexture(0, g_pTextureEffect2D);

			if (pEffect2D->bAdd == true)
			{
				//αブレンディングを加算合成に設定
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEffect2D, 2);

			if (pEffect2D->bAdd == true)
			{
				//αブレンディングを元に戻す
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
		}
	}

	//アルファテストの無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

#ifdef _DEBUG
	DebugEffect2D();
#endif
}

//==================================================================================================
//設定処理
//==================================================================================================
void SetEffect2D(D3DXVECTOR3 pos, D3DXVECTOR3 *pPosOwner, D3DXVECTOR3 move, D3DXCOLOR col,float fRadius, int nLife,bool bAdd, float fGrav)
{
	//変数宣言
	int nCntEffect2D;

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//弾へのポインタ
	Effect2D *pEffect2D = GetEffect2D();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect2D->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT2D; nCntEffect2D++,pEffect2D++)
	{//全ての弾をチェックする
		if (pEffect2D->bUse == false)
		{//使っていない状態なら

			//位置の設定
			pEffect2D->pos = pos;

			if (pPosOwner != NULL)
			{
				//持ち主位置の設定
				pEffect2D->pPosOwner = pPosOwner;

				//相対位置設定
				pEffect2D->relPos = pos - *pPosOwner;
			}

			//位置の設定
			pEffect2D->move = move;

			//色の設定
			pEffect2D->col = col;

			//半径の設定
			pEffect2D->fRadius = fRadius;

			//寿命の設定
			pEffect2D->nLife = nLife;
			pEffect2D->nMaxLife = nLife;

			//頂点座標の設定
			pVtx[0].col = D3DXCOLOR{ pEffect2D->col.r,pEffect2D->col.g,pEffect2D->col.b,pEffect2D->col.a };
			pVtx[1].col = D3DXCOLOR{ pEffect2D->col.r,pEffect2D->col.g,pEffect2D->col.b,pEffect2D->col.a };
			pVtx[2].col = D3DXCOLOR{ pEffect2D->col.r,pEffect2D->col.g,pEffect2D->col.b,pEffect2D->col.a };
			pVtx[3].col = D3DXCOLOR{ pEffect2D->col.r,pEffect2D->col.g,pEffect2D->col.b,pEffect2D->col.a };

			//使用している設定
			pEffect2D->bUse = true;

			//加算合成するかどうか
			pEffect2D->bAdd = bAdd;

			//重力が影響するかどうか
			pEffect2D->fGrav = fGrav;

			//for文を抜ける
			break;
		}

		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffEffect2D->Unlock();
}

//==================================================================================================
//ポインタ取得
//==================================================================================================
Effect2D *GetEffect2D(void)
{
	return &g_aEffect2D[0];
}

//==================================================
//デバッグ表示
//==================================================
void DebugEffect2D(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//場所
	char aStr[1][256];					//数
	int nNum = 0;
	LPD3DXFONT Font = GetFont();		//メインのやつ

	for (int nCnt = 0; nCnt < MAX_EFFECT2D; nCnt++)
	{
		if (g_aEffect2D[nCnt].bUse == true)
		{
			nNum++;
		}
	}

	//文字列に代入
	sprintf(&aStr[0][0], "2Dエフェクトの数：%d", nNum);

	rect.top += (int)(SCREEN_HEIGHT * 0.4f) + 40;
	Font->DrawText(NULL, &aStr[0][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
