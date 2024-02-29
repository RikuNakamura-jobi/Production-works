//==========================================
//
//  メニュー画面(menu.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "menu.h"
#include "menu_item.h"
#include "menu_message.h"
#include "input.h"
#include "fade.h"
#include "game.h"
#include "object.h"
#include "billboard.h"
#include "sound.h"

//==========================================
//  マクロ定義
//==========================================
#define TITLE_FROM_MENU (1800) //放置でタイトルに遷移する時間

//==========================================
//  グローバル変数宣言
//==========================================
int g_nMenu; //選択状態
int g_nCntTimer; //放置タイマー
bool g_bEnter; //決定フラグ

//==========================================
//  初期化処理
//==========================================
void InitMenu()
{
	//変数の初期化
	g_nMenu = MENUSTATE_START;
	g_nCntTimer = 0;
	g_bEnter = false;

	//メニュー項目の初期化
	InitMenuItem();

	//メニューメッセージの初期化
	InitMenuMessage();
}

//==========================================
//  終了処理
//==========================================
void UninitMenu()
{
	//メニュー項目の終了
	UninitMenuItem();

	//メニューメッセージの終了
	UninitMenuMessage();
}

//==========================================
//  更新処理
//==========================================
void UpdateMenu()
{
	//タイマーの更新
	g_nCntTimer++;

	//メニューの決定
	if (GetFade() == FADE_NONE)
	{
		if (g_bEnter != true)
		{
			//選択状態の更新
			if (GetKeyboardTrigger(DIK_W) || GetStickTriggerL(STICK_UP, 0) || GetJoyPadCrossTrigger(CROSS_UP, 0))
			{
				PlaySound(SOUND_LABEL_SE_BUTTON_000);

				g_nMenu += (MENUSTATE_FADE - 1);

				//放置タイマーのリセット
				g_nCntTimer = 0;

				//選択状態の補正
				g_nMenu %= MENUSTATE_FADE;
			}
			if (GetKeyboardTrigger(DIK_S) || GetStickTriggerL(STICK_DOWN, 0) || GetJoyPadCrossTrigger(CROSS_DOWN, 0))
			{
				PlaySound(SOUND_LABEL_SE_BUTTON_000);

				g_nMenu++;

				//放置タイマーのリセット
				g_nCntTimer = 0;

				//選択状態の補正
				g_nMenu %= MENUSTATE_FADE;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) || (GetJoyPadButtonTrigger(KEY_A, 0) || GetJoyPadButtonTrigger(KEY_START, 0)) || GetMouseTrigger(MOUSEBUTTON_LEFT))
		{
			if (g_nMenu != MENUSTATE_FADE)
			{
				PlaySound(SOUND_LABEL_SE_BUTTON_001);
			}

			switch (g_nMenu)
			{
			case MENUSTATE_START: //ゲームスタート
				SetFade(MODE_GAME);
				g_nMenu = MENUSTATE_FADE;
				break;
			case MENUSTATE_TUTORIAL: //チュートリアル
				SetFade(MODE_TUTORIAL);
				break;
			case MENUSTATE_QUIT: //ゲーム終了
				SetFade(MODE_TITLE);
				break;
			default:
				break;
			}

			//フラグをへし折る
			g_bEnter = true;
		}
	}

	//メニュー項目の更新
	UpdateMenuItem();

	//メニューメッセージの更新
	UpdateMenuMessage();

	//タイトルに強制送還
	if (g_nCntTimer >= TITLE_FROM_MENU)
	{
		SetFade(MODE_TITLE);
	}
}

//==========================================
//  描画処理
//==========================================
void DrawMenu()
{
	//メニュー項目の描画
	DrawMenuItem();

	//メニューメッセージの描画
	DrawMenuMessage();
}

//==========================================
//  メニュー状態の取得
//==========================================
int GetMenuState()
{
	return g_nMenu;
}

//==========================================
//  決定フラグを取得
//==========================================
bool GetEnterFrag()
{
	return g_bEnter;
}
