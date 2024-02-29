//==========================================
//
//  �^�C�g�����(title.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _TITLE_H_
#define _TITLE_H_
#include "main.h"

//==========================================
//  �}�N����`
//==========================================
#define IN_TIME (180) //�^�C�g����Ԃ��؂�ւ��܂ł̃^�C�}�[

//==========================================
//  �^�C�g����Ԃ̗񋓌^��`
//==========================================
typedef enum
{
	TITLESTATE_IN = 0,
	TITLESTATE_NORMAL,
	TITLESTATE_OUT,
	TITLESTATE_MENU,
	TITLESTATE_MAX
}TITLESTATE;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
int GetTitleState(void);

#endif
