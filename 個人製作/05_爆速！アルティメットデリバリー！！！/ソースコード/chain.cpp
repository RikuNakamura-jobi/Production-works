//=====================================
//
// 
// Author:������
//
//=====================================
#include "chain.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "collision.h"
#include "objectX.h"
#include "camera.h"
#include "player.h"
#include "model.h"
#include "debugproc.h"
#include "orbit.h"
#include "effect.h"

//�}�N����`---------------------------

//�񋓌^��`---------------------------

//�N���X��`---------------------------

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

//�ÓI�����o�ϐ��錾-------------------
CObjectX::MODELX CHook::m_model = {};

//=====================================
//
//hook
//
//=====================================
//=====================================
// �R���X�g���N�^�E�f�X�g���N�^
//=====================================
CHook::CHook(int nPriority = 3) : CObjectX(nPriority)
{
	m_bShot = false;
	m_bGet = false;
	m_bCollision = false;
}

CHook::~CHook()
{
}

//=====================================
// ��������
//=====================================
CHook *CHook::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CHook *pObjectBlock;

	//2D�I�u�W�F�N�g�̐���
	pObjectBlock = new CHook();

	if (pObjectBlock != NULL)
	{
		pObjectBlock->Set(pos, rot, 0.0f, 0.0f);

		pObjectBlock->SetModel(m_model);
		pObjectBlock->BindModel(pObjectBlock->GetModel());

		//������
		if (FAILED(pObjectBlock->Init()))
		{
			pObjectBlock->Release();
		}
	}

	return pObjectBlock;
}

//=====================================
// �|���S���̃e�N�X�`������
//=====================================
HRESULT CHook::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATERIAL *pMat;

		//x�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX("data/MODEL/hook000.x",
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

		//���_���擾
		nNumVtx = m_model.pMeshModel->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		dwSizeFVF = D3DXGetFVFVertexSize(m_model.pMeshModel->GetFVF());

		//���_�o�b�t�@�����b�N
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

		//���_�o�b�t�@���A�����b�N
		m_model.pMeshModel->UnlockVertexBuffer();

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)m_model.pBuffMatModel->GetBufferPointer();

		m_model.pTexture = new LPDIRECT3DTEXTURE9[m_model.dwNumMatModel];

		for (int nCntMat = 0; nCntMat < (int)m_model.dwNumMatModel; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//�e�N�X�`���̓ǂݍ���
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
void CHook::Unload(void)
{
	//�e�N�X�`���̔j��
	if (m_model.pMeshModel != NULL)
	{
		m_model.pMeshModel->Release();
		m_model.pMeshModel = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (m_model.pBuffMatModel != NULL)
	{
		m_model.pBuffMatModel->Release();
		m_model.pBuffMatModel = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (m_model.pTexture != NULL)
	{
		delete[] m_model.pTexture;
		m_model.pTexture = NULL;
	}
}

//=====================================
// �|���S���̏���������
//=====================================
HRESULT CHook::Init(void)
{
	CObjectX::Init();

	SetType(TYPE_INVISIBLE);

	SetCollider(CCollider::Create(GetPosPointa(), GetRotPointa(), D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f), D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f), CCollider::TAG_NONE));
	GetCollider()->SetType(CCollider::TYPE_BOX);

	m_orbit[0] = COrbit::Create(GetMtx(), D3DXVECTOR3(2.5f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 2);
	m_orbit[1] = COrbit::Create(GetMtx(), D3DXVECTOR3(-2.5f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 2);
	m_orbit[2] = COrbit::Create(GetMtx(), D3DXVECTOR3(2.5f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 2);
	m_orbit[3] = COrbit::Create(GetMtx(), D3DXVECTOR3(-2.5f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -2.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 2);

	return S_OK;
}

//=====================================
// �|���S���̏I������
//=====================================
void CHook::Uninit(void)
{	
	CObjectX::Uninit();
}

//=====================================
// �|���S���̍X�V����
//=====================================
void CHook::Update(void)
{
	CObjectX::Update();
}

//=====================================
// �|���S���̕`�揈��
//=====================================
void CHook::Draw(void)
{
	CPlayer *pPlayer = CManager::Get()->GetScene()->GetPlayer();
	D3DXVECTOR3 pos = GetPos();
	m_posOld = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();

	rot = pPlayer->GetRot();

	D3DXVECTOR3 posParts;
	posParts.x = pPlayer->GetModel(16)->GetMtxWorld()._41;
	posParts.y = pPlayer->GetModel(16)->GetMtxWorld()._42;
	posParts.z = pPlayer->GetModel(16)->GetMtxWorld()._43;
	float length = D3DXVec3Length(&(posParts - pos));
	CManager::Get()->Get()->GetDebugProc()->Print("���Ǝ�̋���: %f\n", length);
	CManager::Get()->Get()->GetDebugProc()->Print("���̈ʒu: %f %f %f\n", pos.x, pos.y, pos.z);
	CManager::Get()->Get()->GetDebugProc()->Print("���̈ړ�����: %f %f %f\n", move.x, move.y, move.z);

	if (m_bShot)
	{
		if (m_bCollision)
		{
			if (length < 300.0f)
			{
				m_bGet = true;
				m_bShot = false;
				m_bCollision = false;
			}
		}
		else
		{
			//�ړ�����
			pos += move;

			if (length > 20000.0f)
			{
				m_bShot = false;
			}
		}

		if (Collision(&pos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f)))
		{
			move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}
	else
	{
		D3DXVECTOR3 vecMove = posParts - pos;
		D3DXVec3Normalize(&vecMove, &vecMove);
		if (length < 700.0f)
		{
			pos.x = pPlayer->GetModel(16)->GetMtxWorld()._41;
			pos.y = pPlayer->GetModel(16)->GetMtxWorld()._42;
			pos.z = pPlayer->GetModel(16)->GetMtxWorld()._43;

			m_bGet = true;
			m_bShot = false;
			m_bCollision = false;
			pos += vecMove * 0.0f;
		}
		else
		{
			pos += vecMove * 700.0f;
		}
	}

	D3DXVECTOR3 vecReticle = CManager::Get()->GetScene()->GetCamera()->GetPosR() - CManager::Get()->GetScene()->GetCamera()->GetPosV();
	D3DXVECTOR3 posToMove;
	D3DXVec3Normalize(&vecReticle, &vecReticle);

	posToMove = pos + (vecReticle * 20000.0f);

	if (CollisionReticle(&posToMove, &vecReticle))
	{
		//CEffect3D::Create(posToMove, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 1, 300.0f, 300.0f);
	}
	else
	{
		//CEffect3D::Create(posToMove, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 1, 200.0f, 200.0f);
	}

	SetPos(pos);
	SetRot(rot);
	SetMove(move);

	for (int nCntOrbit = 0; nCntOrbit < 4; nCntOrbit++)
	{
		if (m_orbit[nCntOrbit] != nullptr)
		{
			D3DXMATRIX mtxTemp = pPlayer->GetModel(16)->GetMtxWorld();
			m_orbit[nCntOrbit]->SetPositionOffset(mtxTemp);
		}
	}

	CObjectX::Draw();

	for (int nCntOrbit = 0; nCntOrbit < 4; nCntOrbit++)
	{
		if (m_orbit[nCntOrbit] != nullptr)
		{
			m_orbit[nCntOrbit]->SetPositionOffset(GetMtx(), D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.9f));
		}
	}
}

//=====================================
// �|���S���̕`�揈��
//=====================================
void CHook::ShotHook(D3DXVECTOR3 rot, float speed)
{
	if (!m_bShot)
	{
		CPlayer *pPlayer = CManager::Get()->GetScene()->GetPlayer(); 
		D3DXVECTOR3 vecMove = CManager::Get()->GetScene()->GetCamera()->GetPosR() - CManager::Get()->GetScene()->GetCamera()->GetPosV();
		D3DXVec3Normalize(&vecMove, &vecMove);

		D3DXVECTOR3 posParts;
		posParts.x = pPlayer->GetModel(16)->GetMtxWorld()._41;
		posParts.y = pPlayer->GetModel(16)->GetMtxWorld()._42;
		posParts.z = pPlayer->GetModel(16)->GetMtxWorld()._43;

		m_bGet = false;
		m_bShot = true;
		m_bCollision = false;
		SetPos(posParts);
		SetRot(rot);
		SetMove(vecMove * speed);
	}
}

//=====================================
// �����蔻��
//=====================================
bool CHook::Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *move)
{
	CCollider *pCollider;

	pCollider = CCollision::Get()->GetTop();

	while (pCollider != NULL)
	{
		CCollider *pColliderNext = pCollider->GetNext();

		CCollider::TAG tag;

		//��ގ擾
		tag = pCollider->GetTag();

		if (tag == CCollider::TAG_BOX)
		{
			D3DXVECTOR3 posParts;
			posParts.x = CManager::Get()->GetScene()->GetPlayer()->GetModel(16)->GetMtxWorld()._41;
			posParts.y = CManager::Get()->GetScene()->GetPlayer()->GetModel(16)->GetMtxWorld()._42;
			posParts.z = CManager::Get()->GetScene()->GetPlayer()->GetModel(16)->GetMtxWorld()._43;

			if (pCollider->CollisionSquare(pos, posParts, move, true) == true)
			{
				m_bCollision = true;
				return true;
			}

			if (pos->y <= 0.0f)
			{
				pos->y = 0.0f;
				m_bShot = false;
				return true;
			}
		}

		pCollider = pColliderNext;
	}

	return false;
}

bool  CHook::CollisionReticle(D3DXVECTOR3 *pos, D3DXVECTOR3 *move)
{
	CCollider *pCollider;

	pCollider = CCollision::Get()->GetTop();

	while (pCollider != NULL)
	{
		CCollider *pColliderNext = pCollider->GetNext();

		CCollider::TAG tag;

		//��ގ擾
		tag = pCollider->GetTag();

		if (tag == CCollider::TAG_BOX)
		{
			D3DXVECTOR3 posParts;
			posParts.x = CManager::Get()->GetScene()->GetPlayer()->GetModel(16)->GetMtxWorld()._41;
			posParts.y = CManager::Get()->GetScene()->GetPlayer()->GetModel(16)->GetMtxWorld()._42;
			posParts.z = CManager::Get()->GetScene()->GetPlayer()->GetModel(16)->GetMtxWorld()._43;

			if (pCollider->CollisionSquare(pos, posParts, move, true) == true)
			{
				return true;
			}
		}

		pCollider = pColliderNext;
	}

	return false;
}