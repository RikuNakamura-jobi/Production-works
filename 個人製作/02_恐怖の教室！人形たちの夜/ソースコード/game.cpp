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
#include "polygon.h"
#include "player.h"
#include "shadow.h"
#include "field.h"
#include "wall.h"
#include "billboard.h"
#include "bullet.h"
#include "effect.h"
#include "explosion.h"
#include "particle.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshceiling.h"
#include "skyceiling.h"
#include "skyfield.h"
#include "skywall.h"
#include "model.h"
#include "enemy.h"
#include "ui.h"
#include "pause.h"
#include "sound.h"

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

	//�e�̏�����
	InitShadow();

	//�G�t�F�N�g�̏�����
	InitEffect();

	//�����̏�����
	InitExplosion();

	//�p�[�e�B�N���̏�����
	InitParticle();

	//�e�̏�����
	InitBullet();

	//���f���̏�����
	InitPlayer();

	//���f���̏�����
	InitModel();

	InitEnemyGame();

	InitBillboard();

	g_gameState = GAMESTATE_NORMAL;
	g_nStateCounter = 120;
	g_bPause = false;

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM002);
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

	//���f���̏�����
	UninitPlayer();

	//�e�̏�����
	UninitShadow();

	//�G�t�F�N�g�̏�����
	UninitEffect();

	//�����̏�����
	UninitExplosion();

	//�p�[�e�B�N���̏�����
	UninitParticle();

	//�|���S���̏�����
	UninitBillboard();

	//�e�̏�����
	UninitBullet();

	//���f���̏�����
	UninitModel();

	

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
		//�J�����̏�����
		UpdateCamera();

		//���C�g�̏�����
		UpdateLight();

		//���f���̏�����
		UpdatePlayer();

		//�e�̏�����
		UpdateShadow();

		//�G�t�F�N�g�̏�����
		UpdateEffect();

		//�����̏�����
		UpdateExplosion();

		//�p�[�e�B�N���̏�����
		UpdateParticle();

		//�|���S���̏�����
		UpdateBillboard();

		//�|���S���̏�����
		UpdateMeshField();

		//�|���S���̏�����
		UpdateMeshWall();

		//�|���S���̏�����
		UpdateMeshCeiling();

		//�|���S���̏�����
		UpdateSkyField();

		//�|���S���̏�����
		UpdateSkywall();

		//�|���S���̏�����
		UpdateSkyCeiling();

		//�e�̏�����
		UpdateBullet();

		//���f���̏�����
		UpdateModel();

		//���f���̏�����
		UpdateEnemy();

		//UI�̏�����
		UpdateUi();
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
	case GAMESTATE_BOSS:
		break;
	case GAMESTATE_BOSSBATTLE:
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
	DrawMeshWall();

	//�|���S���̏�����
	DrawMeshCeiling();

	//�|���S���̏�����
	DrawSkyField();

	//�|���S���̏�����
	DrawSkywall();

	//�|���S���̏�����
	DrawSkyCeiling();

	//�|���S���̏�����
	DrawBillboard();

	//�e�̏�����
	DrawShadow();

	//�G�t�F�N�g�̏�����
	DrawEffect();

	//�p�[�e�B�N���̏�����
	DrawParticle();

	//�����̏�����
	DrawExplosion();

	//�e�̏�����
	DrawBullet();

	//���f���̏�����
	DrawPlayer();

	//���f���̏�����
	DrawModel();

	//���f���̏�����
	DrawEnemy();

	//UI�̏�����
	DrawUi();

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