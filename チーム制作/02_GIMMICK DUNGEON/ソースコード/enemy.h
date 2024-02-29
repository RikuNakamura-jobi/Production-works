//===========================
//
//ダイレクトX.enemyヘッダー
//Author:中村　陸
//
//===========================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//マクロ定義
#define MAX_ENEMY_MODEL (64)
#define MAX_MOTION_KEY (64)
#define MAX_POINT (32)			//エネミーの移動ポイントの最大数
#define ENEMY_LIFE (500) //初期体力
#define MAX_PATTERN (8)			//エネミーの移動パターンの最大数
#define ENEMY_RADIUS	(350.0f)	//当たり判定の球の大きさ
#define ENEMY_SPEED (0.3f)				//速度補正の値

//エネミーの状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,			//通常状態
	ENEMYSTATE_SEARCH,				//索敵状態
	ENEMYSTATE_LOST,				//見失った状態
	ENEMYSTATE_FIND,				//発見状態
	ENEMYSTATE_TRACK,				//追跡状態
	ENEMYSTATE_SLEEP,				//睡眠状態
	ENEMYSTATE_STANDUP,				//起き上がり状態
	ENEMYSTATE_DAMAGE,				//ダメージ状態
	ENEMYSTATE_COUNTER,				//反撃状態
	ENEMYSTATE_JUMPATTACK,			//ジャンプ攻撃状態
	ENEMYSTATE_DEATH,				//死亡状態
	ENEMYSTATE_MAX
}ENEMYSTATE;

//エネミーの種類
typedef enum
{
	ENEMYMOTION_NORMAL = 0,			//ニュートラルモーション
	ENEMYMOTION_MOVE,				//移動モーション
	ENEMYMOTION_SHAKE,				//振り下ろしモーション
	ENEMYMOTION_PIERCE,				//突き刺しモーション
	ENEMYMOTION_STEP,				//足踏みモーション
	ENEMYMOTION_THROW,				//岩飛ばしモーション
	ENEMYMOTION_OVERLOOK,			//見渡しモーション
	ENEMYMOTION_DAMAGE,				//被弾モーション
	ENEMYMOTION_DOWN,				//ダウンモーション
	ENEMYMOTION_BACKJUMP,			//バックジャンプモーション
	ENEMYMOTION_JUMPATTACK,			//ジャンプ攻撃モーション
	ENEMYMOTION_FIND,				//発見モーション
	ENEMYMOTION_DEATH,				//死亡モーション
	ENEMYMOTION_JUMPATTACK_LIGHT,	//弱ジャンプ攻撃モーション
	ENEMYMOTION_SLEEP,				//休眠モーション
	ENEMYMOTION_STANDUP,			//立ち上がりモーション
	ENEMYMOTION_MAX
}ENEMYMOTION;

//エネミーの構造体
typedef struct
{
	D3DXVECTOR3 point[MAX_POINT];		//巡回地点
	int nNumPoint;
	int posCount = 0;					//移動先のポイント番号
}MovePattern;

//エネミーの構造体
typedef struct
{
	D3DXVECTOR3 pos[MAX_ENEMY_MODEL];
	D3DXVECTOR3 rot[MAX_ENEMY_MODEL];
	int nFrame;
}KeyMotion;

//エネミーの構造体
typedef struct
{
	KeyMotion aKeyMotion[MAX_MOTION_KEY];
	int nLoop;
	int nNumKey;
	int nKey;
}EnemyMotion;

//エネミーのモデル構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posMotion;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 rotMotion;
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	LPDIRECT3DTEXTURE9 apTextureEnemy[8] = {};
	LPD3DXMESH pMeshEnemy = NULL;
	LPD3DXBUFFER pBuffMatEnemy = NULL;					//頂点情報を格納
	DWORD dwNumMatEnemy = 0;
	D3DXVECTOR3 vtxMinEnemy;
	D3DXVECTOR3 vtxMaxEnemy;
	int nIdxModelParent;
	char cModelName[128];
}EnemyX;

//エネミーの構造体
typedef struct
{
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 posOld;					//前の位置
	D3DXVECTOR3 move;					//移動量
	D3DXVECTOR3 rot;					//角度
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	D3DXMATRIX mtxWorld;				//ワールドマトリックス
	D3DXVECTOR3 pointMove;				//移動地点
	ENEMYSTATE state;					//状態
	ENEMYSTATE stateOld;					//状態
	ENEMYMOTION motionState;
	ENEMYMOTION motionStateOld;
	EnemyMotion aEnemyMotion[ENEMYMOTION_MAX];
	EnemyX aEnemyX[MAX_ENEMY_MODEL];						//モデル情報
	MovePattern MovePattern[MAX_PATTERN];
	int nTimerMove;						//移動タイマー
	int nFindCount;						//発見時間
	int nAttackCount;					//攻撃移行時間
	int nLife;							//体力
	int nNumModel;						//モデルの数
	int nIdxGauge;						//ゲージの番号
	int nIdxShadow;						//影の番号
	int nIdxOrbit;						//軌跡の番号
	int nCntKey;
	int nCntFrame;
	int nNumPattern;
	int nPattern;
	int nAttackTime;
	int nHitRock;
	float vecL;							//左の外積
	float vecR;							//右の外積
	float fLength;						//プレイヤーとの距離
	float fSpeed;						//エネミーの速さ
	bool bFind;							//発見フラグ
	bool bUse;							//使用フラグ
	bool bDown;							//使用フラグ
	bool bDeath;						//死亡フラグ
}Enemy;

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nType);
Enemy *GetEnemy(void);
bool CollisionEnemy(D3DXVECTOR3 *posMin, D3DXVECTOR3 *posMax);
bool CollisionEnemyAttack(D3DXVECTOR3 *pos, D3DXVECTOR3 *rot);
bool CollisionEnemyPoint(D3DXVECTOR3 *pos);
void ReadFileEnemy(void);
void SetPosEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void HitEnemy(int nDamage, int nCntEnemy);
bool GetEnemyUse(void);
float OuterProduct(D3DXVECTOR3 vecA, D3DXVECTOR3 vecB, D3DXVECTOR3 vecC);
void DrawEnemyDebug(void);
void DebugEnemyPos(void);
void DebugEnemyPoint(void);
void DebugEnemyOuter(void);
void DebugEnemyFind(void);
void DebugEnemyLength(void);
void DebugEnemyRot(void);
void DebugEnemyMotion(void);
void DebugEnemyState(void);

#endif // !_ENEMY_H_