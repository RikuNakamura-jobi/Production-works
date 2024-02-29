//===========================
//
//�_�C���N�gX.enemy�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//�}�N����`
#define MAX_ENEMY (256)

//�G�̎��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,			//�ʏ���
	ENEMYSTATE_DAMAGE,				//�_���[�W���
	ENEMYSTATE_MAX
}ENEMYSTATE;

//�G�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 move;				//�ړ���
	D3DXVECTOR3 rot;				//����
	float fLength;					//�Ίp���̒���
	float fAngle;					//�Ίp���̊p�x
	float fLengthMove;				//�Ίp���̈ړ���
	int nType;						//���
	int nMoveType;					//�ړ��̎��
	int nMoveTime;					//�ړ��̎���
	int nLife;						//�̗�
	ENEMYSTATE state;				//���
	int nCounterState;				//��ԊǗ��J�E���^�[
	bool bUse;						//�g�p���Ă��邩�ǂ���
}Enemy;

//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType, int nMoveType, int nMoveTime);
Enemy *GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);
int GetNumEnemy(void);
void CollisionPlayerENEMY(int nCntEnemy);
int GetBulletCount(void);

#endif // !_ENEMY_H_