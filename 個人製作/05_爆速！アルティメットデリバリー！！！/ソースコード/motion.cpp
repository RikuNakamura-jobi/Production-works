//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "model.h"
#include "motion.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CMotion::CMotion()
{
	m_nNumInfo = 0;
	m_nType = 0;
	m_bLoop = false;
	m_nNumKey = 0;
	m_nKey = 0;
	m_nNumCounter = 0;
	m_bFinish = false;
	m_nNumModel = 0;
	m_ppModel = NULL;
}

CMotion::~CMotion()
{
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CMotion::Init(void)
{
	m_nNumInfo = 0;
	m_nType = 0;
	m_bLoop = false;
	m_nNumKey = 0;
	m_nKey = 0;
	m_nNumCounter = 0;
	m_bFinish = false;
	m_nNumModel = 0;
	m_ppModel = NULL;

	return S_OK;
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CMotion::Update(void)
{
	D3DXVECTOR3 posDiff, rotDiff;
	m_bFinish = false;

	if (m_nKey < m_aInfo[m_nType].nNumKey)
	{
		if (m_nKey != -1)
		{
			if (m_nNumCounter < m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame)
			{
				if (m_nKey == m_aInfo[m_nType].nNumKey - 1)
				{
					for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
					{
						posDiff = m_aInfo[m_nType].aKeyInfo[0].aKey[nCntModel].pos - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos;
						posDiff = m_ppModel[nCntModel]->GetPosDef() + m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos + posDiff * ((float)m_nNumCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame);
						m_ppModel[nCntModel]->SetPos(posDiff);

						rotDiff = m_aInfo[m_nType].aKeyInfo[0].aKey[nCntModel].rot - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot;

						if (rotDiff.x > 3.14f)
						{
							rotDiff.x -= 6.28f;
						}
						else if (rotDiff.x < -3.14f)
						{
							rotDiff.x += 6.28f;
						}

						if (rotDiff.y > 3.14f)
						{
							rotDiff.y -= 6.28f;
						}
						else if (rotDiff.y < -3.14f)
						{
							rotDiff.y += 6.28f;
						}

						if (rotDiff.z > 3.14f)
						{
							rotDiff.z -= 6.28f;
						}
						else if (rotDiff.z < -3.14f)
						{
							rotDiff.z += 6.28f;
						}

						rotDiff = m_ppModel[nCntModel]->GetRotDef() + m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot + rotDiff * ((float)m_nNumCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame);

						m_ppModel[nCntModel]->SetRot(rotDiff);
					}
				}
				else if (m_nKey == -1)
				{
					for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
					{
						posDiff = m_aInfo[m_nType].aKeyInfo[m_nKey + 1].aKey[nCntModel].pos - aKeyInfoBlend.aKey[nCntModel].pos;
						posDiff = m_ppModel[nCntModel]->GetPosDef() + aKeyInfoBlend.aKey[nCntModel].pos + posDiff * ((float)m_nNumCounter / (float)aKeyInfoBlend.nFrame);
						m_ppModel[nCntModel]->SetPos(posDiff);

						rotDiff = m_aInfo[m_nType].aKeyInfo[m_nKey + 1].aKey[nCntModel].rot - aKeyInfoBlend.aKey[nCntModel].rot;

						if (rotDiff.x > 3.14f)
						{
							rotDiff.x -= 6.28f;
						}
						else if (rotDiff.x < -3.14f)
						{
							rotDiff.x += 6.28f;
						}

						if (rotDiff.y > 3.14f)
						{
							rotDiff.y -= 6.28f;
						}
						else if (rotDiff.y < -3.14f)
						{
							rotDiff.y += 6.28f;
						}

						if (rotDiff.z > 3.14f)
						{
							rotDiff.z -= 6.28f;
						}
						else if (rotDiff.z < -3.14f)
						{
							rotDiff.z += 6.28f;
						}

						rotDiff = m_ppModel[nCntModel]->GetRotDef() + aKeyInfoBlend.aKey[nCntModel].rot + rotDiff * ((float)m_nNumCounter / (float)aKeyInfoBlend.nFrame);
						
						m_ppModel[nCntModel]->SetRot(rotDiff);
					}
				}
				else
				{
					for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
					{
						posDiff = m_aInfo[m_nType].aKeyInfo[m_nKey + 1].aKey[nCntModel].pos - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos;
						posDiff = m_ppModel[nCntModel]->GetPosDef() + m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos + posDiff * ((float)m_nNumCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame);
						m_ppModel[nCntModel]->SetPos(posDiff);

						rotDiff = m_aInfo[m_nType].aKeyInfo[m_nKey + 1].aKey[nCntModel].rot - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot;

						if (rotDiff.x > 3.14f)
						{
							rotDiff.x -= 6.28f;
						}
						else if (rotDiff.x < -3.14f)
						{
							rotDiff.x += 6.28f;
						}

						if (rotDiff.y > 3.14f)
						{
							rotDiff.y -= 6.28f;
						}
						else if (rotDiff.y < -3.14f)
						{
							rotDiff.y += 6.28f;
						}

						if (rotDiff.z > 3.14f)
						{
							rotDiff.z -= 6.28f;
						}
						else if (rotDiff.z < -3.14f)
						{
							rotDiff.z += 6.28f;
						}

						rotDiff = m_ppModel[nCntModel]->GetRotDef() + m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot + rotDiff * ((float)m_nNumCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame);
						
						m_ppModel[nCntModel]->SetRot(rotDiff);
					}
				}

				m_nNumCounter++;
			}
			else
			{
				m_nKey++;
				m_nNumCounter = 0;

				if (m_nKey < m_aInfo[m_nType].nNumKey)
				{
					for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
					{
						m_ppModel[nCntModel]->SetRot(m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot);
					}
				}

				Update();
			}
		}
		else
		{
			if (m_nNumCounter < aKeyInfoBlend.nFrame)
			{
				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{
					posDiff = m_aInfo[m_nType].aKeyInfo[m_nKey + 1].aKey[nCntModel].pos - aKeyInfoBlend.aKey[nCntModel].pos;
					posDiff = m_ppModel[nCntModel]->GetPosDef() + aKeyInfoBlend.aKey[nCntModel].pos + posDiff * ((float)m_nNumCounter / (float)aKeyInfoBlend.nFrame);
					m_ppModel[nCntModel]->SetPos(posDiff);

					rotDiff = m_aInfo[m_nType].aKeyInfo[m_nKey + 1].aKey[nCntModel].rot - aKeyInfoBlend.aKey[nCntModel].rot;
					rotDiff = m_ppModel[nCntModel]->GetRotDef() + aKeyInfoBlend.aKey[nCntModel].rot + rotDiff * ((float)m_nNumCounter / (float)aKeyInfoBlend.nFrame);

					if (rotDiff.x > 3.14f)
					{
						rotDiff.x -= 6.28f;
					}
					else if (rotDiff.x < -3.14f)
					{
						rotDiff.x += 6.28f;
					}

					if (rotDiff.y > 3.14f)
					{
						rotDiff.y -= 6.28f;
					}
					else if (rotDiff.y < -3.14f)
					{
						rotDiff.y += 6.28f;
					}

					if (rotDiff.z > 3.14f)
					{
						rotDiff.z -= 6.28f;
					}
					else if (rotDiff.z < -3.14f)
					{
						rotDiff.z += 6.28f;
					}

					m_ppModel[nCntModel]->SetRot(rotDiff);
				}

				m_nNumCounter++;
			}
			else
			{
				m_nKey++;
				m_nNumCounter = 0;

				if (m_nKey < m_aInfo[m_nType].nNumKey)
				{
					for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
					{
						m_ppModel[nCntModel]->SetRot(m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot);
					}
				}

				Update();
			}
		}
	}
	else
	{
		m_nKey = 0;
		m_nNumCounter = 0;

		if (m_aInfo[m_nType].nLoop == 0)
		{
			m_bFinish = true;
		}
		else
		{
			Update();
		}
	}
}

void CMotion::Set(int nType)
{
	D3DXVECTOR3 posDiff, rotDiff;

	if (m_nType != nType)
	{
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			if (m_nKey != -1)
			{
				posDiff = m_aInfo[m_nType].aKeyInfo[m_nKey + 1].aKey[nCntModel].pos - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos;
				aKeyInfoBlend.aKey[nCntModel].pos = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos + posDiff * ((float)m_nNumCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame);

				rotDiff = m_aInfo[m_nType].aKeyInfo[m_nKey + 1].aKey[nCntModel].rot - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot;
				rotDiff = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot + rotDiff * ((float)m_nNumCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame);
				aKeyInfoBlend.aKey[nCntModel].rot = rotDiff;

				aKeyInfoBlend.nFrame = m_aInfo[nType].aKeyInfo[0].nFrame;
			}
			if (m_nKey == m_aInfo[m_nType].nNumKey - 1)
			{
				posDiff = m_aInfo[m_nType].aKeyInfo[0].aKey[nCntModel].pos - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos;
				aKeyInfoBlend.aKey[nCntModel].pos = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].pos + posDiff * ((float)m_nNumCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame);

				rotDiff = m_aInfo[m_nType].aKeyInfo[0].aKey[nCntModel].rot - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot;
				rotDiff = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].rot + rotDiff * ((float)m_nNumCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame);
				aKeyInfoBlend.aKey[nCntModel].rot = rotDiff;

				aKeyInfoBlend.nFrame = m_aInfo[nType].aKeyInfo[0].nFrame;
			}
			else
			{
				posDiff = m_aInfo[m_nType].aKeyInfo[m_nKey + 1].aKey[nCntModel].pos - aKeyInfoBlend.aKey[nCntModel].pos;
				aKeyInfoBlend.aKey[nCntModel].pos = aKeyInfoBlend.aKey[nCntModel].pos + posDiff * ((float)m_nNumCounter / (float)aKeyInfoBlend.nFrame);

				rotDiff = m_aInfo[m_nType].aKeyInfo[m_nKey + 1].aKey[nCntModel].rot - aKeyInfoBlend.aKey[nCntModel].rot;
				rotDiff = aKeyInfoBlend.aKey[nCntModel].rot + rotDiff * ((float)m_nNumCounter / (float)aKeyInfoBlend.nFrame);
				aKeyInfoBlend.aKey[nCntModel].rot = rotDiff;

				aKeyInfoBlend.nFrame = m_aInfo[nType].aKeyInfo[0].nFrame;
			}

			if (aKeyInfoBlend.aKey[nCntModel].rot.x > 3.14f)
			{
				aKeyInfoBlend.aKey[nCntModel].rot.x -= 6.28f;
			}
			else if (aKeyInfoBlend.aKey[nCntModel].rot.x < -3.14f)
			{
				aKeyInfoBlend.aKey[nCntModel].rot.x += 6.28f;
			}

			if (aKeyInfoBlend.aKey[nCntModel].rot.y >= 3.14f)
			{
				aKeyInfoBlend.aKey[nCntModel].rot.y -= 6.28f;
			}
			else if (aKeyInfoBlend.aKey[nCntModel].rot.y < -3.14f)
			{
				aKeyInfoBlend.aKey[nCntModel].rot.y += 6.28f;
			}

			if (aKeyInfoBlend.aKey[nCntModel].rot.z > 3.14f)
			{
				aKeyInfoBlend.aKey[nCntModel].rot.z -= 6.28f;
			}
			else if (aKeyInfoBlend.aKey[nCntModel].rot.z < -3.14f)
			{
				aKeyInfoBlend.aKey[nCntModel].rot.z += 6.28f;
			}
		}

		m_nNumKey = 0;
		m_nNumCounter = 0;
		m_nKey = -1;
		m_bLoop = false;
		m_bFinish = false;
	}

	m_nType = nType;
}

void CMotion::SetInfo(Info Info)
{
	int nIdx = m_nNumInfo;
	m_aInfo[m_nNumInfo] = Info;
	m_nNumInfo++;
}

void CMotion::SetModel(CModel **ppModel, int nNumModel)
{
	m_ppModel = ppModel;
	m_nNumModel = nNumModel;
}