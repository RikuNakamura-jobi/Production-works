//==================================================================================================
//
//meshfield.cpp
//Author:髙山桃也
//
//==================================================================================================

#include "main.h"
#include "meshfield.h"
#include "input.h"

//マクロ定義
#define MESHFIELD_TEX_FILE			"data\\TEXTURE\\field_000.jpg"				//テクスチャファイル名
#define SPLIT_TEX					(20)										//テクスチャ分割数
#define MAX_MESHFIELD				(3)										//最大数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;									//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;								//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;								//インデックスバッファへのポインタ
MeshField g_MeshField[MAX_MESHFIELD];											//構造体の情報

//==================================================================================================
//初期化処理
//==================================================================================================
void InitMeshField(void)
{
	//デバイスポインタ設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		MESHFIELD_TEX_FILE,
		&g_pTextureMeshField);

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{
		g_MeshField[nCntMeshField].nNumVtx = (MESH_U + 1) * (MESH_V + 1);
		g_MeshField[nCntMeshField].bUse = false;

		//向きを初期化
		g_MeshField[nCntMeshField].rot = { 0.0f,0.0f,0.0f };

		//頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_MeshField[nCntMeshField].nNumVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffMeshField,
			NULL);

		//頂点情報のポインタ
		VERTEX_3D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

		int nCountV, nCountU;

		for (nCountV = 0; nCountV < MESH_V + 1; nCountV++)
		{//頂点座標の設定
			for (nCountU = 0; nCountU < MESH_U + 1; nCountU++)
			{
				//頂点座標
				pVtx[nCountV * (MESH_U + 1) + nCountU].pos.x = (nCountU - ((MESH_U) * 0.5f)) * FIELD_RANGE;
				pVtx[nCountV * (MESH_U + 1) + nCountU].pos.z = (((MESH_U) * 0.5f) - nCountV) * FIELD_RANGE;
				pVtx[nCountV * (MESH_U + 1) + nCountU].pos.y = 0.0f;

				//テクスチャ座標
				pVtx[nCountV * (MESH_U + 1) + nCountU].tex = D3DXVECTOR2
				(
					((float)SPLIT_TEX / MESH_U) * nCountU,
					((float)SPLIT_TEX / MESH_V) * nCountV
				);

				//法線ベクトルの設定
				pVtx[nCountV * (MESH_U + 1) + nCountU].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}
		}

		for (int nCnt = 0; nCnt < g_MeshField[nCntMeshField].nNumVtx; nCnt++)
		{
			//頂点カラーの設定
			pVtx[nCnt].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		//頂点バッファをアンロック
		g_pVtxBuffMeshField->Unlock();

		g_MeshField[nCntMeshField].nNumIdx = (MESH_U + 1) * (MESH_V + 1) + ((MESH_V + 1) - 2) * (MESH_U + 1) + (((MESH_V + 1) - 2) * 2);

		//インデックスバッファの生成
		pDevice->CreateIndexBuffer(sizeof(WORD) * g_MeshField[nCntMeshField].nNumIdx,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&g_pIdxBuffMeshField,
			NULL);

		//インデックスバッファへのポインタ
		WORD *pIdx;

		//インデックスバッファをロックし、頂点番号へのポインタを取得
		g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCount = 0; nCount < g_MeshField[nCntMeshField].nNumIdx / 2; nCount++)
		{//インデックス決定
			if (
				nCount % ((MESH_U + 1) + (MESH_U + 2) * (nCount / (MESH_U + 2))) == 0
				&& nCount != 0
				)
			{
				pIdx[nCount * 2 + 1] = ((MESH_U + 1) * 2) + (MESH_U + 1) * (nCount / (MESH_U + 2));

				pIdx[nCount * 2] = MESH_U + (MESH_U + 1) * (nCount / (MESH_U + 2));
			}
			else
			{
				pIdx[nCount * 2 + 1] = nCount - (nCount / (MESH_U + 2));

				pIdx[nCount * 2] = (nCount - (nCount / (MESH_U + 2))) + (MESH_U + 1);
			}
		}

		//インデックスバッファをアンロック
		g_pIdxBuffMeshField->Unlock();
	}

	switch (GetMode())
	{
	case MODE_RANKING:
		SetMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetMeshField(D3DXVECTOR3(1000.0f, 0.0f, 0.0f));
		SetMeshField(D3DXVECTOR3(2000.0f, 0.0f, 0.0f));
		break;

	case MODE_TITLE:
	case MODE_MENU:
		break;


	default:
		SetMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		break;
	}
}

//==================================================================================================
//終了処理
//==================================================================================================
void UninitMeshField(void)
{
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	if (g_pVtxBuffMeshField != NULL)
	{//頂点バッファポインタの破棄
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	if (g_pIdxBuffMeshField != NULL)
	{//インデックスバッファポインタの破棄
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//==================================================================================================
//更新処理
//==================================================================================================
void UpdateMeshField(void)
{
}

//==================================================================================================
//描画処理
//==================================================================================================
void DrawMeshField(void)
{
	//デバイスポインタ設定
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	for (int nCntDraw = 0; nCntDraw < MAX_MESHFIELD; nCntDraw++)
	{
		if (g_MeshField[nCntDraw].bUse == true)
		{
			//ワールドマトリックス初期化
			D3DXMatrixIdentity(&g_MeshField[nCntDraw].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_MeshField[nCntDraw].rot.y, g_MeshField[nCntDraw].rot.x, g_MeshField[nCntDraw].rot.z);
			D3DXMatrixMultiply(&g_MeshField[nCntDraw].mtxWorld, &g_MeshField[nCntDraw].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_MeshField[nCntDraw].pos.x, g_MeshField[nCntDraw].pos.y, g_MeshField[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_MeshField[nCntDraw].mtxWorld, &g_MeshField[nCntDraw].mtxWorld, &mtxTrans);

			//ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &g_MeshField[nCntDraw].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshField);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャ設定
			pDevice->SetTexture(0, g_pTextureMeshField);

			//ポリゴン描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_MeshField[nCntDraw].nNumVtx, 0, g_MeshField[nCntDraw].nNumIdx - 2);
		}
	}

	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

//==================================================================================================
//ランキングでの更新処理
//==================================================================================================
void UpdateMeshFieldRanking(float moveX)
{
	for (int nCntRank = 0; nCntRank < MAX_MESHFIELD; nCntRank++)
	{
		if (g_MeshField[nCntRank].bUse == true)
		{
			g_MeshField[nCntRank].pos.x += moveX;

			if (g_MeshField[nCntRank].pos.x <= -1000.0f)
			{
				g_MeshField[nCntRank].pos.x = 2000.0f;
			}
		}
	}
}

//==================================================================================================
//設置処理
//==================================================================================================
void SetMeshField(D3DXVECTOR3 pos)
{
	for (int nCntRank = 0; nCntRank < MAX_MESHFIELD; nCntRank++)
	{
		if (g_MeshField[nCntRank].bUse == false)
		{
			g_MeshField[nCntRank].pos = pos;
			g_MeshField[nCntRank].bUse = true;
			break;
		}
	}
}

