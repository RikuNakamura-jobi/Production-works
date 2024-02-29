//=============================================================================
//
// �T�E���h���� [sound.h]
// Author :������
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,			// BGM0
	SOUND_LABEL_BGM001,				// BGM1
	SOUND_LABEL_BGM002,				// BGM2
	SOUND_LABEL_BGM003,				// BGM3
	SOUND_LABEL_BGM004,				// BGM4
	SOUND_LABEL_BGM005,				// BGM5
	SOUND_LABEL_BGM006,				// BGM6
	SOUND_LABEL_BGM007,				// BGM7
	SOUND_LABEL_BGM008,				// BGM8
	SOUND_LABEL_BGM009,				// BGM9
	SOUND_LABEL_BGM010,				// BGM10
	SOUND_LABEL_BGM011,				// BGM11
	SOUND_LABEL_BGM012,				// BGM12
	SOUND_LABEL_BGM013,				// BGM13
	SOUND_LABEL_BGM014,				// BGM14
	SOUND_LABEL_SE_SHOT,			// �e���ˉ�1
	SOUND_LABEL_SE_SHOT001,			// �e���ˉ�2
	SOUND_LABEL_SE_SHOT002,			// �e���ˉ�3
	SOUND_LABEL_SE_DECISION001,		// ���艹
	SOUND_LABEL_SE_HIT,				// �q�b�g��
	SOUND_LABEL_SE_EXPLOSION,		// ������
	SOUND_LABEL_SE_RESULT001,		// ���U���g��
	SOUND_LABEL_SE_WARNING001,		// �x����
	SOUND_LABEL_SE_KATANA001,		// ����
	SOUND_LABEL_SE_BOSSWARNING001,	// �U���x����
	SOUND_LABEL_SE_BEAM001,			// �r�[����
	SOUND_LABEL_SE_DORAGON001,		// �h���S���U����
	SOUND_LABEL_SE_STAGESTART001,	// �X�e�[�W�J�n��
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
