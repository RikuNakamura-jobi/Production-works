//==========================================
//
//  軌跡の処理
//  Author : 髙山桃也
//
//==========================================
#include "orbit.h"
#include "effect.h"
#include "input.h"

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureOrbit = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffOrbit = NULL;	//頂点バッファへのポインタ
D3DXMATRIX g_OrbitmtxWorld;		//ワールドマトリックス
Orbit g_aOrbit[MAX_ORBIT];	//構造体の情報

//==========================================
//  初期化処理
//==========================================
void InitOrbit()
{
	//変数初期化
	ZeroMemory(&g_aOrbit[0],sizeof(Orbit) * MAX_ORBIT);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data/TEXTURE/orbit_000.png",
		&g_pTextureOrbit
	);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * MAX_EDGE * NUM_OFFSET * MAX_ORBIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffOrbit,
		NULL
	);

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffOrbit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0;nCntVtx < MAX_EDGE * NUM_OFFSET * MAX_ORBIT;nCntVtx++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f * (nCntVtx % NUM_OFFSET));

		pVtx += 1;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffOrbit->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitOrbit()
{
	if (g_pTextureOrbit != NULL)
	{//テクスチャ破棄
		g_pTextureOrbit->Release();
		g_pTextureOrbit = NULL;
	}
	if (g_pVtxBuffOrbit != NULL)
	{//頂点情報破棄
		g_pVtxBuffOrbit->Release();
		g_pVtxBuffOrbit = NULL;
	}
}

//==========================================
//  更新処理
//==========================================
void UpdateOrbit(void)
{
}

//==========================================
//  ポリゴン更新処理
//==========================================
void UpdateOrbitPolygon(int nCntOrbit)
{
	//変数宣言
	int nCntOffset;
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス
	D3DXVECTOR3 posTemp[NUM_OFFSET];

	//情報取得
	Orbit *pOrbit = GetOrbit();

	//見たいところまでポインタを進める
	pOrbit += nCntOrbit;

	//保存した座標をずらす==========
	for (int nCntVtx = 1; nCntVtx < pOrbit->nNumEdge; nCntVtx++)
	{
		for (nCntOffset = 0; nCntOffset < NUM_OFFSET; nCntOffset++)
		{
			//一つ前の座標にずれる
			pOrbit->aPosPoint[nCntVtx - 1][nCntOffset] = pOrbit->aPosPoint[nCntVtx][nCntOffset];
		}
	}

	for (nCntOffset = 0; nCntOffset < NUM_OFFSET; nCntOffset++)
	{
		//現在のフレームのオフセット位置を保存
		pOrbit->aPosPoint[pOrbit->nNumEdge - 1][nCntOffset] =
		{
			pOrbit->mtxWorld[nCntOffset]._41,
			pOrbit->mtxWorld[nCntOffset]._42,
			pOrbit->mtxWorld[nCntOffset]._43
		};
	}
	//保存した座標をずらす==========

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffOrbit->Lock(0, 0, (void**)&pVtx, 0);

	//最初のポインタまで進める
	pVtx += MAX_EDGE * NUM_OFFSET * nCntOrbit;

	for (int nCntVtx = 0; nCntVtx < pOrbit->nNumEdge; nCntVtx++)
	{//辺ごとの頂点座標設定

		for (nCntOffset = 0; nCntOffset < NUM_OFFSET; nCntOffset++)
		{//オフセットの数分設定

			 //頂点座標の設定
			pVtx[nCntOffset].pos = pOrbit->aPosPoint[nCntVtx][nCntOffset];

			//頂点カラーの設定
			pVtx[nCntOffset].col = D3DXCOLOR(pOrbit->col.r, pOrbit->col.g, pOrbit->col.b, (float)nCntVtx / pOrbit->nNumEdge);
		}

		//ポインタを進める
		pVtx += NUM_OFFSET;
	}
}

//==========================================
//  描画処理
//==========================================
void DrawOrbit()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//情報取得
	Orbit *pOrbit = GetOrbit();

	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス

	//カリングを無効化
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//アルファテストの有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_OrbitmtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&g_OrbitmtxWorld, &g_OrbitmtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&g_OrbitmtxWorld, &g_OrbitmtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_OrbitmtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffOrbit, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntOrbit = 0;nCntOrbit < MAX_ORBIT;nCntOrbit++)
	{
		if (g_aOrbit[nCntOrbit].bUse)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureOrbit);

			//ポリゴンの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,			//プリミティブの種類
				MAX_EDGE * NUM_OFFSET * nCntOrbit,		//描画する最初のインデックス
				g_aOrbit[nCntOrbit].nNumEdge + (g_aOrbit[nCntOrbit].nNumEdge - 2)
			);												//描画するプリミティブ数
		}
	}

	//カリングを有効化
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//アルファテストの無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==========================================
//  設定処理
//==========================================
int SetOrbit(D3DXMATRIX mtxWorld, D3DXVECTOR3 posOffset1, D3DXVECTOR3 posOffset2, D3DXCOLOR col,int nNumEdge)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//変数宣言
	int nCounterOrbit = -1;

	//情報取得
	Orbit *pOrbit = GetOrbit();

	for (int nCntOrbit = 0;nCntOrbit < MAX_ORBIT;nCntOrbit++,pOrbit++)
	{
		if (pOrbit->bUse == false)
		{
			//色の代入
			pOrbit->col = col;

			//オフセットの代入
			pOrbit->posOffset[0] = posOffset1;
			pOrbit->posOffset[1] = posOffset2;

			//辺の数の代入
			pOrbit->nNumEdge = nNumEdge;

			pOrbit->mtxWorld[0]._41 = posOffset1.x + mtxWorld._41;
			pOrbit->mtxWorld[0]._42 = posOffset1.y + mtxWorld._42;
			pOrbit->mtxWorld[0]._43 = posOffset1.z + mtxWorld._43;
			pOrbit->mtxWorld[1]._41 = posOffset2.x + mtxWorld._41;
			pOrbit->mtxWorld[1]._42 = posOffset2.y + mtxWorld._42;
			pOrbit->mtxWorld[1]._43 = posOffset2.z + mtxWorld._43;

			for (int nCntVtx = 0; nCntVtx < pOrbit->nNumEdge; nCntVtx++)
			{
				pOrbit->aPosPoint[nCntVtx][0].x = posOffset1.x + mtxWorld._41;
				pOrbit->aPosPoint[nCntVtx][0].y = posOffset1.y + mtxWorld._42;
				pOrbit->aPosPoint[nCntVtx][0].z = posOffset1.z + mtxWorld._43;
				pOrbit->aPosPoint[nCntVtx][1].x = posOffset2.x + mtxWorld._41;
				pOrbit->aPosPoint[nCntVtx][1].y = posOffset2.y + mtxWorld._42;
				pOrbit->aPosPoint[nCntVtx][1].z = posOffset2.z + mtxWorld._43;
			}

			UpdateOrbitPolygon(nCntOrbit);

			//インデックスの代入
			nCounterOrbit = nCntOrbit;

			//使用状態にする
			pOrbit->bUse = true;

			break;
		}
	}

	return nCounterOrbit;
}

//==========================================
//  位置設定処理
//==========================================
void SetPositionOffset(D3DXMATRIX mtxWorld,int nIdxOrbit)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//情報取得処理
	Orbit *pOrbit = GetOrbit();

	//ポインタを進める
	pOrbit += nIdxOrbit;

	if (pOrbit->bUse)
	{
		for (int nCntOffset = 0; nCntOffset < NUM_OFFSET; nCntOffset++)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pOrbit->mtxWorld[nCntOffset]);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, pOrbit->posOffset[nCntOffset].x, pOrbit->posOffset[nCntOffset].y, pOrbit->posOffset[nCntOffset].z);
			D3DXMatrixMultiply(&pOrbit->mtxWorld[nCntOffset], &pOrbit->mtxWorld[nCntOffset], &mtxTrans);

			//マトリックスをかけ合わせる
			D3DXMatrixMultiply(&pOrbit->mtxWorld[nCntOffset], &pOrbit->mtxWorld[nCntOffset], &mtxWorld);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pOrbit->mtxWorld[nCntOffset]);
		}
	}

	//ポリゴン更新処理
	UpdateOrbitPolygon(nIdxOrbit);
}

//==================================================
//　軌跡の使用状態切り替え
//==================================================
void EnableOrbit(int nIdxOrbit, bool bUse)
{
	//使用状態代入
	g_aOrbit[nIdxOrbit].bUse = bUse;

	//中身をリセット
	ZeroMemory(&g_aOrbit[nIdxOrbit],sizeof(Orbit));
}

//==================================================
//　情報取得
//==================================================
Orbit *GetOrbit(void)
{
	return &g_aOrbit[0];
}

//==================================================
//　軌跡消去
//==================================================
void EnableOrbit(int nIdxOrbit)
{
	//変数初期化
	ZeroMemory(&g_aOrbit[nIdxOrbit], sizeof(Orbit));
}