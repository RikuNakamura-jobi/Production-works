//==========================================
//
//ランキングの処理(ranking.h)
//Author:Kai Takada
//
//==========================================
#ifndef _RANKING_H_							//このマクロ定義がされてなかったら
#define _RANKING_H_							//二重インクルード防止のマクロを定義する

//===============================
//マクロ定義
//===============================
#define RANK_HEI (25)						//ランキングの間の幅
#define RANK_HEIGHT (45)					//タイトル名のサイズ（縦）
#define RANK_WIDTH (30)						//タイトル名のサイズ（横）
#define NUM_RANKING (5)						//ランキングに出すスコアの数

//===============================
//プロトタイプ宣言
//===============================
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void SetRanking(int nScore);
void ResetRanking(void);
void TexRanking(void);

#endif
