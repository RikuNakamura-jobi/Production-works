//========================================
//
//モデル管理(model.h)
//Author:髙山桃也
//
//========================================
#ifndef _MODEL_H_							//このマクロ定義がされていなかったら
#define _MODEL_H_							//2重インクルード防止のマクロを定義するv

#include"main.h"

//*****************************************
// マクロ定義
//*****************************************
#define NUM_TEX	(10)	//テクスチャの数

//*****************************************
// 構造体定義
//*****************************************
typedef struct
{
	LPD3DXMESH pMesh;	//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;	//マテリアルへのポインタ
	DWORD dwNumMat;	//マテリアルの数
	LPDIRECT3DTEXTURE9 apTexture[NUM_TEX];	//テクスチャへのポインタ
	D3DXVECTOR3 vtxMin;	//最小頂点座標
	D3DXVECTOR3 vtxMax;	//最大頂点座標
}Model;

//*****************************************
// プロトタイプ宣言
//*****************************************
//基本処理
void InitModel(void);
void UninitModel(void);
void UninitXFile(Model *pModel);

//読み込み処理
void LoadModel(Model *pModel, const char *pFilePass);	//汎用読み込み処理
void LoadAllModel(void);	//読み込み処理まとめ
void LoadObjectModel(void);	//オブジェクト読み込み
void LoadPlayerModel(void);	//プレイヤー読み込み
void LoadEnemyModel(void);	//エネミー読み込み
void LoadBookModel(void);	//本読み込み

//情報取得
Model *GetModelobject(void);
Model *GetModelPlayer(void);
Model *GetModelEnemy(void);
Model *GetModelBook(void);
int GetNumPlayerPart(void);
int GetNumEnemyPart(void);

#endif