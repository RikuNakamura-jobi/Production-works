//===========================
//
//�_�C���N�gX.skywall�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _SKYWALL_H_
#define _SKYWALL_H_

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
}Skywall;

//�v���g�^�C�v�錾
void InitSkywall(void);
void UninitSkywall(void);
void UpdateSkywall(void);
void DrawSkywall(void);
bool CollisionSkywall(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move);

#endif // !_SKYWALL_H_