//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "enemy.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "bullet.h"
#include "scene.h"
#include "lockon.h"
#include "score.h"
#include "collision.h"
#include "enemymanager.h"
#include "objectX.h"
#include "debugproc.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
CObjectX::MODELX CEnemy::m_model = {};

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CEnemy::CEnemy(int nPriority = 3) : CObjectX(nPriority)
{
	m_nCounter = 0;
	m_nPattern = 0;
	m_nLife = 0;
}

CEnemy::~CEnemy()
{
}

//=====================================
// 生成処理
//=====================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nWave)
{
	CEnemy *pObjectEnemy;

	//2Dオブジェクトの生成
	pObjectEnemy = new CEnemy();

	if (pObjectEnemy != NULL)
	{
		pObjectEnemy->Set(pos, rot, fWidth, fHeight);

		pObjectEnemy->SetModel(m_model);
		pObjectEnemy->BindModel(pObjectEnemy->GetModel());
		pObjectEnemy->m_nWave = nWave;

		//初期化
		if (FAILED(pObjectEnemy->Init()))
		{
			pObjectEnemy->Release();
		}
	}

	return pObjectEnemy;
}

//=====================================
// ポリゴンのテクスチャ処理
//=====================================
HRESULT CEnemy::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATERIAL *pMat;

	//xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/actionEnemy000.x",
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
void CEnemy::Unload(void)
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
HRESULT CEnemy::Init(void)
{
	CObjectX::Init();

	m_nLife = 10;
	SetType(TYPE_ENEMY);

	if (m_nWave == 1)
	{
		SetCollider(CCollider::Create(GetPosPointa(), GetRotPointa(), D3DXVECTOR3(120.0f, 90.0f, 120.0f), D3DXVECTOR3(-120.0f, -90.0f, -120.0f)));
		GetCollider()->SetType(CCollider::TYPE_BOX);
		m_nLife = 20;
	}
	else
	{
		SetCollider(CCollider::Create(GetPosPointa(), GetRotPointa(), D3DXVECTOR3(40.0f, 30.0f, 40.0f), D3DXVECTOR3(-40.0f, -30.0f, -40.0f)));
		GetCollider()->SetType(CCollider::TYPE_BOX);
	}

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CEnemy::Uninit(void)
{
	CManager::Get()->GetScene()->GetEnemyManager()->GetEnemyWave(0)->addNumEnemy(-1);
	CManager::Get()->GetScene()->GetEnemyManager()->GetEnemyWave(0)->SetEnemy(m_nIdxSpawn, NULL);

	if (m_lockon != NULL)
	{
		m_lockon->Uninit();
		m_lockon = NULL;
	}

	if (m_nWave == 1)
	{
		CManager::Get()->GetScene()->SetFinish();
		CManager::Get()->GetScene()->GetScore()->AddScore(1500);
	}
	
	CObjectX::Uninit();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CEnemy::Update(void)
{
	CInput *input = CManager::Get()->GetInputKeyboard();
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	float fHeight = GetHeight();
	float fWidth = GetWidth();
	D3DXVECTOR3 movePos;
	m_nCounter++;				//カウンターを加算

	//パターンN0.を更新する
	m_nPattern = (m_nCounter / 5) % 10;

	movePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fRotMove, fRotDest, fRotDiff;

	//現在の角度と目的の角度の差分を計算
	fRotMove = rot.y;
	fRotDest = atan2f(pos.x - CManager::Get()->GetScene()->GetPlayer()->GetPos().x, pos.z - CManager::Get()->GetScene()->GetPlayer()->GetPos().z);
	fRotDiff = fRotDest - fRotMove;

	if (fRotDiff > 3.14f)
	{
		fRotDiff -= 6.28f;
	}
	else if (fRotDiff <= -3.14f)
	{
		fRotDiff += 6.28f;
	}

	//徐々に足してく
	fRotMove += fRotDiff * 0.2f;

	if (fRotMove > 3.14f)
	{
		fRotMove -= 6.28f;
	}
	else if (fRotMove <= -3.14f)
	{
		fRotMove += 6.28f;
	}

	rot.y = fRotMove;

	//move.y -= 30.8f;

	pos += move;

	move.x += (0 - move.x) * 0.15f;
	move.z += (0 - move.z) * 0.15f;

	if (pos.y < 0.0f)
	{
		pos.y = 0.0f;
		move.y = 0.0f;
	}

	SetPos(pos);
	SetRot(rot);
	SetMove(move);
	SetHeight(fHeight);
	SetWidth(fWidth);

	//CManager::Get()->GetDebugProc()->Print("エネミーのpos: %f, %f, %f\n", pos.x, pos.y, pos.z);

	CObjectX::Update();
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CEnemy::Draw(void)
{
	if (m_nWave == 1)
	{
		SetMtxScale(3.0f);
	}

	CObjectX::Draw();
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CEnemy::SetLockon(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	float fHeight = GetHeight();
	float fWidth = GetWidth();

	if (m_lockon == NULL)
	{
		m_lockon = CLockon::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 30.0f, 30.0f);
	}
}