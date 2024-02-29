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
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM003,			// BGM3
	SOUND_LABEL_BGM004,			// BGM4
	SOUND_LABEL_BGM005,			// BGM5
	SOUND_LABEL_SE_RUN,			// �e���ˉ�1
	SOUND_LABEL_SE_KICK,		// �e���ˉ�1
	SOUND_LABEL_SE_SLIP,		// �e���ˉ�1
	SOUND_LABEL_SE_JUMP,		// �e���ˉ�1
	SOUND_LABEL_SE_DORO,		// �e���ˉ�1
	SOUND_LABEL_SE_BOU,			// �e���ˉ�1
	SOUND_LABEL_SE_FANFARE,		// �e���ˉ�1
	SOUND_LABEL_MAX
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
