//===========================
//
//ダイレクトX.playerヘッダー
//Author:中村　陸
//
//===========================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//プレイヤーの種類
typedef enum
{
	PLAYERSTATE_APPEAR = 0,			//出現状態
	PLAYERSTATE_NORMAL,				//通常状態
	PLAYERSTATE_DAMAGE,				//ダメージ状態
	PLAYERSTATE_WAIT,				//待機状態
	PLAYERSTATE_DEATH,				//死亡状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤー構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 move;				//移動量
	D3DXVECTOR3 rot;				//向き
	int nCounterAnim;				//アニメーションカウンター
	int nPatternAnim;				//アニメーションパターンNo.
	int nLife;						//体力
	int nRemaining;					//残機
	PLAYERSTATE state;				//状態
	int nCounterState;				//状態管理カウンター
	bool bDisp;						//使用しているかどうか
}Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void HitPlayer(int nDamage);
void AddPlayer(int nLife, int nRemain);

#endif // !_PLAYER_H_