//===========================
//
//�_�C���N�gX.game�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "game.h"
#include "player.h"
#include "input.h"
#include "fade.h"
#include "warning.h"
#include "block.h"
#include "effect.h"
#include "particle.h"
#include "stage.h"
#include "start.h"
#include "goal.h"
#include "pause.h"
#include "enemy.h"
#include "coin.h"
#include "stagestart.h"
#include "boss.h"
#include "life.h"
#include "bg.h"
#include "boost.h"
#include "sound.h"

//�O���[�o���ϐ��錾
int g_nCounterGameState;
bool g_bPause = false;												//�|�[�Y��Ԃ�ON/OFF
GAMESTATE g_gameState = GAMESTATE_NONE;								//�Q�[���̏��

//===========================
//�Q�[����ʂ̏���������
//===========================
void InitGame(void)
{
	g_nCounterGameState = 120;
	g_gameState = GAMESTATE_NONE;

	//�e��I�u�W�F�N�g�̏���������
	InitLife(); 
	InitBoost(); 
	InitPlayer();
	InitBlock();
	InitEffect();
	InitParticle();
	InitStart();
	InitGoal();
	InitCoin();
	InitEnemy();
	InitPause();
	InitBoss();
	InitBg();
	InitStagestart();
	SetStagestart();

	//�X�e�[�W�z�u
	InitStage();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM014);
}

//===========================
//�Q�[����ʂ̏I������
//===========================
void UninitGame(void)
{
	//�T�E���h�̒�~
	StopSound();

	//�e��I�u�W�F�N�g�̏I������
	UninitPlayer();
	UninitBlock();
	UninitEffect();
	UninitParticle();
	UninitStart();
	UninitGoal();
	UninitPause();
	UninitEnemy();
	UninitCoin();
	UninitBoss();
	UninitLife();
	UninitBg();
	UninitBoost();
	UninitStagestart();
}

//===========================
//�Q�[����ʂ̍X�V����
//===========================
void UpdateGame(void)
{
	FADE fade = GetFade();
	Player *pPlayer = GetPlayer();

	if (g_bPause == false)
	{
		//�e��I�u�W�F�N�g�̍X�V����
		UpdatePlayer();
		UpdateBlock();
		UpdateEffect();
		UpdateParticle();
		UpdateStart();
		UpdateGoal();
		UpdateEnemy();
		UpdateCoin();
		UpdateBoss();
		UpdateLife();
		UpdateBg();
		UpdateBoost();
		UpdateStagestart();
	}
	else if (g_bPause == true)
	{
		UpdatePause();
	}

	//�|�[�Y
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:
		break;
	case GAMESTATE_END:
		g_nCounterGameState--;
		if (g_nCounterGameState <= 0 && fade == FADE_NONE)
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
	//�e��I�u�W�F�N�g�̕`�揈��
	DrawBg();
	DrawEffect();
	DrawParticle();
	DrawStart();
	DrawGoal();
	DrawEnemy();
	DrawPlayer();
	DrawBlock();
	DrawBoss();
	DrawCoin();
	DrawLife();
	DrawBoost();
	DrawStagestart();

	if (g_bPause == true)
	{
		DrawPause();
	}
}

//===========================
//�Q�[����ʏ�Ԏ擾����
//===========================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
	g_nCounterGameState = nCounter;
}

//===========================
//�|�[�Y��ԕύX����
//===========================
void SetEnablePause(bool pPause)
{
	g_bPause = pPause;
}