//===========================
//
//ダイレクトX.gameファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "game.h"
#include "fade.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "polygon.h"
#include "player.h"
#include "shadow.h"
#include "field.h"
#include "wall.h"
#include "billboard.h"
#include "bullet.h"
#include "effect.h"
#include "explosion.h"
#include "particle.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshceiling.h"
#include "skyceiling.h"
#include "skyfield.h"
#include "skywall.h"
#include "model.h"
#include "enemy.h"
#include "ui.h"
#include "pause.h"
#include "sound.h"

//グローバル変数宣言
bool g_bPause = false;												//ポーズ状態のON/OFF
GAMESTATE g_gameState = GAMESTATE_NONE;								//ゲームの状態
int g_nStateCounter;

//===========================
//ゲーム画面の初期化処理
//===========================
void InitGame(void)
{
	//敵の初期化処理
	srand((unsigned int)time(0));

	//カメラの初期化
	InitCamera();

	//ライトの初期化
	InitLight();

	//ポーズの初期化
	InitPause();

	//影の初期化
	InitShadow();

	//エフェクトの初期化
	InitEffect();

	//爆発の初期化
	InitExplosion();

	//パーティクルの初期化
	InitParticle();

	//弾の初期化
	InitBullet();

	//モデルの初期化
	InitPlayer();

	//モデルの初期化
	InitModel();

	InitEnemyGame();

	InitBillboard();

	g_gameState = GAMESTATE_NORMAL;
	g_nStateCounter = 120;
	g_bPause = false;

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM002);
}

//===========================
//ゲーム画面の終了処理
//===========================
void UninitGame(void)
{
	//サウンドの停止
	StopSound();

	//カメラの初期化
	UninitCamera();

	//ライトの初期化
	UninitLight();

	//ポーズの初期化
	UninitPause();

	//モデルの初期化
	UninitPlayer();

	//影の初期化
	UninitShadow();

	//エフェクトの初期化
	UninitEffect();

	//爆発の初期化
	UninitExplosion();

	//パーティクルの初期化
	UninitParticle();

	//ポリゴンの初期化
	UninitBillboard();

	//弾の初期化
	UninitBullet();

	//モデルの初期化
	UninitModel();

	

	UninitBillboard();
}

//===========================
//ゲーム画面の更新処理
//===========================
void UpdateGame(void)
{
	FADE fade = GetFade();

	if (g_bPause == true)
	{
		//ポーズの初期化
		UpdatePause();
	}
	else
	{
		//カメラの初期化
		UpdateCamera();

		//ライトの初期化
		UpdateLight();

		//モデルの初期化
		UpdatePlayer();

		//影の初期化
		UpdateShadow();

		//エフェクトの初期化
		UpdateEffect();

		//爆発の初期化
		UpdateExplosion();

		//パーティクルの初期化
		UpdateParticle();

		//ポリゴンの初期化
		UpdateBillboard();

		//ポリゴンの初期化
		UpdateMeshField();

		//ポリゴンの初期化
		UpdateMeshWall();

		//ポリゴンの初期化
		UpdateMeshCeiling();

		//ポリゴンの初期化
		UpdateSkyField();

		//ポリゴンの初期化
		UpdateSkywall();

		//ポリゴンの初期化
		UpdateSkyCeiling();

		//弾の初期化
		UpdateBullet();

		//モデルの初期化
		UpdateModel();

		//モデルの初期化
		UpdateEnemy();

		//UIの初期化
		UpdateUi();
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
	case GAMESTATE_BOSS:
		break;
	case GAMESTATE_BOSSBATTLE:
		break;
	case GAMESTATE_END:
		g_nStateCounter--;
		if (g_nStateCounter <= 0 && fade == FADE_NONE)
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
	//カメラの設定
	SetCamera();

	//ポリゴンの初期化
	DrawMeshField();

	//ポリゴンの初期化
	DrawMeshWall();

	//ポリゴンの初期化
	DrawMeshCeiling();

	//ポリゴンの初期化
	DrawSkyField();

	//ポリゴンの初期化
	DrawSkywall();

	//ポリゴンの初期化
	DrawSkyCeiling();

	//ポリゴンの初期化
	DrawBillboard();

	//影の初期化
	DrawShadow();

	//エフェクトの初期化
	DrawEffect();

	//パーティクルの初期化
	DrawParticle();

	//爆発の初期化
	DrawExplosion();

	//弾の初期化
	DrawBullet();

	//モデルの初期化
	DrawPlayer();

	//モデルの初期化
	DrawModel();

	//モデルの初期化
	DrawEnemy();

	//UIの初期化
	DrawUi();

	if (g_bPause == true)
	{
		//ポーズの初期化
		DrawPause();
	}
}

//===========================
//ゲーム画面状態取得処理
//===========================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
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