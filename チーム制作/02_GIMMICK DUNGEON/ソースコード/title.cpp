
//==========================================
//
//  タイトル画面(title.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "title_logo.h"
#include "title_message.h"
#include "camera.h"
#include "game.h"
#include "object.h"
#include "billboard.h"
#include "menu.h"
#include "field.h"
#include "sound.h"
#include "fog.h"

//==========================================
//  マクロ定義
//==========================================
#define MENU_COUNT (30) //メニューに遷移するまでの時間
#define RANKING_FROM_TITLE (1800) //ランキングに遷移するまでの時間

//==========================================
//  グローバル変数宣言
//==========================================
int g_TitleState; //タイトルの状態
int g_nCntFadeIn; //タイトル状態のカウンタ
int g_nCntMenu; //メニューへ遷移する時間のカウンタ
int g_nCntRanking; //ランキングに遷移するカウンタ

//==========================================
//  初期化処理
//==========================================
void InitTitle()
{
	//変数の初期化
	g_TitleState = TITLESTATE_IN;
	g_nCntFadeIn = 0;
	g_nCntMenu = 0;
	g_nCntRanking = 0;

	//ゲームの初期化
	InitGame();

	//タイトルロゴの初期化
	InitLogo();

	//タイトルメッセージの初期化
	InitMessage();

	//メニューの初期化
	InitMenu();

	//床の初期化
	InitField();

	//フォグの設定
	EnableFog(true);
	FOGDATA *pFog = GetFog();

	//フォグ色設定
	pFog->col = FOG_COLOR_BLACK;

	//BGM再生
	PlaySound(SOUND_LABEL_BGM000);
}

//==========================================
//  終了処理
//==========================================
void UninitTitle()
{
	//サウンドの停止
	StopSound();

	//カメラの終了
	UninitGame();

	//タイトルロゴの終了
	UninitLogo();

	//タイトルメッセージの終了
	UninitMessage();

	//メニューの終了
	UninitMenu();

	//フィールドの終了
	UninitField();
}

//==========================================
//  更新処理
//==========================================
void UpdateTitle()
{
	//自動遷移カウンタの更新
	if (g_TitleState == TITLESTATE_NORMAL)
	{
		g_nCntRanking++;
		if (g_nCntRanking >= RANKING_FROM_TITLE)
		{
			SetFade(MODE_RANKING);
		}
		for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
		{
			if (GetKeyboardPress(nCnt))
			{
				g_nCntRanking = 0;
			}
		}
		for (int nCnt = 0; nCnt <= KEY_START; nCnt++)
		{
			if (GetJoyPadButtonPress((JOYKEY_BUTTON)nCnt, 0))
			{
				g_nCntRanking = 0;
			}
		}
		if (GetStickL(0) != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
			g_nCntRanking = 0;
		}
		if (GetMousePress(MOUSEBUTTON_LEFT))
		{
			g_nCntRanking = 0;
		}
		if (GetJoyPadCrossNone(0) == false)
		{
			g_nCntRanking = 0;
		}
	}
	else
	{
		g_nCntRanking = 0;
	}

	//カメラの更新
	UpdateCamera();

	if (g_TitleState != TITLESTATE_MENU)
	{
		//カウンタの加算
		if (g_TitleState == TITLESTATE_IN)
		{
			g_nCntFadeIn++;
			if (g_nCntFadeIn == IN_TIME)
			{
				g_TitleState = TITLESTATE_NORMAL;
			}
		}

		//タイトル状態の更新
		for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
		{
			if (nCnt != DIK_ESCAPE)
			{
				if (GetKeyboardTrigger(nCnt))
				{
					if (g_TitleState == TITLESTATE_NORMAL)
					{
						PlaySound(SOUND_LABEL_SE_BUTTON_002);
					}

					if (g_TitleState != TITLESTATE_OUT)
					{
						g_TitleState++;
						break;
					}
				}
			}
		}
		for (int nCnt = 0; nCnt <= KEY_START; nCnt++)
		{
			if (GetJoyPadButtonTrigger((JOYKEY_BUTTON)nCnt,0))
			{
				if (g_TitleState == TITLESTATE_NORMAL)
				{
					PlaySound(SOUND_LABEL_SE_BUTTON_002);
				}

				if (g_TitleState != TITLESTATE_OUT)
				{
					g_TitleState++;
					break;
				}
			}
		}
		if (GetMousePress(MOUSEBUTTON_LEFT))
		{
			if (g_TitleState == TITLESTATE_NORMAL)
			{
				PlaySound(SOUND_LABEL_SE_BUTTON_002);
			}

			if (g_TitleState != TITLESTATE_OUT)
			{
				g_TitleState++;
			}
		}

		//フェードの開始
		if (g_TitleState == TITLESTATE_OUT)
		{
			g_nCntMenu++;
			if (g_nCntMenu >= MENU_COUNT)
			{
				g_TitleState = TITLESTATE_MENU;
			}
		}

		//タイトルロゴの更新
		UpdateLogo();

		//タイトルメッセージの更新
		UpdateMessage();
	}
	else
	{
		//メニューの更新
		UpdateMenu();
	}

	//ビルボードの更新処理
	UpdateBillboard();

	//オブジェクトの更新
	UpdateObject();

	//フィールドの更新
	UpdateField();

	//ゲーム更新
	UpdateGame();
}

//==========================================
//  描画処理
//==========================================
void DrawTitle()
{
	//カメラの設定
	SetCamera();
	DrawGame();

	//床の描画
	DrawField();

	if (g_TitleState != TITLESTATE_MENU)
	{
		//タイトルロゴの描画
		DrawLogo();

		//タイトルメッセージの描画
		DrawMessage();
	}
	else
	{
		//メニューの描画
		DrawMenu();
	}
}

//==========================================
//  タイトル状態の取得
//==========================================
int GetTitleState()
{
	return g_TitleState;
}
