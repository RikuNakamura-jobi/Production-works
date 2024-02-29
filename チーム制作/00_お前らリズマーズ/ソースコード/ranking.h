//===========================
//
//ダイレクトX.rankingヘッダー
//Author:中村　陸
//
//===========================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

//プロトタイプ宣言
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void SetRanking(int nScore);
void ResetRanking(void);
void LoadData(void);
void SaveData(void);

#endif // !_RANKING_H_