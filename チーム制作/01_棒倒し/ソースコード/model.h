//===========================
//
//�_�C���N�gX.model�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _MODEL_H_
#define _MODEL_H_

//�v���C���[�̍\����
typedef struct
{
	LPDIRECT3DTEXTURE9 apTextureModel[8] = {};
	LPD3DXMESH pMeshModel = NULL;
	LPD3DXBUFFER pBuffMatModel = NULL;					//���_�����i�[
	DWORD dwNumMatModel = 0;
	D3DXVECTOR3 vtxMinModel;
	D3DXVECTOR3 vtxMaxModel;
	int nNumModel;
}ModelX;

//�v���C���[�̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	ModelX aModelX;
	int nType;
	float fLengthMin;
	float fLengthMax;
	bool bUse;
	bool bDraw;
}Model;

//�v���g�^�C�v�錾
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