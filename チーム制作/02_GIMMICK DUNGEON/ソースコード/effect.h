//==================================================================================================
//
//Effect.h
//Author:���R����
//
//==================================================================================================

#ifndef _EFFECT_H_
#define _EFFECT_H_

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
	D3DXMATRIX mtxWorld;							//���[���h�}�g���b�N�X
	D3DXCOLOR col;									//�F
	float fRadius;									//���a
	int nLife;										//����
	int nMaxLife;									//�ő����
	bool bAdd;										//���Z�������邩�ǂ���
	float fGrav;									//�d�͂ɉe������邩�ǂ���
	bool bUse;										//�g�p���Ă��邩�ǂ���
	bool bZtest;									//Z�e�X�g���邩�ǂ���
}Effect;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 *pPosOwner, D3DXVECTOR3 move,D3DXCOLOR col,float fRadius,int nLife,bool Add, float fGrav, bool bZtest);
Effect *GetEffect(void);
void DebugEffect(void);

#endif