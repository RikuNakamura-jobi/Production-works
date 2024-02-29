//==========================================================
//
//���̉_���� [cloud.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _START_H_
#define _START_H_

#include "main.h"

//==========================================================
//�\���̂̒�`
//==========================================================
//�|���S����b���
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	float fWidth;			//��
	float fHeight;			//����
	float fTexU;			//�e�N�X�`�����W
}Start;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitStart(void);
void UninitStart(void);
void UpdateStart(void);
void DrawStart(void);

#endif

