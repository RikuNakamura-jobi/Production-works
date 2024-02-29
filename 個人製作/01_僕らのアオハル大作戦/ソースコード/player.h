//===========================
//
//ダイレクトX.playerヘッダー
//Author:中村　陸
//
//===========================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "block.h"

//プレイヤーの種類
typedef enum
{
	PLAYERSTATE_APPEAR = 0,			//出現状態
	PLAYERSTATE_NORMAL,				//通常状態
	PLAYERSTATE_DAMAGE,				//ダメージ状態
	PLAYERSTATE_INV,				//ダメージ状態
	PLAYERSTATE_WAIT,				//待機状態
	PLAYERSTATE_GOAL,				//待機状態
	PLAYERSTATE_MOVE,				//待機状態
	PLAYERSTATE_ATTACK,				//待機状態
	PLAYERSTATE_DEATH,				//死亡状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤーのタイプ
typedef enum
{
	PLAYERTYPE_0 = 0,			//通常状態
	PLAYERTYPE_1,				//通常状態
	PLAYERTYPE_2,				//通常状態
	PLAYERTYPE_3,				//通常状態
	PLAYERTYPE_MAX
}PLAYERTYPE;

//プレイヤー構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 posOld;				//位置
	D3DXVECTOR3 move;				//移動量
	D3DXVECTOR3 rot;				//向き
	int nCounterAnim;				//アニメーションカウンター
	int nPatternAnim;				//アニメーションパターンNo.
	int nLife;						//アニメーションパターンNo.
	int nCounterState;				//状態管理カウンター
	float fJumpHigh;
	float fJumpG;
	bool bDisp;						//使用しているかどうか
	bool bJump;
	bool bWall;
	Block *pBlock;
	PLAYERSTATE state;				//状態
	PLAYERTYPE playertype;
}Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayer(D3DXVECTOR3 pPos);
void HitPlayer(int nDamage, float move);
Player *GetPlayer(void);
void SetPlayerState(PLAYERSTATE state, int nCounter);

#endif // !_PLAYER_H_