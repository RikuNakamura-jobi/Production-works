//==============================================================
//
//エフェクト処理[effect.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"

//プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nType);

#endif