//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "deliverypoint.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "collision.h"
#include "objectX.h"
#include "debugproc.h"
#include "player.h"
#include "sound.h"
#include "score.h"
#include "combo.h"
#include "map.h"
#include "useful.h"
#include "particle.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
CObjectX::MODELX CDeliverypoint::m_model = {};

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CDeliverypoint::CDeliverypoint(int nPriority = 3) : CObjectX(nPriority)
{
	m_nCntDelivery = 0;
	m_fHue = 0.0f;
}

CDeliverypoint::~CDeliverypoint()
{
}

//=====================================
// 生成処理
//=====================================
CDeliverypoint *CDeliverypoint::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	CDeliverypoint *pObjectBlock;

	//2Dオブジェクトの生成
	pObjectBlock = new CDeliverypoint();

	if (pObjectBlock != NULL)
	{
		pObjectBlock->Set(pos, rot, fWidth, fHeight);

		pObjectBlock->SetModel(m_model);
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
HRESULT CDeliverypoint::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATERIAL *pMat;

	//xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/deliverypoint000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_model.pBuffMatModel,
		NULL,
		&m_model.dwNumMatModel,
		&m_model.pMeshModel);

	int nNumVtx;
	DWORD dwSizeFVF;
	BYTE *pVtxBuff;

	//頂点数取得
	nNumVtx = m_model.pMeshModel->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(m_model.pMeshModel->GetFVF());

	//頂点バッファをロック
	m_model.pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (vtx.x >= m_model.vtxMax.x)
		{
			m_model.vtxMax.x = vtx.x;
		}
		if (vtx.x <= m_model.vtxMin.x)
		{
			m_model.vtxMin.x = vtx.x;
		}

		if (vtx.y >= m_model.vtxMax.y)
		{
			m_model.vtxMax.y = vtx.y;
		}
		if (vtx.y <= m_model.vtxMin.y)
		{
			m_model.vtxMin.y = vtx.y;
		}

		if (vtx.z >= m_model.vtxMax.z)
		{
			m_model.vtxMax.z = vtx.z;
		}
		if (vtx.z <= m_model.vtxMin.z)
		{
			m_model.vtxMin.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;
	}

	//頂点バッファをアンロック
	m_model.pMeshModel->UnlockVertexBuffer();

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_model.pBuffMatModel->GetBufferPointer();

	m_model.pTexture = new LPDIRECT3DTEXTURE9[m_model.dwNumMatModel];

	for (int nCntMat = 0; nCntMat < (int)m_model.dwNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&m_model.pTexture[nCntMat]);
		}
		else
		{
			m_model.pTexture[nCntMat] = NULL;
		}
	}

	return S_OK;
}
void CDeliverypoint::Unload(void)
{
	//テクスチャの破棄
	if (m_model.pMeshModel != NULL)
	{
		m_model.pMeshModel->Release();
		m_model.pMeshModel = NULL;
	}

	//頂点バッファの破棄
	if (m_model.pBuffMatModel != NULL)
	{
		m_model.pBuffMatModel->Release();
		m_model.pBuffMatModel = NULL;
	}

	//頂点バッファの破棄
	if (m_model.pTexture != NULL)
	{
		delete[] m_model.pTexture;
		m_model.pTexture = NULL;
	}
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CDeliverypoint::Init(void)
{
	CObjectX::Init();

	SetType(TYPE_DELIVERYPOINT);

	SetCollider(CCollider::Create(GetPosPointa(), GetRotPointa(), D3DXVECTOR3(100.0f, 5.0f, 100.0f), D3DXVECTOR3(-100.0f, -5.0f, -100.0f), CCollider::TAG_NONE));
	GetCollider()->SetType(CCollider::TYPE_BOX);

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CDeliverypoint::Uninit(void)
{	
	CObjectX::Uninit();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CDeliverypoint::Update(void)
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
	SetMtxScale(4.0f);

	Collision();
	//CManager::Get()->GetDebugProc()->Print("エネミーのpos: %f, %f, %f\n", pos.x, pos.y, pos.z);

	CObjectX::Update();
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CDeliverypoint::Draw(void)
{
	D3DXMATERIAL *pMat;
	//マテリアルのデータのポイントを取得
	pMat = (D3DXMATERIAL*)GetModel()->pBuffMatModel->GetBufferPointer();

	int combo = CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo();

	if (combo > 5)
	{
		m_fHue += 0.5f;
		pMat[0].MatD3D.Emissive = useful::HSLtoRGB(m_fHue);
	}
	else
	{
		pMat[0].MatD3D.Emissive = useful::HSLtoRGB(0.0f);
	}

	CObjectX::Draw();
}

bool CDeliverypoint::Collision(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posPlayer = CManager::Get()->Get()->GetScene()->GetPlayer()->GetPos();
	D3DXVECTOR3 vecPlayer;

	vecPlayer.x = pos.x - posPlayer.x;
	vecPlayer.y = pos.y - posPlayer.y;
	vecPlayer.z = pos.z - posPlayer.z;

	if (D3DXVec3Length(&vecPlayer) < 1200.0f)
	{
		m_nCntDelivery++;

		if (m_nCntDelivery >= 3)
		{
			m_nCntDelivery = 0;

			CManager::Get()->Get()->GetScene()->GetCombo()->AddCombo(10);
			int combo = CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo();
			CManager::Get()->Get()->GetScene()->GetScore()->AddScore(3000 * combo);
			CSound::PlaySound(CSound::SOUND_LABEL_SE_SCORE);

			for (int nCntParticle = 0; nCntParticle < 20; nCntParticle++)
			{
				float fHue = (float)(rand() % 361);
				CParticle::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), useful::HSLtoRGB(fHue), 5, 10, 2, 50, 30.0f, 30.0f);
			}

			SetPos(CManager::Get()->Get()->GetScene()->GetMap()->GetPosAreaCorner());
		}
	}

	return true;
}
