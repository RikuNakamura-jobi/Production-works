//===========================
//
//ダイレクトX.playerヘッダー
//Author:中村　陸
//
//===========================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//マクロ定義
#define MAX_PLAYER_MODEL (32)		//モデルの最大数
#define MAX_PLAYER_KEY (32)			//キーの最大数
#define PLAYER_RADIUS	(30.0f)		//当たり判定の半径

//環境状態
typedef enum
{
	PLAYERENVIRONMENT_NORMAL = 0,
	PLAYERENVIRONMENT_CLOUD,
	PLAYERENVIRONMENT_MAX
}PLAYERENVIRONMENT;

//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_APPEAR = 0,			//出現状態
	PLAYERSTATE_NORMAL,				//通常状態
	PLAYERSTATE_MOVE,				//移動状態
	PLAYERSTATE_DAMAGE,				//ダメージ状態
	PLAYERSTATE_DEATH,				//死亡状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤーのモーション
typedef enum
{
	PLAYERMOTION_NORMAL = 0,		//待機モーション
	PLAYERMOTION_MOVE,				//移動モーション
	PLAYERMOTION_PUSH,				//アクションモーション
	PLAYERMOTION_JUMP,				//ジャンプモーション
	PLAYERMOTION_LAND,				//着地モーション
	PLAYERMOTION_AIR,				//空中モーション
	PLAYERMOTION_DAMAGE,			//ダメージモーション
	PLAYERMOTION_DEATH,				//死亡モーション
	PLAYERMOTION_TIMEUP,			//時間切れモーション
	PLAYERMOTION_EASTEREGG0,		//☆ゲッ☆ダン☆モーション
	PLAYERMOTION_EASTEREGG1,		//ゲッダンゆ～れるま～わるふ～れるせつな～い気持ち～モーション
	PLAYERMOTION_EASTEREGG2,		//ゲッダン腰横振りモーション
	PLAYERMOTION_EASTEREGG3,		//ゲッダン腰振りモーション
	PLAYERMOTION_EASTEREGG4,		//ゲッダンﾋﾟﾀｯモーション
	PLAYERMOTION_EASTEREGG5,		//ゲッダンオペラっぽいやつモーション
	PLAYERMOTION_EASTEREGG6,		//ゲッダンｽﾝｯモーション
	PLAYERMOTION_MAX
}PLAYERMOTION;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posMotion;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 rotMotion;
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	int nIdxModelParent;
}PlayerModel;

//キーの構造体
typedef struct
{
	D3DXVECTOR3 pos[MAX_PLAYER_MODEL];
	D3DXVECTOR3 rot[MAX_PLAYER_MODEL];
	int nFrame;
}KeyMotionPlayer;

//モーションの構造体
typedef struct
{
	KeyMotionPlayer aKeyMotion[MAX_PLAYER_KEY];
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
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	PLAYERSTATE state;
	PLAYERMOTION motionState;
	PLAYERMOTION motionStateOld;
	PlayerModel aPlayerModel[MAX_PLAYER_MODEL];
	PlayerMotion aPlayerMotion[PLAYERMOTION_MAX];
	PLAYERENVIRONMENT environment;
	D3DXMATERIAL pMatDef;
	int nNumModel;
	int nLife;
	int nMaxLife;
	int nTimerState;
	int nIdxLifeGauge;	//ゲージの番号
	int nIdxShadow;	//影の番号
	int nIdxBillboard;					//ビルボードの番号
	int nEasterTimer;
	float fJump;
	float fMove;
	float fAngle;
	float fJumpPow;
	bool bUse;
	bool bJump;
	bool bGod;
	bool bEaster;
}Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void UpdatePlayerRanking(void);
void DrawPlayer(void);
void DrawPlayerShadow(void);
void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
Player *GetPlayer(void);
void ReadFilePlayer(void);
void DeletePlayer(void);
void SetPosPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void HitPlayer(int nDamage);
PLAYERENVIRONMENT GetEnvironment(void);
void SoundPlayer(void);
void DebugPlayerMotion(void);

#endif // !_PLAYER_H_