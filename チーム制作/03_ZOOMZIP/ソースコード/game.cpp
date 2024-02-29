//===================================================
//
//ゲーム画面処理 [game.cpp]
//Author Ibuki Okusada
//
//===================================================
#include "game.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "fileload.h"
#include "texture.h"
#include "meshfield.h"
#include "meshwall.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "score.h"
#include "polygon.h"
#include "enemy.h"
#include "item.h"
#include "high_score.h"
#include "cloud.h"
#include "start.h"

//===================================================
//プロトタイプ宣言
//===================================================
void InitGameMain(void);
void UpdateGameMain(void);
void DrawGameMain(void);

//===================================================
//グローバル変数宣言
//===================================================
GAMESTATE g_gameState = GAMESTATE_NONE;		//ゲームの状態
int g_nCounterGameState;	//状態管理カウンター
bool g_bPause = false;		//ポーズ状態のON/OFF
bool g_bEdit = false;		//エディットのON/OFF

//===================================================
//ゲーム画面の初期化処理
//===================================================
void InitGame(void)
{
	//ゲームプレイ画面の初期化処理
	InitGameMain();

	g_gameState = GAMESTATE_NONE;	//通常状態に設定
	g_nCounterGameState = 0;		//カウンターを初期化
	g_bPause = false;				//ポーズ状態初期化
	g_bEdit = false;

	PlaySound(SOUND_LABEL_GAME);
}

//===================================================
//ゲームプレイ画面の初期化処理
//===================================================
void InitGameMain(void)
{
	//プレイヤーの初期化
	InitPlayer();

	//エフェクトの初期化
	InitEffect();

	//パーティクルの初期化
	InitParticle();

	//スコアの初期化
	InitScore();

	//ハイスコアの初期化
	InitHighScore();

	//ポリゴンの初期化
	InitPolygon();

	//エネミーの初期化処理
	InitEnemy();

	//アイテムの初期化処理
	InitItem();

	//雲の初期化処理
	InitCloud();

	//スタートの初期化処理
	InitStart();
}

//===================================================
//ゲーム画面の終了処理
//===================================================
void UninitGame(void)
{
	StopSound();	//BGMの停止

	//プレイヤーの終了処理
	UninitPlayer();

	//エフェクトの終了処理
	UninitEffect();

	//スコアの終了処理
	UninitScore();

	//ハイスコアの終了処理
	UninitHighScore();

	//ポリゴンの終了処理
	UninitPolygon();

	//エネミーの終了処理
	UninitEnemy();

	//アイテムの終了処理
	UninitItem();

	//雲の終了
	UninitCloud();

	//スタートの終了
	UninitStart();
}

//===================================================
//ゲーム画面の更新処理
//===================================================
void UpdateGame(void)
{
	switch (g_gameState)
	{
	case GAMESTATE_NONE:
		//ポリゴンの更新処理
		UpdatePolygon();

		//
		UpdateCloud();

		//スタートの更新処理
		UpdateStart();
		break;
	case GAMESTATE_NORMAL:
		UpdateGameMain();
		break;
	case GAMESTATE_END:
		//ポリゴンの更新処理
		UpdatePolygon();

		//
		UpdateCloud();

		//スタートの更新処理
		UpdateHighScore();
		break;
	}
}

//===================================================
//ゲームプレイ画面更新処理
//===================================================
void UpdateGameMain(void)
{
	//プレイヤーの更新処理
	UpdatePlayer();

	//エフェクトの更新処理
	UpdateEffect();

	//スコアの更新処理
	UpdateScore();

	//ハイスコアの更新処理
	UpdateHighScore();

	//ポリゴンの更新処理
	UpdatePolygon();

	//エネミーの更新処理
	UpdateEnemy();

	//アイテムの更新処理
	UpdateItem();

	//雲の更新処理
	UpdateCloud();
}	

//===================================================
//ゲーム画面描画処理
//===================================================
void DrawGame(void)
{	
	//メイン描画
	DrawGameMain();
}

//===================================================
//ゲーム状態設定
//===================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
	g_nCounterGameState = nCounter;
}

//===================================================
//ゲーム状態取得
//===================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//===================================================
//ポーズ状態設定
//===================================================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause ? false: true;
}

//===================================================
//通常描画　
//===================================================
void DrawGameMain(void)
{
	//ポリゴンの描画処理
	DrawPolygon();

	//プレイヤーの描画処理
	DrawPlayer();

	//エフェクトの描画処理
	DrawEffect();

	//エネミーの描画処理
	DrawEnemy();

	//雲の描画処理
	DrawCloud();

	//スコアの描画処理
	DrawScore();

	//アイテムの描画処理
	DrawItem();

	if (g_gameState == GAMESTATE_NONE)
	{
		//スタートの描画
		DrawStart();
	}

	if (g_gameState == GAMESTATE_END)
	{
		//ハイスコアの描画処理
 		DrawHighScore();
	}

}
