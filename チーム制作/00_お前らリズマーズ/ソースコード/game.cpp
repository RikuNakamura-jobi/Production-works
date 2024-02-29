//===========================
//
//ダイレクトX.gameファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "game.h"
#include "player.h"
#include "bg.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "fade.h"
#include "effect.h"
#include "particle.h"
#include "warning.h"
#include "pause.h"
#include "bullet.h"
#include "area.h"

//グローバル変数宣言
bool g_bPause = false;												//ポーズ状態のON/OFF
GAMESTATE g_gameState = GAMESTATE_NONE;								//ゲームの状態
int g_nCounterGameState = 0;										//状態管理カウンター
bool g_bEnemySpawn;													//全滅確認

//===========================
//ゲーム画面の初期化処理
//===========================
void InitGame(void)
{
	Enemy aENEMY;

	//各種オブジェクトの初期化処理
	InitExplosion();
	InitPlayer();
	InitBg();
	InitScore();
	SetScore(0);
	InitEffect();
	InitParticle();
	InitBullet();
	InitArea();

	//敵の初期化処理
	srand((unsigned int)time(0));

	InitEnemy();

	SetEnemy(D3DXVECTOR3(100.0f, 550.0f, 0.0f), 0, 0, 0);

	g_gameState = GAMESTATE_NORMAL;
	g_nCounterGameState = 0;
	g_bPause = false;
	g_bEnemySpawn = false;

	PlaySound(SOUND_LABEL_BGM015);
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
	UninitBg();
	UninitExplosion();
	UninitEnemy();
	UninitScore();
	UninitEffect();
	UninitParticle();
	UninitBullet();
	UninitPause();
	UninitArea();
}

//===========================
//ゲーム画面の更新処理
//===========================
void UpdateGame(void)
{
	Enemy aENEMY;
	Player *pPlayer = GetPlayer();
	FADE fade = GetFade();
	D3DXVECTOR3 posBullet;
	D3DXVECTOR3 moveBullet;
	D3DXVECTOR3 rotBullet;
	int nNumEnemy = GetNumEnemy();

	if (g_bPause == false)
	{
		//各種オブジェクトの更新処理
		UpdateExplosion();
		UpdatePlayer();
		UpdateBg();
		UpdateEnemy();
		UpdateScore();
		UpdateEffect();
		UpdateParticle();
		UpdateBullet();
		UpdateArea();
	}
	else if (g_bPause == true)
	{
		UpdatePause();
	}

	//ポーズ
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		InitPause();
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
	DrawEnemy();
	DrawPlayer();
	DrawArea();
	DrawBullet();
	DrawExplosion();

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
//ゲーム画面状態提供処理
//===========================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//===========================
//ポーズ状態変更処理
//===========================
void SetEnablePause(bool pPause)
{
	g_bPause = pPause;
}