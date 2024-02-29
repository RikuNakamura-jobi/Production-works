//=============================================================================
//
// サウンド処理 [sound.h]
// Author 奥定伊吹
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGMTITLE = 0,		// BGMタイトル
	SOUND_LABEL_BGMTUTORIAL,		// BGMチュートリアル
	SOUND_LABEL_BGMGAME,			// BGMゲーム
	SOUND_LABEL_BGMCLEAR,			// BGMクリア
	SOUND_LABEL_BGMGAMEOVER,		// BGMゲームオーバー
	SOUND_LABEL_BGMRANKING,			// BGMランキング
	SOUND_LABEL_SEBUTTON,			// 決定音
	SOUND_LABEL_SECURSOR,			// カーソル音
	SOUND_LABEL_SERUN,				// 足音
	SOUND_LABEL_SCORE,				// スコア
	SOUND_LABEL_DAMAGE,				// ダメージ
	SOUND_LABEL_TUTORIAL,
	SOUND_LABEL_TITLE,
	SOUND_LABEL_GAME,
	SOUND_LABEL_MAX
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
