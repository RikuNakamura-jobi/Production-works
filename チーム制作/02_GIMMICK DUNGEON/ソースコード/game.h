//========================================
//
//2Dアクションゲーム(ゲーム処理)[game.h]
//Author:佐久間優香
//
//========================================
#ifndef _GAME_H_							//このマクロ定義がされていなかったら
#define _GAME_H_							//2重インクルード防止のマクロを定義する

#include "main.h"

//ゲームの状態
typedef enum
{
	GAMESTATE_NONE = 0,						//何もしていない状態
	GAMESTATE_READY,					//カウントダウン状態
	GAMESTATE_NORMAL,						//通常状態
	GAMESTATE_END,							//終了状態
	GAMESTATE_RESULT,						//リザルト状態
	GAMESTATE_MAX
}GAMESTATE;

//=============================
//プロトタイプ宣言
//=============================
//基本処理
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

//設定処理
void SetGameState(GAMESTATE state, int nCounter);
void SetEnablePause(bool bPause);

//情報取得処理
bool GetPause(void);
GAMESTATE GetGameState(void);
bool GetPhotoMode(void);

#endif
