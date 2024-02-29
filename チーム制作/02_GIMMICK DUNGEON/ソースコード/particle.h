//========================================
//
// 3Dパーティクルの処理[particle.h]
// Author:Kai Takada
//
//========================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_		//二重インクルード防止のマクロを定義する

#include "effect.h"

//===========================
//タイプ列挙型定義
//===========================
typedef enum
{
	PARTICLETYPE_SMOG = 0,
	PARTICLETYPE_SPARK,
	PARTICLETYPE_SANDPOLE,
	PARTICLETYPE_SANDDUST,
	PARTICLETYPE_LANDED,
	PARTICLETYPE_MAX
}PARTICLETYPE;

//===========================
//構造体定義
//===========================
typedef struct
{
	Effect effect;
	D3DXVECTOR3 Randpos;	//ランダム座標
	int nSpeed;		//スピードの10倍
	int nLife;		//エフェクト寿命
	int nLifePtcl;		//パーティクル寿命
	int nNumEf;		//出すエフェクト数
	bool bUse;
}Particle;

//===========================
//プロトタイプ宣言
//===========================
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, PARTICLETYPE type);
Particle *GetParticle(void);

#endif