//===========================
//
//ダイレクトX.bulletヘッダー
//Author:中村　陸
//
//===========================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//弾の種類
typedef enum
{
	BULLETTYPE_PLAYER = 0,			//プレイヤーの通常弾
	BULLETTYPE_ENEMY,				//敵の通常弾
	BULLETTYPE_HOMING,				//プレイヤーのホーミング弾
	BULLETTYPE_ENEMY_HOMING,		//敵のホーミング弾
	BULLETTYPE_MAX
}BULLETTYPE;

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type, D3DXCOLOR col, int nEnemyType);

#endif // !_BULLET_H_