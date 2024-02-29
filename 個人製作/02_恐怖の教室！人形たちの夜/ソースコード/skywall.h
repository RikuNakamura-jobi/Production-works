//===========================
//
//ダイレクトX.skywallヘッダー
//Author:中村　陸
//
//===========================
#ifndef _SKYWALL_H_
#define _SKYWALL_H_

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
}Skywall;

//プロトタイプ宣言
void InitSkywall(void);
void UninitSkywall(void);
void UpdateSkywall(void);
void DrawSkywall(void);
bool CollisionSkywall(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move);

#endif // !_SKYWALL_H_