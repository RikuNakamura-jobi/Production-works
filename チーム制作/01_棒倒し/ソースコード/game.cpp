//===========================
//
//�_�C���N�gX.game�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "game.h"
#include "fade.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "billboard.h"
#include "meshfield.h"
#include "meshwall.h"
#include "ui.h"
#include "pause.h"
#include "sound.h"
#include "model.h"
#include "time.h"

//�O���[�o���ϐ��錾
bool g_bPause = false;												//�|�[�Y��Ԃ�ON/OFF
GAMESTATE g_gameState = GAMESTATE_NONE;								//�Q�[���̏��
int g_nStateCounter;

//===========================
//�Q�[����ʂ̏���������
//===========================
void InitGame(void)
{
	//�G�̏���������
	srand((unsigned int)time(0));

	//�J�����̏�����
	InitCamera();

	//���C�g�̏�����
	InitLight();

	//�|�[�Y�̏�����
	InitPause();

	//�|�[�Y�̏�����
	InitTime();

	InitBillboard();

	SetPlayer();
	ResetModel();

	g_gameState = GAMESTATE_NORMAL;
	g_nStateCounter = 120;
	g_bPause = false;

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM001);
}

//===========================
//�Q�[����ʂ̏I������
//===========================
void UninitGame(void)
{
	//�T�E���h�̒�~
	StopSound();

	//�J�����̏�����
	UninitCamera();

	//���C�g�̏�����
	UninitLight();

	//�|�[�Y�̏�����
	UninitPause();

	//�|�[�Y�̏�����
	UninitTime();

	//�|���S���̏�����
	UninitBillboard();
}

//===========================
//�Q�[����ʂ̍X�V����
//===========================
void UpdateGame(void)
{
	FADE fade = GetFade();

	if (g_bPause == true)
	{
		//�|�[�Y�̏�����
		UpdatePause();
	}
	else
	{
		//���f���̏�����
		UpdatePlayer();

		//�J�����̏�����
		UpdateCamera();

		//���C�g�̏�����
		UpdateLight();

		//�|�[�Y�̏�����
		UpdateTime();

		//�|���S���̏�����
		UpdateBillboard();

		//�|���S���̏�����
		UpdateMeshField();

		//UI�̏�����
		UpdateUi();
	}

	//�|�[�Y
	if (GetKeyboardTrigger(DIK_P) == true || GetPadButtonTrigger(7) == true)
	{
		ResetPause();
		g_bPause = g_bPause ? false : true;
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:
		break;
	case GAMESTATE_END:
		g_nStateCounter--;
		if (g_nStateCounter <= 0 && fade == FADE_NONE)
		{
			SetFade(MODE_RESULT);
		}
		break;
	}
}

//===========================
//�Q�[����ʂ̕`�揈��
//===========================
void DrawGame(void)
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

	//�|�[�Y�̏�����
	DrawTime();

	if (g_bPause == true)
	{
		//�|�[�Y�̏�����
		DrawPause();
	}
}

//===========================
//�Q�[����ʏ�Ԏ擾����
//===========================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
}

//===========================
//�Q�[����ʏ�Ԓ񋟏���
//===========================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//===========================
//�|�[�Y��ԕύX����
//===========================
void SetEnablePause(bool pPause)
{
	g_bPause = pPause;
}