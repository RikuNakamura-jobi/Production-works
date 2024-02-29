//==========================================================
//
//Xファイルモデル処理 [model.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"	//main.hで定義しているものが必要なためinclude
#include <stdio.h>

//==========================================================
//マクロ定義
//==========================================================
#define MAX_MODELTEX	(64)		//Xファイルテクスチャ格納最大数
#define MAX_MODEL		(1024)		//配置できるモデルの最大数
#define MAX_MODELFILE	(256)		//読み込めるモデルの最大数
#define CATCH_LENGTH	(115.0f)	//引くの判定範囲
#define GRAB_HEIGHT		(100.0f)	//掴む高さ
#define GRAB_LENGTH		(150.0f)	//掴める高さ

//==========================================================
//列挙型の定義
//==========================================================
//モデルタイプ
typedef enum
{
	MODELTYPE_OBJECT = 0,		//障害物
	MODELTYPE_BLOCK,			//押し引き可能
	MODELTYPE_RAIL,				//レール
	MODELTYPE_RACK,				//足場
	MODELTYPE_DECO,				//飾り
	MODELTYPE_ITEM,				//アイテム
	MODELTYPE_CONTAINER,		//コンテナ
	MODELTYPE_BOX,				//箱入りコンテナ
	MODELTYPE_LIFTUP,			//搬入装置
	MODELTYPE_CRANECTRL,		//クレーン操作盤
	MODELTYPE_MAX
}MODELTYPE;

//モデルタイプ
typedef enum
{
	WEIGHT_LIGHT = 0,		//軽い(重さ1)
	WEIGHT_HEAVY,			//重い(重さ2)
	WEIGHT_OBJECT,			//動かない
	WEIGHT_MAX
}WEIGHT;

//==========================================================
//構造体の定義
//==========================================================
//モデルファイルデータ
typedef struct
{
	LPD3DXMESH pMesh = NULL;			//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat = NULL;		//マテリアルへのポインタ
	LPDIRECT3DTEXTURE9 apTexture[MAX_MODELTEX] = {};	//テクスチャのポインタ
	DWORD dwNumMat = 0;					//マテリアルの数
	D3DXVECTOR3 vtxMin;					//最小値
	D3DXVECTOR3 vtxMax;					//最大値
	int nType;							//タイプ
}ModelFileData;

//モデル情報(model.cpp)
typedef struct
{
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 posOld;					//前回の位置
	D3DXVECTOR3 rot;					//向き
	float fMoveY;						//重力
	D3DXMATRIX mtxWorld;				//ワールドマトリックス
	ModelFileData *pModelFiledata;		//モデルファイルデータのポインタ
	int nShadow;						//使用している影の番号
	int nType;							//種類
	bool bUse;							//使用しているかどうか
}Model;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
ModelFileData *GetModelData(int nModelType);
void LoadModelFile(int nMaxModelFile);
void LoadModelFileData(FILE *pFile);
void LoadModelData(FILE *pFile);
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nSetShadow);
Model *GetModel(void);
void DeleteModel(int nCntModel);

#endif