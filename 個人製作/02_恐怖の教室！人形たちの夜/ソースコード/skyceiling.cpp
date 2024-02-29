//===========================
//
//ダイレクトX.skyceilingファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "skyceiling.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureSkyCeiling = NULL;							//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSkyCeiling = NULL;					//頂点情報を格納
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSkyCeiling = NULL;					//インデックスバッファへのポインタ
D3DXVECTOR3 g_posSkyCeiling;
D3DXVECTOR3 g_rotSkyCeiling;
D3DXMATRIX g_mtxWorldSkyCeiling;		//ワールドマトリックス
int g_NumSkyCeilingx;
int g_NumSkyCeilingy;
int g_NumVtxSkyCeiling;
int g_NumIdxSkyCeiling;

//===========================
//プレイヤーの初期化処理
//===========================
void InitSkyCeiling(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\sky001.jpg",
		&g_pTextureSkyCeiling);

	g_posSkyCeiling = D3DXVECTOR3(0.0f, 3600.0f, 0.0f);
	g_rotSkyCeiling = D3DXVECTOR3(3.14f, 0.0f, 0.0f);
	g_NumSkyCeilingx = 1;
	g_NumSkyCeilingy = 1;

	g_NumVtxSkyCeiling = (g_NumSkyCeilingx + 1) * (g_NumSkyCeilingy + 1);

	g_NumIdxSkyCeiling = (((g_NumSkyCeilingx + 2) * 2) * g_NumSkyCeilingy);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_NumVtxSkyCeiling,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffSkyCeiling,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSkyCeiling->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_NumVtxSkyCeiling; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = -3600.0f + (18000.0f * (nCnt % (g_NumSkyCeilingx + 1)));
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 3600.0f - (18000.0f * (nCnt / (g_NumSkyCeilingy + 1)));

		//頂点座標の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.5f * (nCnt % (g_NumSkyCeilingx + 1)), 0.5f * (nCnt / (g_NumSkyCeilingy + 1)));

		pVtx++;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffSkyCeiling->Unlock();

	//インデックスの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_NumIdxSkyCeiling,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSkyCeiling,
		NULL);

	WORD *pIdx;				//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffSkyCeiling->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0, nIdx = 0; nCnt < (g_NumIdxSkyCeiling / 2); nCnt++)
	{
		if (nCnt == (g_NumIdxSkyCeiling / 2) - 1)
		{
			pIdx[nCnt * 2] = g_NumSkyCeilingx + nIdx + (nIdx * g_NumSkyCeilingx);
		}
		else if (nCnt % (g_NumSkyCeilingx + 2) == g_NumSkyCeilingx + 1)
		{
			pIdx[nCnt * 2] = g_NumSkyCeilingx + nIdx + (nIdx * g_NumSkyCeilingx);
			pIdx[(nCnt * 2) + 1] = (g_NumSkyCeilingx + 1) * (nIdx + 2);
			nIdx++;
		}
		else
		{
			pIdx[nCnt * 2] = (nCnt - nIdx) + g_NumSkyCeilingx + 1;
			pIdx[(nCnt * 2) + 1] = nCnt - nIdx;
		}
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffSkyCeiling->Unlock();
}

//===========================
//プレイヤーの終了処理
//===========================
void UninitSkyCeiling(void)
{
	//テクスチャの破棄
	if (g_pTextureSkyCeiling != NULL)
	{
		g_pTextureSkyCeiling->Release();
		g_pTextureSkyCeiling = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSkyCeiling != NULL)
	{
		g_pVtxBuffSkyCeiling->Release();
		g_pVtxBuffSkyCeiling = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIdxBuffSkyCeiling != NULL)
	{
		g_pIdxBuffSkyCeiling->Release();
		g_pIdxBuffSkyCeiling = NULL;
	}
}

//===========================
//プレイヤーの更新処理
//===========================
void UpdateSkyCeiling(void)
{

}

//===========================
//プレイヤーの描画処理
//===========================
void DrawSkyCeiling(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldSkyCeiling);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_rotSkyCeiling.y, g_rotSkyCeiling.x, g_rotSkyCeiling.z);
	D3DXMatrixMultiply(&g_mtxWorldSkyCeiling, &g_mtxWorldSkyCeiling, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_posSkyCeiling.x, g_posSkyCeiling.y, g_posSkyCeiling.z);
	D3DXMatrixMultiply(&g_mtxWorldSkyCeiling, &g_mtxWorldSkyCeiling, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldSkyCeiling);
	
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffSkyCeiling,
		0,
		sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffSkyCeiling);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureSkyCeiling);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVtxSkyCeiling, 0, g_NumIdxSkyCeiling - 1);
}