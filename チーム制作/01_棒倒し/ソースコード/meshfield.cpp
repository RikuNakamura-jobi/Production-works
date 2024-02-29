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
MESHFIELD g_meshField;

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

	g_meshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_meshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_meshField.Numx = 1;
	g_meshField.Numy = 1;

	g_meshField.NumVtx = (g_meshField.Numx + 1) * (g_meshField.Numy + 1);

	g_meshField.NumIdx = (((g_meshField.Numx + 2) * 2) * g_meshField.Numy);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_meshField.NumVtx,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_meshField.NumVtx; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = -150.0f + (300.0f * (nCnt % (g_meshField.Numx + 1)));
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 1800.0f - (18000.0f * (nCnt / (g_meshField.Numy + 1)));

		//頂点座標の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.5f * (nCnt % (g_meshField.Numx + 1)), 0.5f * (nCnt / (g_meshField.Numy + 1)));

		pVtx++;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshField->Unlock();

	//インデックスの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_meshField.NumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	WORD *pIdx;				//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0, nIdx = 0; nCnt < (g_meshField.NumIdx / 2); nCnt++)
	{
		if (nCnt == (g_meshField.NumIdx / 2) - 1)
		{
			pIdx[nCnt * 2] = g_meshField.Numx + nIdx + (nIdx * g_meshField.Numx);
		}
		else if (nCnt % (g_meshField.Numx + 2) == g_meshField.Numx + 1)
		{
			pIdx[nCnt * 2] = g_meshField.Numx + nIdx + (nIdx * g_meshField.Numx);
			pIdx[(nCnt * 2) + 1] = (g_meshField.Numx + 1) * (nIdx + 2);
			nIdx++;
		}
		else
		{
			pIdx[nCnt * 2] = (nCnt - nIdx) + g_meshField.Numx + 1;
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
	D3DXMatrixIdentity(&g_meshField.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_meshField.rot.y, g_meshField.rot.x, g_meshField.rot.z);
	D3DXMatrixMultiply(&g_meshField.mtxWorld, &g_meshField.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_meshField.pos.x, g_meshField.pos.y, g_meshField.pos.z);
	D3DXMatrixMultiply(&g_meshField.mtxWorld, &g_meshField.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_meshField.mtxWorld);
	
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
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_meshField.NumVtx, 0, g_meshField.NumIdx - 1);
}

//===========================
//プレイヤーの描画処理
//===========================
void SetMeshField(void)
{
	g_meshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_meshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_meshField.Numx = 3;
	g_meshField.Numy = 3;

	g_meshField.NumVtx = (g_meshField.Numx + 1) * (g_meshField.Numy + 1);

	g_meshField.NumIdx = (((g_meshField.Numx + 2) * 2) * g_meshField.Numy);
}