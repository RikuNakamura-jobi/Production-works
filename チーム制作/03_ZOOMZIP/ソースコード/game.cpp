//===================================================
//
//�Q�[����ʏ��� [game.cpp]
//Author Ibuki Okusada
//
//===================================================
#include "game.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "fileload.h"
#include "texture.h"
#include "meshfield.h"
#include "meshwall.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "score.h"
#include "polygon.h"
#include "enemy.h"
#include "item.h"
#include "high_score.h"
#include "cloud.h"
#include "start.h"

//===================================================
//�v���g�^�C�v�錾
//===================================================
void InitGameMain(void);
void UpdateGameMain(void);
void DrawGameMain(void);

//===================================================
//�O���[�o���ϐ��錾
//===================================================
GAMESTATE g_gameState = GAMESTATE_NONE;		//�Q�[���̏��
int g_nCounterGameState;	//��ԊǗ��J�E���^�[
bool g_bPause = false;		//�|�[�Y��Ԃ�ON/OFF
bool g_bEdit = false;		//�G�f�B�b�g��ON/OFF

//===================================================
//�Q�[����ʂ̏���������
//===================================================
void InitGame(void)
{
	//�Q�[���v���C��ʂ̏���������
	InitGameMain();

	g_gameState = GAMESTATE_NONE;	//�ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;		//�J�E���^�[��������
	g_bPause = false;				//�|�[�Y��ԏ�����
	g_bEdit = false;

	PlaySound(SOUND_LABEL_GAME);
}

//===================================================
//�Q�[���v���C��ʂ̏���������
//===================================================
void InitGameMain(void)
{
	//�v���C���[�̏�����
	InitPlayer();

	//�G�t�F�N�g�̏�����
	InitEffect();

	//�p�[�e�B�N���̏�����
	InitParticle();

	//�X�R�A�̏�����
	InitScore();

	//�n�C�X�R�A�̏�����
	InitHighScore();

	//�|���S���̏�����
	InitPolygon();

	//�G�l�~�[�̏���������
	InitEnemy();

	//�A�C�e���̏���������
	InitItem();

	//�_�̏���������
	InitCloud();

	//�X�^�[�g�̏���������
	InitStart();
}

//===================================================
//�Q�[����ʂ̏I������
//===================================================
void UninitGame(void)
{
	StopSound();	//BGM�̒�~

	//�v���C���[�̏I������
	UninitPlayer();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�X�R�A�̏I������
	UninitScore();

	//�n�C�X�R�A�̏I������
	UninitHighScore();

	//�|���S���̏I������
	UninitPolygon();

	//�G�l�~�[�̏I������
	UninitEnemy();

	//�A�C�e���̏I������
	UninitItem();

	//�_�̏I��
	UninitCloud();

	//�X�^�[�g�̏I��
	UninitStart();
}

//===================================================
//�Q�[����ʂ̍X�V����
//===================================================
void UpdateGame(void)
{
	switch (g_gameState)
	{
	case GAMESTATE_NONE:
		//�|���S���̍X�V����
		UpdatePolygon();

		//
		UpdateCloud();

		//�X�^�[�g�̍X�V����
		UpdateStart();
		break;
	case GAMESTATE_NORMAL:
		UpdateGameMain();
		break;
	case GAMESTATE_END:
		//�|���S���̍X�V����
		UpdatePolygon();

		//
		UpdateCloud();

		//�X�^�[�g�̍X�V����
		UpdateHighScore();
		break;
	}
}

//===================================================
//�Q�[���v���C��ʍX�V����
//===================================================
void UpdateGameMain(void)
{
	//�v���C���[�̍X�V����
	UpdatePlayer();

	//�G�t�F�N�g�̍X�V����
	UpdateEffect();

	//�X�R�A�̍X�V����
	UpdateScore();

	//�n�C�X�R�A�̍X�V����
	UpdateHighScore();

	//�|���S���̍X�V����
	UpdatePolygon();

	//�G�l�~�[�̍X�V����
	UpdateEnemy();

	//�A�C�e���̍X�V����
	UpdateItem();

	//�_�̍X�V����
	UpdateCloud();
}	

//===================================================
//�Q�[����ʕ`�揈��
//===================================================
void DrawGame(void)
{	
	//���C���`��
	DrawGameMain();
}

//===================================================
//�Q�[����Ԑݒ�
//===================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
	g_nCounterGameState = nCounter;
}

//===================================================
//�Q�[����Ԏ擾
//===================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//===================================================
//�|�[�Y��Ԑݒ�
//===================================================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause ? false: true;
}

//===================================================
//�ʏ�`��@
//===================================================
void DrawGameMain(void)
{
	//�|���S���̕`�揈��
	DrawPolygon();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	//�G�l�~�[�̕`�揈��
	DrawEnemy();

	//�_�̕`�揈��
	DrawCloud();

	//�X�R�A�̕`�揈��
	DrawScore();

	//�A�C�e���̕`�揈��
	DrawItem();

	if (g_gameState == GAMESTATE_NONE)
	{
		//�X�^�[�g�̕`��
		DrawStart();
	}

	if (g_gameState == GAMESTATE_END)
	{
		//�n�C�X�R�A�̕`�揈��
 		DrawHighScore();
	}

}
