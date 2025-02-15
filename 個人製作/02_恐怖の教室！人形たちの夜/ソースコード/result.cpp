//===========================
//
//ダイレクトX.resultファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "effect.h"
#include "light.h"
#include "ui.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshceiling.h"
#include "skyceiling.h"
#include "skyfield.h"
#include "skywall.h"
#include "model.h"
#include "sound.h"

//マクロ定義
#define NUM_RESULT (3)												//リザルト画面の数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureResult[NUM_RESULT] = {};				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffResult = NULL;						//頂点情報を格納
RESULT g_Result;
int g_nCounterTime;													//リザルト画面の継続時間

//===========================
//リザルト画面の初期化処理
//===========================
void InitResult(void)
{
	g_nCounterTime = 600;

	InitCamera();
	InitLight();
	InitShadow();
	InitEffect();
	InitPlayerResult();
	InitModel();

	if (g_Result == RESULT_CLEAR)
	{
		//サウンドの再生
		PlaySound(SOUND_LABEL_BGM005);
	}
	else if (g_Result == RESULT_OVER)
	{
		//サウンドの再生
		PlaySound(SOUND_LABEL_BGM006);
	}
}

//===========================
//リザルト画面の終了処理
//===========================
void UninitResult(void)
{
	//サウンドの停止
	StopSound();

	UninitCamera();
	UninitLight();
	UninitShadow();
	UninitEffect();
	UninitPlayer();
	UninitModel();
}

//===========================
//リザルト画面の更新処理
//===========================
void UpdateResult(void)
{
	FADE fade = GetFade();

	UpdateCamera();
	UpdateLight();
	UpdateMeshField();
	UpdateMeshWall();
	UpdateMeshCeiling();
	UpdateSkyField();
	UpdateSkywall();
	UpdateSkyCeiling();
	UpdateModel();
	UpdatePlayerResult();
	UpdateUi();
	UpdateShadow();
	UpdateEffect();

	g_nCounterTime--;

	if ((GetKeyboardTrigger(DIK_RETURN) == true && fade == FADE_NONE) || (g_nCounterTime <= 0 && fade == FADE_NONE) || (GetPadButtonTrigger(0) == true && fade == FADE_NONE))
	{
		//モード設定
		SetFade(MODE_TITLE);
	}
}

//===========================
//リザルト画面の描画処理
//===========================
void DrawResult(void)
{
	SetCamera();
	if (g_Result == RESULT_CLEAR)
	{
		DrawMeshField();
		DrawMeshWall();
		DrawMeshCeiling();
		DrawSkyField();
		DrawSkywall();
		DrawSkyCeiling();
		DrawModel();
		DrawEffect();
	}
	DrawPlayer();
	DrawUi();
}

//===========================
//リザルト画面の状態処理
//===========================
RESULT GetResult(void)
{
	return g_Result;
}

//===========================
//リザルト画面の状態処理
//===========================
void SetResult(RESULT Result)
{
	g_Result = Result;
}