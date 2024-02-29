//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : ���v�ԗD�� / ���R����
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
	SOUND_LABEL_BGM000 = 0,				// �^�C�g��
	SOUND_LABEL_BGM001,					// ��퓬
	SOUND_LABEL_BGM002,					// ����
	SOUND_LABEL_BGM003,					// �����L���O
	SOUND_LABEL_BGM004,					// ���U���g-����(��)
	SOUND_LABEL_BGM005,					// ���U���g-�s�k(��)
	SOUND_LABEL_SE_BUTTON_000,			// ���j���[���ڂ̐؂�ւ���
	SOUND_LABEL_SE_BUTTON_001,			// ���艹
	SOUND_LABEL_SE_BUTTON_002,			// �^�C�g����ʂ̌��艹
	SOUND_LABEL_SE_BUTTON_003,			// ���U���g�Ȃǂ̑J�ډ�
	SOUND_LABEL_SE_BUTTON_004,			// ���U���g�Ȃǂ̑J�ډ�
	SOUND_LABEL_SE_STEP_000,			// �v���C���[�̑���
	SOUND_LABEL_SE_STEP_001,			// �S�[�����̑���
	SOUND_LABEL_SE_JUMP_000,			// �v���C���[�̃W�����v��
	SOUND_LABEL_SE_JUMP_001,			// �S�[�����̃W�����v��
	SOUND_LABEL_SE_ATTACK_000,			// ����@�����鉹
	SOUND_LABEL_SE_ATTACK_001,			// �U���\����
	SOUND_LABEL_SE_LAND,				// �v���C���[�̒��n��
	SOUND_LABEL_SE_ROLL,				// �}�O�X�g�[�����]���鉹
	SOUND_LABEL_SE_SHIFT,				// �΂����炷��
	SOUND_LABEL_SE_CRASH,				// �}�O�X�g�[�����ӂ��鉹
	SOUND_LABEL_SE_DAMAGE,				// �_���[�W��
	SOUND_LABEL_SE_VICTORY,				// �����̉�
	SOUND_LABEL_SE_RESULT,				// ���U���g�̉�
	SOUND_LABEL_SE_COLROCK,				// ��Ɣ��˔̔��ˉ�
	SOUND_LABEL_SE_ADDSCORE_000,		// �X�R�A���Z��
	SOUND_LABEL_SE_ADDSCORE_001,		// �X�R�A���Z��
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �T�E���h�̃t�F�[�h���
//*****************************************************************************
typedef enum
{
	FADESTATE_SOUND_NONE = 0,	//�������Ă��Ȃ����
	FADESTATE_SOUND_IN,	//�t�F�[�h�C�����
	FADESTATE_SOUND_OUT,	//�t�F�[�h�A�E�g���
	FADESTATE_SOUND_MAX
}FADESTATE_SOUND;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
void UpdateSound(void);

HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

void SetVolume(SOUND_LABEL label,float fVolume);
void SetFadeSound(SOUND_LABEL LabelNow, SOUND_LABEL LabelNext, float fSpeed);
#endif
