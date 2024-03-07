//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "time.h"
#include "renderer.h"
#include "manager.h"
#include "object2D.h"
#include "number.h"
#include "combo.h"
#include "bg.h"
#include "debugproc.h"
#include "useful.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
DWORD CTime::m_startTime = 0;
DWORD CTime::m_gameTime = 0;

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CTime::CTime(int nPriority = 6) : CObject(nPriority)
{
	m_nTime = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fHue = 0.0f;
	m_nTime = 0;
}

CTime::~CTime()
{
}

//=====================================
// 生成処理
//=====================================
CTime *CTime::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	CTime *pScore;

	//2Dオブジェクトの生成
	pScore = new CTime();

	if (pScore != NULL)
	{
		pScore->Set(pos, rot, fWidth, fHeight);

		//初期化
		if (FAILED(pScore->Init()))
		{
			pScore->Release();
		}

		CBg::CreateMin(D3DXVECTOR3(pos.x + 30.0f, pos.y - 75.0f, pos.z), 100.0f, 50.0f, CBg::TEXTURE_TIME);
	}

	return pScore;
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CTime::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_PLACE_TIME; nCnt++)
	{
		m_apObject2D[nCnt] = CNumber::Create();

		if (m_apObject2D[nCnt] != NULL)
		{
			if (FAILED(m_apObject2D[nCnt]->Init()))
			{
				return E_FAIL;
			}

			if (nCnt == 2)
			{
				m_apObject2D[nCnt]->Set(D3DXVECTOR3((m_pos.x - (m_fWidth + 5.0f) * nCnt) + 65.0f, m_pos.y, 0.0f), m_rot, m_fWidth, m_fHeight);
			}
			else
			{
				m_apObject2D[nCnt]->Set(D3DXVECTOR3((m_pos.x - (m_fWidth + 5.0f) * nCnt) + 95.0f, m_pos.y, 0.0f), m_rot, m_fWidth, m_fHeight);
			}

			m_apObject2D[nCnt]->SetNumber(0);
		}
	}

	m_startTime = timeGetTime();

	SetType(TYPE_SCORE);

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CTime::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_PLACE_TIME; nCnt++)
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
void CTime::Update(void)
{
	if (CManager::Get()->Get()->GetScene()->GetStartCount() < 1)
	{
		m_posOld = m_pos;
		int nMinutes = 0, nSeconds = 0;
		m_gameTime = timeGetTime() - m_startTime;

		m_nTime = (int)(m_gameTime * 0.001f);

		nSeconds = 60 - (m_nTime % 60);
		if (nSeconds == 60)
		{
			nSeconds = 0;
		}

		nMinutes = (3 - ((m_nTime + 59) / 60)) * 100;

		m_nTime = nSeconds + nMinutes;

		if (m_nTime < 0)
		{
			m_nTime = 0;
		}

		for (int nCnt = 0; nCnt < MAX_PLACE_TIME; nCnt++)
		{
			if (m_apObject2D[nCnt] != NULL)
			{
				D3DXVECTOR2 *texPos = NULL;
				m_apObject2D[nCnt]->SetNumber(m_nTime % (int)pow(10, nCnt + 1) / (int)pow(10, nCnt));

				m_apObject2D[nCnt]->Update();
			}
		}
	}

	if (CManager::Get()->Get()->GetScene()->GetStartCount() >= 210)
	{
		m_posOld = m_pos;
		int nMinutes = 0, nSeconds = 0;
		m_gameTime = timeGetTime() - m_startTime;

		m_nTime = (int)(m_gameTime * 0.001f);

		nSeconds = 60 - (m_nTime % 60);
		if (nSeconds == 60)
		{
			nSeconds = 0;
		}

		nMinutes = (3 - ((m_nTime + 59) / 60)) * 100;

		m_nTime = nSeconds + nMinutes;

		if (m_nTime < 0)
		{
			m_nTime = 0;
		}

		for (int nCnt = 0; nCnt < MAX_PLACE_TIME; nCnt++)
		{
			if (m_apObject2D[nCnt] != NULL)
			{
				D3DXVECTOR2 *texPos = NULL;
				m_apObject2D[nCnt]->SetNumber(m_nTime % (int)pow(10, nCnt + 1) / (int)pow(10, nCnt));

				if (CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo() > 5)
				{
					m_apObject2D[nCnt]->SetCol(useful::HSLtoRGB(m_fHue));

					m_fHue += 2.0f;
				}

				m_apObject2D[nCnt]->Update();
			}
		}
	}
	else
	{
		m_startTime = timeGetTime();
	}

	CManager::Get()->Get()->GetDebugProc()->Print("時間: %d\n", m_nTime);
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CTime::Draw(void)
{
	
}

//=====================================
// ポリゴンの設定処理
//=====================================
void CTime::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	m_pos = pos;
	m_rot = rot;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}