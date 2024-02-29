//==========================================================
//
//プレイヤー処理 [player.h]
//Author:佐藤根詩音
//
//==========================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"	//main.hで定義しているものが必要なためinclude

//==========================================================
//マクロ定義
//==========================================================
#define PLAYER_GRAVITY	(0.1f)		//プレイヤーの重力
#define WIDTH_PLAYER	(35.0f)		//プレイヤーの幅
#define HEIGHT_PLAYER	(35.0f)		//プレイヤーの高さ

//==========================================================
//列挙型の定義
//==========================================================
//プレイヤーの状態列挙型
typedef enum
{
	PLAYER_APPEAR = 0,		//出現状態
	PLAYERSTATE_NORMAL,		//通常状態
	PLAYERSTATE_DEATH,		//死亡状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//==========================================================
//構造体の定義
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 moveOld;	//前回の移動量
	int nSwitchLR;			//プレイヤーの左右切り替え
	int nCntAnim;			//アニメーションカウンター
	int nPatternAnim;		//アニメーションNo.
	int nCntGravity;		//重力カウンター
	float fSpeed;			//移動量
	float fJump;			//ジャンプ力
	float fLength;			//対角線の長さ
	float fAngle;			//対角線の角度
	float fWidth;			//幅
	float fHeight;			//高さ
	bool bJump;				//ジャンプしているかどうか
	bool bUse;				//使用してるか
	bool bDisp;				//描画するか
	PLAYERSTATE state;		//プレイヤーの状態
}Player;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif

