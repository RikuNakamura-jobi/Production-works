//==================================================================================================
//
//shadow.cpp
//Author:髙山桃也
//
//==================================================================================================

#include "main.h"
#include "shadow.h"
#include "input.h"

//マクロ定義
#define NUM_TEX					(2)											//テクスチャ数
#define SHADOW_TEX_FILE			"data\\TEXTURE\\shadow000.jpg"		//テクスチャファイル名
#define SHADOW_TEX_FILE2		"data\\TEXTURE\\shadow001.jpg"		//テクスチャファイル名
#define SHADOW_WIDTH			(50)										//幅
#define SHADOW_HEIGHT			(50)										//高さ
#define MAX_SHADOW				(256)										//影の最大数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureShadow[NUM_TEX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//頂点バッファへのポインタ
Shadow g_aShadow[MAX_SHADOW];						//構造体の情報

//==================================================================================================
//初期化処理
//==================================================================================================
void InitShadow(void)
{
	//デバイスポインタ設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		SHADOW_TEX_FILE,
		&g_apTextureShadow[0]);

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		SHADOW_TEX_FILE2,
		&g_apTextureShadow[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	//情報取得
	Shadow *pShadow = GetShadow();

	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++,pShadow++)
	{
		//変数の初期化
		ZeroMemory(pShadow, sizeof(Shadow));

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
	g_pVtxBuffShadow->Unlock();
}

//==================================================================================================
//終了処理
//==================================================================================================
void UninitShadow(void)
{
	for (int nCntTex = 0;nCntTex < NUM_TEX;nCntTex++)
	{//テクスチャ破棄
		if (g_apTextureShadow[nCntTex] != NULL)
		{
			g_apTextureShadow[nCntTex]->Release();
			g_apTextureShadow[nCntTex] = NULL;
		}
	}
	

	if (g_pVtxBuffShadow != NULL)
	{//頂点バッファポインタの破棄
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//==================================================================================================
//更新処理
//==================================================================================================
void UpdateShadow(void)
{

}

//==================================================================================================
//描画処理
//==================================================================================================
void DrawShadow(void)
{
	//デバイスポインタ設定
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//情報取得
	Shadow *pShadow = GetShadow();

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++,pShadow++)
	{
		if (pShadow->bUse == true)
		{//使用しているなら
		 //ワールドマトリックス初期化
			D3DXMatrixIdentity(&pShadow->mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				pShadow->rot.y, pShadow->rot.x, pShadow->rot.z);
			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);
			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxTrans);

			//ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &pShadow->mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャ設定
			pDevice->SetTexture(0, g_apTextureShadow[pShadow->type]);

			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntShadow, 2);

		}
	}

	//加算合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==================================================================================================
//設定処理
//==================================================================================================
int SetShadow(float fRadius,SHADOWTYPE type, float fWidth, float fHeight ,D3DXVECTOR3 rot)
{
	//変数宣言
	int nCntShadow = -1;

	//情報取得
	Shadow *pShadow = GetShadow();

	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_SHADOW; nCount++, pShadow++)
	{
		if (pShadow->bUse == false)
		{//使用していない状態なら

			//半径代入
			pShadow->rot = rot;
			pShadow->fRadius = fRadius;
			pShadow->fWidth = fWidth;
			pShadow->fHeight = fHeight;
			pShadow->type = type;

			//頂点座標の設定
			if (type == SHADOWTYPE_SQUARE)
			{
				pVtx[0].pos = D3DXVECTOR3(-fWidth, 0.0f, fHeight);
				pVtx[1].pos = D3DXVECTOR3(fWidth, 0.0f, fHeight);
				pVtx[2].pos = D3DXVECTOR3(-fWidth, 0.0f, -fHeight);
				pVtx[3].pos = D3DXVECTOR3(fWidth, 0.0f, -fHeight);
			}
			else
			{
				pVtx[0].pos = D3DXVECTOR3(-pShadow->fRadius, 0.0f, pShadow->fRadius);
				pVtx[1].pos = D3DXVECTOR3(pShadow->fRadius, 0.0f, pShadow->fRadius);
				pVtx[2].pos = D3DXVECTOR3(-pShadow->fRadius, 0.0f, -pShadow->fRadius);
				pVtx[3].pos = D3DXVECTOR3(pShadow->fRadius, 0.0f, -pShadow->fRadius);
			}

			//法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

			nCntShadow = nCount;

			//使用状態にする
			pShadow->bUse = true;

			//for文を抜ける
			break;
		}

		//ポインタを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;
}

//==================================================================================================
//位置設定処理
//==================================================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fScale)
{
	//使用数制限
	if (nIdxShadow == -1)
	{
		return;
	}

	//変数宣言
	float fRadius, fHeight, fWidth;

	//位置・向きの受け取り
	g_aShadow[nIdxShadow].pos = pos;
	g_aShadow[nIdxShadow].rot = rot;

	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	//ポインタを進める
	pVtx += 4 * nIdxShadow;

	//頂点座標の設定
	switch (g_aShadow[nIdxShadow].type)
	{
	case SHADOWTYPE_CIRCLE:
		fRadius = g_aShadow[nIdxShadow].fRadius * fScale;

		pVtx[0].pos = D3DXVECTOR3(-fRadius, 0.0f, fRadius);
		pVtx[1].pos = D3DXVECTOR3(fRadius, 0.0f, fRadius);
		pVtx[2].pos = D3DXVECTOR3(-fRadius, 0.0f, -fRadius);
		pVtx[3].pos = D3DXVECTOR3(fRadius, 0.0f, -fRadius);
		break;

	case SHADOWTYPE_SQUARE:
		fHeight = g_aShadow[nIdxShadow].fHeight * fScale;
		fWidth = g_aShadow[nIdxShadow].fWidth * fScale;

		pVtx[0].pos = D3DXVECTOR3(-fWidth, 0.0f, fHeight);
		pVtx[1].pos = D3DXVECTOR3(fWidth, 0.0f, fHeight);
		pVtx[2].pos = D3DXVECTOR3(-fWidth, 0.0f, -fHeight);
		pVtx[3].pos = D3DXVECTOR3(fWidth, 0.0f, -fHeight);
		break;
	}
	
	//頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
}

//==================================================================================================
//情報取得処理
//==================================================================================================
Shadow *GetShadow(void)
{
	return &g_aShadow[0];
}

//==================================================================================================
//影有効無効管理
//==================================================================================================
void EnableShadow(int nIdxShadow, bool bUse)
{
	g_aShadow[nIdxShadow].bUse = bUse;
}

//==================================================================================================
//影のデバッグ処理
//==================================================================================================
void DebugShadow()
{
	RECT rect = { 0,160,SCREEN_WIDTH,SCREEN_HEIGHT };	//場所
	char aStr[1][256];					//数
	LPD3DXFONT Font = GetFont();		//メインのやつ
	int nUseCnter = 0;

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		if (g_aShadow[nCnt].bUse == true)
		{
			nUseCnter++;
		}
	}
	
	//文字列に代入
	sprintf(&aStr[0][0], "影の数：%d", nUseCnter);

	for (int nCntDebug = 0; nCntDebug < 1; nCntDebug++)
	{
		Font->DrawText(NULL, &aStr[nCntDebug][0], -1, &rect, DT_RIGHT, D3DXCOLOR(0.7f, 0.0f, 0.7f, 1.0f));
		rect.top += 20;
	}
}
