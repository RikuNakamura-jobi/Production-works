//===================================================
//
//ゲーム画面処理 [game.h]
//Author Ibuki Okusda
//
//===================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//===================================================
//マクロ定義
//===================================================
#define GAMESTOP_COUNT	(100)		//ゲーム終了までのカウント

//===================================================
//列挙型を定義
//===================================================
//ゲームの状態
typedef enum
{
	GAMESTATE_NONE = 0,			//何もない状態
	GAMESTATE_NORMAL,			//通常状態
	GAMESTATE_END,				//終了状態
	GAMESTATE_MAX
}GAMESTATE;

//===================================================
//プロトタイプ宣言
//===================================================
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state, int nCounter);
GAMESTATE GetGameState(void);
void SetEnablePause(bool bPause);

#endif
