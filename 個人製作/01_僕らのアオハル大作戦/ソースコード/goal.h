//===========================
//
//ダイレクトX.goalヘッダー
//Author:中村　陸
//
//===========================
#ifndef _GOAL_H_
#define _GOAL_H_

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
}Goal;

//プロトタイプ宣言
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
void SetGoal(D3DXVECTOR3 pos, int nType);
bool CollisionGoal(D3DXVECTOR3 *pPos);

#endif // !_Goal_H_