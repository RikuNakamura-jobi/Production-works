//===========================
//
//ダイレクトX.Meshfieldファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "meshfield.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;							//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;					//頂点情報を格納
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;					//インデックスバッファへのポインタ
D3DXVECTOR3 g_posMeshField;
D3DXVECTOR3 g_rotMeshField;
D3DXMATRIX g_mtxWorldMeshField;		//ワールドマトリックス
int g_NumMeshFieldx;
int g_NumMeshFieldy;
int g_NumVtxMeshField;
int g_NumIdxMeshField;

//===========================
//プレイヤーの初期化処理
//===========================
void InitMeshField(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\field000.png",
		&g_pTextureMeshField);

	g_posMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_NumMeshFieldx = 3;
	g_NumMeshFieldy = 3;

	g_NumVtxMeshField = (g_NumMeshFieldx + 1) * (g_NumMeshFieldy + 1);

	g_NumIdxMeshField = (((g_NumMeshFieldx + 2) * 2) * g_NumMeshFieldy);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_NumVtxMeshField,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_NumVtxMeshField; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = -2700.0f + (1800.0f * (nCnt % (g_NumMeshFieldx + 1)));
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 2700.0f - (1800.0f * (nCnt / (g_NumMeshFieldy + 1)));

		//頂点座標の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.5f * (nCnt % (g_NumMeshFieldx + 1)), 0.5f * (nCnt / (g_NumMeshFieldy + 1)));

		pVtx++;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshField->Unlock();

	//インデックスの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_NumIdxMeshField,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	WORD *pIdx;				//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0, nIdx = 0; nCnt < (g_NumIdxMeshField / 2); nCnt++)
	{
		if (nCnt == (g_NumIdxMeshField / 2) - 1)
		{
			pIdx[nCnt * 2] = g_NumMeshFieldx + nIdx + (nIdx * g_NumMeshFieldx);
		}
		else if (nCnt % (g_NumMeshFieldx + 2) == g_NumMeshFieldx + 1)
		{
			pIdx[nCnt * 2] = g_NumMeshFieldx + nIdx + (nIdx * g_NumMeshFieldx);
			pIdx[(nCnt * 2) + 1] = (g_NumMeshFieldx + 1) * (nIdx + 2);
			nIdx++;
		}
		else
		{
			pIdx[nCnt * 2] = (nCnt - nIdx) + g_NumMeshFieldx + 1;
			pIdx[(nCnt * 2) + 1] = nCnt - nIdx;
		}
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshField->Unlock();
}

//===========================
//プレイヤーの終了処理
//===========================
void UninitMeshField(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//===========================
//プレイヤーの更新処理
//===========================
void UpdateMeshField(void)
{

}

//===========================
//プレイヤーの描画処理
//===========================
void DrawMeshField(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshField);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_rotMeshField.y, g_rotMeshField.x, g_rotMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);
	
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffMeshField,
		0,
		sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshField);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVtxMeshField, 0, g_NumIdxMeshField - 1);
}