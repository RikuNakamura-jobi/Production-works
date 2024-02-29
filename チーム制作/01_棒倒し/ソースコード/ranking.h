//============================
//
//ランキング処理(ranking.h)
//Author:早川 友貴
//
//============================

#ifndef _RANKING_H_   //このマクロ定義がされていなかったら
#define _RANKING_H_   //2重インクルード防止のマクロを定義する

#include "main.h"

//プロトタイプ宣言
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void SaveData(void);
void LoadData(void);
void ResetData(void);

#endif

