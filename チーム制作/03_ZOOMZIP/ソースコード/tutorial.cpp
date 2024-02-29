//==========================================================
//
//チュートリアル画面処理 [tutorial.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "polygon.h"

//==========================================================
//グローバル変数
//==========================================================

//==========================================================
//チュートリアル画面の初期化処理
//==========================================================
void InitTutorial(void)
{
	InitPolygon();

	PlaySound(SOUND_LABEL_TUTORIAL);	//BGMの再生
}

//==========================================================
//チュートリアル画面の終了処理
//==========================================================
void UninitTutorial(void)
{
	UninitPolygon();

	StopSound();	//BGMの停止
}

//==========================================================
//チュートリアル画面の更新処理
//==========================================================
void UpdateTutorial(void)
{
	UpdatePolygon();

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true || 
		GetGamepadTrigger(BUTTON_A, 0) == true || GetGamepadTrigger(BUTTON_START, 0) == true)
	{//遷移キーが押された
		SetFade(MODE_GAME);
		PlaySound(SOUND_LABEL_SEBUTTON);
	}
}

//==========================================================
//チュートリアル画面の描画処理
//==========================================================
void DrawTutorial(void)
{
	DrawPolygon();
}