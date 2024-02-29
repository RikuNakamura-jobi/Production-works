//===========================
//
//ダイレクトX.meshfieldヘッダー
//Author:中村　陸
//
//===========================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//影の構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	int Numx;
	int Numy;
	int NumVtx;
	int NumIdx;
}MESHFIELD;

//プロトタイプ宣言
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
void SetMeshField(void);

#endif // !_MESHFIELD_H_