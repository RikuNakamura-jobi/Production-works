//===========================
//
//ダイレクトX.wallヘッダー
//Author:中村　陸
//
//===========================
#ifndef _WALL_H_
#define _WALL_H_

//影の構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	float fAngle;
	float fLength;
	float fRadiusX;
	float fRadiusY;
	float fRadiusZ;
	bool bUse;
}Wall;

//プロトタイプ宣言
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

#endif // !_WALL_H_