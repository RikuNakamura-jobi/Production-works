//===========================
//
//ダイレクトX.playerヘッダー
//Author:中村　陸
//
//===========================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//マクロ定義
#define MAX_PLAYER_MODEL (32)
#define MAX_MOTION_KEY (32)

//プレイヤーの種類
typedef enum
{
	PLAYERSTATE_APPEAR = 0,			//出現状態
	PLAYERSTATE_NORMAL,				//通常状態
	PLAYERSTATE_MOVE,				//移動状態
	PLAYERSTATE_DAMAGE,				//ダメージ状態
	PLAYERSTATE_SHOT,				//射撃状態
	PLAYERSTATE_DEATH,				//死亡状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤーの種類
typedef enum
{
	PLAYERMOTION_NORMAL = 0,		//待機モーション
	PLAYERMOTION_MOVE,				//移動モーション
	PLAYERMOTION_ACTION,			//アクションモーション
	PLAYERMOTION_TITLE,				//ジャンプモーション
	PLAYERMOTION_CLEAR,				//着地モーション
	PLAYERMOTION_DEATH,				//着地モーション
	PLAYERMOTION_RAND,				//着地モーション
	PLAYERMOTION_MAX
}PLAYERMOTION;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posMotion;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 rotMotion;
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	LPDIRECT3DTEXTURE9 apTexturePlayer[8] = {};
	LPD3DXMESH pMeshPlayer = NULL;
	LPD3DXBUFFER pBuffMatPlayer = NULL;					//頂点情報を格納
	DWORD dwNumMatPlayer = 0;
	int nIdxModelParent;
}PlayerModel;

//プレイヤーの構造体
typedef struct
{
	D3DXVECTOR3 pos[MAX_PLAYER_MODEL];
	D3DXVECTOR3 rot[MAX_PLAYER_MODEL];
	int nFrame;
}KeyMotion;

//プレイヤーの構造体
typedef struct
{
	KeyMotion aKeyMotion[MAX_MOTION_KEY];
	int nLoop;
	int nNumKey;
	int nKey;
}PlayerMotion;

//プレイヤーの構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 point;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3 vtxMinModel;
	D3DXVECTOR3 vtxMaxModel;
	PLAYERSTATE state;
	PLAYERMOTION motionState;
	PLAYERMOTION motionStateOld;
	PlayerModel aPlayerModel[MAX_PLAYER_MODEL];
	PlayerMotion aPlayerMotion[PLAYERMOTION_MAX];
	int nNumModel;
	int nLife;
	int nTimerState;
	float fJump;
	float fMove;
	float fAngle;
}Player;

//プロトタイプ宣言
void InitPlayer(void);
void InitPlayerTitle(void);
void InitPlayerResult(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void UpdatePlayerResult(void);
void DrawPlayer(void);
Player *GetPlayer(void);
bool CollisionPlayer(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, int nType);
void ReadFilePlayer(void);

#endif // !_PLAYER_H_