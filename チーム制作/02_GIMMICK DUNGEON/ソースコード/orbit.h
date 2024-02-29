//==========================================
//
//  �O�Ղ̐���(orbit.h)
//  Author : ���R�@����
//
//==========================================
#ifndef _ORBIT_H_
#define _ORBIT_H_
#include "main.h"

//==========================================
//  �}�N����`
//==========================================
#define MAX_ORBIT	(18)	//�O�Ղ̍ő吔
#define MAX_EDGE	(500)	//�ӂ̍ő吔
#define NUM_OFFSET	(2)	//�I�t�Z�b�g�̐�

//==========================================
//  �\���̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 posOffset[NUM_OFFSET];	//�I�t�Z�b�g�ʒu
	D3DXVECTOR3 aPosPoint[MAX_EDGE][NUM_OFFSET];	//���_���W�ۑ��p
	D3DXCOLOR col;	//�F
	D3DXCOLOR aColPoint[MAX_EDGE][NUM_OFFSET];	//���_�J���[�ۑ��p
	D3DXMATRIX mtxWorld[NUM_OFFSET];	//�I�t�Z�b�g�̃}�g���b�N�X
	bool bUse;	//�g�p���Ă��邩�ǂ���
	int nNumEdge;	//�ӂ̐�
}Orbit;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
//��{����
void InitOrbit(void);
void UninitOrbit(void);
void UpdateOrbit(void);
void DrawOrbit(void);

//�X�V����
void UpdateOrbitPolygon(int nCntOrbit);

//�ݒ菈��
void SetPositionOffset(D3DXMATRIX mtxWorld,int nIdxOrbit);
int SetOrbit(D3DXMATRIX mtxWorld,D3DXVECTOR3 posOffset1, D3DXVECTOR3 posOffset2, D3DXCOLOR col,int nNumEdge);

//���擾����
Orbit *GetOrbit(void);

//�O�Տ�������
void EnableOrbit(int nIdxOrbit);

#endif