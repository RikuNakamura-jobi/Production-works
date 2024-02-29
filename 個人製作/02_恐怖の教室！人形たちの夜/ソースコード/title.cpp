//===========================
//
//�_�C���N�gX.title�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "title.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "effect.h"
#include "light.h"
#include "billboard.h"
#include "ui.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshceiling.h"
#include "skyceiling.h"
#include "skyfield.h"
#include "skywall.h"
#include "model.h"
#include "sound.h"

//�}�N����`
#define NUM_TITLE (3)											//�^�C�g����ʂ̐�

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureTitle[NUM_TITLE] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffTitle = NULL;					//���_�����i�[
D3DXVECTOR3 g_posTitle;
int g_nCounterTitleState;										//��ԊǗ��J�E���^�[
int nTimerRanking;												//�^�C�g���̃����L���O�J�ڃ^�C�}�[
float g_fCntTitle = 0.0f;										//�^�C�g���̓_�ŗp�ϐ�
bool g_TitleEnd;												//�I������

//===========================
//�^�C�g����ʂ̏���������
//===========================
void InitTitle(void)
{
	g_nCounterTitleState = 60;
	g_TitleEnd = false;

	InitCamera();
	InitLight();
	InitShadow();
	InitEffect();
	InitPlayerTitle();
	InitModel();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM000);
}

//===========================
//�^�C�g����ʂ̏I������
//===========================
void UninitTitle(void)
{
	//�T�E���h�̒�~
	StopSound();

	UninitCamera();
	UninitLight();
	UninitShadow();
	UninitEffect();
	UninitPlayer();
	UninitModel();
}

//===========================
//�^�C�g����ʂ̍X�V����
//===========================
void UpdateTitle(void)
{
	FADE fade = GetFade();

	UpdateCameraTitle();
	UpdateLight();
	UpdateMeshField();
	UpdateMeshWall();
	UpdateMeshCeiling();
	UpdateSkyField();
	UpdateSkywall();
	UpdateSkyCeiling();
	UpdateModel();
	UpdateShadow();
	UpdateEffect();
	UpdateUi();

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetPadButtonTrigger(0) == true) && g_TitleEnd == false)
	{
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_START);

		g_TitleEnd = true;
	}

	if (g_TitleEnd == true)
	{
		g_nCounterTitleState--;

		if ((g_nCounterTitleState <= 0 && fade == FADE_NONE))
		{
			//���[�h�ݒ�
			SetFade(MODE_TUTORIAL);
		}
	}
}

//===========================
//�^�C�g����ʂ̕`�揈��
//===========================
void DrawTitle(void)
{
	SetCamera();
	DrawMeshField();
	DrawMeshWall();
	DrawMeshCeiling();
	DrawSkyField();
	DrawSkywall();
	DrawSkyCeiling();
	DrawPlayer();
	DrawModel();
	DrawShadow();
	DrawEffect();
	DrawUi();
}