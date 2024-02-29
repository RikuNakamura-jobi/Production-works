//==========================================================
//
//���̉_���� [cloud.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _CLOUD_H_
#define _CLOUD_H_

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
}Cloud;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitCloud(void);
void UninitCloud(void);
void UpdateCloud(void);
void DrawCloud(void);

#endif

