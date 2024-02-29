//========================================
//
//2Dシューティングゲーム(ポーズ処理)[pause.h]
//Author:佐久間優香
//
//========================================
#ifndef _PAUSE_H_							//このマクロ定義がされていなかったら
#define _PAUSE_H_							//2重インクルード防止のマクロを定義する

#include "main.h"

//ポーズメニュー
typedef enum
{
	PAUSE_MENU_CONTENUE = 0,				//ゲームに戻る
	PAUSE_MENU_RETRY,						//ゲームをやり直す
	PAUSE_MENU_QUIT,						//タイトル画面に戻る
	PAUSE_MENU_MAX
}PAUSE_MENU;

//プロトタイプ宣言
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

#endif
