//===========================
//
//ダイレクトX.modelヘッダー
//Author:中村　陸
//
//===========================
#ifndef _MODEL_H_
#define _MODEL_H_

//敵の種類
typedef enum
{
	MODELSTATE_NORMAL = 0,			//通常状態
	MODELSTATE_DAMAGE,				//ダメージ状態
	MODELSTATE_MAX
}MODELSTATE;

//プレイヤーの構造体
typedef struct
{
	LPDIRECT3DTEXTURE9 apTextureModel[8] = {};
	LPD3DXMESH pMeshModel = NULL;
	LPD3DXBUFFER pBuffMatModel = NULL;					//頂点情報を格納
	DWORD dwNumMatModel = 0;
	D3DXVECTOR3 vtxMinModel;
	D3DXVECTOR3 vtxMaxModel;
}ModelX;

//プレイヤーの構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 point;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 vtxMinModel;
	D3DXVECTOR3 vtxMaxModel;
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	MODELSTATE state;
	D3DXCOLOR col[8];
	ModelX aModelX;
	int Parent;
	int nTimer;
	int nLife;
	int nType;
	int nTimerState;
	float fLengthMin;
	float fLengthMax;
	bool bUse;
}Model;

//プロトタイプ宣言
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
void SetModel(void);
Model *GetModel(void);
bool CollisionModel(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, int nType);
void ReadFileModel(void);

#endif // !_MODEL_H_