//===========================
//
//�_�C���N�gX.bullet�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//�e�̎��
typedef enum
{
	BULLETTYPE_0 = 0,			//�v���C���[�̒ʏ�e
	BULLETTYPE_1,				//�G�̒ʏ�e
	BULLETTYPE_2,				//�v���C���[�̃z�[�~���O�e
	BULLETTYPE_3,				//�G�̃z�[�~���O�e
	BULLETTYPE_4,				//�G�̃z�[�~���O�e
	BULLETTYPE_5,				//�G�̃z�[�~���O�e
	BULLETTYPE_6,				//�G�̃z�[�~���O�e
	BULLETTYPE_7,				//�G�̃z�[�~���O�e
	BULLETTYPE_8,				//�G�̃z�[�~���O�e
	BULLETTYPE_MAX
}BULLETTYPE;

//�e�̏��
typedef enum
{
	BULLETSTATE_NORMAL = 0,			
	BULLETSTATE_WAIT,
	BULLETSTATE_HIT,
	BULLETSTATE_MAX
}BULLETSTATE;

//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;												//�ʒu
	D3DXVECTOR3 move;												//�ړ���
	D3DXVECTOR3 rot;												//����
	int nLife;														//����
	BULLETTYPE type;												//�e�̃^�C�v
	D3DXCOLOR col;													//�F
	bool bUse;														//�g�p���Ă��邩�ǂ���
}Bullet;

//�v���g�^�C�v�錾
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type, D3DXCOLOR col, int nEnemyType);
void CollisionPlayer(Bullet *pBullet);
Bullet *GetBullet(void);
BULLETSTATE GetBulletState(void);
void SetBulletState(BULLETSTATE bulletState);
int *GetBulletHit(void);
int GetBulletScore(void);

#endif // !_BULLET_H_