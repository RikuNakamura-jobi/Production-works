//===================================================
//
//階層構造によるモーション処理 [motion.h]
//Author Ibuki Okusada
//
//===================================================
#ifndef _MOITON_H_
#define _MOITON_H_

#include "main.h"	//main.hで定義しているものが必要なためinclude
#include "model.h"

//===================================================
//マクロ定義
//===================================================
#define MAX_PARTS	(32)	//パーツの最大数
#define MAX_KEY		(64)	//キーの最大数
#define MAX_MOTION	(64)	//モーションの最大数

//===================================================
//列挙型の定義
//===================================================
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	//待機モーション
	MOTIONTYPE_WALK,		//移動モーション
	MOTIONTYPE_ACTION,		//アクションモーション
	MOTIONTYPE_JUMP,		//ジャンプモーション
	MOTIONTYPE_LAND,		//着地モーション
	MOTIONTYPE_MAX
}MOTIONTYPE;

//===================================================
//構造体の定義
//===================================================
//キー
typedef struct
{
	float fPosX;
	float fPosY;
	float fPosZ;
	float fRotX;
	float fRotY;
	float fRotZ;
}KEY;

//キー情報
typedef struct
{
	int nFrame;					//再生フレーム
	KEY aKey[MAX_PARTS];		//パーツの総数
}KEY_INFO;

//モーション情報
typedef struct
{
	bool bLoop;					//ループするかどうか
	int nNumKey;				//キー数
	KEY_INFO aKeyInfo[MAX_KEY];	//キーの最大数
}MOTION_INFO;

//パーツの情報
typedef struct
{
	ModelFileData FileData;		//Xファイルデータ
	Model model;				//モデル情報
	D3DXVECTOR3 SetPos;			//初期設置座標
	D3DXVECTOR3 SetRot;			//初期設置向き
	int nIndex;					//自分のインデックス番号
	int nIdexParent;			//親のインデックス番号
}Parts;

//胴体ファイル情報
typedef struct
{
	MOTION_INFO aMotionInfo[MAX_MOTION];	//モーションの総数
	Parts aParts[MAX_PARTS];			//パーツの総数
	int nNumParts;						//パーツ数
}BodyFileData;

//半身情報
typedef struct
{
	BodyFileData FileData;	//ファイル情報
	KEY aSet[MAX_PARTS];		//パーツ使用情報
	D3DXMATRIX aSetMtx[MAX_PARTS];	//パーツ情報
	D3DXVECTOR3 pos;			//座標
	D3DXVECTOR3 rot;			//向き
	int nNowFrame;				//現在のフレーム数
	int nNowMotion;				//現在のモーション番号
	int nNowKey;				//現在のキー数
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
}Body;

//===================================================
//プロトタイプ宣言
//===================================================
void InitMotion(void);
void UninitMotion(Body *pBody);
void UpdateMotion(Body *pBody);
void SetMotion(Body *pBody, int nMotionType);

//外部ファイル読み込み
void OpenBodyFile(const char *pFileData, BodyFileData *pBodyFile);
void LoadBodyFile(FILE *pFile, BodyFileData *pBodyFile);
void LoadBodyXFileName(FILE *pFile, BodyFileData *pBodyFile);
void LoadPartsSetData(FILE *pFile, BodyFileData *pBodyFile);
void LoadPartsFileData(Parts *pParts, const char *pFileName);
void LoadMotionData(FILE *pFile, BodyFileData *pBodyFile, int nMotion);
void ResetBodyData(Body * pBody);
void ResetBodyFileData(Body * pBody);

#endif
