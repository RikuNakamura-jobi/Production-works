//==========================================
//
//  ゲーム内設定処理(option.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _OPTION_H_
#define _OPTION_H_
#include "main.h"

//==========================================
//  プロトタイプ宣言
//==========================================
void InitOption(void);
void UninitOption(void);
void UpdateOption(void);
void DrawOption(void);
void SetMouseSensCorrection(void);
D3DXVECTOR3 GetMouseSensCorrection(void);

#endif
