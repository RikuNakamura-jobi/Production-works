//==========================================
//
//ランキング順位の処理(ranking_rank.cpp)
//Author Kai Takada
//
//==========================================
#ifndef _RANKING_RANK_H_							//このマクロ定義がされてなかったら
#define _RANKING_RANK_H_							//二重インクルード防止のマクロを定義する

//プロトタイプ宣言
void InitRankingRank(void);
void UninitRankingRank(void);
void UpdateRankingRank(void);
void DrawRankingRank(void);
D3DXVECTOR3 *GetRankPos();

#endif
