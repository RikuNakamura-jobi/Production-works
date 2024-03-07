//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "particle.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "effect.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CParticle::CParticle(int nPriority = 2) : CObjectBillboard(nPriority)
{
	m_nLife = 0; 
	m_nLifePtcl = 0;
	m_nNumEffect = 0;
	m_nSpeed = 0;
}

CParticle::~CParticle()
{
}

//=====================================
// 生成処理
//=====================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, int nLifePtcl, int nNumEffect, int nSpeed, float fWidth, float fHeight)
{
	CParticle *pObjectEffect;

	//2Dオブジェクトの生成
	pObjectEffect = new CParticle();

	if (pObjectEffect != NULL)
	{
		//初期化
		if (FAILED(pObjectEffect->Init()))
		{
			pObjectEffect->Release();

			return NULL;
		}

		pObjectEffect->Set(pos, rot, fWidth, fHeight);
		pObjectEffect->SetCol(col);
		pObjectEffect->m_nLife = nLife;
		pObjectEffect->m_nLifePtcl = nLifePtcl;
		pObjectEffect->m_nSpeed = nSpeed;
		pObjectEffect->m_nNumEffect = nNumEffect;
	}

	return pObjectEffect;
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CParticle::Init(void)
{
	SetType(TYPE_PARTICLE);

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CParticle::Uninit(void)
{
	Release();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CParticle::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	float fHeight = GetHeight();
	float fWidth = GetWidth();

	for (int nCntEf = 0; nCntEf < m_nNumEffect; nCntEf++)
	{
		//移動量を決定する
		D3DXVec3Normalize(&move, &D3DXVECTOR3((float)(rand() % (m_nSpeed * 2) - m_nSpeed), (float)(rand() % (m_nSpeed * 2) - m_nSpeed), (float)(rand() % (m_nSpeed * 2) - m_nSpeed)));

		pos.x = (float)(rand() % 21) + GetPos().x;
		pos.y = (float)(rand() % 21) + GetPos().y;
		pos.z = (float)(rand() % 21) + GetPos().z;

		move.x *= rand() % m_nSpeed;
		move.y *= rand() % m_nSpeed;
		move.z *= rand() % m_nSpeed;

		fHeight = (rand() % 20) * 0.1f + GetHeight();
		fWidth = fHeight;

		CEffect3D::Create(pos, move, rot, GetCol(), rand() % 60 + m_nLife, fWidth, fHeight);
	}

	//寿命減少
	m_nLifePtcl--;

	if (m_nLifePtcl <= 0)
	{
		//使用していない状態にする
		Uninit();
	}
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CParticle::Draw(void)
{
	
}