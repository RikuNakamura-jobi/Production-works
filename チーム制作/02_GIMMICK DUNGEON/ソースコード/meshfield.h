//==================================================================================================
//
//meshfield.h
//Author:髙山桃也
//
//==================================================================================================

#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//マクロ定義
#define FIELD_RANGE					(30.0f)									//メッシュの一辺の長さ
#define MESH_U						(100)											//横のブロック数
#define MESH_V						(100)											//縦のブロック数
#define MESH_TUTORIAL_U				(50.0f)											//横のブロック数
#define MESH_TUTORIAL_V				(50.0f)											//縦のブロック数

//構造体宣言
typedef struct
{
	D3DXVECTOR3 pos;						//位置
	D3DXVECTOR3 rot;						//向き
	D3DXMATRIX mtxWorld;					//ワールドマトリックス
	bool bUse;								//使用状況
	int nNumIdx;							//インデックス数
	int nNumVtx;							//頂点数
}MeshField;

//プロトタイプ宣言
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
void UpdateMeshFieldRanking(float moveX);
void SetMeshField(D3DXVECTOR3 pos);

#endif