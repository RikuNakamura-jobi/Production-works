//===========================
//
//ダイレクトX.meshwallヘッダー
//Author:中村　陸
//
//===========================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

//影の構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posVec[2];
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	int Numx;
	int Numy;
	int NumVtx;
	int NumIdx;
	float fWallLength;
}Meshwall;

//プロトタイプ宣言
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
bool CollisionMeshWall(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move);

#endif // !_MESHWALL_H_