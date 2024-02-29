//===========================
//
//ダイレクトX.modelヘッダー
//Author:中村　陸
//
//===========================
#ifndef _MODEL_H_
#define _MODEL_H_

//プレイヤーの構造体
typedef struct
{
	LPDIRECT3DTEXTURE9 apTextureModel[8] = {};
	LPD3DXMESH pMeshModel = NULL;
	LPD3DXBUFFER pBuffMatModel = NULL;					//頂点情報を格納
	DWORD dwNumMatModel = 0;
	D3DXVECTOR3 vtxMinModel;
	D3DXVECTOR3 vtxMaxModel;
	int nNumModel;
}ModelX;

//プレイヤーの構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	ModelX aModelX;
	int nType;
	float fLengthMin;
	float fLengthMax;
	bool bUse;
	bool bDraw;
}Model;

//プロトタイプ宣言
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nType);
void ResetModel(void);
void ResetModelTutorial(void);
Model *GetModel(void);
int CollisionModel(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, int nType);
void ReadFileModel(void);

#endif // !_MODEL_H_