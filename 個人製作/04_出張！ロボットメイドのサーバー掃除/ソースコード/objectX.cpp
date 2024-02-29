//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "objectX.h"
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
CObjectX::CObjectX(int nPriority = 6) : CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxScale = 1.0f;
	//m_model->pTexture = NULL;
	//m_model->pMeshModel = NULL;
	//m_model->pBuffMatModel = NULL;					//頂点情報を格納
	//m_model->dwNumMatModel = 0;
	//m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

CObjectX::~CObjectX()
{
}

//=====================================
// 生成処理
//=====================================
CObjectX *CObjectX::Create(void)
{
	CObjectX *pObject3D;

	//2Dオブジェクトの生成
	pObject3D = new CObjectX();

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
HRESULT CObjectX::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CObjectX::Uninit(void)
{
	Release();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CObjectX::Update(void)
{
	
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CObjectX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	m_mtxWorld._11 *= m_mtxScale;
	m_mtxWorld._22 *= m_mtxScale;
	m_mtxWorld._33 *= m_mtxScale;

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

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルのデータのポイントを取得
	pMat = (D3DXMATERIAL*)m_model->pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_model->dwNumMatModel; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_model->pTexture[nCntMat]);

		//モデル(パーツ)の描画
		m_model->pMeshModel->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=====================================
// ポリゴンの設定処理
//=====================================
void CObjectX::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	m_pos = pos;
	m_rot = rot;
	m_fHeight = fHeight;
	m_fWidth = fWidth;
}