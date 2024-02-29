//===========================
//
//ダイレクトX.gameヘッダー
//Author:中村　陸
//
//===========================
#ifndef _GAME_H_
#define _GAME_H_

//ゲームの状態
typedef enum
{
	GAMESTATE_NONE = 0,				//何もしてない状態
	GAMESTATE_NORMAL,				//通常状態
	GAMESTATE_END,					//終了状態
	GAMESTATE_MAX
}GAMESTATE;

//プロトタイプ宣言
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state, int nCounter);
void SetEnablePause(bool pPause);

#endif // !_GAME_H_