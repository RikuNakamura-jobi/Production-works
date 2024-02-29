//===========================
//
//�_�C���N�gX.meshwall�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

//�e�̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posVec[2];
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	int Numx;
	int Numy;
	int NumVtx;
	int NumIdx;
	float fWallLength;
}Meshwall;

//�v���g�^�C�v�錾
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
bool CollisionMeshWall(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move);

#endif // !_MESHWALL_H_