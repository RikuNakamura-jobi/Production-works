//===========================
//
//�_�C���N�gX.goal�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _GOAL_H_
#define _GOAL_H_

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
}Goal;

//�v���g�^�C�v�錾
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
void SetGoal(D3DXVECTOR3 pos, int nType);
bool CollisionGoal(D3DXVECTOR3 *pPos);

#endif // !_Goal_H_