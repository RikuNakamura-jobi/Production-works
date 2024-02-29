//===========================
//
//ダイレクトX.shadowファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "shadow.h"

//マクロ定義
#define MAX_SHADOW (256)

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;							//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;					//頂点情報を格納
Shadow g_aShadow[MAX_SHADOW];

//===========================
//プレイヤーの初期化処理
//===========================
void InitShadow(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_pTextureShadow);

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		g_aShadow[nCnt].posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCnt].rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCnt].bTrue = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = -30.0f;
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 30.0f;

		pVtx[1].pos.x = 30.0f;
		pVtx[1].pos.y = 0.0f;
		pVtx[1].pos.z = 30.0f;

		pVtx[2].pos.x = -30.0f;
		pVtx[2].pos.y = 0.0f;
		pVtx[2].pos.z = -30.0f;

		pVtx[3].pos.x = 30.0f;
		pVtx[3].pos.y = 0.0f;
		pVtx[3].pos.z = -30.0f;

		//頂点座標の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}

//===========================
//プレイヤーの終了処理
//===========================
void UninitShadow(void)
{
	//テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//===========================
//プレイヤーの更新処理
//===========================
void UpdateShadow(void)
{

}

//===========================
//プレイヤーの描画処理
//===========================
void DrawShadow(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aShadow[nCnt].mtxWorldShadow);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_aShadow[nCnt].rotShadow.y, g_aShadow[nCnt].rotShadow.x, g_aShadow[nCnt].rotShadow.z);
		D3DXMatrixMultiply(&g_aShadow[nCnt].mtxWorldShadow, &g_aShadow[nCnt].mtxWorldShadow, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			g_aShadow[nCnt].posShadow.x, g_aShadow[nCnt].posShadow.y, g_aShadow[nCnt].posShadow.z);
		D3DXMatrixMultiply(&g_aShadow[nCnt].mtxWorldShadow, &g_aShadow[nCnt].mtxWorldShadow, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCnt].mtxWorldShadow);

		if (g_aShadow[nCnt].bTrue == true)
		{
			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0,
				g_pVtxBuffShadow,
				0,
				sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===========================
//プレイヤーの設定処理
//===========================
int SetShadow(float rad)
{
	int nCntShadow = -1;

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bTrue == false)
		{
			g_aShadow[nCntShadow].posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aShadow[nCntShadow].rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//頂点座標の設定
			pVtx[0].pos.x = -rad;
			pVtx[0].pos.y = 0.0f;
			pVtx[0].pos.z = rad;

			pVtx[1].pos.x = rad;
			pVtx[1].pos.y = 0.0f;
			pVtx[1].pos.z = rad;

			pVtx[2].pos.x = -rad;
			pVtx[2].pos.y = 0.0f;
			pVtx[2].pos.z = -rad;

			pVtx[3].pos.x = rad;
			pVtx[3].pos.y = 0.0f;
			pVtx[3].pos.z = -rad;

			//頂点座標の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

			g_aShadow[nCntShadow].bTrue = true;

			break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;
}

//===========================
//プレイヤーの設定処理
//===========================
void SetShadowMat(void)
{
	//D3DXMATRIX mtxShadow;
	//D3DLIGHT9 light;
	//D3DXVECTOR4 posLight;
	//D3DXVECTOR3 pos, normal;
	//D3DXPLANE plane;

	//pDevice->GetLight(0, &light);
	//posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

	//pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	////ワールドマトリックスの初期化
	//D3DXMatrixIdentity(&mtxShadow);

	//D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
	//D3DXMatrixMultiply(&mtxShadow, &g_player.mtxWorld, &mtxShadow);

	////ワールドマトリックスの設定
	//pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	//for (int nCntMat = 0; nCntMat < (int)g_dwNumMatPlayer; nCntMat++)
	//{
	//	//マテリアルの設定
	//	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

	//	//テクスチャの設定
	//	pDevice->SetTexture(0, NULL);

	//	//モデル(パーツ)の描画
	//	g_pMeshPlayer->DrawSubset(nCntMat);
	//}
}

//===========================
//プレイヤーの位置設定処理
//===========================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	g_aShadow[nIdxShadow].posShadow.x = pos.x;
	g_aShadow[nIdxShadow].posShadow.z = pos.z;
}

//===========================
//プレイヤーの位置設定処理
//===========================
void SetShadowFalse(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bTrue = false;
}