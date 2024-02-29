//===========================
//
//ダイレクトX.titleファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "title.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "effect.h"
#include "light.h"
#include "billboard.h"
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
#define NUM_TITLE (3)											//タイトル画面の数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureTitle[NUM_TITLE] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffTitle = NULL;					//頂点情報を格納
D3DXVECTOR3 g_posTitle;
int g_nCounterTitleState;										//状態管理カウンター
int nTimerRanking;												//タイトルのランキング遷移タイマー
float g_fCntTitle = 0.0f;										//タイトルの点滅用変数
bool g_TitleEnd;												//終了判定

//===========================
//タイトル画面の初期化処理
//===========================
void InitTitle(void)
{
	g_nCounterTitleState = 60;
	g_TitleEnd = false;

	InitCamera();
	InitLight();
	InitShadow();
	InitEffect();
	InitPlayerTitle();
	InitModel();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM000);
}

//===========================
//タイトル画面の終了処理
//===========================
void UninitTitle(void)
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
//タイトル画面の更新処理
//===========================
void UpdateTitle(void)
{
	FADE fade = GetFade();

	UpdateCameraTitle();
	UpdateLight();
	UpdateMeshField();
	UpdateMeshWall();
	UpdateMeshCeiling();
	UpdateSkyField();
	UpdateSkywall();
	UpdateSkyCeiling();
	UpdateModel();
	UpdateShadow();
	UpdateEffect();
	UpdateUi();

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetPadButtonTrigger(0) == true) && g_TitleEnd == false)
	{
		//サウンドの再生
		PlaySound(SOUND_LABEL_SE_START);

		g_TitleEnd = true;
	}

	if (g_TitleEnd == true)
	{
		g_nCounterTitleState--;

		if ((g_nCounterTitleState <= 0 && fade == FADE_NONE))
		{
			//モード設定
			SetFade(MODE_TUTORIAL);
		}
	}
}

//===========================
//タイトル画面の描画処理
//===========================
void DrawTitle(void)
{
	SetCamera();
	DrawMeshField();
	DrawMeshWall();
	DrawMeshCeiling();
	DrawSkyField();
	DrawSkywall();
	DrawSkyCeiling();
	DrawPlayer();
	DrawModel();
	DrawShadow();
	DrawEffect();
	DrawUi();
}