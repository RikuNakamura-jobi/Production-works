//=============================================================================
//
// サウンド処理 [sound.h]
// Author :中村陸
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
	SOUND_LABEL_BGM000 = 0,			// BGM0
	SOUND_LABEL_BGM001,				// BGM1
	SOUND_LABEL_BGM002,				// BGM2
	SOUND_LABEL_BGM003,				// BGM3
	SOUND_LABEL_BGM004,				// BGM4
	SOUND_LABEL_BGM005,				// BGM5
	SOUND_LABEL_BGM006,				// BGM6
	SOUND_LABEL_BGM007,				// BGM7
	SOUND_LABEL_BGM008,				// BGM8
	SOUND_LABEL_BGM009,				// BGM9
	SOUND_LABEL_BGM010,				// BGM10
	SOUND_LABEL_BGM011,				// BGM11
	SOUND_LABEL_BGM012,				// BGM12
	SOUND_LABEL_BGM013,				// BGM13
	SOUND_LABEL_BGM014,				// BGM14
	SOUND_LABEL_SE_SHOT,			// 弾発射音1
	SOUND_LABEL_SE_SHOT001,			// 弾発射音2
	SOUND_LABEL_SE_SHOT002,			// 弾発射音3
	SOUND_LABEL_SE_DECISION001,		// 決定音
	SOUND_LABEL_SE_HIT,				// ヒット音
	SOUND_LABEL_SE_EXPLOSION,		// 爆発音
	SOUND_LABEL_SE_RESULT001,		// リザルト音
	SOUND_LABEL_SE_WARNING001,		// 警告音
	SOUND_LABEL_SE_KATANA001,		// 刀音
	SOUND_LABEL_SE_BOSSWARNING001,	// 攻撃警告音
	SOUND_LABEL_SE_BEAM001,			// ビーム音
	SOUND_LABEL_SE_DORAGON001,		// ドラゴン攻撃音
	SOUND_LABEL_SE_STAGESTART001,	// ステージ開始音
	SOUND_LABEL_MAX,
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
