//==================================================================================================
//
//shadow.h
//Author:髙山桃也
//
//==================================================================================================

#ifndef _SHADOW_H_
#define _SHADOW_H_

//種類
typedef enum
{
	SHADOWTYPE_CIRCLE = 0,						//円形
	SHADOWTYPE_SQUARE,							//矩形
	SHADOWTYPE_MAX
}SHADOWTYPE;

//構造体宣言
typedef struct
{
	D3DXVECTOR3 pos;							//位置
	D3DXVECTOR3 rot;							//向き
	D3DXMATRIX mtxWorld;						//ワールドマトリックス
	SHADOWTYPE type;							//種類
	float fRadius;								//半径
	float fWidth;								//x軸
	float fHeight;								//z軸
	bool bUse;									//使用しているかどうか
}Shadow;

//プロトタイプ宣言
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(float fRadius, SHADOWTYPE type,float fWidth, float fHeight, D3DXVECTOR3 rot);
void SetPositionShadow(int nIdxShadow,D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fScale);
Shadow *GetShadow(void);
void EnableShadow(int nIdxShadow, bool bUse);
void DebugShadow(void);

#endif
