//==================================================================================================
//
//Effect.h
//Author:髙山桃也
//
//==================================================================================================

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"

//==========================================
//  構造体宣言
//==========================================
//エフェクトの構造体
typedef struct
{
	D3DXVECTOR3 pos;								//位置
	D3DXVECTOR3 relPos;								//持ち主からの相対位置
	D3DXVECTOR3 *pPosOwner = NULL;					//持ち主の位置のポインタ
	D3DXVECTOR3 move;								//移動量
	D3DXMATRIX mtxWorld;							//ワールドマトリックス
	D3DXCOLOR col;									//色
	float fRadius;									//半径
	int nLife;										//寿命
	int nMaxLife;									//最大寿命
	bool bAdd;										//加算合成するかどうか
	float fGrav;									//重力に影響されるかどうか
	bool bUse;										//使用しているかどうか
	bool bZtest;									//Zテストするかどうか
}Effect;

//==========================================
//  プロトタイプ宣言
//==========================================
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 *pPosOwner, D3DXVECTOR3 move,D3DXCOLOR col,float fRadius,int nLife,bool Add, float fGrav, bool bZtest);
Effect *GetEffect(void);
void DebugEffect(void);

#endif