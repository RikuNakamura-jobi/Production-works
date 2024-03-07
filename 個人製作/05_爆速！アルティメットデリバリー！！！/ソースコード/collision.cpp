//=============================================================================
//
// 
// Author :������
//
//=============================================================================
#include "collision.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "useful.h"

//�}�N����`---------------------------

//�񋓌^��`---------------------------

//�N���X��`---------------------------

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

//�ÓI�����o�ϐ��錾-------------------
CCollision *CCollision::m_pCollision = NULL;

//=====================================
// �R���X�g���N�^�E�f�X�g���N�^
//=====================================
CCollision::CCollision()
{
	m_pTop = NULL;
	m_pCur = NULL;
	m_nNumAll = 0;
}

CCollision::~CCollision()
{
}

CCollision *CCollision::Get(void)
{
	if (m_pCollision == NULL)
	{
		return m_pCollision = new CCollision;
	}
	else
	{
		return m_pCollision;
	}
}

HRESULT CCollision::Release(void)
{
	if (m_pCollision != NULL)
	{
		m_pCollision->Uninit();

		delete m_pCollision;
		m_pCollision = NULL;
	}

	return S_OK;
}

//=====================================
// �|���S���̏���������
//=====================================
HRESULT CCollision::Init(void)
{
	return S_OK;
}

//=====================================
// �|���S���̏I������
//=====================================
void CCollision::Uninit(void)
{
	CCollider *pCollider = m_pTop;

	while (pCollider != NULL)
	{
		CCollider *pColliderNext = pCollider->GetNext();
		pCollider->Uninit();
		pCollider = pColliderNext;
	}

	Update();
}

//=====================================
// �|���S���̍X�V����
//=====================================
void CCollision::Update(void)
{
	CCollider *pCollider;
	int nEffect = 0;

	pCollider = m_pTop;

	while (pCollider != NULL)
	{
		CCollider *pColliderNext = pCollider->GetNext();

		if (pCollider->GetDeath() != true)
		{
			pCollider->Update();
		}

		pCollider = pColliderNext;
	}

	pCollider = m_pTop;

	while (pCollider != NULL)
	{
		CCollider *pColliderNext = pCollider->GetNext();

		if (pCollider->GetDeath() == true)
		{
			if (pCollider->GetPrev() == NULL)
			{
				m_pTop = pCollider->GetNext();
			}
			else
			{
				pCollider->GetPrev()->SetNext(pCollider->GetNext());
			}

			if (pCollider->GetNext() == NULL)
			{
				m_pCur = pCollider->GetPrev();
			}
			else
			{
				pCollider->GetNext()->SetPrev(pCollider->GetPrev());
			}

			delete pCollider;
		}

		pCollider = pColliderNext;
	}
}


//=====================================
// �R���X�g���N�^�E�f�X�g���N�^
//=====================================
CCollider::CCollider()
{
	m_pos = NULL;
	m_rot = NULL;
	m_offsetMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_offsetMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_nHue = 0.0f;
}

CCollider::~CCollider()
{
}

CCollider *CCollider::Create(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot, D3DXVECTOR3 offsetMax, D3DXVECTOR3 offsetMin, TAG tag)
{
	CCollider *pCllider = new CCollider;

	if (pCllider != NULL)
	{
		pCllider->Init();

		pCllider->m_pos = pPos;
		pCllider->m_rot = pRot;
		pCllider->m_offsetMax = offsetMax;
		pCllider->m_offsetMin = offsetMin;
		pCllider->m_tag = tag;
	}

	return pCllider;
}

//=====================================
// �|���S���̏���������
//=====================================
HRESULT CCollider::Init(void)
{
	if (CCollision::Get()->GetTop() == NULL)
	{
		CCollision::Get()->SetTop(this);
	}

	if (CCollision::Get()->GetCur() == NULL)
	{
		CCollision::Get()->SetCur(this);
		m_pPrev = NULL;
	}
	else
	{
		m_pPrev = CCollision::Get()->GetCur();
		CCollision::Get()->GetCur()->m_pNext = this;
		CCollision::Get()->SetCur(this);
	}

	m_pNext = NULL;
	m_nID = CCollision::Get()->GetNumAll();
	m_bDeath = false;
	CCollision::Get()->SetNumAll(CCollision::Get()->GetNumAll() + 1);

	return S_OK;
}

//=====================================
// �|���S���̏I������
//=====================================
void CCollider::Uninit(void)
{
	m_bDeath = true;
}

//=====================================
// �|���S���̍X�V����
//=====================================
void CCollider::Update(void)
{
#ifdef _DEBUG
	/*CEffect3D::Create(*m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), *m_rot, useful::HSLtoRGB(m_nHue), 1, 20.0f, 20.0f);
	CEffect3D::Create(useful::PosRelativeMtx(*m_pos, *m_rot, m_offsetMax), D3DXVECTOR3(0.0f, 0.0f, 0.0f), *m_rot, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 1, 20.0f, 20.0f);
	CEffect3D::Create(useful::PosRelativeMtx(*m_pos, *m_rot, m_offsetMin), D3DXVECTOR3(0.0f, 0.0f, 0.0f), *m_rot, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 1, 20.0f, 20.0f);*/

	m_nHue += 0.7f;

#endif //_DEBUG
}

//==================================================================================================
//�_�Ɣ��̓����蔻��
//==================================================================================================
bool CCollider::CollisionSquare(D3DXVECTOR3 *posTarget, D3DXVECTOR3 posTargetOld, D3DXVECTOR3 *move, bool bStop, D3DXVECTOR3 *vecNor)
{
	D3DXVECTOR3 posCorner[8] = {};
	D3DXVECTOR3 posPlaneCenter[6] = {};
	D3DXVECTOR3 vecIntersect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vecNorPlaneCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXPLANE plane = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float lengthX, lengthY, lengthZ;

	lengthX = fabsf(m_offsetMax.x - m_offsetMin.x) * 0.5f;
	lengthY = fabsf(m_offsetMax.y - m_offsetMin.y) * 0.5f;
	lengthZ = fabsf(m_offsetMax.z - m_offsetMin.z) * 0.5f;

	//���̊e�ʂ̒��S�����߂�
	posPlaneCenter[0] = useful::PosRelativeMtx(*m_pos, *m_rot, D3DXVECTOR3(m_offsetMax.x, 0.0f, 0.0f));
	posPlaneCenter[1] = useful::PosRelativeMtx(*m_pos, *m_rot, D3DXVECTOR3(m_offsetMin.x, 0.0f, 0.0f));
	posPlaneCenter[2] = useful::PosRelativeMtx(*m_pos, *m_rot, D3DXVECTOR3(0.0f, m_offsetMax.y, 0.0f));
	posPlaneCenter[3] = useful::PosRelativeMtx(*m_pos, *m_rot, D3DXVECTOR3(0.0f, m_offsetMin.y, 0.0f));
	posPlaneCenter[4] = useful::PosRelativeMtx(*m_pos, *m_rot, D3DXVECTOR3(0.0f, 0.0f, m_offsetMax.z));
	posPlaneCenter[5] = useful::PosRelativeMtx(*m_pos, *m_rot, D3DXVECTOR3(0.0f, 0.0f, m_offsetMin.z));

	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		//�e�ʂ̖@���x�N�g�����v�Z����
		vecNorPlaneCenter = *m_pos - posPlaneCenter[nCnt];
		D3DXVec3Normalize(&vecNorPlaneCenter, &vecNorPlaneCenter);

		//�@���x�N�g�����畽�ʂ̎����v�Z����
		D3DXPlaneFromPointNormal(&plane, &posPlaneCenter[nCnt], &vecNorPlaneCenter);

		//���ʂ̎��Ɠ_����
		if (D3DXPlaneDotCoord(&plane, posTarget) >= 0.0f && D3DXPlaneDotCoord(&plane, &posTargetOld) <= 0.0f)
		{
			D3DXPlaneIntersectLine(&vecIntersect, &plane, posTarget, &posTargetOld);

			if (CollisionSquareTrigger(vecIntersect) == true)
			{
				if (vecNor != nullptr)
				{
					*vecNor = vecNorPlaneCenter;
				}

				D3DXVECTOR3 vecMove, vecMoveRef;
				float fDot;

				vecMove = vecIntersect - *posTarget;

				fDot = D3DXVec3Dot(&vecMove, &vecNorPlaneCenter);

				vecMove = *posTarget - vecIntersect;

				vecMoveRef = vecMove + (vecNorPlaneCenter * fDot * 1.0f);

				if (bStop)
				{
					*posTarget = vecIntersect;
					*move = vecMoveRef * 0.0f;
				}
				else
				{
					if (vecNorPlaneCenter.y > -0.8f)
					{
						*move = vecMoveRef * 0.0f;
					}
					*posTarget = vecIntersect + vecMoveRef;
					//*move = vecMoveRef;
				}

				return true;
			}
		}
	}
	return false;
}

//==================================================================================================
//�_�Ɣ��̓����蔻��(���O����)
//==================================================================================================
bool CCollider::CollisionSquareTrigger(D3DXVECTOR3 posTarget)
{
	bool bSquare;
	bSquare = false;

	D3DXVECTOR3 posCorner[8];
	D3DXVECTOR3 posPlaneCenter[6];
	D3DXVECTOR3 vecNorPlaneCenter[6];
	D3DXPLANE plane[6];
	int nCheckCollision = 0;

	//���̊e�ʂ̒��S�����߂�
	posPlaneCenter[0] = useful::PosRelativeMtx(*m_pos, *m_rot, D3DXVECTOR3(m_offsetMax.x, 0.0f, 0.0f));
	posPlaneCenter[1] = useful::PosRelativeMtx(*m_pos, *m_rot, D3DXVECTOR3(m_offsetMin.x, 0.0f, 0.0f));
	posPlaneCenter[2] = useful::PosRelativeMtx(*m_pos, *m_rot, D3DXVECTOR3(0.0f, m_offsetMax.y, 0.0f));
	posPlaneCenter[3] = useful::PosRelativeMtx(*m_pos, *m_rot, D3DXVECTOR3(0.0f, m_offsetMin.y, 0.0f));
	posPlaneCenter[4] = useful::PosRelativeMtx(*m_pos, *m_rot, D3DXVECTOR3(0.0f, 0.0f, m_offsetMax.z));
	posPlaneCenter[5] = useful::PosRelativeMtx(*m_pos, *m_rot, D3DXVECTOR3(0.0f, 0.0f, m_offsetMin.z));

	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		//�e�ʂ̖@���x�N�g�����v�Z����
		vecNorPlaneCenter[nCnt] = *m_pos - posPlaneCenter[nCnt];
		D3DXVec3Normalize(&vecNorPlaneCenter[nCnt], &vecNorPlaneCenter[nCnt]);

		//�@���x�N�g�����畽�ʂ̎����v�Z����
		D3DXPlaneFromPointNormal(&plane[nCnt], &posPlaneCenter[nCnt], &vecNorPlaneCenter[nCnt]);

		//���ʂ̎��Ɠ_����
		if (D3DXPlaneDotCoord(&plane[nCnt], &posTarget) >= 0.0f)
		{
			nCheckCollision++;
		}
	}

	if (nCheckCollision == 6)
	{
		bSquare = true;
	}

	return bSquare;
}