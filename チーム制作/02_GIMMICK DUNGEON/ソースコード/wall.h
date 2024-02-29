//========================================
//
//3Dモデルの壁ポリゴン処理[wall.h]
//Author:Kai Takada
//
//========================================
#ifndef _WALL_H_								//このマクロ定義がされてなかったら
#define _WALL_H_								//二重インクルード防止のマクロを定義する

#define MAX_WALL (8)			//最大壁配置数

//壁の構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//向き
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	float fHeight;				//高さ
	float fWidth;				//幅
	float fDivX;				//テクスチャ分割数x
	float fDivY;				//テクスチャ分割数z
	D3DXVECTOR3 aVtxpos[4];		//四頂点の座標
	bool bUse;					//使用しているか
	int nIdx;					//インデックス
}Wall;

//プロトタイプ宣言
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);
void DeleteWall(int nIdx);
Wall *GetWall(void);
int GetUseWall(void);
bool CollisionWall(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, float fLength);
void UpdateWallRanking(float moveX);

#endif