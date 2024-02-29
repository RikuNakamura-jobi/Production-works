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
#include "light.h"
#include "billboard.h"
#include "ui.h"
#include "meshfield.h"
#include "sound.h"

//�}�N����`
#define NUM_TITLE (3)											//�^�C�g����ʂ̐�

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureTitle[NUM_TITLE] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffTitle = NULL;					//���_�����i�[
int g_nCounterTitleState;										//��ԊǗ��J�E���^�[
bool g_TitleEnd;												//�I������

//===========================
//�^�C�g����ʂ̏���������
//===========================
void InitTitle(void)
{
	g_nCounterTitleState = 60;
	g_TitleEnd = false;

	InitCameraTitle();
	InitLight();
	SetPlayer();

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
	UpdateUi();

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetPadButtonTrigger(0) == true) && g_TitleEnd == false)
	{
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_KICK);
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
	//DrawPlayer();
	DrawUi();
}

bool GetTitle(void)
{
	return g_TitleEnd;
}