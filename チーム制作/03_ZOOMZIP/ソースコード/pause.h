//==========================================================
//
//�|�[�Y����
//Author ����ɐ�
//
//==========================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"	//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include

//�|�[�Y��ԗ񋓌^
typedef enum
{
	PAUSEMENU_CONTINUE = 0,	//�Q�[���ɖ߂�
	PAUSEMENU_RETRY,		//�Q�[������蒼��
	PAUSEMENU_QUIT,			//�^�C�g����ʂɖ߂�
	PAUSEMENU_MAX
}PAUSEMENU;

//�v���g�^�C�v�錾
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

#endif
