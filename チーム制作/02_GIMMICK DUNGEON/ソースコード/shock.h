//===========================
//
//ダイレクトX.shockヘッダー
//Author:中村　陸
//
//===========================
#ifndef _SHOCK_H_
#define _SHOCK_H_

//マクロ定義
#define NUM_TEX_SHOCK	(10)	//テクスチャの数

//プレイヤーの構造体
typedef struct
{
	LPDIRECT3DTEXTURE9 apTextureShock[NUM_TEX_SHOCK] = {};
	LPD3DXMESH pMeshShock = NULL;
	LPD3DXBUFFER pBuffMatShock = NULL;					//頂点情報を格納
	DWORD dwNumMatShock = 0;
	D3DXVECTOR3 vtxMinShock;
	D3DXVECTOR3 vtxMaxShock;
}ShockX;

//プレイヤーの構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 vtxMinShock;
	D3DXVECTOR3 vtxMaxShock;
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	D3DXMATRIX mtxScale;		//計算用マトリックス
	int nLife;
	float fRadiusMax;
	float fRadiusMin;
	float fRadiusMaxOld;
	float fRadiusMinOld;
	float fRadSpeed;
	bool bUse;
}Shock;

//プロトタイプ宣言
void InitShock(void);
void UninitShock(void);
void UpdateShock(void);
void DrawShock(void);
void SetShock(D3DXVECTOR3 pos, int life, float speed);
Shock *GetShock(void);
bool CollisionShock(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot,bool bJump);

#endif // !_SHOCK_H_