//
//
//ランキング処理 [ranking.h]
//Author Ibuki Okusada
//
//=======================================
#ifndef _RANKING_H_     //このマクロ定義が定義されていなかったら
#define _RANKING_H_      //2連インクルード防止のマクロを定義する

#include "main.h"    //作成したmain.hをインクルードする

//=======================================
//マクロ定義
//=======================================

//プロトタイプ宣言
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);

#endif