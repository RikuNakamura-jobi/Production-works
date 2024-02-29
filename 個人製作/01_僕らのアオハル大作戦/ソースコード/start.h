//===========================
//
//ダイレクトX.startヘッダー
//Author:中村　陸
//
//===========================
#ifndef _START_H_
#define _START_H_

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
}Start;

//プロトタイプ宣言
void InitStart(void);
void UninitStart(void);
void UpdateStart(void);
void DrawStart(void);
void SetStart(D3DXVECTOR3 pos, int nType);
bool CollisionStart(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fAngle, float fLength, float fRot, Start **pStart);

#endif // !_START_H_