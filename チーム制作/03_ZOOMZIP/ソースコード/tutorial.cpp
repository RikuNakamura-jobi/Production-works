//==========================================================
//
//�`���[�g���A����ʏ��� [tutorial.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "polygon.h"

//==========================================================
//�O���[�o���ϐ�
//==========================================================

//==========================================================
//�`���[�g���A����ʂ̏���������
//==========================================================
void InitTutorial(void)
{
	InitPolygon();

	PlaySound(SOUND_LABEL_TUTORIAL);	//BGM�̍Đ�
}

//==========================================================
//�`���[�g���A����ʂ̏I������
//==========================================================
void UninitTutorial(void)
{
	UninitPolygon();

	StopSound();	//BGM�̒�~
}

//==========================================================
//�`���[�g���A����ʂ̍X�V����
//==========================================================
void UpdateTutorial(void)
{
	UpdatePolygon();

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true || 
		GetGamepadTrigger(BUTTON_A, 0) == true || GetGamepadTrigger(BUTTON_START, 0) == true)
	{//�J�ڃL�[�������ꂽ
		SetFade(MODE_GAME);
		PlaySound(SOUND_LABEL_SEBUTTON);
	}
}

//==========================================================
//�`���[�g���A����ʂ̕`�揈��
//==========================================================
void DrawTutorial(void)
{
	DrawPolygon();
}