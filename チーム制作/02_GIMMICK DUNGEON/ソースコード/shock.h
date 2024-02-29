//===========================
//
//�_�C���N�gX.shock�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _SHOCK_H_
#define _SHOCK_H_

//�}�N����`
#define NUM_TEX_SHOCK	(10)	//�e�N�X�`���̐�

//�v���C���[�̍\����
typedef struct
{
	LPDIRECT3DTEXTURE9 apTextureShock[NUM_TEX_SHOCK] = {};
	LPD3DXMESH pMeshShock = NULL;
	LPD3DXBUFFER pBuffMatShock = NULL;					//���_�����i�[
	DWORD dwNumMatShock = 0;
	D3DXVECTOR3 vtxMinShock;
	D3DXVECTOR3 vtxMaxShock;
}ShockX;

//�v���C���[�̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 vtxMinShock;
	D3DXVECTOR3 vtxMaxShock;
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	D3DXMATRIX mtxScale;		//�v�Z�p�}�g���b�N�X
	int nLife;
	float fRadiusMax;
	float fRadiusMin;
	float fRadiusMaxOld;
	float fRadiusMinOld;
	float fRadSpeed;
	bool bUse;
}Shock;

//�v���g�^�C�v�錾
void InitShock(void);
void UninitShock(void);
void UpdateShock(void);
void DrawShock(void);
void SetShock(D3DXVECTOR3 pos, int life, float speed);
Shock *GetShock(void);
bool CollisionShock(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot,bool bJump);

#endif // !_SHOCK_H_