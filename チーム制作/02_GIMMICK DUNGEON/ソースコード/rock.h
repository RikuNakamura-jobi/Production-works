//========================================
//
//2Dシューティングゲーム(アイテム取得処理)[getitem.h]
//Author:佐久間優香
//
//========================================
#ifndef _ROCK_H_							//このマクロ定義がされていなかったら
#define _ROCK_H_							//2重インクルード防止のマクロを定義するv

#include"main.h"

//========================
// マクロ定義
//========================
#define MAX_ROCK	(256)							//モデルの最大数
#define SPEED_ROCK	(2.3f)							//岩のスピード
#define LIFE_ROCK	(6000)							//岩の寿命
#define ROCK_TIME	(30)							//岩の無敵時間
#define ROLLSOUND_RANGE		(900.0f)				//転がる音の聞こえる範囲

//========================
// 列挙型定義
//========================
// マグストーンの種類
typedef enum
{
	ROCKTYPE_GIMMICK = 0,					//転がるやつ
	ROCKTYPE_THROW,						//飛ばす奴
	ROCKTYPE_INVISIBLE,		//不可視タイプ
	ROCKTYPE_DROP,		//落下タイプ
	ROCKTYPE_MAX
}ROCKTYPE;

// モデルの種類
typedef enum
{
	ROCKSTATE_NORMAL = 0,						//通常
	ROCKSTATE_DAMEGE,						//ダメージ状態
	ROCKSTATE_DEATH,						//死亡状態
	ROCKSTATE_MAX
}ROCKSTATE;

//========================
// 構造体定義
//========================
// モデル構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;									//位置
	D3DXVECTOR3 posOld;									//位置
	D3DXVECTOR3 move;									//移動量
	D3DXVECTOR3 rot;									//向き
	D3DXVECTOR3 vtxMin;									//最小
	D3DXVECTOR3 vtxMax;									//最大
	D3DXMATRIX mtxWorld;								//ワールドマトリックス
	float fSpeed;										//速さ
	int nCounter;										//カウンター
	int nType;											//種類
	bool bUse;											//使用しているかどうか
	bool bWallInvisible;								//壁に当たったかどうか
	ROCKSTATE state;									//状態
	int nCounterState;									//状態管理カウンター
	D3DXMATERIAL RedMat;								//マテリアルデータへのポインタ
	int nLife;											//体力
	int nldxModelParent;								//親モデルのインデックス数
	int nIdxEditModel;									//対象のエディットのインデックス(番号)
	int nHitRockCounter;									//岩のカウンター
	int nCntSound;										//音を鳴らすカウンター
	int nIdxShadow;										//影の番号
	int nIdxOrbit;										//軌跡の番号
}Rock;

//========================
// プロトタイプ宣言
//========================
//基本処理
void InitRock(void);
void UninitRock(void);
void UpdateRock(void);
void DrawRock(void);

//当たり判定
void CollisionRockPlayer(int nCntRock);
void CollisionRockEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pPoint, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, bool bJump);
void HitRock(int nDamage, int nCntRock);

//設定処理
void SetRock(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nLife, int nType);
void DisableRock(Rock *pRock);	//岩を消す処理

//情報取得処理
Rock *GetRock(void);
int *GetHitRockNum(void);

//その他
void RockRollSound(Rock *pRock,bool *bSound);
void PlaySoundRockCrash(Rock *pRock);

#endif