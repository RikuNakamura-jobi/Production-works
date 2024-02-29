//===========================
//
//�_�C���N�gX.wall�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _WALL_H_
#define _WALL_H_

//�e�̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	float fAngle;
	float fLength;
	float fRadiusX;
	float fRadiusY;
	float fRadiusZ;
	bool bUse;
}Wall;

//�v���g�^�C�v�錾
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

#endif // !_WALL_H_