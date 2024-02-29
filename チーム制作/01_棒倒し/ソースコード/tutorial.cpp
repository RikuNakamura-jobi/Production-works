//===========================
//
//ダイレクトX.tutorialファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "tutorial.h"
#include "fade.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "billboard.h"
#include "meshfield.h"
#include "ui.h"
#include "pause.h"
#include "sound.h"
#include "model.h"

//グローバル変数宣言
TUTORIALSTATE g_TUTORIALSTATE = TUTORIALSTATE_NONE;								//ゲームの状態
int g_nStateCounterTutorial;

//===========================
//ゲーム画面の初期化処理
//===========================
void InitTutorial(void)
{
	//敵の初期化処理
	srand((unsigned int)time(0));

	//カメラの初期化
	InitCamera();

	//ライトの初期化
	InitLight();

	//ポーズの初期化
	InitPause();

	InitBillboard();

	SetPlayer();
	ResetModelTutorial();

	g_TUTORIALSTATE = TUTORIALSTATE_NORMAL;
	g_nStateCounterTutorial = 120;

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM002);
}

//===========================
//ゲーム画面の終了処理
//===========================
void UninitTutorial(void)
{
	//サウンドの停止
	StopSound();

	//カメラの初期化
	UninitCamera();

	//ライトの初期化
	UninitLight();

	//ポーズの初期化
	UninitPause();

	//ポリゴンの初期化
	UninitBillboard();
}

//===========================
//ゲーム画面の更新処理
//===========================
void UpdateTutorial(void)
{
	FADE fade = GetFade();

	//モデルの初期化
	UpdatePlayerTutorial();

	//カメラの初期化
	UpdateCamera();

	//ライトの初期化
	UpdateLight();

	//ポリゴンの初期化
	UpdateBillboard();

	//ポリゴンの初期化
	UpdateMeshField();

	//UIの初期化
	UpdateModel();

	//UIの初期化
	UpdateUi();

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetPadButtonTrigger(1) == true))
	{
		if (fade == FADE_NONE)
		{
			//モード設定
			SetFade(MODE_GAME);
		}
	}

	switch (g_TUTORIALSTATE)
	{
	case TUTORIALSTATE_NORMAL:
		break;
	case TUTORIALSTATE_END:
		g_nStateCounterTutorial--;
		if (g_nStateCounterTutorial <= 0 && fade == FADE_NONE)
		{
			SetFade(MODE_RESULT);
		}
		break;
	}
}

//===========================
//ゲーム画面の描画処理
//===========================
void DrawTutorial(void)
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
}

//===========================
//ゲーム画面状態取得処理
//===========================
void SetTUTORIALSTATE(TUTORIALSTATE state, int nCounter)
{
	g_TUTORIALSTATE = state;
}

//===========================
//ゲーム画面状態提供処理
//===========================
TUTORIALSTATE GetTUTORIALSTATE(void)
{
	return g_TUTORIALSTATE;
}