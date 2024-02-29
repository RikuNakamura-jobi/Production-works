//===========================
//
//ダイレクトX.pauseヘッダー
//Author:中村　陸
//
//===========================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//ゲームの状態
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,		//コンティニュー状態
	PAUSE_MENU_RETRY,				//リトライ状態
	PAUSE_MENU_QUIT,				//退出状態
	PAUSE_MENU_MAX
}PAUSE_MENU;

//プロトタイプ宣言
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void ResetPause(void);

#endif // !_PAUSE_H_