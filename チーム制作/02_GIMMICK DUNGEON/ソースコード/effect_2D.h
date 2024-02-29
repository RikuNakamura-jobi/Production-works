//==================================================================================================
//
//effect_2D.h
//Author:���R����		edit: ���c����
//
//==================================================================================================

#ifndef _EFFECT_2D_H_
#define _EFFECT_2D_H_

#include"main.h"

//==========================================
//  �\���̐錾
//==========================================
//�G�t�F�N�g�̍\����
typedef struct
{
	D3DXVECTOR3 pos;								//�ʒu
	D3DXVECTOR3 relPos;								//�����傩��̑��Έʒu
	D3DXVECTOR3 *pPosOwner = NULL;					//������̈ʒu�̃|�C���^
	D3DXVECTOR3 move;								//�ړ���
	D3DXCOLOR col;									//�F
	float fRadius;									//���a
	int nLife;										//����
	int nMaxLife;									//�ő����
	bool bAdd;										//���Z�������邩�ǂ���
	float fGrav;									//�d�͂ɉe������邩�ǂ���
	bool bUse;										//�g�p���Ă��邩�ǂ���
}Effect2D;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitEffect2D(void);
void UninitEffect2D(void);
void UpdateEffect2D(void);
void DrawEffect2D(void);
void SetEffect2D(D3DXVECTOR3 pos, D3DXVECTOR3 *pPosOwner, D3DXVECTOR3 move,D3DXCOLOR col,float fRadius,int nLife,bool Add, float fGrav);
Effect2D *GetEffect2D(void);
#endif