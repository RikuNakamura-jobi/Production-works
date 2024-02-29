//==========================================================
//
//障害物処理 [enemy.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _ENEMY_H_	 //このマクロ定義が定義されていなかったら
#define _ENEMY_H_    //2連インクルード防止のマクロを定義する

#include "main.h"    //作成したmain.hをインクルードする
#include "polygon.h"

//==========================================================
//列挙型の定義
//==========================================================
typedef enum
{
	ENEMYTYPE_BIED = 0,	//鳥
	ENEMYTYPE_JET,		//飛行機
	ENEMYTYPE_CLOUD,	//雷雲
	ENEMYTYPE_METEOR,	//隕石
	ENEMYTYPE_MAX
}ENEMYTYPE;

//==========================================================
//構造体の定義
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col;		//色
	float fAngle;		//角度
	float fLength;		//対角線の長さ
	float fHeight;		//高さ
	float fWidth;		//幅
	bool bUse;			//使用しているかどうか
	int nType;			//種類
	int nTexCounter;	//テクスチャカウンター
	int nTexAnim;		//テクスチャ座標
	int nRot;			//回る向き
	int nNear;			//ニアミスしたかどうか
}Enemy;

typedef struct
{
	polygon Polygon;
	D3DXVECTOR3 move;	//移動量
	bool bUse;			//使用しているかどうか
	int nLife;
}NearMiss;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy *GetEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, float fWidth, float fHeight, int nType);
bool CollisionEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 posOld);
void SetNearMiss(void);

#endif
