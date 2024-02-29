//==========================================
//
//  ���j���[���(menu.h)
//  Author : Kanazaki Tomoya
//
//==========================================
#ifndef _MENU_H_
#define _MENU_H_
#include "main.h"

//==========================================
//  ���j���[��Ԃ̗񋓌^��`
//==========================================
typedef enum
{
	MENUSTATE_START = 0,
	MENUSTATE_TUTORIAL,
	MENUSTATE_QUIT,
	MENUSTATE_FADE,
	MENUSTATE_MAX
}MENUSTATE;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitMenu(void);
void UninitMenu(void);
void UpdateMenu(void);
void DrawMenu(void);
int GetMenuState(void);
bool GetEnterFrag(void);

#endif
