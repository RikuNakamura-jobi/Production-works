//===========================
//
//ダイレクトX.coinヘッダー
//Author:中村　陸
//
//===========================
#ifndef _COIN_H_
#define _COIN_H_

#include "main.h"

//敵構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 posOld;				//位置
	D3DXVECTOR3 move;				//移動量
	int nType;
	float fHeight;					//高さ
	float fWidth;					//幅
	bool bUse;						//使用しているかどうか
}Coin;

//プロトタイプ宣言
void InitCoin(void);
void UninitCoin(void);
void UpdateCoin(void);
void DrawCoin(void);
void SetCoin(D3DXVECTOR3 pos, int nType);
bool CollisionCoin(D3DXVECTOR3 *pPos);
int GetNumCoin(void);

#endif // !_COIN_H_