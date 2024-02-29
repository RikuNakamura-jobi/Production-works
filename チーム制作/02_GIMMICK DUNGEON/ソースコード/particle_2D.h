//========================================
//
// 2Dパーティクルの処理[particle_2D.h]
// Author:Kai Takada
//
//========================================
#ifndef _PARTICLE_2D_H_
#define _PARTICLE_2D_H_		//二重インクルード防止のマクロを定義する

#include "effect_2D.h"

//===========================
//タイプ列挙型定義
//===========================
typedef enum
{
	PARTICLETYPE_2D_SMOG = 0,
	PARTICLETYPE_2D_RESULT,
	PARTICLETYPE_2D_MAX
}PARTICLETYPE_2D;

//===========================
//構造体定義
//===========================
typedef struct
{
	Effect2D effect2D;
	D3DXVECTOR3 Randpos;	//ランダム座標
	D3DXVECTOR3 move; //移動量
	int nSpeed;		//スピードの10倍
	int nLife;		//エフェクト寿命
	int nLifePtcl;		//パーティクル寿命
	int nNumEf;		//出すエフェクト数
	bool bUse;
}Particle2D;

//===========================
//プロトタイプ宣言
//===========================
void InitParticle2D(void);
void UninitParticle2D(void);
void UpdateParticle2D(void);
void DrawParticle2D(void);
void SetParticle2D(D3DXVECTOR3 pos, PARTICLETYPE_2D type);
Particle2D *GetParticle2D(void);

#endif