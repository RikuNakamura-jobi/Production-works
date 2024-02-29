//========================================
//
//2Dシューティングゲーム(アイテム取得処理)[getitem.h]
//Author:佐久間優香
//
//========================================
#ifndef _MESHWALL_H_							//このマクロ定義がされていなかったら
#define _MESHWALL_H_							//2重インクルード防止のマクロを定義する

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;									//位置
	D3DXVECTOR3 rot;									//移動量
	D3DXMATRIX mtxWorld;								//ワールドマトリックス
	bool bUse;
}MeshWall;

//プロトタイプ宣言
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
MeshWall *GetMeshWall(void);

#endif