//===========================
//
//�_�C���N�gX.pause�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//�Q�[���̏��
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,		//�R���e�B�j���[���
	PAUSE_MENU_RETRY,				//���g���C���
	PAUSE_MENU_QUIT,				//�ޏo���
	PAUSE_MENU_MAX
}PAUSE_MENU;

//�v���g�^�C�v�錾
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void ResetPause(void);

#endif // !_PAUSE_H_