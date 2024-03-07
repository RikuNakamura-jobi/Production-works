//=====================================
//
// 
// Author:������
//
//=====================================
#include "deliveryarrow.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "collision.h"
#include "combo.h"
#include "objectX.h"
#include "debugproc.h"
#include "player.h"
#include "score.h"
#include "map.h"
#include "deliverypoint.h"
#include "useful.h"

//�}�N����`---------------------------

//�񋓌^��`---------------------------

//�N���X��`---------------------------

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

//�ÓI�����o�ϐ��錾-------------------
CObjectX::MODELX CDeliveryarrow::m_model = {};

//=====================================
// �R���X�g���N�^�E�f�X�g���N�^
//=====================================
CDeliveryarrow::CDeliveryarrow(int nPriority = 3) : CObjectX(nPriority)
{
	m_nCntDelivery = 0;
	m_fHue = 0.0f;
	m_bPoint = false;
}

CDeliveryarrow::~CDeliveryarrow()
{
}

//=====================================
// ��������
//=====================================
CDeliveryarrow *CDeliveryarrow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	CDeliveryarrow *pObjectBlock;

	//2D�I�u�W�F�N�g�̐���
	pObjectBlock = new CDeliveryarrow();

	if (pObjectBlock != NULL)
	{
		pObjectBlock->Set(pos, rot, fWidth, fHeight);

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
HRESULT CDeliveryarrow::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATERIAL *pMat;

	//x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/deliveryarrow000.x",
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
void CDeliveryarrow::Unload(void)
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
HRESULT CDeliveryarrow::Init(void)
{
	CObjectX::Init();

	SetType(TYPE_DELIVERYPOINT);

	/*SetCollider(CCollider::Create(GetPosPointa(), GetRotPointa(), D3DXVECTOR3(100.0f, 5.0f, 100.0f), D3DXVECTOR3(-100.0f, -5.0f, -100.0f)));
	GetCollider()->SetType(CCollider::TYPE_BOX);*/

	return S_OK;
}

//=====================================
// �|���S���̏I������
//=====================================
void CDeliveryarrow::Uninit(void)
{	
	CObjectX::Uninit();
}

//=====================================
// �|���S���̍X�V����
//=====================================
void CDeliveryarrow::Update(void)
{
	CInput *input = CManager::Get()->GetInputKeyboard();
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	float fHeight = GetHeight();
	float fWidth = GetWidth();
	D3DXVECTOR3 movePos;

	D3DXVECTOR3 Deliverypoint = CManager::Get()->GetScene()->GetDeliverypoint()->GetPos();
	D3DXVECTOR3 Playerpoint = CManager::Get()->GetScene()->GetPlayer()->GetPos();
	D3DXVECTOR3 posDef;
	float scale = 1.0f;

	posDef = Playerpoint;
	float lengthDP = D3DXVec3Length(&(Deliverypoint - posDef));

	if (lengthDP < 6000.0f && !m_bPoint)
	{
		pos += (Deliverypoint - pos) * 0.05f;
		pos.y = Deliverypoint.y + lengthDP * 0.1f;

		if (pos.y < Deliverypoint.y + 100.0f)
		{
			pos.y = Deliverypoint.y + 100.0f;
		}
		
		posDef = pos;
		float lengthDA = D3DXVec3Length(&(Deliverypoint - posDef));
		CManager::Get()->Get()->GetDebugProc()->Print("���Ɣz�B��̋���: %f\n", lengthDA);

		rot.x += (1.57f - rot.x) * 0.08f;
		if (lengthDA > 600.0f)
		{
			rot.y += 0.8f;
		}
		else
		{
			rot.y += 0.05f;
		}
		rot.z += (0.0f - rot.z) * 0.08f;

		scale = lengthDP * 0.003f;

		if (scale < 1.0f)
		{
			scale = 1.0f;
		}
	}
	else
	{
		pos += (Playerpoint - pos) * 0.85f;
		pos.y = Playerpoint.y + 70.0f;

		// �t���C���̍����ɍ��킹�č��̐�[��������
		float cosRot1, cosRot2, acosRot, rotNow;

		cosRot1 = Deliverypoint.y - pos.y;
		rotNow = 0.0f;

		posDef = pos;
		float lengthDA = D3DXVec3Length(&(Deliverypoint - posDef));
		CManager::Get()->Get()->GetDebugProc()->Print("���Ɣz�B��̋���: %f\n", lengthDA);
		if (cosRot1 < lengthDA)
		{
			cosRot2 = cosRot1 / lengthDA;
			acosRot = acosf(cosRot2);

			rotNow = -(D3DX_PI * 0.5f) + acosRot;
		}

		rot.x += (rotNow - rot.x) * 0.08f;
		rot.y += (atan2f(Deliverypoint.x - pos.x, Deliverypoint.z - pos.z) - rot.y) * 0.3f;
		rot.z += (1.57f - rot.z) * 0.2f;
	}
	
	m_fHue += 2.0f;

	SetPos(pos);
	SetRot(rot);
	SetMove(move);
	SetHeight(fHeight);
	SetWidth(fWidth);
	SetMtxScale(scale);

	CManager::Get()->Get()->GetDebugProc()->Print("�v���C���[�Ɣz�B��̋���: %f\n", lengthDP);

	Collision();
	//CManager::Get()->GetDebugProc()->Print("�G�l�~�[��pos: %f, %f, %f\n", pos.x, pos.y, pos.z);

	CObjectX::Update();
}

//=====================================
// �|���S���̕`�揈��
//=====================================
void CDeliveryarrow::Draw(void)
{
	D3DXMATERIAL *pMat;
	//�}�e���A���̃f�[�^�̃|�C���g���擾
	pMat = (D3DXMATERIAL*)GetModel()->pBuffMatModel->GetBufferPointer();

	int combo = CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo();

	if (combo > 5)
	{
		pMat[0].MatD3D.Emissive = useful::HSLtoRGB(m_fHue);
	}
	else
	{
		pMat[0].MatD3D.Emissive = useful::HSLtoRGB(40.0f);
	}

	CObjectX::Draw();
}

bool CDeliveryarrow::Collision(void)
{
	m_bPoint = false;
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
			D3DXVECTOR3 Deliverypoint = CManager::Get()->GetScene()->GetDeliverypoint()->GetPos();
			D3DXVECTOR3 DeliverypointDef = CManager::Get()->GetScene()->GetDeliverypoint()->GetPos();
			D3DXVECTOR3 Playerpoint = CManager::Get()->GetScene()->GetPlayer()->GetPos();

			if (pCollider->CollisionSquare(&Deliverypoint, Playerpoint, &D3DXVECTOR3(0.0f,0.0f,0.0f)) == true)
			{
				m_bPoint = true;
			}
		}

		pCollider = pColliderNext;
	}

	return true;
}
