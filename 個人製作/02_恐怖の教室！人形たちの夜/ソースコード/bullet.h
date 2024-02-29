//===========================
//
//�_�C���N�gX.bullet�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _BULLET_H_
#define _BULLET_H_

//�e�̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXCOLOR col;
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	int nLife;
	int nType;
	int nIdxShadow;
	bool bTrue;
}Bullet;

//�v���g�^�C�v�錾
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 point, D3DXVECTOR3 rot , D3DXCOLOR col, int nLife, int nType);

#endif // !_BULLET_H_