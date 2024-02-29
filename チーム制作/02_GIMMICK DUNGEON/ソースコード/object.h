//========================================
//
//2Dシューティングゲーム(アイテム取得処理)[getitem.h]
//Author:佐久間優香
//
//========================================
#ifndef _OBJECT_H_							//このマクロ定義がされていなかったら
#define _OBJECT_H_							//2重インクルード防止のマクロを定義するv

#include"main.h"

//====================
// マクロ定義
//====================
#define MAX_OBJECT	(120)	//モデルの最大数
#define NUM_TEX_OBJECT	(10)	// テクスチャの数

//===================
// 列挙型宣言
//===================
//オブジェクトの種類
typedef enum
{
	OBJECTTYPE_FIRST = 0,					//花
	OBJECTTYPE_CATAPULT,					//発射台
	OBJECTTYPE_REFLECTOR,					//反射板
	OBJECTTYPE_REFLECTOR_BASE,				//反射板の台座
	OBJECTTYPE_LEVER_BAR,					//風制御のレバーの棒
	OBJECTTYPE_LEVER_BASE,					//風制御のレバーの支点
	OBJECTTYPE_TORCH,						//松明
	OBJECTTYPE_BUTTON,						//ボタン
	OBJECTTYPE_SANDPOLE,					//砂の柱
	OBJECTTYPE_SANDPOLE_BASE,				//砂の柱の台座
	OBJECTTYPE_STONEPOLE,					//石柱
	OBJECTTYPE_DOOR,						//扉
	OBJECTTYPE_BOOK,						//本
	OBJECTTYPE_ROCK_FRAG,					//岩の破片
	OBJECTTYPE_RUBBLE,						//がれき
	OBJECTTYPE_STATUE_0,					//銅像0
	OBJECTTYPE_DEBRIS,						//残骸
	OBJECTTYPE_SKULL,						//頭蓋骨
	OBJECTTYPE_SKULLMT,						//頭蓋骨の山
	OBJECTTYPE_STATUE_1,					//壊れ？銅像1
	OBJECTTYPE_STATUE_3,					//ルネサンス銅像3
	OBJECTTYPE_STATUE_4,					//座った銅像4
	OBJECTTYPE_STATUE_5,					//座った銅像5
	OBJECTTYPE_STATUE_6,					//座った銅像6
	OBJECTTYPE_STATUE_7,					//座った銅像7
	OBJECTTYPE_STATUE_8,					//座った銅像8
	OBJECTTYPE_SWORD,						//地面に刺さった剣
	OBJECTTYPE_TREASUREBOX,					//宝箱
	OBJECTTYPE_STATUE_100,					//現場猫銅像
	OBJECTTYPE_STATUE_101,					//ジャックオー銅像
	OBJECTTYPE_BUTTON_POLE,					//石柱落下ボタン
	OBJECTTYPE_MAX
}OBJECTTYPE;

//色の種類
typedef enum
{
	COLTYPE_NORMAL = 0,						//通常時
	COLTYPE_DAMAGE,							//ダメージ時
	COLTYPE_DEATH,							//死亡時
	COLTYPE_EDIT,							//エディット時
	COLTYPE_DELETETARGET,					//削除選択時
	COLTYPE_MAX,
}COLTYPE;

//オブジェクトの状態
typedef enum
{
	OBJECTSTATE_NORMAL = 0,					//通常
	OBJECTSTATE_DAMEGE,						//ダメージ状態
	OBJECTSTATE_DEATH,						//死亡状態
	OBJECTSTATE_MAX
}BLOCKSTATE;

//====================
// 構造体定義
//===================
//Xファイル構造体の定義
typedef struct
{
	LPD3DXMESH pMesh;									//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;								//マテリアルへのポインタ
	DWORD dwNumMat;										//マテリアルの数	
	D3DXVECTOR3 vtxMin, vtxMax;							//最大値、最小値	
	LPDIRECT3DTEXTURE9 apTexture[10];					//テクスチャへのポインタ
}Xfile;

//オブジェクト構造体
typedef struct
{
	D3DXVECTOR3 pos;									//位置
	D3DXVECTOR3 posOld;									//前回の位置
	D3DXVECTOR3 move;									//移動量
	D3DXVECTOR3 rot;									//向き
	D3DXVECTOR3 rotBase;								//向きの基準(レバー用)
	D3DXVECTOR3 rotate;									//回転量
	D3DXVECTOR3 vtxMin;									//最小
	D3DXVECTOR3 vtxMax;									//最大
	D3DXVECTOR3 point[4];								//判定の基準点
	D3DXVECTOR3 WindMove;								//風の移動量
	D3DXVECTOR3 posBlock;								//初期位置
	D3DXVECTOR3 posCollision;							//当たり判定のオフセット
	D3DXMATRIX mtxWorld;								//ワールドマトリックス
	D3DXMATRIX mtxWorldCollision;						//当たり判定のワールドマトリックス
	int nType;											//種類
	bool bUse;											//使用しているかどうか
	int nCounter;										//岩が出るようのカウンター
	int nCounterObject;									//カウンター	
	BLOCKSTATE state;									//状態
	int nCounterState;									//状態管理カウンター
	D3DXMATERIAL RedMat;								//マテリアルデータへのポインタ
	int nLife;											//体力
	int nIdxEdit;										//エディットの番号
	int nIdxBillboard;									//ビルボードの番号
	int nIdxshadow;										//影の番号
	bool bBillboard;									//ビルボードを表示しているかどうか
	int RockCounter;									//カウンター
	int nCounterEff;									//エフェクトを出すカウンター
	int nCntSound;										//音を出すカウンター
	float fScale;										//スケール
}Object;

//====================
// プロトタイプ宣言
//====================
//基本処理
void InitObject(void);
void UninitObject(void);
void UpdateObject(void);
void DrawObject(void);
void UpdateObjectRanking(float moveX);

//当たり判定
bool CollisionObject2(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, bool bJump);
bool CollisionRockInvisible(D3DXVECTOR3 *pos, float *vtxMax);
void HitObject(int nDamage, int nCntObject);
bool CollisionReflector(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, float fLength, int type);
bool PushReflector(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, float *rot, bool jump);

//情報取得処理
Object *ObjectGet(void);
LPD3DXMESH ObjectMeshGet(OBJECTTYPE type);
LPD3DXBUFFER ObjectBuffmatGet(OBJECTTYPE type);
DWORD ObjectdwNummatGet(OBJECTTYPE type);
int GetUseObject(void);
float LengthPointToLine(D3DXVECTOR3 point, D3DXVECTOR3 line1, D3DXVECTOR3 line2);

//設定処理
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
void DeleteObject(int nIdx);
void SetPoint(int nNum);
void SetBreakObj(D3DXVECTOR3 pos, int nNum);

//その他
void WindMove(int nCntRock);

//デバッグ表示
void DebugObject(void);

#endif