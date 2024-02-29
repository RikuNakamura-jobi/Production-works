//===========================
//
//�_�C���N�gX.meshfield�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//�e�̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	int Numx;
	int Numy;
	int NumVtx;
	int NumIdx;
}MESHFIELD;

//�v���g�^�C�v�錾
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
void SetMeshField(void);

#endif // !_MESHFIELD_H_