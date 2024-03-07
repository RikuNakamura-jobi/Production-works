//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "score.h"
#include "renderer.h"
#include "manager.h"
#include "object2D.h"
#include "number.h"
#include "combo.h"
#include "bg.h"
#include "useful.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
int CScore::m_nScoreResult = 0;

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CScore::CScore(int nPriority = 6) : CObject(nPriority)
{
	m_nScore = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fHue = 0.0f;
	m_nScore = 0;
}

CScore::~CScore()
{
}

//=====================================
// 生成処理
//=====================================
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, bool result)
{
	CScore *pScore;

	//2Dオブジェクトの生成
	pScore = new CScore();

	if (pScore != NULL)
	{
		pScore->Set(pos, rot, fWidth, fHeight);

		//初期化
		if (FAILED(pScore->Init()))
		{
			pScore->Release();
		}

		if (!result)
		{
			CBg::CreateMin(D3DXVECTOR3(pos.x - 480.0f, pos.y, pos.z), 100.0f, 50.0f, CBg::TEXTURE_SCORE);

		}
	}

	return pScore;
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CScore::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_PLACE; nCnt++)
	{
		m_apObject2D[nCnt] = CNumber::Create();

		if (m_apObject2D[nCnt] != NULL)
		{
			if (FAILED(m_apObject2D[nCnt]->Init()))
			{
				return E_FAIL;
			}

			m_apObject2D[nCnt]->Set(D3DXVECTOR3(m_pos.x - (m_fWidth + 5.0f) * nCnt, m_pos.y, 0.0f), m_rot, m_fWidth, m_fHeight);
			m_apObject2D[nCnt]->SetNumber(0);
		}
	}

	SetType(TYPE_SCORE);

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CScore::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_PLACE; nCnt++)
	{
		if (m_apObject2D[nCnt] != NULL)
		{
			m_apObject2D[nCnt]->Uninit();
		}
	}

	Release();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CScore::Update(void)
{
	m_posOld = m_pos;

	if (m_nScore < 0)
	{
		m_nScore = 0;
	}

	for (int nCnt = 0; nCnt < MAX_PLACE; nCnt++)
	{
		if (m_apObject2D[nCnt] != NULL)
		{
			D3DXVECTOR2 *texPos = NULL;
			m_apObject2D[nCnt]->SetNumber(m_nScore % (int)pow(10, nCnt + 1) / (int)pow(10, nCnt));

			if (CManager::Get()->Get()->GetScene()->GetCombo() != nullptr)
			{
				if (CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo() > 5)
				{
					m_apObject2D[nCnt]->SetCol(useful::HSLtoRGB(m_fHue));

					m_fHue += 1.0f;
				}
			}

			m_apObject2D[nCnt]->Update();
		}
	}
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CScore::Draw(void)
{
	
}

//=====================================
// ポリゴンの設定処理
//=====================================
void CScore::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	m_pos = pos;
	m_rot = rot;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}