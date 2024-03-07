//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "combo.h"
#include "renderer.h"
#include "manager.h"
#include "object2D.h"
#include "bg.h"
#include "number.h"
#include "useful.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
int CCombo::m_nComboResult = 0;

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CCombo::CCombo(int nPriority = 6) : CObject(nPriority)
{
	m_nCombo = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fHue = 0.0f;
	m_nCombo = 0;
}

CCombo::~CCombo()
{
}

//=====================================
// 生成処理
//=====================================
CCombo *CCombo::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	CCombo *pScore;

	//2Dオブジェクトの生成
	pScore = new CCombo();

	if (pScore != NULL)
	{
		pScore->Set(pos, rot, fWidth, fHeight);

		//初期化
		if (FAILED(pScore->Init()))
		{
			pScore->Release();
		}

		CBg::CreateMin(D3DXVECTOR3(pos.x - 200.0f, pos.y, pos.z), 100.0f, 50.0f, CBg::TEXTURE_COMBO);
	}

	return pScore;
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CCombo::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_PLACE_COMBO; nCnt++)
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
void CCombo::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_PLACE_COMBO; nCnt++)
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
void CCombo::Update(void)
{
	m_posOld = m_pos;

	if (m_nCombo < 0)
	{
		m_nCombo = 0;
	}

	for (int nCnt = 0; nCnt < MAX_PLACE_COMBO; nCnt++)
	{
		if (m_apObject2D[nCnt] != NULL)
		{
			D3DXVECTOR2 *texPos = NULL;
			m_apObject2D[nCnt]->SetNumber(m_nCombo % (int)pow(10, nCnt + 1) / (int)pow(10, nCnt));

			if (m_nCombo > 5)
			{
				m_apObject2D[nCnt]->SetCol(useful::HSLtoRGB(m_fHue));

				m_fHue += 3.0f;
			}

			m_apObject2D[nCnt]->Update();
		}
	}
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CCombo::Draw(void)
{
	
}

//=====================================
// ポリゴンの設定処理
//=====================================
void CCombo::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	m_pos = pos;
	m_rot = rot;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}