//========================================
//
//2Dアクションゲーム(ゲーム処理)[game.cpp]
//Author:佐久間優香
//
//========================================
#include "game.h"
#include "input.h"
#include "fade.h"
#include "meshfield.h"
#include "object.h"
#include "pause.h"
#include "camera.h"
#include "light.h"
#include "ceiling.h"
#include "player.h"
#include "edit.h"
#include "enemy.h"
#include "rock.h"
#include "wall.h"
#include "rock.h"
#include "wall.h"
#include "billboard.h"
#include "shock.h"
#include "result.h"
#include "time.h"
#include "fog.h"
#include "gauge.h"
#include "score.h"
#include "effect.h"
#include "particle.h"
#include "ranking.h"
#include "shadow.h"
#include "orbit.h"
#include "countdown.h"
#include "sound.h"
#include "point.h"
#include "point_log.h"

//====================
// マクロ定義
//====================
#define NUM_GAME0			(0)
#define NUM_GAME1			(1)
#define NUM_GAME2			(2)
#define COUNT_END			(300)	//遷移猶予
#define TIMESCORE_MLT		(500)	//勝利時、残り時間に掛ける値
#define RESULT_SCORE_POS	(D3DXVECTOR3(300.0f, 450.0f, 0.0f))	//リザルト時スコア座標
#define RESULT_POINT_POS	(D3DXVECTOR3(350.0f, 600.0f, 0.0f))	//リザルト時ポイント座標

//====================
// グローバル変数宣言
//====================
GAMESTATE g_gameState;							//ゲーム状態
int g_nCounterGameState;									//状態管理カウンター
bool g_bPause;											//ポーズ状態のON/OFF
bool g_bEdit;											//エディット状態のON/OFF
float g_fStart;	//フォグの開始距離
float g_fEnd;	//フォグの終了距離
int g_nEndCnter;	//リザルトまでの猶予
bool g_bStopEnemy;	//敵を止めるかどうか
bool g_bPhoto;	//フォトモードかどうか
bool g_bDebug;	//デバッグ表示をするかどうか

//====================
// プロトタイプ宣言
//====================
void DebugFOG(void);
void SetGameEnd(RESULTTYPE result);

//========================================
//ゲームの初期化処理
//========================================
void InitGame(void)
{
	//カウントダウンの初期化
	InitCountdown();

	//影の初期化
	InitShadow();

	//ゲージの初期化
	InitGauge();

	//ビルボードの初期化処理
	InitBillboard();

	//カメラの初期化処理
	InitCamera();

	//ライトの初期化処理
	InitLight();

	//オブジェクトの初期化
	InitObject();

	//メッシュフィールドの初期化
	InitMeshField();

	//天井の初期化処理
	InitCeiling();

	//壁の初期化処理
	InitWall();

	//岩の初期化処理
	InitRock();

	//プレイヤーの初期化処理
	InitPlayer();

	//エネミーの初期化
	InitEnemy();
	if (GetMode() == MODE_GAME)
	{
		SetEnemy(D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
		SetPositionShadow(GetEnemy()->nIdxGauge, GetEnemy()->pos, GetEnemy()->rot, 1.0f);
	}

	//衝撃波の初期化
	InitShock();

	//軌跡の初期化
	InitOrbit();

	//エフェクトの初期化処理
	InitEffect();

	//パーティクルの初期化処理
	InitParticle();

	//スコアの初期化
	InitScore();

	//タイマーの初期化
	InitTime();

	//ポーズの初期化処理
	InitPause();

	//エディットの初期化処理
	InitEdit();
	LoadEdit();

	//取得スコア表示の初期化
	InitPoint();

	//取得スコアメッセージの初期化
	InitPointLog();

	if (GetMode() == MODE_GAME)
	{
		//サウンド再生 =
		PlaySound(SOUND_LABEL_BGM001);
	}

	//ゲームシーン以外での状態を設定
	MODE mode = GetMode();

	switch (mode)
	{
	case MODE_TITLE:
	case MODE_MENU:
		g_gameState = GAMESTATE_NONE;
		break;
	case MODE_GAME:
		g_gameState = GAMESTATE_READY;
		break;
	default:
		g_gameState = GAMESTATE_NORMAL;		//通常状態に設定
		break;
	}

	g_nCounterGameState = 0;
	g_bPause = false;
	g_bEdit = false;
	g_nEndCnter = COUNT_END;
	g_bStopEnemy = false;
	g_bPhoto = false;

	g_fStart = 1.0f;
	g_fEnd = 3000.0f;

	//フォグポインタ取得
	FOGDATA *pFog = GetFog();

	switch (GetMode())
	{//フォグの設定
	case MODE_TITLE:
		//色設定
		pFog->col = FOG_COLOR_BLACK;

		//終了・開始距離設定
		pFog->fEnd = FOG_END_TITLE;
		pFog->fStart = FOG_START_TITLE;

		break;
	case MODE_GAME:
		//色設定
		pFog->col = FOG_COLOR_SAND;

		//終了・開始距離設定
		pFog->fEnd = FOG_END_GAME;
		pFog->fStart = FOG_START_GAME;

		break;
	case MODE_TUTORIAL:
		//色設定
		pFog->col = FOG_COLOR_SAND;

		//終了・開始距離設定
		pFog->fEnd = FOG_END_GAME;
		pFog->fStart = FOG_START_GAME;

		break;
	default:
		break;
	}
}

//========================================
//ゲームの終了処理
//========================================
void UninitGame(void)
{
	if (GetMode() == MODE_GAME)
	{
		//サウンドの停止
		StopSound();
	}

	//影の終了処理
	UninitShadow();

	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	//メッシュフィールドの終了処理
	UninitMeshField();

	//天井の終了処理
	UninitCeiling();

	//壁の終了処理
	UninitWall();

	//ビルボードの終了処理
	UninitBillboard();

	//エフェクトの終了処理
	UninitEffect();

	//パーティクルの終了処理
	UninitParticle();

	//プレイヤーの終了処理
	UninitPlayer();

	//岩の終了処理
	UninitRock();

	//オブジェクトの終了処理
	UninitObject();

	//衝撃波の終了処理
	UninitShock();

	//エネミーの終了処理
	UninitEnemy();

	if (GetMode() != MODE_TUTORIAL)
	{
		//カウントダウンの終了処理
		UninitCountdown();
	}

	//ポーズの終了処理
	UninitPause();

	//タイマーの終了
	UninitTime();

	//リザルトの終了
	UninitResult();

	//ゲージの終了処理
	UninitGauge();

	//スコアの終了
	UninitScore();

	//軌跡の終了
	UninitOrbit();

	//取得スコアの表示の終了
	UninitPoint();

	//取得スコアメッセージの終了
	UninitPointLog();
}

//========================================
//ゲームの更新処理
//========================================
void UpdateGame(void)
{
	if (g_gameState == GAMESTATE_NORMAL || g_gameState == GAMESTATE_END || g_gameState == GAMESTATE_READY)
	{//ゲーム中の処理
		if (g_bPause == false)
		{
			if (g_gameState == GAMESTATE_READY)
			{
				//カウントダウンの更新
				UpdateCountdown();
			}

			//カメラの更新処理
			UpdateCamera();

			if (g_bEdit == false && g_bPhoto == false)
			{
				//ライトの更新処理
				UpdateLight();

				//メッシュフィールドの更新処理
				UpdateMeshField();

				//天井の更新処理
				UpdateCeiling();

				//壁の更新処理
				UpdateWall();

				//ビルボードの更新処理
				UpdateBillboard();

				//エフェクトの更新処理
				UpdateEffect();

				//パーティクルの更新処理
				UpdateParticle();

				//プレイヤーの更新処理
				UpdatePlayer();

				//影更新処理
				UpdateShadow();

				if (g_gameState != GAMESTATE_READY && g_bStopEnemy == false)
				{
					//エネミーの更新処理
					UpdateEnemy();
				}

				//岩の更新処理
				UpdateRock();

				//オブジェクトの更新処理
				UpdateObject();

				//衝撃波の更新処理
				UpdateShock();

				if (GetMode() != MODE_TUTORIAL)
				{
					if (g_gameState != GAMESTATE_READY)
					{
						//タイマーの更新処理
						UpdateTime();
					}
				}

				//ゲージの更新処理
				UpdateGauge();

				//スコアの更新
				UpdateScore();

				//軌跡の更新
				UpdateOrbit();

				//取得スコアの更新
				UpdatePoint();

				//取得スコアメッセージの更新
				UpdatePointLog();
			}
			else
			{
				//エディットの更新処理
				UpdateEdit();
			}
		}

		//エディットモードの有無
#ifdef _DEBUG
		if (GetKeyboardTrigger(DIK_F8) == true)
		{
			g_bEdit = (g_bEdit == false) ? true : false;
		}

		if (GetKeyboardTrigger(DIK_BACK) == true)
		{//敵更新切り替え
			g_bStopEnemy = (g_bStopEnemy == false) ? true : false;
		}

		if (GetKeyboardTrigger(DIK_RCONTROL) == true)
		{//フォトモード切り替え
			g_bPhoto = (g_bPhoto == false) ? true : false;
		}
#endif //_DEBUG

		//勝敗設定処理
		if (g_nEndCnter == COUNT_END)
		{
			if (GetEnemy()->nLife == 0)
			{//敵が倒れたら
				SetGameEnd(RESULTTYPE_WIN);
				GameEndCamera(RESULTTYPE_WIN);

				//勝利の音再生
				PlaySound(SOUND_LABEL_SE_VICTORY);
			}
			if (GetPlayer()->bUse == false)
			{//プレイヤーが死んだら
				SetGameEnd(RESULTTYPE_LOSE);
				GameEndCamera(RESULTTYPE_LOSE);
				g_nEndCnter -= 100;
			}
			if (GetTimeUp() == true)
			{//時間切れになったら
				SetGameEnd(RESULTTYPE_LOSE);
				GameEndCamera(RESULTTYPE_LOSE);
				g_nEndCnter -= 100;
			}
		}

		//リザルト状態へ移行
		if (g_nEndCnter == 0)
		{//タイマーが切れたら
			g_gameState = GAMESTATE_RESULT;

			if (GetResult() == RESULTTYPE_WIN)
			{//リザルト時のタイムボーナスの位置
				SetPointScore(GetTime() * TIMESCORE_MLT, RESULT_POINT_POS, RESULT_POINT_POS, POINTTYPE_STAY, 0.5f, LOGTYPE_TIMEBONUS);
			}

			//リザルト時の合計スコアの位置
			SetPointScore(GetScore(), RESULT_SCORE_POS, RESULT_SCORE_POS, POINTTYPE_STAY, 1.0f, LOGTYPE_TOTALSCORE);
		}

		//リザルトへの遷移待ち
		if (g_nEndCnter > 0 && g_gameState == GAMESTATE_END)
		{
			g_nEndCnter--;
		}
		else
		{
			if (g_bPause == true)
			{
				//ポーズの更新処理
				UpdatePause();
			}

			if (GetMode() != MODE_TUTORIAL && g_gameState != GAMESTATE_READY)
			{
				if (GetKeyboardTrigger(DIK_P) == true || GetJoyPadButtonTrigger(KEY_START, 0))
				{//ポーズ切り替え
					g_bPause = (g_bPause == false) ? true : false;
				}
			}
		}
	}

	switch (g_gameState)
	{
	case GAMESTATE_RESULT:
		UpdateResult();

		//タイマーの更新処理
		UpdateTimeResult();

		//スコアの更新
		//UpdateScore();

		//取得スコアの更新
		UpdatePoint();

		//取得スコアメッセージの更新
		UpdatePointLog();
		break;
	}
}

//========================================
//ゲームの描画処理
//========================================
void DrawGame(void)
{
	//カメラの設定処理
	SetCamera();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//メッシュフィールドの描画処理
	DrawMeshField();

	//天井の描画処理
	DrawCeiling();

	//壁の描画処理
	DrawWall();

	//岩の描画処理
	DrawRock();

	//オブジェクトの描画処理
	DrawObject();

	//パーティクルの描画処理
	DrawParticle();

	//ビルボードの描画処理
	DrawBillboard();

	//衝撃波の描画処理
	DrawShock();

	if (GetMode() != MODE_TITLE)
	{
		//モデルの描画処理
		DrawPlayer();

		//エネミーの描画処理
		DrawEnemy();
	}

	//影描画処理
	DrawShadow();

	if (g_gameState == GAMESTATE_READY)
	{
		//カウントダウンの描画
		DrawCountdown();
	}

	//軌跡の描画
	DrawOrbit();

	//エフェクトの描画処理
	DrawEffect();

	if (GetMode() != MODE_TITLE)
	{
		if ((GetMode() == MODE_GAME && g_gameState == GAMESTATE_NORMAL) && GetPhotoMode() == false)
		{
			//ゲージの描画
			DrawGauge();

			//タイマーの描画処理
			DrawTime();

			//スコアの描画
			DrawScore();

			//取得スコアの描画
			DrawPoint();

			//取得スコアメッセージの描画
			DrawPointLog();
		}
	}

	if (GetMode() == MODE_TUTORIAL)
	{//チュートリアル用の描画
		//ゲージの描画
		DrawGauge();
	}

	if (g_bPause == true)
	{
		DrawPause();
	}

	//リザルトの描画
	if (g_gameState == GAMESTATE_RESULT)
	{
		DrawResult();

		//タイマーの描画処理
		DrawTime();

		//取得スコアの描画
		DrawPoint();
	
		//取得スコアメッセージの描画
		DrawPointLog();
	}

#ifdef _DEBUG
	if (g_bEdit == true)
	{
		//エディットの描画処理
		DrawEdit();
	}
#endif //_DEBUG
}

//========================================
//ゲームの状態設定
//========================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
	g_nCounterGameState = nCounter;
}

//========================================
//ゲームの状態の取得
//========================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//========================================
//ポーズの有効無効設定
//========================================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}

//========================================
//ポーズの取得
//========================================
bool GetPause(void)
{
	return g_bPause;
}

//========================================
//フォトモードかどうかの取得
//========================================
bool GetPhotoMode(void)
{
	return g_bPhoto;
}

//========================================
//ゲーム終了時・勝敗設定
//========================================
void SetGameEnd(RESULTTYPE result)
{
	//BGM停止
	StopSound();

	//各種終了
	g_gameState = GAMESTATE_END;
	SetResult(result);

	if (result == RESULTTYPE_WIN)
	{
		AddScore(GetTime() * TIMESCORE_MLT);
		SetRanking(GetScore());
	}

	//リザルトの初期化
	InitResult();
}

void DebugFOG(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,560,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "START : %.2f\nEND : %.2f", g_fStart, g_fEnd);

	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
}