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
	BULLETTYPE_0 = 0,			//プレイヤーの通常弾
	BULLETTYPE_1,				//敵の通常弾
	BULLETTYPE_2,				//プレイヤーのホーミング弾
	BULLETTYPE_3,				//敵のホーミング弾
	BULLETTYPE_4,				//敵のホーミング弾
	BULLETTYPE_5,				//敵のホーミング弾
	BULLETTYPE_6,				//敵のホーミング弾
	BULLETTYPE_7,				//敵のホーミング弾
	BULLETTYPE_8,				//敵のホーミング弾
	BULLETTYPE_MAX
}BULLETTYPE;

//弾の状態
typedef enum
{
	BULLETSTATE_NORMAL = 0,			
	BULLETSTATE_WAIT,
	BULLETSTATE_HIT,
	BULLETSTATE_MAX
}BULLETSTATE;

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;												//位置
	D3DXVECTOR3 move;												//移動量
	D3DXVECTOR3 rot;												//向き
	int nLife;														//寿命
	BULLETTYPE type;												//弾のタイプ
	D3DXCOLOR col;													//色
	bool bUse;														//使用しているかどうか
}Bullet;

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type, D3DXCOLOR col, int nEnemyType);
void CollisionPlayer(Bullet *pBullet);
Bullet *GetBullet(void);
BULLETSTATE GetBulletState(void);
void SetBulletState(BULLETSTATE bulletState);
int *GetBulletHit(void);
int GetBulletScore(void);

#endif // !_BULLET_H_