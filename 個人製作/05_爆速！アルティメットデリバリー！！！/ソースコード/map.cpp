//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "map.h"
#include "renderer.h"
#include "manager.h"
#include "car.h"
#include "drone.h"
#include "useful.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CMap::CMap()
{
	for (int nCnt1 = 0; nCnt1 < NUM_AREA_X; nCnt1++)
	{
		for (int nCnt2 = 0; nCnt2 < NUM_AREA_Y; nCnt2++)
		{
			m_pArea[nCnt1][nCnt2] = nullptr;
			m_bRoad[nCnt1][nCnt2] = true;
		}
	}
}

CMap::~CMap()
{
}

//=====================================
// 生成処理
//=====================================
CMap *CMap::Create()
{
	CMap *pObjectBg;

	//2Dオブジェクトの生成
	pObjectBg = new CMap();

	if (pObjectBg != NULL)
	{
		//初期化
		if (FAILED(pObjectBg->Init()))
		{
			return nullptr;
		}
	}

	return pObjectBg;
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CMap::Init(void)
{
	int areaNum = 0;
	int setArea = 0;

	//敵の初期化処理
	srand((unsigned int)time(0));

	areaNum = NUM_BIGBUILDING;
	setArea = 0;

	while (setArea < areaNum && setArea < NUM_AREA_X * NUM_AREA_Y)
	{
		int areaX = rand() % 10;
		int areaY = rand() % 10;

		if (m_pArea[areaX][areaY] == nullptr && areaX + areaY != 0)
		{
			m_pArea[areaX][areaY] = new CAreaBigBuilding;
			m_bRoad[areaX][areaY] = false;

			m_pArea[areaX][areaY]->GetBlock()->SetPos(D3DXVECTOR3(8000.0f * areaX, 0.0f, 8000.0f * areaY));
			m_pArea[areaX][areaY]->GetRand()->SetPos(D3DXVECTOR3(8000.0f * areaX, 0.0f, 8000.0f * areaY));

			setArea++;
		}
	}

	areaNum = (rand() % (MAX_SMALLBUILDING - MIN_SMALLBUILDING)) + MIN_SMALLBUILDING;
	setArea = 0;

	while (setArea < areaNum && setArea < NUM_AREA_X * NUM_AREA_Y)
	{
		int areaX = rand() % 10;
		int areaY = rand() % 10;

		if (m_pArea[areaX][areaY] == nullptr && areaX + areaY != 0)
		{
			m_pArea[areaX][areaY] = new CAreaSmallBuilding;
			m_bRoad[areaX][areaY] = false;

			m_pArea[areaX][areaY]->GetBlock()->SetPos(D3DXVECTOR3(8000.0f * areaX, 0.0f, 8000.0f * areaY));
			m_pArea[areaX][areaY]->GetRand()->SetPos(D3DXVECTOR3(8000.0f * areaX, 0.0f, 8000.0f * areaY));

			setArea++;
		}
	}

	areaNum = (rand() % (MAX_SMALLAPART - MIN_SMALLAPART)) + MIN_SMALLAPART;
	setArea = 0;

	while (setArea < areaNum && setArea < NUM_AREA_X * NUM_AREA_Y)
	{
		int areaX = rand() % 10;
		int areaY = rand() % 10;

		if (m_pArea[areaX][areaY] == nullptr && areaX + areaY != 0)
		{
			m_pArea[areaX][areaY] = new CAreaApart;
			m_bRoad[areaX][areaY] = false;

			m_pArea[areaX][areaY]->GetBlock()->SetPos(D3DXVECTOR3(8000.0f * areaX, 0.0f, 8000.0f * areaY));
			m_pArea[areaX][areaY]->GetRand()->SetPos(D3DXVECTOR3(8000.0f * areaX, 0.0f, 8000.0f * areaY));

			setArea++;
		}
	}

	for (int nCnt1 = 0; nCnt1 < NUM_AREA_X; nCnt1++)
	{
		for (int nCnt2 = 0; nCnt2 < NUM_AREA_Y; nCnt2++)
		{
			if (m_pArea[nCnt1][nCnt2] == nullptr)
			{
				bool bUP = false, bDown = false, bAreaL = false, bAreaR = false;
				int nCntNumRoad = 0;

				if (nCnt1 > 0)
				{
					bAreaL = m_bRoad[nCnt1 - 1][nCnt2];
					if (bAreaL)
					{
						nCntNumRoad++;
					}
				}

				if (nCnt2 > 0)
				{
					bUP = m_bRoad[nCnt1][nCnt2 - 1];
					if (bUP)
					{
						nCntNumRoad++;
					}
				}

				if (nCnt1 < 9)
				{
					bAreaR = m_bRoad[nCnt1 + 1][nCnt2];
					if (bAreaR)
					{
						nCntNumRoad++;
					}
				}

				if (nCnt2 < 9)
				{
					bDown = m_bRoad[nCnt1][nCnt2 + 1];
					if (bDown)
					{
						nCntNumRoad++;
					}
				}

				m_pArea[nCnt1][nCnt2] = CAreaRoad::Create(bUP, bDown, bAreaL, bAreaR, nCntNumRoad);

				//m_pArea[nCnt1][nCnt2]->GetBlock()->SetPos(D3DXVECTOR3(4000.0f * nCnt1, 4000.0f * nCnt2, 0.0f));
				m_pArea[nCnt1][nCnt2]->GetRand()->SetPos(D3DXVECTOR3(8000.0f * nCnt1, 0.0f, 8000.0f * nCnt2));

				int nRand = rand() % 3;
				if (nRand == 0)
				{
					D3DXVECTOR3 posCar = useful::PosRelativeMtx(m_pArea[nCnt1][nCnt2]->GetRand()->GetPos(), m_pArea[nCnt1][nCnt2]->GetRand()->GetRot(), D3DXVECTOR3(1000.0f, 5000.0f, 0.0f));

					CDrone::Create(posCar, m_pArea[nCnt1][nCnt2]->GetRand()->GetRot(), 0.0f, 0.0f, CDrone::TYPE_NONE);
				}
				else if (nRand == 1)
				{
					D3DXVECTOR3 posCar = useful::PosRelativeMtx(m_pArea[nCnt1][nCnt2]->GetRand()->GetPos(), m_pArea[nCnt1][nCnt2]->GetRand()->GetRot(), D3DXVECTOR3(1000.0f, 0.0f, 0.0f));

					CCar::Create(posCar, m_pArea[nCnt1][nCnt2]->GetRand()->GetRot(), 0.0f, 0.0f, CCar::TYPE_NONE);
				}
			}
		}
	}

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CMap::Uninit(void)
{
	for (int nCnt1 = 0; nCnt1 < NUM_AREA_X; nCnt1++)
	{
		for (int nCnt2 = 0; nCnt2 < NUM_AREA_Y; nCnt2++)
		{
			if (m_pArea[nCnt1][nCnt2] != nullptr)
			{
				delete m_pArea[nCnt1][nCnt2];
				m_pArea[nCnt1][nCnt2] = nullptr;
			}
		}
	}
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CMap::Update(void)
{
	for (int nCnt1 = 0; nCnt1 < NUM_AREA_X; nCnt1++)
	{
		for (int nCnt2 = 0; nCnt2 < NUM_AREA_Y; nCnt2++)
		{
			if (m_pArea[nCnt1][nCnt2] != nullptr)
			{
				m_pArea[nCnt1][nCnt2]->GetRand()->Update();
			}
		}
	}
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CMap::Draw(void)
{
	for (int nCnt1 = 0; nCnt1 < NUM_AREA_X; nCnt1++)
	{
		for (int nCnt2 = 0; nCnt2 < NUM_AREA_Y; nCnt2++)
		{
			if (m_pArea[nCnt1][nCnt2] != nullptr)
			{
				m_pArea[nCnt1][nCnt2]->GetRand()->Draw();
			}
		}
	}
}

//=====================================
// マップのエリアの角取得処理
//=====================================
D3DXVECTOR3 CMap::GetPosAreaCorner()
{
	int areaX = rand() % 8 + 1;
	int areaY = rand() % 8 + 1;
	D3DXVECTOR3 areaCornerPos;
	D3DXVECTOR3 areaPos;
	areaPos = m_pArea[areaX][areaY]->GetRand()->GetPos();

	int areaXPos = rand() % 3 - 1;
	int areaYPos = rand() % 3 - 1;
	areaXPos = 0;
	areaYPos = 0;
	areaCornerPos.x = areaPos.x + (4000.0f * (float)areaXPos);
	areaCornerPos.z = areaPos.z + (4000.0f * (float)areaYPos);

	if (areaXPos == 0 && areaYPos == 0)
	{
		areaCornerPos.y = m_pArea[areaX][areaY]->GetHight();
	}
	else
	{
		areaCornerPos.y = 0.0f;
	}

	return areaCornerPos;
}

//=====================================
// 道路の生成処理
//=====================================
CAreaRoad *CAreaRoad::Create(bool bRoadUP, bool bRoadDown, bool bRoadL, bool bRoadR, int nNumRoad)
{
	CAreaRoad *pAreaRoad = nullptr;

	switch (nNumRoad)
	{
	case 0:

		pAreaRoad = new CAreaRoadO;
		pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		break;
	case 1:

		pAreaRoad = new CAreaRoadU;

		if (bRoadUP)
		{
			pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		else if (bRoadDown)
		{
			pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, 3.14f, 0.0f));
		}
		else if (bRoadL)
		{
			pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, 1.57f, 0.0f));
		}
		else if (bRoadR)
		{
			pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, -1.57f, 0.0f));
		}
		else
		{
			pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}

		break;
	case 2:

		if (bRoadUP && bRoadDown)
		{
			pAreaRoad = new CAreaRoadS;

			pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		else if (bRoadL && bRoadR)
		{
			pAreaRoad = new CAreaRoadS;

			pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, 1.57f, 0.0f));
		}
		else
		{
			pAreaRoad = new CAreaRoadL;

			if (bRoadUP)
			{
				if (bRoadL)
				{
					pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, -1.57f, 0.0f));
				}

				if (bRoadR)
				{
					pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, 3.14f, 0.0f));
				}
			}

			if (bRoadDown)
			{
				if (bRoadL)
				{
					pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}

				if (bRoadR)
				{
					pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, 1.57f, 0.0f));
				}
			}
		}

		break;
	case 3:

		pAreaRoad = new CAreaRoadT;

		if (!bRoadUP)
		{
			pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		else if (!bRoadDown)
		{
			pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, 3.14f, 0.0f));
		}
		else if (!bRoadL)
		{
			pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, 1.57f, 0.0f));
		}
		else if (!bRoadR)
		{
			pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, -1.57f, 0.0f));
		}
		else
		{
			pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}

		break;
	case 4:

		pAreaRoad = new CAreaRoadCross;
		pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		break;
	default:

		pAreaRoad = new CAreaRoadO;
		pAreaRoad->m_pRand->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		break;
	}

	return pAreaRoad;
}