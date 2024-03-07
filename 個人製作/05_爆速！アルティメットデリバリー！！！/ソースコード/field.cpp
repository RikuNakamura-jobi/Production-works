//=====================================
//
// 
// Author:������
//
//=====================================
#include "field.h"
#include "renderer.h"
#include "manager.h"
#include "useful.h"
#include "debugproc.h"

//�}�N����`---------------------------

//�񋓌^��`---------------------------

//�N���X��`---------------------------

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

//�ÓI�����o�ϐ��錾-------------------
LPDIRECT3DTEXTURE9 CField::m_pTexture[CField::TYPE_MAX] = {};
const char *CField::m_apFilename[CField::TYPE_MAX] =
{
	"data/TEXTURE/sea000.png",
	"data/TEXTURE/roadCross000.png",
	"data/TEXTURE/roadT000.png",
	"data/TEXTURE/roadL000.png",
	"data/TEXTURE/roadS000.png",
	"data/TEXTURE/roadO000.png",
	"data/TEXTURE/roadS000.png",
	"data/TEXTURE/rand000.png"
};
//=====================================
// �R���X�g���N�^�E�f�X�g���N�^
//=====================================
CField::CField(int nPriority = 0) : CObject3D(nPriority)
{
	m_nHue = 0;
}

CField::~CField()
{
}

//=====================================
// ��������
//=====================================
CField *CField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CField::TYPE type)
{
	CField *pObjectBg;

	//2D�I�u�W�F�N�g�̐���
	pObjectBg = new CField();

	if (pObjectBg != NULL)
	{
		pObjectBg->SetPos(pos);
		pObjectBg->SetSize(size);
		pObjectBg->m_type = type;

		//������
		if (FAILED(pObjectBg->Init()))
		{
			pObjectBg->Release();
		}

		pObjectBg->BindTexture(m_pTexture[type]);
	}

	return pObjectBg;
}

//=====================================
// �|���S���̃e�N�X�`������
//=====================================
HRESULT CField::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		//�e�N�X�`���̓ǂݍ���
		if (FAILED(D3DXCreateTextureFromFile(pDevice,
			m_apFilename[nCnt],
			&m_pTexture[nCnt])))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}
void CField::Unload(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (m_pTexture != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=====================================
// �|���S���̏���������
//=====================================
HRESULT CField::Init(void)
{
	CObject3D::Init();

	//SetType(TYPE_BG);

	return S_OK;
}

//=====================================
// �|���S���̏I������
//=====================================
void CField::Uninit(void)
{
	CObject3D::Uninit();
}

//=====================================
// �|���S���̍X�V����
//=====================================
void CField::Update(void)
{
	//Set(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1280.0f, 720.0f);

	//SetCol(useful::HSLtoRGB(m_nHue));
	m_nHue++;

	if (m_type == CField::TYPE_NONE)
	{
		SetCol(useful::HSLtoRGB(180.0f));
	}

	CObject3D::Update();
}

//=====================================
// �|���S���̕`�揈��
//=====================================
void CField::Draw(void)
{
	CObject3D::Draw();
}