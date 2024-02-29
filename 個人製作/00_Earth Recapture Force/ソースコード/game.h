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

//waveの状態
typedef enum
{
	WAVESTATE_NONE = 0,				//何もしてない状態
	WAVESTATE_NORMAL,				//wave1状態
	WAVESTATE_BOSS,					//boss状態
	WAVESTATE_END,					//boss状態
	WAVESTATE_MAX
}WAVESTATE;

//ステージの状態
typedef enum
{
	STAGESTATE_NONE = 0,			//何もしてない状態
	STAGESTATE_1,					//STAGE1状態
	STAGESTATE_2,					//STAGE2状態
	STAGESTATE_3,					//STAGE3状態
	STAGESTATE_EX,					//STAGE3状態
	STAGESTATE_MAX
}STAGESTATE;

//プロトタイプ宣言
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state, int nCounter);
GAMESTATE GetGameState(void);
void SetWaveState(WAVESTATE state, int nCounter);
WAVESTATE GetWaveState(void);
void SetStageState(STAGESTATE state, int nCounter);
STAGESTATE GetStageState(void);
void SetEnablePause(bool pPause);
bool GetHardMode(void);
bool SetNoContinue(bool bNoContinue);

#endif // !_GAME_H_