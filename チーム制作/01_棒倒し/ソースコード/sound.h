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
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM003,			// BGM3
	SOUND_LABEL_BGM004,			// BGM4
	SOUND_LABEL_BGM005,			// BGM5
	SOUND_LABEL_SE_RUN,			// 弾発射音1
	SOUND_LABEL_SE_KICK,		// 弾発射音1
	SOUND_LABEL_SE_SLIP,		// 弾発射音1
	SOUND_LABEL_SE_JUMP,		// 弾発射音1
	SOUND_LABEL_SE_DORO,		// 弾発射音1
	SOUND_LABEL_SE_BOU,			// 弾発射音1
	SOUND_LABEL_SE_FANFARE,		// 弾発射音1
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
