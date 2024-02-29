//==========================================================
//
//ポーズ処理
//Author 奥定伊吹
//
//==========================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"	//main.hで定義しているものが必要なためinclude

//ポーズ状態列挙型
typedef enum
{
	PAUSEMENU_CONTINUE = 0,	//ゲームに戻る
	PAUSEMENU_RETRY,		//ゲームをやり直す
	PAUSEMENU_QUIT,			//タイトル画面に戻る
	PAUSEMENU_MAX
}PAUSEMENU;

//プロトタイプ宣言
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

#endif
