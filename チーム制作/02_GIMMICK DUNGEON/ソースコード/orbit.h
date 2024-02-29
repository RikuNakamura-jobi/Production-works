//==========================================
//
//  軌跡の生成(orbit.h)
//  Author : 髙山　桃也
//
//==========================================
#ifndef _ORBIT_H_
#define _ORBIT_H_
#include "main.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_ORBIT	(18)	//軌跡の最大数
#define MAX_EDGE	(500)	//辺の最大数
#define NUM_OFFSET	(2)	//オフセットの数

//==========================================
//  構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 posOffset[NUM_OFFSET];	//オフセット位置
	D3DXVECTOR3 aPosPoint[MAX_EDGE][NUM_OFFSET];	//頂点座標保存用
	D3DXCOLOR col;	//色
	D3DXCOLOR aColPoint[MAX_EDGE][NUM_OFFSET];	//頂点カラー保存用
	D3DXMATRIX mtxWorld[NUM_OFFSET];	//オフセットのマトリックス
	bool bUse;	//使用しているかどうか
	int nNumEdge;	//辺の数
}Orbit;

//==========================================
//  プロトタイプ宣言
//==========================================
//基本処理
void InitOrbit(void);
void UninitOrbit(void);
void UpdateOrbit(void);
void DrawOrbit(void);

//更新処理
void UpdateOrbitPolygon(int nCntOrbit);

//設定処理
void SetPositionOffset(D3DXMATRIX mtxWorld,int nIdxOrbit);
int SetOrbit(D3DXMATRIX mtxWorld,D3DXVECTOR3 posOffset1, D3DXVECTOR3 posOffset2, D3DXCOLOR col,int nNumEdge);

//情報取得処理
Orbit *GetOrbit(void);

//軌跡消去処理
void EnableOrbit(int nIdxOrbit);

#endif