//===========================
//
//ダイレクトX.Meshcylinderファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "meshcylinder.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder = NULL;							//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;					//頂点情報を格納
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;					//インデックスバッファへのポインタ
D3DXVECTOR3 g_posMeshCylinder;
D3DXVECTOR3 g_rotMeshCylinder;
D3DXMATRIX g_mtxWorldMeshCylinder;		//ワールドマトリックス
int g_NumMeshCylinderx;
int g_NumMeshCylindery;
int g_NumVtxMeshCylinder;
int g_NumIdxMeshCylinder;

//===========================
//プレイヤーの初期化処理
//===========================
void InitMeshCylinder(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\block000.jpg",
		&g_pTextureMeshCylinder);

	g_posMeshCylinder = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshCylinder = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_NumMeshCylinderx = 3;
	g_NumMeshCylindery = 3;

	g_NumVtxMeshCylinder = (g_NumMeshCylinderx + 1) * (g_NumMeshCylindery + 1);

	g_NumIdxMeshCylinder = (((g_NumMeshCylinderx + 2) * 2) * g_NumMeshCylindery);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_NumVtxMeshCylinder,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_NumVtxMeshCylinder; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = -1800.0f + (1800.0f * (nCnt % (g_NumMeshCylinderx + 1)));
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 1800.0f - (1800.0f * (nCnt / (g_NumMeshCylindery + 1)));

		//頂点座標の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.5f * (nCnt % (g_NumMeshCylinderx + 1)), 0.5f * (nCnt / (g_NumMeshCylindery + 1)));

		pVtx++;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshCylinder->Unlock();

	//インデックスの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_NumIdxMeshCylinder,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);

	WORD *pIdx;				//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0, nIdx = 0; nCnt < (g_NumIdxMeshCylinder / 2); nCnt++)
	{
		if (nCnt == (g_NumIdxMeshCylinder / 2) - 1)
		{
			pIdx[nCnt * 2] = g_NumMeshCylinderx + nIdx + (nIdx * g_NumMeshCylinderx);
		}
		else if (nCnt % (g_NumMeshCylinderx + 2) == g_NumMeshCylinderx + 1)
		{
			pIdx[nCnt * 2] = g_NumMeshCylinderx + nIdx + (nIdx * g_NumMeshCylinderx);
			pIdx[(nCnt * 2) + 1] = (g_NumMeshCylinderx + 1) * (nIdx + 2);
			nIdx++;
		}
		else
		{
			pIdx[nCnt * 2] = (nCnt - nIdx) + g_NumMeshCylinderx + 1;
			pIdx[(nCnt * 2) + 1] = nCnt - nIdx;
		}
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshCylinder->Unlock();
}

//===========================
//プレイヤーの終了処理
//===========================
void UninitMeshCylinder(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//===========================
//プレイヤーの更新処理
//===========================
void UpdateMeshCylinder(void)
{

}

//===========================
//プレイヤーの描画処理
//===========================
void DrawMeshCylinder(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshCylinder);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_rotMeshCylinder.y, g_rotMeshCylinder.x, g_rotMeshCylinder.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshCylinder, &g_mtxWorldMeshCylinder, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_posMeshCylinder.x, g_posMeshCylinder.y, g_posMeshCylinder.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshCylinder, &g_mtxWorldMeshCylinder, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshCylinder);
	
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffMeshCylinder,
		0,
		sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshCylinder);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshCylinder);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVtxMeshCylinder, 0, g_NumIdxMeshCylinder - 1);
}