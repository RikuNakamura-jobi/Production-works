//==========================================================
//
//フォグ処理 [fog.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _FOG_H_
#define _FOG_H_

#include "main.h"

//==========================================================
//構造体の定義
//==========================================================
typedef struct
{
	D3DCOLOR col;	//色
	float fStart;	//開始点
	float fEnd;		//終了点
	float fDensity;	//フォグ密度
}Fog;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitFog(void);
void UninitFog(void);
void UpdateFog(void);
void DrawFog(void);
void ResetStateFog(void);

#endif
