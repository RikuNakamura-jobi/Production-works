//==================================================================================================
//
//collision.h
//Author:中村陸
//
//==================================================================================================

#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"

//================================
// プロトタイプ宣言
//================================
bool CollisionCircle(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float length);											//円の当たり判定
bool CollisionCircle2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float length);										//円(2D)の当たり判定
bool CollisionSphere(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float length);											//球の当たり判定
bool CollisionEllipse(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS);			//楕円の当たり判定
bool CollisionEllipse2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS);			//楕円(2D)の当たり判定
bool CollisionOval(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS);				//楕円球の当たり判定
bool CollisionRectangle(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float height, float width);			//矩形の当たり判定
bool CollisionRectangle2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float height, float width);		//矩形(2D)の当たり判定
bool CollisionSquare(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS);			//箱の当たり判定

#endif
