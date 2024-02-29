//==========================================================
//
//下の雲処理 [cloud.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _START_H_
#define _START_H_

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
	float fTexU;			//テクスチャ座標
}Start;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitStart(void);
void UninitStart(void);
void UpdateStart(void);
void DrawStart(void);

#endif

