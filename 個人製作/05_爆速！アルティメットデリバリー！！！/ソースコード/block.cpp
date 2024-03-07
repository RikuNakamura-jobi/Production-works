//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "block.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "collision.h"
#include "objectX.h"
#include "debugproc.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
CObjectX::MODELX CBlock::m_model[CBlock::TYPE_MAX] = {};
const char *CBlock::m_apFilename[CBlock::TYPE_MAX] =
{
	"data/MODEL/block000.x",
	"data/MODEL/block000.x",
	"data/MODEL/BigBuilding000.x",
	"data/MODEL/SmallBuilding000.x",
	"data/MODEL/Apart000.x"
};
//=====================================
// コンストラクタ・デストラクタ
//=====================================
CBlock::CBlock(int nPriority = 3) : CObjectX(nPriority)
{
}

CBlock::~CBlock()
{
}

//=====================================
// 生成処理
//=====================================
CBlock *CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, CBlock::TYPE type)
{
	CBlock *pObjectBlock;

	//2Dオブジェクトの生成
	pObjectBlock = new CBlock();

	if (pObjectBlock != NULL)
	{
		pObjectBlock->Set(pos, rot, fWidth, fHeight);
		pObjectBlock->m_type = type;

		pObjectBlock->SetModel(m_model[type]);
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
HRESULT CBlock::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATERIAL *pMat;

	for (int nCnt = 0; nCnt < CBlock::TYPE_MAX; nCnt++)
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
void CBlock::Unload(void)
{
	for (int nCnt = 0; nCnt < CBlock::TYPE_MAX; nCnt++)
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
HRESULT CBlock::Init(void)
{
	CObjectX::Init();

	SetType(TYPE_BLOCK);

	if (m_type == TYPE_BIGBUILDING)
	{
		SetCollider(CCollider::Create(GetPosPointa(), GetRotPointa(), D3DXVECTOR3(1800.0f* 2.0f, 14400.0f * 2.0f, 1800.0f* 2.0f), D3DXVECTOR3(-1800.0f* 2.0f, -10.0f, -1800.0f* 2.0f), CCollider::TAG_BOX));
		GetCollider()->SetType(CCollider::TYPE_BOX);
	}
	else if (m_type == TYPE_SMALLBUILDING)
	{
		SetCollider(CCollider::Create(GetPosPointa(), GetRotPointa(), D3DXVECTOR3(3000.0f, 20000.0f, 3000.0f), D3DXVECTOR3(-3000.0f, -10.0f, -3000.0f), CCollider::TAG_BOX));
		GetCollider()->SetType(CCollider::TYPE_BOX);
	}
	else if (m_type == TYPE_APART)
	{
		SetCollider(CCollider::Create(GetPosPointa(), GetRotPointa(), D3DXVECTOR3(3000.0f, 8000.0f, 3000.0f), D3DXVECTOR3(-3000.0f, -10.0f, -3000.0f), CCollider::TAG_BOX));
		GetCollider()->SetType(CCollider::TYPE_BOX);
	}
	else
	{
		SetCollider(CCollider::Create(GetPosPointa(), GetRotPointa(), D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f), D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f), CCollider::TAG_BOX));
		GetCollider()->SetType(CCollider::TYPE_BOX);
	}

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CBlock::Uninit(void)
{	
	CObjectX::Uninit();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CBlock::Update(void)
{
	CInput *input = CManager::Get()->GetInputKeyboard();
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	float fHeight = GetHeight();
	float fWidth = GetWidth();
	D3DXVECTOR3 movePos;

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
void CBlock::Draw(void)
{
	CObjectX::Draw();
}