//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "drone.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "collision.h"
#include "combo.h"
#include "objectX.h"
#include "debugproc.h"
#include "useful.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
CObjectX::MODELX CDrone::m_model[CDrone::TYPE_MAX] = {};
const char *CDrone::m_apFilename[CDrone::TYPE_MAX] =
{
	"data/MODEL/deliveryDrone000.x",
};
//=====================================
// コンストラクタ・デストラクタ
//=====================================
CDrone::CDrone(int nPriority = 3) : CObjectX(nPriority)
{
	m_fHueOrg = 0.0f;
	m_fHue = 0.0f;
}

CDrone::~CDrone()
{
}

//=====================================
// 生成処理
//=====================================
CDrone *CDrone::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, CDrone::TYPE type)
{
	CDrone *pObjectBlock;

	//2Dオブジェクトの生成
	pObjectBlock = new CDrone();

	if (pObjectBlock != NULL)
	{
		pObjectBlock->Set(pos, rot, fWidth, fHeight);
		pObjectBlock->m_type = TYPE_NONE;

		pObjectBlock->SetModel(m_model[TYPE_NONE]);
		pObjectBlock->BindModel(pObjectBlock->GetModel());

		//初期化
		if (FAILED(pObjectBlock->Init()))
		{
			pObjectBlock->Release();
		}
	}

	return pObjectBlock;
}

//=====================================
// ポリゴンのテクスチャ処理
//=====================================
HRESULT CDrone::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATERIAL *pMat;

	for (int nCnt = 0; nCnt < CDrone::TYPE_MAX; nCnt++)
	{
		//xファイルの読み込み
		D3DXLoadMeshFromX(m_apFilename[nCnt],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_model[nCnt].pBuffMatModel,
			NULL,
			&m_model[nCnt].dwNumMatModel,
			&m_model[nCnt].pMeshModel);

		int nNumVtx;
		DWORD dwSizeFVF;
		BYTE *pVtxBuff;

		//頂点数取得
		nNumVtx = m_model[nCnt].pMeshModel->GetNumVertices();

		//頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(m_model[nCnt].pMeshModel->GetFVF());

		//頂点バッファをロック
		m_model[nCnt].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (vtx.x >= m_model[nCnt].vtxMax.x)
			{
				m_model[nCnt].vtxMax.x = vtx.x;
			}
			if (vtx.x <= m_model[nCnt].vtxMin.x)
			{
				m_model[nCnt].vtxMin.x = vtx.x;
			}

			if (vtx.y >= m_model[nCnt].vtxMax.y)
			{
				m_model[nCnt].vtxMax.y = vtx.y;
			}
			if (vtx.y <= m_model[nCnt].vtxMin.y)
			{
				m_model[nCnt].vtxMin.y = vtx.y;
			}

			if (vtx.z >= m_model[nCnt].vtxMax.z)
			{
				m_model[nCnt].vtxMax.z = vtx.z;
			}
			if (vtx.z <= m_model[nCnt].vtxMin.z)
			{
				m_model[nCnt].vtxMin.z = vtx.z;
			}

			pVtxBuff += dwSizeFVF;
		}

		//頂点バッファをアンロック
		m_model[nCnt].pMeshModel->UnlockVertexBuffer();

		//マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)m_model[nCnt].pBuffMatModel->GetBufferPointer();

		m_model[nCnt].pTexture = new LPDIRECT3DTEXTURE9[m_model[nCnt].dwNumMatModel];

		for (int nCntMat = 0; nCntMat < (int)m_model[nCnt].dwNumMatModel; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&m_model[nCnt].pTexture[nCntMat]);
			}
			else
			{
				m_model[nCnt].pTexture[nCntMat] = NULL;
			}
		}
	}
	return S_OK;
}
void CDrone::Unload(void)
{
	for (int nCnt = 0; nCnt < CDrone::TYPE_MAX; nCnt++)
	{
		//テクスチャの破棄
		if (m_model[nCnt].pMeshModel != NULL)
		{
			m_model[nCnt].pMeshModel->Release();
			m_model[nCnt].pMeshModel = NULL;
		}

		//頂点バッファの破棄
		if (m_model[nCnt].pBuffMatModel != NULL)
		{
			m_model[nCnt].pBuffMatModel->Release();
			m_model[nCnt].pBuffMatModel = NULL;
		}

		//頂点バッファの破棄
		if (m_model[nCnt].pTexture != NULL)
		{
			delete[] m_model[nCnt].pTexture;
			m_model[nCnt].pTexture = NULL;
		}
	}
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CDrone::Init(void)
{
	CObjectX::Init();

	SetType(TYPE_BLOCK);

	SetCollider(CCollider::Create(GetPosPointa(), GetRotPointa(), D3DXVECTOR3(310.0f, 160.0f, 310.0f), D3DXVECTOR3(-310.0f, -160.0f, -310.0f), CCollider::TAG_DRONE));
	GetCollider()->SetType(CCollider::TYPE_BOX);

	m_fHueOrg = (float)(rand() % 3600) * 0.1f;

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CDrone::Uninit(void)
{	
	CObjectX::Uninit();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CDrone::Update(void)
{
	CInput *input = CManager::Get()->GetInputKeyboard();
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	float fHeight = GetHeight();
	float fWidth = GetWidth();
	D3DXVECTOR3 movePos;

	int combo = CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo();

	if (combo > 5)
	{
		m_fHue += 1.0f;
	}
	else
	{
		m_fHue = m_fHueOrg;
	}

	SetPos(pos);
	SetRot(rot);
	SetMove(move);
	SetHeight(fHeight);
	SetWidth(fWidth);
	SetMtxScale(1.0f);

	//CManager::Get()->GetDebugProc()->Print("エネミーのpos: %f, %f, %f\n", pos.x, pos.y, pos.z);

	CObjectX::Update();
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CDrone::Draw(void)
{
	D3DXMATERIAL *pMat;
	//マテリアルのデータのポイントを取得
	pMat = (D3DXMATERIAL*)GetModel()->pBuffMatModel->GetBufferPointer();

	pMat[0].MatD3D.Diffuse = useful::HSLtoRGB(m_fHue);

	int combo = CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo();

	if (combo > 5)
	{
		pMat[0].MatD3D.Emissive = useful::HSLtoRGB(m_fHue);
	}

	CObjectX::Draw();
}