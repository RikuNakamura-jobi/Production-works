//==========================================================
//
//ポリゴン処理 [polygon.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

//==========================================================
//構造体の定義
//==========================================================
//ポリゴン基礎情報
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	float fWidth;			//幅
	float fHeight;			//高さ
}polygon;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void DrawPolygon(void);

#endif

