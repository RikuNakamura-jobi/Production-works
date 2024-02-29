//===========================
//
//ダイレクトX.Meshceilingファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "meshceiling.h"
#include "game.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureMeshCeiling = NULL;							//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCeiling = NULL;					//頂点情報を格納
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCeiling = NULL;					//インデックスバッファへのポインタ
D3DXVECTOR3 g_posMeshCeiling;
D3DXVECTOR3 g_rotMeshCeiling;
D3DXCOLOR g_colMeshCeiling;
D3DXMATRIX g_mtxWorldMeshCeiling;		//ワールドマトリックス
int g_NumMeshCeilingx;
int g_NumMeshCeilingy;
int g_NumVtxMeshCeiling;
int g_NumIdxMeshCeiling;

//===========================
//プレイヤーの初期化処理
//===========================
void InitMeshCeiling(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ceiling000.png",
		&g_pTextureMeshCeiling);

	g_posMeshCeiling = D3DXVECTOR3(0.0f, 1800.0f, 0.0f);
	g_rotMeshCeiling = D3DXVECTOR3(3.14f, 0.0f, 0.0f);
	g_colMeshCeiling = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_NumMeshCeilingx = 3;
	g_NumMeshCeilingy = 3;

	g_NumVtxMeshCeiling = (g_NumMeshCeilingx + 1) * (g_NumMeshCeilingy + 1);

	g_NumIdxMeshCeiling = (((g_NumMeshCeilingx + 2) * 2) * g_NumMeshCeilingy);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_NumVtxMeshCeiling,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCeiling,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshCeiling->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_NumVtxMeshCeiling; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = -2700.0f + (1800.0f * (nCnt % (g_NumMeshCeilingx + 1)));
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 2700.0f - (1800.0f * (nCnt / (g_NumMeshCeilingy + 1)));

		//頂点座標の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = g_colMeshCeiling;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.5f * (nCnt % (g_NumMeshCeilingx + 1)), 0.5f * (nCnt / (g_NumMeshCeilingy + 1)));

		pVtx++;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshCeiling->Unlock();

	//インデックスの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_NumIdxMeshCeiling,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCeiling,
		NULL);

	WORD *pIdx;				//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshCeiling->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0, nIdx = 0; nCnt < (g_NumIdxMeshCeiling / 2); nCnt++)
	{
		if (nCnt == (g_NumIdxMeshCeiling / 2) - 1)
		{
			pIdx[nCnt * 2] = g_NumMeshCeilingx + nIdx + (nIdx * g_NumMeshCeilingx);
		}
		else if (nCnt % (g_NumMeshCeilingx + 2) == g_NumMeshCeilingx + 1)
		{
			pIdx[nCnt * 2] = g_NumMeshCeilingx + nIdx + (nIdx * g_NumMeshCeilingx);
			pIdx[(nCnt * 2) + 1] = (g_NumMeshCeilingx + 1) * (nIdx + 2);
			nIdx++;
		}
		else
		{
			pIdx[nCnt * 2] = (nCnt - nIdx) + g_NumMeshCeilingx + 1;
			pIdx[(nCnt * 2) + 1] = nCnt - nIdx;
		}
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshCeiling->Unlock();
}

//===========================
//プレイヤーの終了処理
//===========================
void UninitMeshCeiling(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshCeiling != NULL)
	{
		g_pTextureMeshCeiling->Release();
		g_pTextureMeshCeiling = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshCeiling != NULL)
	{
		g_pVtxBuffMeshCeiling->Release();
		g_pVtxBuffMeshCeiling = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIdxBuffMeshCeiling != NULL)
	{
		g_pIdxBuffMeshCeiling->Release();
		g_pIdxBuffMeshCeiling = NULL;
	}
}

//===========================
//プレイヤーの更新処理
//===========================
void UpdateMeshCeiling(void)
{
	if (GetGameState() == GAMESTATE_BOSS || GetGameState() == GAMESTATE_BOSSBATTLE)
	{
		if (g_colMeshCeiling.r >= 0.0f)
		{
			g_colMeshCeiling.r -= 0.01f;
		}
		if (g_colMeshCeiling.g >= 0.0f)
		{
			g_colMeshCeiling.g -= 0.01f;
		}
		if (g_colMeshCeiling.b >= 0.0f)
		{
			g_colMeshCeiling.b -= 0.01f;
		}
	}
	else
	{
		g_colMeshCeiling.r = 1.0f;
		g_colMeshCeiling.g = 1.0f;
		g_colMeshCeiling.b = 1.0f;
	}

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshCeiling->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_NumVtxMeshCeiling; nCnt++)
	{
		//頂点カラーの設定
		pVtx[0].col = g_colMeshCeiling;

		pVtx++;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshCeiling->Unlock();
}

//===========================
//プレイヤーの描画処理
//===========================
void DrawMeshCeiling(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshCeiling);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_rotMeshCeiling.y, g_rotMeshCeiling.x, g_rotMeshCeiling.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshCeiling, &g_mtxWorldMeshCeiling, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_posMeshCeiling.x, g_posMeshCeiling.y, g_posMeshCeiling.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshCeiling, &g_mtxWorldMeshCeiling, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshCeiling);
	
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffMeshCeiling,
		0,
		sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshCeiling);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshCeiling);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVtxMeshCeiling, 0, g_NumIdxMeshCeiling - 1);
}