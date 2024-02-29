//===========================
//
//�_�C���N�gX.start�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _START_H_
#define _START_H_

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
}Start;

//�v���g�^�C�v�錾
void InitStart(void);
void UninitStart(void);
void UpdateStart(void);
void DrawStart(void);
void SetStart(D3DXVECTOR3 pos, int nType);
bool CollisionStart(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fAngle, float fLength, float fRot, Start **pStart);

#endif // !_START_H_