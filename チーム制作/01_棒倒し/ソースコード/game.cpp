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
#include "player.h"
#include "billboard.h"
#include "meshfield.h"
#include "meshwall.h"
#include "ui.h"
#include "pause.h"
#include "sound.h"
#include "model.h"
#include "time.h"

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

	//ポーズの初期化
	InitTime();

	InitBillboard();

	SetPlayer();
	ResetModel();

	g_gameState = GAMESTATE_NORMAL;
	g_nStateCounter = 120;
	g_bPause = false;

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM001);
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

	//ポーズの初期化
	UninitTime();

	//ポリゴンの初期化
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
		//モデルの初期化
		UpdatePlayer();

		//カメラの初期化
		UpdateCamera();

		//ライトの初期化
		UpdateLight();

		//ポーズの初期化
		UpdateTime();

		//ポリゴンの初期化
		UpdateBillboard();

		//ポリゴンの初期化
		UpdateMeshField();

		//UIの初期化
		UpdateUi();
	}

	//ポーズ
	if (GetKeyboardTrigger(DIK_P) == true || GetPadButtonTrigger(7) == true)
	{
		ResetPause();
		g_bPause = g_bPause ? false : true;
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:
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
	//DrawMeshWall();

	//ポリゴンの初期化
	//DrawBillboard();

	//モデルの初期化
	DrawPlayer();

	//モデルの初期化
	DrawModel();

	//UIの初期化
	DrawUi();

	//ポーズの初期化
	DrawTime();

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