//===========================
//
//ダイレクトX.bossヘッダー
//Author:中村　陸
//
//===========================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"

//プロトタイプ宣言
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
void SetBoss(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 rot, float fRadiusX, float fRadiusY, int nLife);
bool CollisionBoss(D3DXVECTOR3 *pPos);

#endif // !_BOSS_H_