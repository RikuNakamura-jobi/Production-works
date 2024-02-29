//==========================================
//
//  砂煙の制御(fog.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _FOG_H_
#define _FOG_H_
#include "main.h"

//==========================================
//  マクロ定義
//==========================================
#define FOG_START_GAME (1000.0f) //霧の発生位置
#define FOG_END_GAME (5000.0f) //霧の終了位置

#define FOG_START_TITLE (400.0f) //霧の発生位置
#define FOG_END_TITLE (800.0f) //霧の終了位置

#define FOG_COLOR_SAND (D3DXCOLOR(0.65f, 0.5f, 0.0f, 1.0f)) //砂の霧
#define FOG_COLOR_BLACK (D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)) //黒い霧

//==========================================
//  霧の構造体定義
//==========================================
typedef struct
{
	bool bUse; //使用状態
	float fStart; //発生位置
	float fEnd; //終了位置
	D3DXCOLOR col; //色
}FOGDATA;

//==========================================
//  プロトタイプ宣言
//==========================================
void InitFog(void);
void UninitFog(void);
void UpdateFog(void);
void SetFog(void);
void EnableFog(bool bUse);
FOGDATA *GetFog(void);

#endif
