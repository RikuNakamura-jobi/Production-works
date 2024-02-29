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
#include "sound.h"

//�O���[�o���ϐ��錾
bool g_bTutorial = false;												//�|�[�Y��Ԃ�ON/OFF
int g_nTutorialCounter;

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

	InitBillboard();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM001);

	g_bTutorial = false;
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
	UninitField();

	//�e�̏�����
	UninitBullet();

	//���f���̏�����
	UninitModel();

	UninitBillboard();
}

//===========================
//�Q�[����ʂ̍X�V����
//===========================
void UpdateTutorial(void)
{
	FADE fade = GetFade();

	//�J�����̏�����
	UpdateCamera();

	//���C�g�̏�����
	UpdateLight();

	//���f���̏�����
	UpdatePlayer();

	//�|���S���̏�����
	UpdateBillboard();

	//�e�̏�����
	UpdateShadow();

	//�G�t�F�N�g�̏�����
	UpdateEffect();

	//�����̏�����
	UpdateExplosion();

	//�p�[�e�B�N���̏�����
	UpdateParticle();

	//�|���S���̏�����
	UpdateField();

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

	//UI�̏�����
	UpdateUi();

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetPadButtonTrigger(0) == true) && g_bTutorial == false)
	{
		g_bTutorial = true;
	}

	if (g_bTutorial == true)
	{
		g_nTutorialCounter--;

		if ((g_nTutorialCounter <= 0 && fade == FADE_NONE))
		{
			//���[�h�ݒ�
			SetFade(MODE_GAME);
		}
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

	//UI�̏�����
	DrawUi();
}