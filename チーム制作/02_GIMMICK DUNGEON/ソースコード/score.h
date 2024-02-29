//==========================================
//
//  スコア表示(score.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _SCORE_H_
#define _SCORE_H_
#include "main.h"

//==========================================
//  プロトタイプ宣言
//==========================================
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void DiffAddScore(int nValue);
void AddScore(int nValue);
void SetAdjScore(D3DXVECTOR3 pos, float fMgn);
int GetScore(void);
void DebugScore(void);

#endif
