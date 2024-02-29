//==========================================
//
//  マップクラス(map.cpp)
//  Autho : Tomoya Kanazaki
//
//==========================================
#include "map.h"
#include "spot.h"
#include "player.h"
#include "gameManager.h"

//==========================================
//  静的メンバ変数宣言
//==========================================
const CMap::DIFF CMap::m_Diff = { 100, 200, 100, SCREEN_WIDTH - 300 };
float CMap::m_vecMove = 1.0f;

//==========================================
//  コンストラクタ
//==========================================
CMap::CMap()
{
	m_nCntAdd = 0;
	m_fSpeed = m_vecMove;
	for (int nCnt = 0; nCnt < NUM; nCnt++)
	{
		m_pSpot[nCnt] = nullptr;
	}
}

//==========================================
//  デストラクタ
//==========================================
CMap::~CMap()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CMap::Init(void)
{
	//座標生成処理
	SetSpot();

	return S_OK;
}

//==========================================
//  終了処理
//==========================================
void CMap::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM; nCnt++)
	{
		m_pSpot[nCnt]->Uninit();
		m_pSpot[nCnt] = nullptr;
	}
}

//==========================================
//  更新処理
//==========================================
void CMap::Update(void)
{
	if (GetHeightMin().y <= SCREEN_CENT.y)
	{
		m_fSpeed = m_vecMove * 20.0f;
	}
	else
	{
		m_fSpeed = m_vecMove;
	}

	//位置を更新
	for (int nCnt = 0; nCnt < NUM; nCnt++)
	{
		if (m_pSpot[nCnt] != nullptr)
		{
			D3DXVECTOR3 pos = m_pSpot[nCnt]->GetPosition();
			pos.y += m_fSpeed;
			m_pSpot[nCnt]->SetPosition(pos);
			m_pSpot[nCnt]->Update();
		}
	}
}

//==========================================
//  描画処理
//==========================================
void CMap::Draw(void)
{
	//位置を描画
	for (int nCnt = 0; nCnt < NUM; nCnt++)
	{
		m_pSpot[nCnt]->Draw();
	}
}

//==========================================
//  一番低い地点の取得
//==========================================
D3DXVECTOR3 CMap::GetHeightMin(void)
{
	//返り値のための変数
	D3DXVECTOR3 pos;

	//データを保管
	float* pPos = new float[NUM];
	for (int nCnt = 0; nCnt < NUM; nCnt++)
	{
		if (m_pSpot[nCnt] != nullptr)
		{
			pPos[nCnt] = m_pSpot[nCnt]->GetPosHeight();
		}
		else
		{
			pPos[nCnt] = -10000000.0f;
		}
	}

	//ソート処理
	useful::SortNum(pPos, NUM);

	for (int nCnt = 0; nCnt < NUM; nCnt++)
	{
		if (m_pSpot[nCnt] != nullptr)
		{
			if (m_pSpot[nCnt]->GetPosHeight() == pPos[0])
			{
				pos = m_pSpot[nCnt]->GetPosition();
			}
		}
	}

	delete[] pPos;

	return pos;
}

//==========================================
//  二番目に低い地点の取得
//==========================================
D3DXVECTOR3 CMap::GetHeightNext(void)
{
	//返り値のための変数
	D3DXVECTOR3 pos;

	//データを保管
	float *pPos = new float[NUM];
	for (int nCnt = 0; nCnt < NUM; nCnt++)
	{
		if (m_pSpot[nCnt] != nullptr)
		{
			pPos[nCnt] = m_pSpot[nCnt]->GetPosHeight();
		}
		else
		{
			pPos[nCnt] = -10000000.0f;
		}
	}

	//ソート処理
	useful::SortNum(pPos, NUM);

	for (int nCnt = 0; nCnt < NUM; nCnt++)
	{
		if (m_pSpot[nCnt] != nullptr)
		{
			if (m_pSpot[nCnt]->GetPosHeight() == pPos[1])
			{
				pos = m_pSpot[nCnt]->GetPosition();
			}
		}
	}

	delete[] pPos;

	return pos;
}

//==========================================
//  一番雑魚を殺す
//==========================================
void CMap::DeleteMin(void)
{
	//データを保管
	float* pPos = new float[NUM];
	for (int nCnt = 0; nCnt < NUM; nCnt++)
	{
		if (m_pSpot[nCnt] != nullptr)
		{
			pPos[nCnt] = m_pSpot[nCnt]->GetPosHeight();
		}
		else
		{
			pPos[nCnt] = -10000000.0f;
		}
	}

	//ソート処理
	useful::SortNum(pPos, NUM);

	for (int nCnt = 0; nCnt < NUM; nCnt++)
	{
		if (m_pSpot[nCnt] != nullptr)
		{
			if (m_pSpot[nCnt]->GetPosHeight() == pPos[0])
			{
				m_pSpot[nCnt]->Uninit();
				m_pSpot[nCnt] = nullptr;
				delete[] pPos;

				//座標を生成
				SetSpot();

				return;
			}
		}
	}
}

//==========================================
//  座標生成処理
//==========================================
void CMap::SetSpot(void)
{
	for (int nCntSpot = 0; nCntSpot < NUM; nCntSpot++)
	{
		if (m_pSpot[nCntSpot] == nullptr)
		{
			//基準座標Yの取得
			float fHeight = (float)SCREEN_HEIGHT;

			//データを保管
			float* pPos = new float[NUM];
			for (int nCnt = 0; nCnt < NUM; nCnt++)
			{
				if (m_pSpot[nCnt] != nullptr)
				{
					pPos[nCnt] = m_pSpot[nCnt]->GetPosHeight();
				}
				else
				{
					pPos[nCnt] = (float)SCREEN_HEIGHT;
				}
			}

			//ソート処理
			useful::SortNum(pPos, NUM);

			fHeight = pPos[NUM - 1];
			delete[] pPos;

			//基準座標Yに乱数を加算
			if (fHeight >= SCREEN_HEIGHT)
			{
				fHeight = SCREEN_HEIGHT - 300.0f;
			}
			else
			{
				fHeight -= (float)((rand() % (m_Diff.nMaxY + m_Diff.nMinY)) + m_Diff.nMinY);
			}

			//座標Xの乱数を生成
			float fWidth = (float)((rand() % (m_Diff.nMaxX + m_Diff.nMinX)) + m_Diff.nMinX);

			//座標を生成
			m_pSpot[nCntSpot] = CSpot::Create(D3DXVECTOR3(fWidth, fHeight, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f));
		}
	}
}
