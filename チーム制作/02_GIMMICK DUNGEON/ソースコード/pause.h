//========================================
//
//2D�V���[�e�B���O�Q�[��(�|�[�Y����)[pause.h]
//Author:���v�ԗD��
//
//========================================
#ifndef _PAUSE_H_							//���̃}�N����`������Ă��Ȃ�������
#define _PAUSE_H_							//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�|�[�Y���j���[
typedef enum
{
	PAUSE_MENU_CONTENUE = 0,				//�Q�[���ɖ߂�
	PAUSE_MENU_RETRY,						//�Q�[������蒼��
	PAUSE_MENU_QUIT,						//�^�C�g����ʂɖ߂�
	PAUSE_MENU_MAX
}PAUSE_MENU;

//�v���g�^�C�v�錾
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

#endif
