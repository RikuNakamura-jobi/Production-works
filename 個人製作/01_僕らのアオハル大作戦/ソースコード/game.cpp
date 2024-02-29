//===========================
//
//ダイレクトX.gameファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "game.h"
#include "player.h"
#include "input.h"
#include "fade.h"
#include "warning.h"
#include "block.h"
#include "effect.h"
#include "particle.h"
#include "stage.h"
#include "start.h"
#include "goal.h"
#include "pause.h"
#include "enemy.h"
#include "coin.h"
#include "stagestart.h"
#include "boss.h"
#include "life.h"
#include "bg.h"
#include "boost.h"
#include "sound.h"

//グローバル変数宣言
int g_nCounterGameState;
bool g_bPause = false;												//ポーズ状態のON/OFF
GAMESTATE g_gameState = GAMESTATE_NONE;								//ゲームの状態

//===========================
//ゲーム画面の初期化処理
//===========================
void InitGame(void)
{
	g_nCounterGameState = 120;
	g_gameState = GAMESTATE_NONE;

	//各種オブジェクトの初期化処理
	InitLife(); 
	InitBoost(); 
	InitPlayer();
	InitBlock();
	InitEffect();
	InitParticle();
	InitStart();
	InitGoal();
	InitCoin();
	InitEnemy();
	InitPause();
	InitBoss();
	InitBg();
	InitStagestart();
	SetStagestart();

	//ステージ配置
	InitStage();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM014);
}

//===========================
//ゲーム画面の終了処理
//===========================
void UninitGame(void)
{
	//サウンドの停止
	StopSound();

	//各種オブジェクトの終了処理
	UninitPlayer();
	UninitBlock();
	UninitEffect();
	UninitParticle();
	UninitStart();
	UninitGoal();
	UninitPause();
	UninitEnemy();
	UninitCoin();
	UninitBoss();
	UninitLife();
	UninitBg();
	UninitBoost();
	UninitStagestart();
}

//===========================
//ゲーム画面の更新処理
//===========================
void UpdateGame(void)
{
	FADE fade = GetFade();
	Player *pPlayer = GetPlayer();

	if (g_bPause == false)
	{
		//各種オブジェクトの更新処理
		UpdatePlayer();
		UpdateBlock();
		UpdateEffect();
		UpdateParticle();
		UpdateStart();
		UpdateGoal();
		UpdateEnemy();
		UpdateCoin();
		UpdateBoss();
		UpdateLife();
		UpdateBg();
		UpdateBoost();
		UpdateStagestart();
	}
	else if (g_bPause == true)
	{
		UpdatePause();
	}

	//ポーズ
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:
		break;
	case GAMESTATE_END:
		g_nCounterGameState--;
		if (g_nCounterGameState <= 0 && fade == FADE_NONE)
		{
			SetFade(MODE_RESULT);
		}
		break;
	}
}

//===========================
//ゲーム画面の描画処理
//===========================
void DrawGame(void)
{
	//各種オブジェクトの描画処理
	DrawBg();
	DrawEffect();
	DrawParticle();
	DrawStart();
	DrawGoal();
	DrawEnemy();
	DrawPlayer();
	DrawBlock();
	DrawBoss();
	DrawCoin();
	DrawLife();
	DrawBoost();
	DrawStagestart();

	if (g_bPause == true)
	{
		DrawPause();
	}
}

//===========================
//ゲーム画面状態取得処理
//===========================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
	g_nCounterGameState = nCounter;
}

//===========================
//ポーズ状態変更処理
//===========================
void SetEnablePause(bool pPause)
{
	g_bPause = pPause;
}