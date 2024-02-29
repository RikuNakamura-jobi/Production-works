//===========================
//
//ダイレクトX.enemyヘッダー
//Author:中村　陸
//
//===========================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//マクロ定義
#define MAX_ENEMY (256)

//敵の種類
typedef enum
{
	ENEMYSTATE_NORMAL = 0,			//通常状態
	ENEMYSTATE_DAMAGE,				//ダメージ状態
	ENEMYSTATE_MAX
}ENEMYSTATE;

//敵構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 move;				//移動量
	D3DXVECTOR3 rot;				//向き
	float fLength;					//対角線の長さ
	float fAngle;					//対角線の角度
	float fLengthMove;				//対角線の移動量
	int nType;						//種類
	int nMoveType;					//移動の種類
	int nMoveTime;					//移動の時間
	int nLife;						//体力
	ENEMYSTATE state;				//状態
	int nCounterState;				//状態管理カウンター
	bool bUse;						//使用しているかどうか
}Enemy;

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType, int nMoveType, int nMoveTime);
Enemy *GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);
int GetNumEnemy(void);
void CollisionPlayerENEMY(int nCntEnemy);
int GetBulletCount(void);

#endif // !_ENEMY_H_