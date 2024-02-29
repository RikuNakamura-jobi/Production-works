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
#include "sound.h"

//グローバル変数宣言
bool g_bTutorial = false;												//ポーズ状態のON/OFF
int g_nTutorialCounter;

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

	InitBillboard();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM001);

	g_bTutorial = false;
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
	UninitField();

	//弾の初期化
	UninitBullet();

	//モデルの初期化
	UninitModel();

	UninitBillboard();
}

//===========================
//ゲーム画面の更新処理
//===========================
void UpdateTutorial(void)
{
	FADE fade = GetFade();

	//カメラの初期化
	UpdateCamera();

	//ライトの初期化
	UpdateLight();

	//モデルの初期化
	UpdatePlayer();

	//ポリゴンの初期化
	UpdateBillboard();

	//影の初期化
	UpdateShadow();

	//エフェクトの初期化
	UpdateEffect();

	//爆発の初期化
	UpdateExplosion();

	//パーティクルの初期化
	UpdateParticle();

	//ポリゴンの初期化
	UpdateField();

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

	//UIの初期化
	UpdateUi();

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetPadButtonTrigger(0) == true) && g_bTutorial == false)
	{
		g_bTutorial = true;
	}

	if (g_bTutorial == true)
	{
		g_nTutorialCounter--;

		if ((g_nTutorialCounter <= 0 && fade == FADE_NONE))
		{
			//モード設定
			SetFade(MODE_GAME);
		}
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

	//UIの初期化
	DrawUi();
}