//===========================
//
//�_�C���N�gX.coin�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _COIN_H_
#define _COIN_H_

#include "main.h"

//�G�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 posOld;				//�ʒu
	D3DXVECTOR3 move;				//�ړ���
	int nType;
	float fHeight;					//����
	float fWidth;					//��
	bool bUse;						//�g�p���Ă��邩�ǂ���
}Coin;

//�v���g�^�C�v�錾
void InitCoin(void);
void UninitCoin(void);
void UpdateCoin(void);
void DrawCoin(void);
void SetCoin(D3DXVECTOR3 pos, int nType);
bool CollisionCoin(D3DXVECTOR3 *pPos);
int GetNumCoin(void);

#endif // !_COIN_H_