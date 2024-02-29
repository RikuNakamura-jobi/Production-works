//==========================================================
//
//メッシュウォール処理 [meshwall.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"
#include <stdio.h>
#include "polygon.h"

//==========================================================
//マクロ定義
//==========================================================
#define MAX_MESHWALL	(256)		//壁の最大数

//==========================================================
//壁の構造体
//==========================================================
typedef struct
{
	polygon Polygon;		//ポリゴン基礎情報
	int nHeightMesh;		//高さ枚数
	int nWidthMesh;			//幅枚数
	int nVertex;			//頂点数
	int nIndex;				//描画番号数
	int nStartIndex;		//開始インデックス番号
	bool bUse;				//使用されているかどうか
	int nTextype;			//テクスチャタイプ
}MeshWall;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth, int nType);
void SetIndexMeshWall(int nCntWall);
void SetpVtxMeshWall(int nCntWall);
MeshWall *GetMeshWall(void);
void LoadMeshWallData(FILE *pFile);
void CreateWallBuff(void);
void DrawResultMeshWall(void);

#endif