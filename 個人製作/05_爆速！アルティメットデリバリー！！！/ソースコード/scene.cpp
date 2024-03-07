//=====================================
//
// 
// Author:������
//
//=====================================
#include "scene.h"
#include "object.h"
#include "object3D.h"
#include "bg.h"
#include "bgmulti.h"
#include "score.h"
#include "camera.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "time.h"
#include "field.h"
#include "sky.h"
#include "block.h"
#include "deliverypoint.h"
#include "sound.h"
#include "map.h"
#include "car.h"
#include "combo.h"

//�}�N����`---------------------------
#define TIME_FADE (3600)

//�񋓌^��`---------------------------

//�N���X��`---------------------------

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

//�ÓI�����o�ϐ��錾-------------------

//=====================================
// �R���X�g���N�^�E�f�X�g���N�^
//=====================================
CScene::CScene()
{
	m_nCntFade = 0;
}

CScene::~CScene()
{
}

//=====================================
// ����
//=====================================
CScene *CScene::Create(MODE mode)
{
	CScene *pScene = NULL;

	switch (mode)
	{
	case MODE_TITLE:

		pScene = new CTitle;

		break;

	case MODE_TUTORIAL:

		pScene = new CTutorial;

		break;

	case MODE_GAME:

		pScene = new CGame;

		break;

	case MODE_RESULT:

		pScene = new CResult;

		break;

	default:
		break;
	}

	return pScene;
}

//=====================================
// �^�C�g�����
//=====================================
// �R���X�g���N�^�E�f�X�g���N�^
//=====================================
CTitle::CTitle()
{
	m_pBgMulti = NULL;
}

CTitle::~CTitle()
{
}

//=====================================
// �|���S���̏���������
//=====================================
HRESULT CTitle::Init(void)
{
	m_pBgMulti = CBgMulti::Create();

	CSound::PlaySound(CSound::SOUND_LABEL_BGM001);

	return S_OK;
}

//=====================================
// �|���S���̏I������
//=====================================
void CTitle::Uninit(void)
{
	CSound::StopSound();

	CObject::ReleaseTYPEN(CObject::TYPE_FADE);
}

//=====================================
// �|���S���̍X�V����
//=====================================
void CTitle::Update(void)
{
	CInput *input = CManager::Get()->GetInputKeyboard();
	CInput *inputPad = CManager::Get()->GetInputPad();

	if (input->GetTrigger(DIK_RETURN) == true && CFade::GetState() == 0)
	{
		CFade::Create(CFade::TYPE_OUT, MODE_RESULT);
		CSound::PlaySound(CSound::SOUND_LABEL_SE_YES);
	}

	if (inputPad != NULL)
	{
		if (inputPad->GetButtonTrigger(11) == true && CFade::GetState() == 0)
		{
			CFade::Create(CFade::TYPE_OUT, MODE_RESULT);
			CSound::PlaySound(CSound::SOUND_LABEL_SE_YES);
		}
	}

	if (GetbFade() == true)
	{
		CManager::Get()->SetMode(CScene::MODE_GAME);
		SetbFade(false);
	}

	addCntFade();
}

//=====================================
// �|���S���̕`�揈��
//=====================================
void CTitle::Draw(void)
{

}
// �^�C�g�����
//=====================================

//=====================================
// �`���[�g���A�����
//=====================================
// �R���X�g���N�^�E�f�X�g���N�^
//=====================================
CTutorial::CTutorial()
{
	m_pBg = NULL;
	m_bFinish = NULL;
}

CTutorial::~CTutorial()
{
}

//=====================================
// �|���S���̏���������
//=====================================
HRESULT CTutorial::Init(void)
{
	m_pBg = CBg::Create(CBg::TEXTURE_RESULT);

	CSound::PlaySound(CSound::SOUND_LABEL_BGM002);

	return S_OK;
}

//=====================================
// �|���S���̏I������
//=====================================
void CTutorial::Uninit(void)
{
	CSound::StopSound();
	
	CObject::ReleaseTYPEN(CObject::TYPE_FADE);

	if (m_pCamera != NULL)
	{
		//�I������
		m_pCamera->Uninit();

		delete m_pCamera;
		m_pCamera = NULL;
	}
}

//=====================================
// �|���S���̍X�V����
//=====================================
void CTutorial::Update(void)
{
	CInput *input = CManager::Get()->GetInputKeyboard();
	CInput *inputMouse = CManager::Get()->GetInputMouse();
	CInput *inputPad = CManager::Get()->GetInputPad();
	CBg::TEXTURE textureType = m_pBg->GetTextureType();

	if (m_bFinish == true)
	{
		if (GetCntFade() == 0 && CFade::GetState() == 0)
		{
			CFade::Create(CFade::TYPE_OUT, MODE_RESULT);
		}
	}
	else
	{
		if (input->GetTrigger(DIK_RETURN) == true && CFade::GetState() == 0)
		{
				m_bFinish = true;
		}

		if (inputPad != NULL)
		{
			if (inputPad->GetButtonTrigger(11) == true && CFade::GetState() == 0)
			{
					m_bFinish = true;
			}
		}
	}

	if (GetbFade() == true)
	{
		CManager::Get()->SetMode(CScene::MODE_GAME);
		SetbFade(false);
	}
}

//=====================================
// �|���S���̕`�揈��
//=====================================
void CTutorial::Draw(void)
{
	if (m_pCamera != NULL)
	{
		m_pCamera->Set();
	}
}

void CTutorial::SetPlayer(CPlayer *player)
{
	m_pPlayer->SetMove(player->GetMove());
	m_pPlayer->SetPos(player->GetPos());
	m_pPlayer->SetPosOld(player->GetPosOld());
	m_pPlayer->SetRot(player->GetRot());
	m_pPlayer->SetType(player->GetType());
}
// �`���[�g���A�����
//=====================================

//=====================================
// �Q�[�����
//=====================================
// �R���X�g���N�^�E�f�X�g���N�^
//=====================================
CGame::CGame()
{
	m_pScore = NULL;
	m_pField = NULL;
	m_pPlayer = NULL;
	m_pPlayerSave = NULL;
	m_pCamera = NULL;
	m_pTime = NULL;
	m_pBgStart = nullptr;
	m_bFinish = false;
	m_nStartCount = 0;
	m_nTutorialCount = 0;
	m_nPadCrossCount = -1;
}

CGame::~CGame()
{
}

//=====================================
// �|���S���̏���������
//=====================================
HRESULT CGame::Init(void)
{
	m_pField = CField::Create(D3DXVECTOR3(36000.0f, -10.0f, 36000.0f), D3DXVECTOR3(100000.0f, 0.0f, 100000.0f), CField::TYPE_NONE);
	m_pSky = CSky::Create();
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -2.335f, 0.0f));
	m_pMap = CMap::Create();
	m_pScore = CScore::Create(D3DXVECTOR3(1250.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 52.0f, 128.0f, false);
	m_pCombo = CCombo::Create(D3DXVECTOR3(1250.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 52.0f, 128.0f);
	m_pTime = CTime::Create(D3DXVECTOR3(100.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 52.0f, 128.0f);
	m_pBgStart = CBg::CreateMin(D3DXVECTOR3(640.0f, 360.0f, 0.0f), 900.0f, 600.0f, CBg::TEXTURE_TUTORIAL_KEY0);
	m_pDeliverypoint = CDeliverypoint::Create(D3DXVECTOR3(10600.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1000.0f, 1000.0f);
	m_pDeliverypoint->SetPos(m_pMap->GetPosAreaCorner());

	//�J�����̐���
	if (m_pCamera != NULL)
	{
		//�I������
		m_pCamera->Uninit();

		delete m_pCamera;
		m_pCamera = NULL;

		m_pCamera = new CCamera;
	}
	else
	{
		m_pCamera = new CCamera;
	}

	//����������
	if (m_pCamera != NULL)
	{
		if (FAILED(m_pCamera->Init()))
		{//���������������s�����ꍇ
			return E_FAIL;
		}
	}
	else
	{
		return E_FAIL;
	}

	m_nStartCount = 0;

	return S_OK;
}

//=====================================
// �|���S���̏I������
//=====================================
void CGame::Uninit(void)
{
	CSound::StopSound();
	
	CObject::ReleaseTYPEN(CObject::TYPE_FADE);

	if (m_pMap != nullptr)
	{
		m_pMap->Uninit();

		delete m_pMap;
		m_pMap = nullptr;
	}

	if (m_pCamera != NULL)
	{
		//�I������
		m_pCamera->Uninit();

		delete m_pCamera;
		m_pCamera = NULL;
	}
}

//=====================================
// �|���S���̍X�V����
//=====================================
void CGame::Update(void)
{
	if (m_nStartCount > 0)
	{
		m_nStartCount++;
	}

	if (m_nStartCount == 0)
	{
		CInput *input = CManager::Get()->GetInputKeyboard();
		CInput *inputMouse = CManager::Get()->GetInputMouse();
		CInput *inputPad = CManager::Get()->GetInputPad();

		if (input->GetTrigger(DIK_RETURN) == true && m_nTutorialCount == 2)
		{
			m_nStartCount++;
			m_pBgStart->SetTextureType(CBg::TEXTURE_MAX);
		}

		if (input->GetTrigger(DIK_D) == true && m_nTutorialCount < 2)
		{
			m_nTutorialCount++;
		}

		if (input->GetTrigger(DIK_A) == true && m_nTutorialCount > 0)
		{
			m_nTutorialCount--;
		}

		if (input->GetAll() || inputMouse->GetAll())
		{
			m_pBgStart->SetTextureType((CBg::TEXTURE)((int)(CBg::TEXTURE_TUTORIAL_KEY0) + m_nTutorialCount));
		}

		if (inputPad != NULL)
		{
			if (inputPad->GetButtonTrigger(11) == true && m_nTutorialCount == 2)
			{
				m_nStartCount++;
				m_pBgStart->SetTextureType(CBg::TEXTURE_MAX);
			}

			if (inputPad->GetButtonCrossPress() == 9000 && m_nPadCrossCount != 9000 && m_nTutorialCount < 2)
			{
				m_nTutorialCount++;
			}

			if (inputPad->GetButtonCrossPress() == 27000 && m_nPadCrossCount != 27000 && m_nTutorialCount > 0)
			{
				m_nTutorialCount--;
			}

			m_nPadCrossCount = inputPad->GetButtonCrossPress();

			if (inputPad->GetAll())
			{
				m_pBgStart->SetTextureType((CBg::TEXTURE)((int)(CBg::TEXTURE_TUTORIAL_PAD0) + m_nTutorialCount));
			}
		}
	}
	else if (m_nStartCount == 30)
	{
		m_pBgStart->Set(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800.0f, 200.0f);
		m_pBgStart->SetTextureType(CBg::TEXTURE_START);
	}
	else if (m_nStartCount == 150)
	{
		m_pBgStart->SetTextureType(CBg::TEXTURE_READY);
	}
	else if (m_nStartCount == 210)
	{
		m_pBgStart->SetTextureType(CBg::TEXTURE_GO);
		CSound::PlaySound(CSound::SOUND_LABEL_BGM003);

		m_pPlayer->SetControl(true);
	}
	else if(m_nStartCount == 240)
	{
		m_pBgStart->SetTextureType(CBg::TEXTURE_MAX);
	}
	else if (m_nStartCount > 300)
	{
		m_nStartCount = 300;
	}

	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}

	CInput *input = CManager::Get()->GetInputKeyboard();
	if (m_nStartCount == 300)
	{
		if (m_bFinish == true)
		{
			addCntFade();
		}
		else
		{
			if (m_pTime->GetTime() == 0)
			{
				CBg::CreateMin(D3DXVECTOR3(640.0f, 360.0f, 0.0f), 800.0f, 200.0f, CBg::TEXTURE_SUCCESS);
				CScore::SetScoreResult(m_pScore->GetScore());
				m_bFinish = true;
				m_pPlayer->SetControl(false);
			}

			if (m_pTime->GetTime() == 100)
			{
				CSound::StopSound(CSound::SOUND_LABEL_BGM003);
				CSound::PlaySound(CSound::SOUND_LABEL_BGM004);
			}
		}

		if (GetCntFade() == 300 && CFade::GetState() == 0)
		{
			m_pScore->SetScoreResult(m_pScore->GetScore());
			CFade::Create(CFade::TYPE_OUT, MODE_RESULT);
		}

		if (GetbFade() == true)
		{
			CManager::Get()->SetMode(CScene::MODE_RESULT);
			SetbFade(false);
		}
	}
}

//=====================================
// �|���S���̕`�揈��
//=====================================
void CGame::Draw(void)
{
	if (m_pCamera != NULL)
	{
		m_pCamera->Set();
	}
}

void CGame::SetPlayer(CPlayer *player)
{
	m_pPlayer->SetMove(player->GetMove());
	m_pPlayer->SetPos(player->GetPos());
	m_pPlayer->SetPosOld(player->GetPosOld());
	m_pPlayer->SetRot(player->GetRot());
	m_pPlayer->SetType(player->GetType());
}
// �Q�[�����
//=====================================

//=====================================
// ���U���g���
//=====================================
// �R���X�g���N�^�E�f�X�g���N�^
//=====================================
CResult::CResult()
{
	m_pBg = NULL;
}

CResult::~CResult()
{
}

//=====================================
// �|���S���̏���������
//=====================================
HRESULT CResult::Init(void)
{
	m_pBg = CBg::Create(CBg::TEXTURE_RESULT);

	FILE *pFile;

	//�t�@�C���������ǂݍ���
	pFile = fopen("data\\RANKING\\ranking.bin", "rb");

	int nScore[6] = {};

	if (pFile != NULL)
	{//�J�����ꍇ
		fread(&nScore[0], sizeof(int), 6, pFile);
		fclose(pFile);
	}
	else
	{//�J���Ȃ������ꍇ

	 //�����L���O������
		for (int nCnt = 0; nCnt < 6; nCnt++)
		{
			nScore[nCnt] = 0;
		}
	}

	nScore[5] = CScore::GetScoreResult();

	int nMaxNumber, nTemp;
	//�~���}���\�[�g
	for (int nCnt1 = 1; nCnt1 < 6; nCnt1++)
	{
		nMaxNumber = nCnt1;

		while (nMaxNumber > 0 && nScore[nMaxNumber - 1] < nScore[nMaxNumber])
		{
			nTemp = nScore[nMaxNumber - 1];
			nScore[nMaxNumber - 1] = nScore[nMaxNumber];
			nScore[nMaxNumber] = nTemp;

			nMaxNumber--;
		}
	}

	//�t�@�C���������ǂݍ���
	pFile = fopen("data\\RANKING\\ranking.bin", "wb");

	if (pFile != NULL)
	{//�J�����ꍇ

		fwrite(&nScore[0], sizeof(int), 6, pFile);
		fclose(pFile);
	}
	else
	{//�J���Ȃ������ꍇ

	}

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		m_pScore[nCnt] = CScore::Create(D3DXVECTOR3(830.0f, 101.0f * nCnt + 235.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 52.0f, 128.0f, true);
		m_pScore[nCnt]->AddScore(nScore[nCnt]);
	}

	m_pScore[5] = CScore::Create(D3DXVECTOR3(830.0f, 70.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 52.0f, 128.0f, true);
	m_pScore[5]->AddScore(CScore::GetScoreResult());

	CSound::PlaySound(CSound::SOUND_LABEL_BGM005);

	return S_OK;
}

//=====================================
// �|���S���̏I������
//=====================================
void CResult::Uninit(void)
{
	CSound::StopSound();

	CObject::ReleaseTYPEN(CObject::TYPE_FADE);
}

//=====================================
// �|���S���̍X�V����
//=====================================
void CResult::Update(void)
{
	CInput *input = CManager::Get()->GetInputKeyboard();
	CInput *inputPad = CManager::Get()->GetInputPad();

	if ((input->GetTrigger(DIK_RETURN) == true || GetCntFade() == TIME_FADE) && CFade::GetState() == 0)
	{
		CSound::PlaySound(CSound::SOUND_LABEL_SE_YES);
		CFade::Create(CFade::TYPE_OUT, MODE_TITLE);
	}

	if (inputPad != NULL)
	{
		if ((inputPad->GetButtonTrigger(11) == true || GetCntFade() == TIME_FADE) && CFade::GetState() == 0)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_SE_YES);
			CFade::Create(CFade::TYPE_OUT, MODE_TITLE);
		}
	}

	if (GetbFade() == true)
	{
		CManager::Get()->SetMode(CScene::MODE_TITLE);
		SetbFade(false);
	}

	addCntFade();
}

//=====================================
// �|���S���̕`�揈��
//=====================================
void CResult::Draw(void)
{
	
}
// ���U���g���
//=====================================