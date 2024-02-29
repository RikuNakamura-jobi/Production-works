//===========================
//
//�_�C���N�gX.result�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "player.h"
#include "light.h"
#include "ui.h"
#include "meshfield.h"
#include "sound.h"

//�}�N����`
#define NUM_RESULT (3)												//���U���g��ʂ̐�

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureResult[NUM_RESULT] = {};				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffResult = NULL;						//���_�����i�[
RESULT g_Result;
int g_nCounterTime;													//���U���g��ʂ̌p������

//===========================
//���U���g��ʂ̏���������
//===========================
void InitResult(void)
{
	g_nCounterTime = 600;

	InitCamera();
	InitLight();
	SetPlayer();

	if (g_Result == RESULT_CLEAR)
	{
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_BGM004);
	}
	if (g_Result == RESULT_OVER)
	{
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_BGM003);
	}
}

//===========================
//���U���g��ʂ̏I������
//===========================
void UninitResult(void)
{
	//�T�E���h�̒�~
	StopSound();

	UninitCamera();
	UninitLight();
}

//===========================
//���U���g��ʂ̍X�V����
//===========================
void UpdateResult(void)
{
	FADE fade = GetFade();

	UpdateCamera();
	UpdateLight();
	UpdateMeshField();
	UpdateUi();

	g_nCounterTime--;

	if ((GetKeyboardTrigger(DIK_RETURN) == true && fade == FADE_NONE) || (g_nCounterTime <= 0 && fade == FADE_NONE) || (GetPadButtonTrigger(0) == true && fade == FADE_NONE))
	{
		if (g_Result == RESULT_CLEAR)
		{
			//���[�h�ݒ�
			SetFade(MODE_RANKING);
		}
		if (g_Result == RESULT_OVER)
		{
			//���[�h�ݒ�
			SetFade(MODE_TITLE);
		}
	}
}

//===========================
//���U���g��ʂ̕`�揈��
//===========================
void DrawResult(void)
{
	SetCamera();
	if (g_Result == RESULT_CLEAR)
	{
		DrawMeshField();
	}
	DrawPlayer();
	DrawUi();
}

//===========================
//���U���g��ʂ̏�ԏ���
//===========================
RESULT GetResult(void)
{
	return g_Result;
}

//===========================
//���U���g��ʂ̏�ԏ���
//===========================
void SetResult(RESULT Result)
{
	g_Result = Result;
}