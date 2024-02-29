//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "orbit.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
LPDIRECT3DTEXTURE9 COrbit::m_pTexture = NULL;

//=====================================
// コンストラクタ・デストラクタ
//=====================================
COrbit::COrbit(int nPriority = 5) : CObject(nPriority)
{
	m_pVtxBuff = NULL;				//頂点情報を格納
	m_pTexture = NULL;				//テクスチャへのポインタ

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

COrbit::~COrbit()
{
}

//=====================================
// 生成処理
//=====================================
COrbit *COrbit::Create(D3DXMATRIX mtxWorld, D3DXVECTOR3 posOffset1, D3DXVECTOR3 posOffset2, D3DXCOLOR col, int nNumEdge)
{
	COrbit *pOrbit = NULL;

	//2Dオブジェクトの生成
	pOrbit = new COrbit();

	if (pOrbit != NULL)
	{
		//初期化
		if (FAILED(pOrbit->Init()))
		{
			pOrbit->Release();

			return NULL;
		}

		//色の代入
		pOrbit->SetCol(col);

		//オフセットの代入
		pOrbit->SetposOffset(posOffset1, 0);
		pOrbit->SetposOffset(posOffset2, 1);
		
		//辺の数の代入
		pOrbit->SetEdge(nNumEdge);

		D3DXMATRIX mtxWorldOffsetTemp = {};
		mtxWorldOffsetTemp._41 = posOffset1.x + mtxWorld._41;
		mtxWorldOffsetTemp._42 = posOffset1.y + mtxWorld._42;
		mtxWorldOffsetTemp._43 = posOffset1.z + mtxWorld._43;
		pOrbit->SetmtxWorldOffset(mtxWorldOffsetTemp, 0);

		mtxWorldOffsetTemp._41 = posOffset2.x + mtxWorld._41;
		mtxWorldOffsetTemp._42 = posOffset2.y + mtxWorld._42;
		mtxWorldOffsetTemp._43 = posOffset2.z + mtxWorld._43;
		pOrbit->SetmtxWorldOffset(mtxWorldOffsetTemp, 1);

		for (int nCntVtx = 0; nCntVtx < pOrbit->m_nNumEdge; nCntVtx++)
		{
			pOrbit->SetPosPoint(D3DXVECTOR3(posOffset1.x + mtxWorld._41, posOffset1.y + mtxWorld._42, posOffset1.z + mtxWorld._43), nCntVtx, 0);
			pOrbit->SetPosPoint(D3DXVECTOR3(posOffset2.x + mtxWorld._41, posOffset2.y + mtxWorld._42, posOffset2.z + mtxWorld._43), nCntVtx, 1);
		}

		pOrbit->SetPositionOffset(mtxWorld);
	}

	return pOrbit;
}

//=====================================
// ポリゴンのテクスチャ処理
//=====================================
HRESULT COrbit::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//テクスチャの読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\orbit_000.png",
		&m_pTexture)))
	{
		return E_FAIL;
	}

	return S_OK;
}
void COrbit::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT COrbit::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * MAX_EDGE * NUM_OFFSET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < MAX_EDGE * NUM_OFFSET; nCntVtx++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f * (nCntVtx % NUM_OFFSET));

		pVtx++;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void COrbit::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	Release();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void COrbit::Update(void)
{
	
}

//=====================================
// ポリゴンの更新処理
//=====================================
void COrbit::UpdatePolygon(void)
{
	//変数宣言
	int nCntOffset;
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス
	D3DXVECTOR3 posTemp[NUM_OFFSET];

	//保存した座標をずらす==========
	for (int nCntVtx = 1; nCntVtx < m_nNumEdge; nCntVtx++)
	{
		for (nCntOffset = 0; nCntOffset < NUM_OFFSET; nCntOffset++)
		{
			//一つ前の座標にずれる
			m_aPosPoint[nCntVtx - 1][nCntOffset] = m_aPosPoint[nCntVtx][nCntOffset];
		}
	}

	for (nCntOffset = 0; nCntOffset < NUM_OFFSET; nCntOffset++)
	{
		//現在のフレームのオフセット位置を保存
		m_aPosPoint[m_nNumEdge - 1][nCntOffset] =
		{
			m_mtxWorldOffset[nCntOffset]._41,
			m_mtxWorldOffset[nCntOffset]._42,
			m_mtxWorldOffset[nCntOffset]._43
		};
	}
	//保存した座標をずらす==========

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < m_nNumEdge; nCntVtx++)
	{//辺ごとの頂点座標設定

		for (nCntOffset = 0; nCntOffset < NUM_OFFSET; nCntOffset++)
		{//オフセットの数分設定

			//頂点座標の設定
			pVtx[nCntOffset].pos = m_aPosPoint[nCntVtx][nCntOffset];

			//頂点カラーの設定
			pVtx[nCntOffset].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, (float)nCntVtx / m_nNumEdge);
		}

		//ポインタを進める
		pVtx += NUM_OFFSET;
	}

	m_pVtxBuff->Unlock();
}

//=====================================
// ポリゴンの描画処理
//=====================================
void COrbit::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

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
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive
	(D3DPT_TRIANGLESTRIP, 0, m_nNumEdge + (m_nNumEdge - 2));			//描画するプリミティブ数

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

//=====================================
// ポリゴンの設定処理
//=====================================
void COrbit::SetPositionOffset(D3DXMATRIX mtxWorld)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxOffset, mtxTrans;

	for (int nCntOffset = 0; nCntOffset < NUM_OFFSET; nCntOffset++)
	{
		mtxOffset = m_mtxWorldOffset[nCntOffset];

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxOffset);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_posOffset[nCntOffset].x, m_posOffset[nCntOffset].y, m_posOffset[nCntOffset].z);
		D3DXMatrixMultiply(&mtxOffset, &mtxOffset, &mtxTrans);

		//マトリックスをかけ合わせる
		D3DXMatrixMultiply(&mtxOffset, &mtxOffset, &mtxWorld);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxOffset);

		m_mtxWorldOffset[nCntOffset] = mtxOffset;
	}

	//ポリゴン更新処理
	UpdatePolygon();
}

//=====================================
// ポリゴンの設定処理
//=====================================
void COrbit::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	
}