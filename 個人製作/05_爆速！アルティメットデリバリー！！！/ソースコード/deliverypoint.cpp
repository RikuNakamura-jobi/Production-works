//=====================================
//
// 
// Author:������
//
//=====================================
#include "deliverypoint.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "collision.h"
#include "objectX.h"
#include "debugproc.h"
#include "player.h"
#include "sound.h"
#include "score.h"
#include "combo.h"
#include "map.h"
#include "useful.h"
#include "particle.h"

//�}�N����`---------------------------

//�񋓌^��`---------------------------

//�N���X��`---------------------------

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

//�ÓI�����o�ϐ��錾-------------------
CObjectX::MODELX CDeliverypoint::m_model = {};

//=====================================
// �R���X�g���N�^�E�f�X�g���N�^
//=====================================
CDeliverypoint::CDeliverypoint(int nPriority = 3) : CObjectX(nPriority)
{
	m_nCntDelivery = 0;
	m_fHue = 0.0f;
}

CDeliverypoint::~CDeliverypoint()
{
}

//=====================================
// ��������
//=====================================
CDeliverypoint *CDeliverypoint::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	CDeliverypoint *pObjectBlock;

	//2D�I�u�W�F�N�g�̐���
	pObjectBlock = new CDeliverypoint();

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
HRESULT CDeliverypoint::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATERIAL *pMat;

	//x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/deliverypoint000.x",
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
void CDeliverypoint::Unload(void)
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
HRESULT CDeliverypoint::Init(void)
{
	CObjectX::Init();

	SetType(TYPE_DELIVERYPOINT);

	SetCollider(CCollider::Create(GetPosPointa(), GetRotPointa(), D3DXVECTOR3(100.0f, 5.0f, 100.0f), D3DXVECTOR3(-100.0f, -5.0f, -100.0f), CCollider::TAG_NONE));
	GetCollider()->SetType(CCollider::TYPE_BOX);

	return S_OK;
}

//=====================================
// �|���S���̏I������
//=====================================
void CDeliverypoint::Uninit(void)
{	
	CObjectX::Uninit();
}

//=====================================
// �|���S���̍X�V����
//=====================================
void CDeliverypoint::Update(void)
{
	CInput *input = CManager::Get()->GetInputKeyboard();
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	float fHeight = GetHeight();
	float fWidth = GetWidth();
	D3DXVECTOR3 movePos;

	SetPos(pos);
	SetRot(rot);
	SetMove(move);
	SetHeight(fHeight);
	SetWidth(fWidth);
	SetMtxScale(4.0f);

	Collision();
	//CManager::Get()->GetDebugProc()->Print("�G�l�~�[��pos: %f, %f, %f\n", pos.x, pos.y, pos.z);

	CObjectX::Update();
}

//=====================================
// �|���S���̕`�揈��
//=====================================
void CDeliverypoint::Draw(void)
{
	D3DXMATERIAL *pMat;
	//�}�e���A���̃f�[�^�̃|�C���g���擾
	pMat = (D3DXMATERIAL*)GetModel()->pBuffMatModel->GetBufferPointer();

	int combo = CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo();

	if (combo > 5)
	{
		m_fHue += 0.5f;
		pMat[0].MatD3D.Emissive = useful::HSLtoRGB(m_fHue);
	}
	else
	{
		pMat[0].MatD3D.Emissive = useful::HSLtoRGB(0.0f);
	}

	CObjectX::Draw();
}

bool CDeliverypoint::Collision(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posPlayer = CManager::Get()->Get()->GetScene()->GetPlayer()->GetPos();
	D3DXVECTOR3 vecPlayer;

	vecPlayer.x = pos.x - posPlayer.x;
	vecPlayer.y = pos.y - posPlayer.y;
	vecPlayer.z = pos.z - posPlayer.z;

	if (D3DXVec3Length(&vecPlayer) < 1200.0f)
	{
		m_nCntDelivery++;

		if (m_nCntDelivery >= 3)
		{
			m_nCntDelivery = 0;

			CManager::Get()->Get()->GetScene()->GetCombo()->AddCombo(10);
			int combo = CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo();
			CManager::Get()->Get()->GetScene()->GetScore()->AddScore(3000 * combo);
			CSound::PlaySound(CSound::SOUND_LABEL_SE_SCORE);

			for (int nCntParticle = 0; nCntParticle < 20; nCntParticle++)
			{
				float fHue = (float)(rand() % 361);
				CParticle::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), useful::HSLtoRGB(fHue), 5, 10, 2, 50, 30.0f, 30.0f);
			}

			SetPos(CManager::Get()->Get()->GetScene()->GetMap()->GetPosAreaCorner());
		}
	}

	return true;
}
