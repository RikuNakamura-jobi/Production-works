//===========================
//
//ダイレクトX.particleヘッダー
//Author:中村　陸
//
//===========================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, int nType, int nRot, D3DXVECTOR3 move, int nMax);

#endif // !_PARTICLE_H_