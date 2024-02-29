//===============================
//
//タイム処理(time.h)
//
//===============================
#ifndef _TIME_H_   //このマクロ定義がされていなかったら
#define _TIME_H_   //2重インクルード防止のマクロを定義する

#include "main.h"

//プロトタイプ宣言
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nTime);
void SetPosTime(D3DXVECTOR3 pos, float fWindth, float fHeight);
int GetTime(void);
void ResetTime();

#endif
