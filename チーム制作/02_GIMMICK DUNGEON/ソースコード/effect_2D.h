//==================================================================================================
//
//effect_2D.h
//Author:髙山桃也		edit: 髙田佳依
//
//==================================================================================================

#ifndef _EFFECT_2D_H_
#define _EFFECT_2D_H_

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
	D3DXCOLOR col;									//色
	float fRadius;									//半径
	int nLife;										//寿命
	int nMaxLife;									//最大寿命
	bool bAdd;										//加算合成するかどうか
	float fGrav;									//重力に影響されるかどうか
	bool bUse;										//使用しているかどうか
}Effect2D;

//==========================================
//  プロトタイプ宣言
//==========================================
void InitEffect2D(void);
void UninitEffect2D(void);
void UpdateEffect2D(void);
void DrawEffect2D(void);
void SetEffect2D(D3DXVECTOR3 pos, D3DXVECTOR3 *pPosOwner, D3DXVECTOR3 move,D3DXCOLOR col,float fRadius,int nLife,bool Add, float fGrav);
Effect2D *GetEffect2D(void);
#endif