//==========================================================
//
//�|���S������ [polygon.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

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
}polygon;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void DrawPolygon(void);

#endif

