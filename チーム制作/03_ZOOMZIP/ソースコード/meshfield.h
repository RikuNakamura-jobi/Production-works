//==========================================================
//
//メッシュフィールド処理 [meshfield.h]
//Author 奥定伊吹
//
//==========================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "polygon.h"
#include "main.h"
#include <stdio.h>

//==========================================================
//構造体の定義
//==========================================================
//メッシュフィールド
typedef struct
{
	polygon Polygon;		//ポリゴン基礎情報
	int nHeightMesh;		//高さ枚数
	int nWidthMesh;			//幅枚数
	int nVertex;			//頂点数
	int nIndex;				//描画番号数
	bool bUse;				//使用されているかどうか
	int nTextype;			//テクスチャ番号
}MeshField;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth, int nType);
void SetIndexMeshField(int nCntField);
void SetpVtxMeshField(int nCntField);
void LoadMeshFieldData(FILE *pFile);
void CreateFieldBuff(void);
bool CollisionMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);

#endif