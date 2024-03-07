//=====================================
//
// 
// Author:������
//
//=====================================
#include "effect.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"

//�}�N����`---------------------------

//�񋓌^��`---------------------------

//�N���X��`---------------------------

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

//�ÓI�����o�ϐ��錾-------------------
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;

//=====================================
// �|���S���̃e�N�X�`������
//=====================================
HRESULT CEffect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.jpg",
		&m_pTexture)))
	{
		return E_FAIL;
	}

	return S_OK;
}
void CEffect::Unload(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=====================================
// �R���X�g���N�^�E�f�X�g���N�^
//=====================================
CEffect3D::CEffect3D(int nPriority = 4) : CObjectBillboard(nPriority)
{
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
	m_nLife = 0;
	m_nLifeMax = 1;
}

CEffect3D::~CEffect3D()
{
}

//=====================================
// ��������
//=====================================
CEffect3D *CEffect3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, float fWidth, float fHeight, bool bCenter)
{
	CEffect3D *pObjectEffect;

	//2D�I�u�W�F�N�g�̐���
	pObjectEffect = new CEffect3D();

	if (pObjectEffect != NULL)
	{
		pObjectEffect->Set(pos, rot, fWidth, fHeight);

		//������
		if (FAILED(pObjectEffect->Init()))
		{
			pObjectEffect->Release();

			return NULL;
		}
		
		pObjectEffect->m_nLife = nLife;
		pObjectEffect->m_nLifeMax = nLife;
		pObjectEffect->SetMove(move);
		pObjectEffect->SetCol(col);
		pObjectEffect->SetCenter(bCenter);
		pObjectEffect->BindTexture(CEffect::GetTexture());
	}

	return pObjectEffect;
}

//=====================================
// �|���S���̏���������
//=====================================
HRESULT CEffect3D::Init(void)
{
	CObjectBillboard::Init();

	SetType(TYPE_EFFECT);

	return S_OK;
}

//=====================================
// �|���S���̏I������
//=====================================
void CEffect3D::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//=====================================
// �|���S���̍X�V����
//=====================================
void CEffect3D::Update(void)
{
	CInput *input = CManager::Get()->GetInputKeyboard();
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	float fHeight = GetHeight();
	float fWidth = GetWidth();

	pos += move;

	//�ړ��ʌ���
	move *= 0.96f;

	//�T�C�Y�k��
	fHeight *= ((float)m_nLife / (float)m_nLifeMax);
	fWidth *= ((float)m_nLife / (float)m_nLifeMax);

	//��������
	m_nLife--;

	SetPos(pos);
	SetRot(rot);
	SetMove(move);
	SetHeight(fHeight);
	SetWidth(fWidth);

	CObjectBillboard::Update();

	if (fHeight <= 0.3f || fWidth <= 0.3f || m_nLife <= 0)
	{
		//�g�p���Ă��Ȃ���Ԃɂ���
		Uninit();
	}
}

//=====================================
// �|���S���̕`�揈��
//=====================================
void CEffect3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//Z�e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�A���t�@�e�X�g�̗L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObjectBillboard::Draw();

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�A���t�@�e�X�g�̖�����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//=====================================
// �R���X�g���N�^�E�f�X�g���N�^
//=====================================
CEffect2D::CEffect2D(int nPriority = 4) : CObject2D(nPriority)
{
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
	m_nLife = 0;
	m_nLifeMax = 1;
}

CEffect2D::~CEffect2D()
{
}

//=====================================
// ��������
//=====================================
CEffect2D *CEffect2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, float fWidth, float fHeight, bool bCenter)
{
	CEffect2D *pObjectEffect;

	//2D�I�u�W�F�N�g�̐���
	pObjectEffect = new CEffect2D();

	if (pObjectEffect != NULL)
	{
		pObjectEffect->Set(pos, rot, fWidth, fHeight);

		//������
		if (FAILED(pObjectEffect->Init()))
		{
			pObjectEffect->Release();

			return NULL;
		}

		pObjectEffect->m_nLife = nLife;
		pObjectEffect->m_nLifeMax = nLife;
		pObjectEffect->SetMove(move);
		pObjectEffect->SetCol(col);
		pObjectEffect->BindTexture(CEffect::GetTexture());
	}

	return pObjectEffect;
}

//=====================================
// �|���S���̏���������
//=====================================
HRESULT CEffect2D::Init(void)
{
	CObject2D::Init();

	SetType(TYPE_EFFECT);

	return S_OK;
}

//=====================================
// �|���S���̏I������
//=====================================
void CEffect2D::Uninit(void)
{
	CObject2D::Uninit();
}

//=====================================
// �|���S���̍X�V����
//=====================================
void CEffect2D::Update(void)
{
	CInput *input = CManager::Get()->GetInputKeyboard();
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	float fHeight = GetHeight();
	float fWidth = GetWidth();

	pos += move;

	//�ړ��ʌ���
	move *= 0.96f;

	//�T�C�Y�k��
	fHeight *= ((float)m_nLife / (float)m_nLifeMax);
	fWidth *= ((float)m_nLife / (float)m_nLifeMax);

	//��������
	m_nLife--;

	SetPos(pos);
	SetRot(rot);
	SetMove(move);
	SetHeight(fHeight);
	SetWidth(fWidth);

	CObject2D::Update();

	if (fHeight <= 0.3f || fWidth <= 0.3f || m_nLife <= 0)
	{
		//�g�p���Ă��Ȃ���Ԃɂ���
		Uninit();
	}
}

//=====================================
// �|���S���̕`�揈��
//=====================================
void CEffect2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//Z�e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�A���t�@�e�X�g�̗L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject2D::Draw();

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�A���t�@�e�X�g�̖�����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}