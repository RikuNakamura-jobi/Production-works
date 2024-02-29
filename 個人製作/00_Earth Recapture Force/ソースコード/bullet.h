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
	BULLETTYPE_PLAYER = 0,			//�v���C���[�̒ʏ�e
	BULLETTYPE_ENEMY,				//�G�̒ʏ�e
	BULLETTYPE_HOMING,				//�v���C���[�̃z�[�~���O�e
	BULLETTYPE_ENEMY_HOMING,		//�G�̃z�[�~���O�e
	BULLETTYPE_MAX
}BULLETTYPE;

//�v���g�^�C�v�錾
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type, D3DXCOLOR col, int nEnemyType);

#endif // !_BULLET_H_