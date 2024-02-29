//===========================
//
//�_�C���N�gX.game�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "game.h"
#include "player.h"
#include "bg.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "fade.h"
#include "effect.h"
#include "particle.h"
#include "warning.h"
#include "pause.h"
#include "bullet.h"
#include "area.h"

//�O���[�o���ϐ��錾
bool g_bPause = false;												//�|�[�Y��Ԃ�ON/OFF
GAMESTATE g_gameState = GAMESTATE_NONE;								//�Q�[���̏��
int g_nCounterGameState = 0;										//��ԊǗ��J�E���^�[
bool g_bEnemySpawn;													//�S�Ŋm�F

//===========================
//�Q�[����ʂ̏���������
//===========================
void InitGame(void)
{
	Enemy aENEMY;

	//�e��I�u�W�F�N�g�̏���������
	InitExplosion();
	InitPlayer();
	InitBg();
	InitScore();
	SetScore(0);
	InitEffect();
	InitParticle();
	InitBullet();
	InitArea();

	//�G�̏���������
	srand((unsigned int)time(0));

	InitEnemy();

	SetEnemy(D3DXVECTOR3(100.0f, 550.0f, 0.0f), 0, 0, 0);

	g_gameState = GAMESTATE_NORMAL;
	g_nCounterGameState = 0;
	g_bPause = false;
	g_bEnemySpawn = false;

	PlaySound(SOUND_LABEL_BGM015);
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
	UninitBg();
	UninitExplosion();
	UninitEnemy();
	UninitScore();
	UninitEffect();
	UninitParticle();
	UninitBullet();
	UninitPause();
	UninitArea();
}

//===========================
//�Q�[����ʂ̍X�V����
//===========================
void UpdateGame(void)
{
	Enemy aENEMY;
	Player *pPlayer = GetPlayer();
	FADE fade = GetFade();
	D3DXVECTOR3 posBullet;
	D3DXVECTOR3 moveBullet;
	D3DXVECTOR3 rotBullet;
	int nNumEnemy = GetNumEnemy();

	if (g_bPause == false)
	{
		//�e��I�u�W�F�N�g�̍X�V����
		UpdateExplosion();
		UpdatePlayer();
		UpdateBg();
		UpdateEnemy();
		UpdateScore();
		UpdateEffect();
		UpdateParticle();
		UpdateBullet();
		UpdateArea();
	}
	else if (g_bPause == true)
	{
		UpdatePause();
	}

	//�|�[�Y
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		InitPause();
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
	DrawEnemy();
	DrawPlayer();
	DrawArea();
	DrawBullet();
	DrawExplosion();

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