//===========================
//
//�_�C���N�gX.model�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _MODEL_H_
#define _MODEL_H_

//�G�̎��
typedef enum
{
	MODELSTATE_NORMAL = 0,			//�ʏ���
	MODELSTATE_DAMAGE,				//�_���[�W���
	MODELSTATE_MAX
}MODELSTATE;

//�v���C���[�̍\����
typedef struct
{
	LPDIRECT3DTEXTURE9 apTextureModel[8] = {};
	LPD3DXMESH pMeshModel = NULL;
	LPD3DXBUFFER pBuffMatModel = NULL;					//���_�����i�[
	DWORD dwNumMatModel = 0;
	D3DXVECTOR3 vtxMinModel;
	D3DXVECTOR3 vtxMaxModel;
}ModelX;

//�v���C���[�̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 point;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 vtxMinModel;
	D3DXVECTOR3 vtxMaxModel;
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
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

//�v���g�^�C�v�錾
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
void SetModel(void);
Model *GetModel(void);
bool CollisionModel(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, int nType);
void ReadFileModel(void);

#endif // !_MODEL_H_