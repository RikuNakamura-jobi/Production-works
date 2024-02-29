//===========================
//
//�_�C���N�gX.tutorial�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "tutorial.h"
#include "fade.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "billboard.h"
#include "meshfield.h"
#include "ui.h"
#include "pause.h"
#include "sound.h"
#include "model.h"

//�O���[�o���ϐ��錾
TUTORIALSTATE g_TUTORIALSTATE = TUTORIALSTATE_NONE;								//�Q�[���̏��
int g_nStateCounterTutorial;

//===========================
//�Q�[����ʂ̏���������
//===========================
void InitTutorial(void)
{
	//�G�̏���������
	srand((unsigned int)time(0));

	//�J�����̏�����
	InitCamera();

	//���C�g�̏�����
	InitLight();

	//�|�[�Y�̏�����
	InitPause();

	InitBillboard();

	SetPlayer();
	ResetModelTutorial();

	g_TUTORIALSTATE = TUTORIALSTATE_NORMAL;
	g_nStateCounterTutorial = 120;

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM002);
}

//===========================
//�Q�[����ʂ̏I������
//===========================
void UninitTutorial(void)
{
	//�T�E���h�̒�~
	StopSound();

	//�J�����̏�����
	UninitCamera();

	//���C�g�̏�����
	UninitLight();

	//�|�[�Y�̏�����
	UninitPause();

	//�|���S���̏�����
	UninitBillboard();
}

//===========================
//�Q�[����ʂ̍X�V����
//===========================
void UpdateTutorial(void)
{
	FADE fade = GetFade();

	//���f���̏�����
	UpdatePlayerTutorial();

	//�J�����̏�����
	UpdateCamera();

	//���C�g�̏�����
	UpdateLight();

	//�|���S���̏�����
	UpdateBillboard();

	//�|���S���̏�����
	UpdateMeshField();

	//UI�̏�����
	UpdateModel();

	//UI�̏�����
	UpdateUi();

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetPadButtonTrigger(1) == true))
	{
		if (fade == FADE_NONE)
		{
			//���[�h�ݒ�
			SetFade(MODE_GAME);
		}
	}

	switch (g_TUTORIALSTATE)
	{
	case TUTORIALSTATE_NORMAL:
		break;
	case TUTORIALSTATE_END:
		g_nStateCounterTutorial--;
		if (g_nStateCounterTutorial <= 0 && fade == FADE_NONE)
		{
			SetFade(MODE_RESULT);
		}
		break;
	}
}

//===========================
//�Q�[����ʂ̕`�揈��
//===========================
void DrawTutorial(void)
{
	//�J�����̐ݒ�
	SetCamera();

	//�|���S���̏�����
	DrawMeshField();

	//�|���S���̏�����
	//DrawMeshWall();

	//�|���S���̏�����
	//DrawBillboard();

	//���f���̏�����
	DrawPlayer();

	//���f���̏�����
	DrawModel();

	//UI�̏�����
	DrawUi();
}

//===========================
//�Q�[����ʏ�Ԏ擾����
//===========================
void SetTUTORIALSTATE(TUTORIALSTATE state, int nCounter)
{
	g_TUTORIALSTATE = state;
}

//===========================
//�Q�[����ʏ�Ԓ񋟏���
//===========================
TUTORIALSTATE GetTUTORIALSTATE(void)
{
	return g_TUTORIALSTATE;
}