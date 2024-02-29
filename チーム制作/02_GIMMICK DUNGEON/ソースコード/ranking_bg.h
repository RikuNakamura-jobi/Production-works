//==========================================
//
//ランキングの背景の処理(ranking_bg.h)
//Author:Kai Takada
//
//==========================================
#ifndef _RANKING_BG_H_							//このマクロ定義がされてなかったら
#define _RANKING_BG_H_							//二重インクルード防止のマクロを定義する

//===============================
//プロトタイプ宣言
//===============================
void InitRankingBg(void);
void UninitRankingBg(void);
void UpdateRankingBg(void);
void DrawRankingBg(void);

#endif
