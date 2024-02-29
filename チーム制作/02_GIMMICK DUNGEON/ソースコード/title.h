//==========================================
//
//  タイトル画面(title.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _TITLE_H_
#define _TITLE_H_
#include "main.h"

//==========================================
//  マクロ定義
//==========================================
#define IN_TIME (180) //タイトル状態が切り替わるまでのタイマー

//==========================================
//  タイトル状態の列挙型定義
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
//  プロトタイプ宣言
//==========================================
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
int GetTitleState(void);

#endif
