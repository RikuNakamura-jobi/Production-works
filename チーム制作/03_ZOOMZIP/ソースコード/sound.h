//=============================================================================
//
// �T�E���h���� [sound.h]
// Author ����ɐ�
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
	SOUND_LABEL_BGMTITLE = 0,		// BGM�^�C�g��
	SOUND_LABEL_BGMTUTORIAL,		// BGM�`���[�g���A��
	SOUND_LABEL_BGMGAME,			// BGM�Q�[��
	SOUND_LABEL_BGMCLEAR,			// BGM�N���A
	SOUND_LABEL_BGMGAMEOVER,		// BGM�Q�[���I�[�o�[
	SOUND_LABEL_BGMRANKING,			// BGM�����L���O
	SOUND_LABEL_SEBUTTON,			// ���艹
	SOUND_LABEL_SECURSOR,			// �J�[�\����
	SOUND_LABEL_SERUN,				// ����
	SOUND_LABEL_SCORE,				// �X�R�A
	SOUND_LABEL_DAMAGE,				// �_���[�W
	SOUND_LABEL_TUTORIAL,
	SOUND_LABEL_TITLE,
	SOUND_LABEL_GAME,
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
