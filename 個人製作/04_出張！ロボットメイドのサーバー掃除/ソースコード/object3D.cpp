//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "object3D.h"
#include "renderer.h"
#include "manager.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CObject3D::CObject3D(int nPriority = 3) : CObject(nPriority)
{
	m_pVtxBuff = NULL;						//頂点情報を格納
	m_pTexturePolygon = NULL;				//テクスチャへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_texPos[0] = D3DXVECTOR2(0.0f, 0.0f);
	m_texPos[1] = D3DXVECTOR2(1.0f, 0.0f);
	m_texPos[2] = D3DXVECTOR2(0.0f, 1.0f);
	m_texPos[3] = D3DXVECTOR2(1.0f, 1.0f);

	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
}

CObject3D::~CObject3D()
{
}

//=====================================
// 生成処理
//=====================================
CObject3D *CObject3D::Create(void)
{
	CObject3D *pObject3D;

	//2Dオブジェクトの生成
	pObject3D = new CObject3D();

	if (pObject3D != NULL)
	{
		//初期化
		if (FAILED(pObject3D->Init()))
		{
			pObject3D->Release();
		}
	}

	return pObject3D;
}


//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CObject3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_size.x;
	pVtx[0].pos.y = m_size.y;
	pVtx[0].pos.z = -m_size.z;

	pVtx[1].pos.x = -m_size.x;
	pVtx[1].pos.y = m_size.y;
	pVtx[1].pos.z = -m_size.z;

	pVtx[2].pos.x = m_size.x;
	pVtx[2].pos.y = -m_size.y;
	pVtx[2].pos.z = m_size.z;

	pVtx[3].pos.x = -m_size.x;
	pVtx[3].pos.y = -m_size.y;
	pVtx[3].pos.z = m_size.z;

	//頂点座標の設定
	D3DXVec3Cross(&pVtx[0].nor, &(pVtx[0].pos - pVtx[1].pos), &(pVtx[0].pos - pVtx[2].pos));
	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

	pVtx[0].nor.x = ((pVtx[0].pos.y - pVtx[1].pos.y) * (pVtx[0].pos.z - pVtx[2].pos.z)) - ((pVtx[0].pos.z - pVtx[1].pos.z) * (pVtx[0].pos.y - pVtx[2].pos.y));
	pVtx[0].nor.y = ((pVtx[0].pos.z - pVtx[1].pos.z) * (pVtx[0].pos.x - pVtx[2].pos.x)) - ((pVtx[0].pos.x - pVtx[1].pos.x) * (pVtx[0].pos.z - pVtx[2].pos.z));
	pVtx[0].nor.z = ((pVtx[0].pos.x - pVtx[1].pos.x) * (pVtx[0].pos.y - pVtx[2].pos.y)) - ((pVtx[0].pos.y - pVtx[1].pos.y) * (pVtx[0].pos.x - pVtx[2].pos.x));

	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

	D3DXVec3Cross(&pVtx[1].nor, &(pVtx[1].pos - pVtx[3].pos), &(pVtx[1].pos - pVtx[0].pos));
	D3DXVec3Normalize(&pVtx[1].nor, &pVtx[1].nor);

	D3DXVec3Cross(&pVtx[2].nor, &(pVtx[2].pos - pVtx[0].pos), &(pVtx[2].pos - pVtx[3].pos));
	D3DXVec3Normalize(&pVtx[2].nor, &pVtx[2].nor);

	D3DXVec3Cross(&pVtx[3].nor, &(pVtx[3].pos - pVtx[2].pos), &(pVtx[3].pos - pVtx[1].pos));
	D3DXVec3Normalize(&pVtx[3].nor, &pVtx[3].nor);

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

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CObject3D::Uninit(void)
{
	TYPE type = GetType();

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
void CObject3D::Update(void)
{
	
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CObject3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		m_pVtxBuff,
		0,
		sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexturePolygon);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=====================================
// ポリゴンの設定処理
//=====================================
void CObject3D::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	m_pos = pos;
	m_rot = rot;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}

//=====================================
// ポリゴンのサイズ設定処理
//=====================================
void CObject3D::SetPosSize(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_size.x;
	pVtx[0].pos.y = m_size.y;
	pVtx[0].pos.z = -m_size.z;

	pVtx[1].pos.x = -m_size.x;
	pVtx[1].pos.y = m_size.y;
	pVtx[1].pos.z = -m_size.z;

	pVtx[2].pos.x = m_size.x;
	pVtx[2].pos.y = -m_size.y;
	pVtx[2].pos.z = m_size.z;

	pVtx[3].pos.x = -m_size.x;
	pVtx[3].pos.y = -m_size.y;
	pVtx[3].pos.z = m_size.z;

	//頂点座標の設定
	D3DXVec3Cross(&pVtx[0].nor, &(pVtx[0].pos - pVtx[1].pos), &(pVtx[0].pos - pVtx[2].pos));
	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

	pVtx[0].nor.x = ((pVtx[0].pos.y - pVtx[1].pos.y) * (pVtx[0].pos.z - pVtx[2].pos.z)) - ((pVtx[0].pos.z - pVtx[1].pos.z) * (pVtx[0].pos.y - pVtx[2].pos.y));
	pVtx[0].nor.y = ((pVtx[0].pos.z - pVtx[1].pos.z) * (pVtx[0].pos.x - pVtx[2].pos.x)) - ((pVtx[0].pos.x - pVtx[1].pos.x) * (pVtx[0].pos.z - pVtx[2].pos.z));
	pVtx[0].nor.z = ((pVtx[0].pos.x - pVtx[1].pos.x) * (pVtx[0].pos.y - pVtx[2].pos.y)) - ((pVtx[0].pos.y - pVtx[1].pos.y) * (pVtx[0].pos.x - pVtx[2].pos.x));

	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

	D3DXVec3Cross(&pVtx[1].nor, &(pVtx[1].pos - pVtx[3].pos), &(pVtx[1].pos - pVtx[0].pos));
	D3DXVec3Normalize(&pVtx[1].nor, &pVtx[1].nor);

	D3DXVec3Cross(&pVtx[2].nor, &(pVtx[2].pos - pVtx[0].pos), &(pVtx[2].pos - pVtx[3].pos));
	D3DXVec3Normalize(&pVtx[2].nor, &pVtx[2].nor);

	D3DXVec3Cross(&pVtx[3].nor, &(pVtx[3].pos - pVtx[2].pos), &(pVtx[3].pos - pVtx[1].pos));
	D3DXVec3Normalize(&pVtx[3].nor, &pVtx[3].nor);

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

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CObject3D::SetPosSizeX(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_size.x;
	pVtx[0].pos.y = m_size.y;
	pVtx[0].pos.z = m_size.z;

	pVtx[1].pos.x = -m_size.x;
	pVtx[1].pos.y = m_size.y;
	pVtx[1].pos.z = -m_size.z;

	pVtx[2].pos.x = m_size.x;
	pVtx[2].pos.y = -m_size.y;
	pVtx[2].pos.z = m_size.z;

	pVtx[3].pos.x = -m_size.x;
	pVtx[3].pos.y = -m_size.y;
	pVtx[3].pos.z = -m_size.z;

	//頂点座標の設定
	D3DXVec3Cross(&pVtx[0].nor, &(pVtx[0].pos - pVtx[1].pos), &(pVtx[0].pos - pVtx[2].pos));
	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

	pVtx[0].nor.x = ((pVtx[0].pos.y - pVtx[1].pos.y) * (pVtx[0].pos.z - pVtx[2].pos.z)) - ((pVtx[0].pos.z - pVtx[1].pos.z) * (pVtx[0].pos.y - pVtx[2].pos.y));
	pVtx[0].nor.y = ((pVtx[0].pos.z - pVtx[1].pos.z) * (pVtx[0].pos.x - pVtx[2].pos.x)) - ((pVtx[0].pos.x - pVtx[1].pos.x) * (pVtx[0].pos.z - pVtx[2].pos.z));
	pVtx[0].nor.z = ((pVtx[0].pos.x - pVtx[1].pos.x) * (pVtx[0].pos.y - pVtx[2].pos.y)) - ((pVtx[0].pos.y - pVtx[1].pos.y) * (pVtx[0].pos.x - pVtx[2].pos.x));

	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

	D3DXVec3Cross(&pVtx[1].nor, &(pVtx[1].pos - pVtx[3].pos), &(pVtx[1].pos - pVtx[0].pos));
	D3DXVec3Normalize(&pVtx[1].nor, &pVtx[1].nor);

	D3DXVec3Cross(&pVtx[2].nor, &(pVtx[2].pos - pVtx[0].pos), &(pVtx[2].pos - pVtx[3].pos));
	D3DXVec3Normalize(&pVtx[2].nor, &pVtx[2].nor);

	D3DXVec3Cross(&pVtx[3].nor, &(pVtx[3].pos - pVtx[2].pos), &(pVtx[3].pos - pVtx[1].pos));
	D3DXVec3Normalize(&pVtx[3].nor, &pVtx[3].nor);

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

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

float CObject3D::GetColHeight(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, D3DXVECTOR3 *move)
{
	D3DXVECTOR3 posV[4], vecP, vecPOld, nor, vecTemp;
	float fHeight, fHeightOld;
	D3DXPLANE plane = {};

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	posV[0] = pVtx[0].pos;
	posV[1] = pVtx[1].pos;
	posV[2] = pVtx[2].pos;
	posV[3] = pVtx[3].pos;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	if (D3DXVec3Cross(&vecTemp, &(pos - posV[0]), &(posV[1] - pVtx[0].pos))->y < 0 &&
		D3DXVec3Cross(&vecTemp, &(pos - posV[1]), &(posV[2] - posV[1]))->y <= 0 &&
		D3DXVec3Cross(&vecTemp, &(pos - posV[2]), &(posV[0] - posV[2]))->y < 0)
	{
		nor = pVtx[0].nor;
		vecP = pos - posV[0];
		vecPOld = posOld - posV[0];

		if (nor.y != 0.0f)
		{
			fHeight = -(((nor.x * vecP.x) + (nor.z * vecP.z)) / nor.y) + posV[0].y;
			fHeightOld = -(((nor.x * vecPOld.x) + (nor.z * vecPOld.z)) / nor.y) + posV[0].y;

			if (fHeight > pos.y && fHeightOld <= posOld.y)
			{
				move->y = 0.0f;
				return fHeight;
			}

			if (D3DXVec3Cross(&vecTemp, &(posOld - posV[3]), &(posV[2] - posV[3]))->y < 0 &&
				D3DXVec3Cross(&vecTemp, &(posOld - posV[2]), &(posV[1] - posV[2]))->y <= 0 &&
				D3DXVec3Cross(&vecTemp, &(posOld - posV[1]), &(posV[3] - posV[1]))->y < 0)
			{
				if (fHeight > pos.y && fHeightOld > posOld.y)
				{
					move->y = 0.0f;
					return fHeight;
				}
			}
		}
	}

	if (D3DXVec3Cross(&vecTemp, &(pos - posV[3]), &(posV[2] - posV[3]))->y < 0 &&
		D3DXVec3Cross(&vecTemp, &(pos - posV[2]), &(posV[1] - posV[2]))->y <= 0 &&
		D3DXVec3Cross(&vecTemp, &(pos - posV[1]), &(posV[3] - posV[1]))->y < 0)
	{
		nor = pVtx[3].nor;
		vecP = pos - posV[3];
		vecPOld = posOld - posV[3];

		if (nor.y != 0.0f)
		{
			fHeight = -(((nor.x * vecP.x) + (nor.z * vecP.z)) / nor.y) + posV[3].y;
			fHeightOld = -(((nor.x * vecPOld.x) + (nor.z * vecPOld.z)) / nor.y) + posV[3].y;

			if (fHeight > pos.y && fHeightOld <= posOld.y)
			{
				move->y = 0.0f;
				return fHeight;
			}

			if (D3DXVec3Cross(&vecTemp, &(posOld - posV[0]), &(posV[1] - pVtx[0].pos))->y < 0 &&
				D3DXVec3Cross(&vecTemp, &(posOld - posV[1]), &(posV[2] - posV[1]))->y <= 0 &&
				D3DXVec3Cross(&vecTemp, &(posOld - posV[2]), &(posV[0] - posV[2]))->y < 0)
			{
				if (fHeight > pos.y && fHeightOld > posOld.y)
				{
					move->y = 0.0f;
					return fHeight;
				}
			}
		}
	}

	return pos.y;
}