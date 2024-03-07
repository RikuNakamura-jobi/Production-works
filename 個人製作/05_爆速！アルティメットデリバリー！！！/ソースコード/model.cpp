//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "model.h"
#include "renderer.h"
#include "manager.h"
#include "useful.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CModel::CModel()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fHue = 0.0f;
	m_pParent = nullptr;
}

CModel::~CModel()
{
}

//=====================================
// 生成処理
//=====================================
CModel *CModel::Create(char *pFilename)
{
	CModel *pModel;

	//2Dオブジェクトの生成
	pModel = new CModel;

	if (pModel != NULL)
	{
		//初期化
		if (FAILED(pModel->Init(pFilename)))
		{
			pModel->Uninit();

			return NULL;
		}
	}

	return pModel;
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CModel::Init(char *pFilename)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();
	D3DXMATERIAL *pMat;

	//xファイルの読み込み
	D3DXLoadMeshFromX(pFilename,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMatModel,
		NULL,
		&m_dwNumMatModel,
		&m_pMeshModel);

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();

	m_pTexture = new LPDIRECT3DTEXTURE9[m_dwNumMatModel];

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&m_pTexture[nCntMat]);
		}
		else
		{
			m_pTexture[nCntMat] = NULL;
		}
	}

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CModel::Uninit(void)
{
	//テクスチャの破棄
	if (m_pMeshModel != NULL)
	{
		m_pMeshModel->Release();
		m_pMeshModel = NULL;
	}

	//頂点バッファの破棄
	if (m_pBuffMatModel != NULL)
	{
		m_pBuffMatModel->Release();
		m_pBuffMatModel = NULL;
	}

	if (m_pTexture != NULL)
	{
		delete[] m_pTexture;
		m_pTexture = NULL;
	}
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CModel::Update(void)
{
	m_fHue += 2.0f;
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;
	D3DXMATRIX mtxRotModel, mtxTransModel, mtxPalent;		//計算用マトリックス

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

	//パーツの親マトリックスを設定
	if (m_pParent != NULL)
	{
		mtxPalent = m_pParent->GetMtxWorld();
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxPalent);
	}

	//算出したパーツのワールドマトリックスと親のマトリックスをかけ合わせる
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxPalent);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルのデータのポイントを取得
	pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMatModel; nCntMat++)
	{
		/*pMat[nCntMat].MatD3D.Diffuse = useful::HSLtoRGB(m_fHue);
		pMat[nCntMat].MatD3D.Emissive = useful::HSLtoRGB(m_fHue);*/

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture[nCntMat]);

		//モデル(パーツ)の描画
		m_pMeshModel->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}