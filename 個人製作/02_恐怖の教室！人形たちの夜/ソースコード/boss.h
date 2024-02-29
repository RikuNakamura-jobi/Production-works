//===========================
//
//ダイレクトX.bossヘッダー
//Author:中村　陸
//
//===========================
#ifndef _BOSS_H_
#define _BOSS_H_

//マクロ定義
#define MAX_BOSS_MODEL (32)
#define MAX_MOTIONBOSS_KEY (32)

//プレイヤーの種類
typedef enum
{
	BOSSSTATE_APPEAR = 0,			//出現状態
	BOSSSTATE_NORMAL,				//通常状態
	BOSSSTATE_MOVE,				//移動状態
	BOSSSTATE_DAMAGE,				//ダメージ状態
	BOSSSTATE_SHOT,				//射撃状態
	BOSSSTATE_DEATH,				//死亡状態
	BOSSSTATE_MAX
}BOSSSTATE;

//プレイヤーの種類
typedef enum
{
	BOSSMOTION_NORMAL = 0,		//待機モーション
	BOSSMOTION_MOVE,				//移動モーション
	BOSSMOTION_ACTION,			//アクションモーション
	BOSSMOTION_JUMP,				//ジャンプモーション
	BOSSMOTION_RAND,				//着地モーション
	BOSSMOTION_MAX
}BOSSMOTION;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posMotion;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 rotMotion;
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	LPDIRECT3DTEXTURE9 apTextureBoss[8] = {};
	LPD3DXMESH pMeshBoss = NULL;
	LPD3DXBUFFER pBuffMatBoss = NULL;					//頂点情報を格納
	DWORD dwNumMatBoss = 0;
	int nIdxModelParent;
}BossModel;

//プレイヤーの構造体
typedef struct
{
	D3DXVECTOR3 pos[MAX_BOSS_MODEL];
	D3DXVECTOR3 rot[MAX_BOSS_MODEL];
	int nFrame;
}KeyMotion;

//プレイヤーの構造体
typedef struct
{
	KeyMotion aKeyMotion[MAX_MOTIONBOSS_KEY];
	int nLoop;
	int nNumKey;
	int nKey;
}BossMotion;

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
	BOSSSTATE state;
	BOSSMOTION motionState;
	BOSSMOTION motionStateOld;
	BossModel aBossModel[MAX_BOSS_MODEL];
	BossMotion aBossMotion[BOSSMOTION_MAX];
	int nNumModel;
	int nLife;
	int nTimerState;
	float fJump;
	float fMove;
	float fAngle;
}Boss;

//プロトタイプ宣言
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
Boss *GetBoss(void);
bool CollisionBoss(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, int nType);
void ReadFileBoss(void);

#endif // !_BOSS_H_