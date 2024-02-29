//===========================
//
//ダイレクトX.skyfieldファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "skyfield.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureSkyField = NULL;							//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSkyField = NULL;					//頂点情報を格納
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSkyField = NULL;					//インデックスバッファへのポインタ
D3DXVECTOR3 g_posSkyField;
D3DXVECTOR3 g_rotSkyField;
D3DXMATRIX g_mtxWorldSkyField;		//ワールドマトリックス
int g_NumSkyFieldx;
int g_NumSkyFieldy;
int g_NumVtxSkyField;
int g_NumIdxSkyField;

//===========================
//プレイヤーの初期化処理
//===========================
void InitSkyField(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\sky001.jpg",
		&g_pTextureSkyField);

	g_posSkyField = D3DXVECTOR3(0.0f, -180.0f, 0.0f);
	g_rotSkyField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_NumSkyFieldx = 1;
	g_NumSkyFieldy = 1;

	g_NumVtxSkyField = (g_NumSkyFieldx + 1) * (g_NumSkyFieldy + 1);

	g_NumIdxSkyField = (((g_NumSkyFieldx + 2) * 2) * g_NumSkyFieldy);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_NumVtxSkyField,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffSkyField,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSkyField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_NumVtxSkyField; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = -3600.0f + (18000.0f * (nCnt % (g_NumSkyFieldx + 1)));
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 3600.0f - (18000.0f * (nCnt / (g_NumSkyFieldy + 1)));

		//頂点座標の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.5f * (nCnt % (g_NumSkyFieldx + 1)), 0.5f * (nCnt / (g_NumSkyFieldy + 1)));

		pVtx++;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffSkyField->Unlock();

	//インデックスの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_NumIdxSkyField,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSkyField,
		NULL);

	WORD *pIdx;				//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffSkyField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0, nIdx = 0; nCnt < (g_NumIdxSkyField / 2); nCnt++)
	{
		if (nCnt == (g_NumIdxSkyField / 2) - 1)
		{
			pIdx[nCnt * 2] = g_NumSkyFieldx + nIdx + (nIdx * g_NumSkyFieldx);
		}
		else if (nCnt % (g_NumSkyFieldx + 2) == g_NumSkyFieldx + 1)
		{
			pIdx[nCnt * 2] = g_NumSkyFieldx + nIdx + (nIdx * g_NumSkyFieldx);
			pIdx[(nCnt * 2) + 1] = (g_NumSkyFieldx + 1) * (nIdx + 2);
			nIdx++;
		}
		else
		{
			pIdx[nCnt * 2] = (nCnt - nIdx) + g_NumSkyFieldx + 1;
			pIdx[(nCnt * 2) + 1] = nCnt - nIdx;
		}
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffSkyField->Unlock();
}

//===========================
//プレイヤーの終了処理
//===========================
void UninitSkyField(void)
{
	//テクスチャの破棄
	if (g_pTextureSkyField != NULL)
	{
		g_pTextureSkyField->Release();
		g_pTextureSkyField = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSkyField != NULL)
	{
		g_pVtxBuffSkyField->Release();
		g_pVtxBuffSkyField = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIdxBuffSkyField != NULL)
	{
		g_pIdxBuffSkyField->Release();
		g_pIdxBuffSkyField = NULL;
	}
}

//===========================
//プレイヤーの更新処理
//===========================
void UpdateSkyField(void)
{

}

//===========================
//プレイヤーの描画処理
//===========================
void DrawSkyField(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldSkyField);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_rotSkyField.y, g_rotSkyField.x, g_rotSkyField.z);
	D3DXMatrixMultiply(&g_mtxWorldSkyField, &g_mtxWorldSkyField, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_posSkyField.x, g_posSkyField.y, g_posSkyField.z);
	D3DXMatrixMultiply(&g_mtxWorldSkyField, &g_mtxWorldSkyField, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldSkyField);
	
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffSkyField,
		0,
		sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffSkyField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureSkyField);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVtxSkyField, 0, g_NumIdxSkyField - 1);
}