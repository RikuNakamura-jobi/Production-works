//===========================
//
//�_�C���N�gX.block�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//�v���C���[�̎��
typedef enum
{
	BLOCKTYPE_0,				//�ʏ���
	BLOCKTYPE_1,				//�ʏ���
	BLOCKTYPE_2,				//�ʏ���
	BLOCKTYPE_3,				//�ʏ���
	BLOCKTYPE_4,				//�ʏ���
	BLOCKTYPE_MAX
}BLOCKTYPE;

//�G�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 posOld;				//�ʒu
	D3DXVECTOR3 posInit;			//�ʒu
	D3DXVECTOR3 move;				//�ړ���
	D3DXCOLOR col;
	BLOCKTYPE blocktype;
	int nType;
	float fHeight;					//����
	float fWidth;					//��
	int nBlockBreak;
	bool bUse;						//�g�p���Ă��邩�ǂ���
	bool bPlayer;
}Block;

//�v���g�^�C�v�錾
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, int nType);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, Block **pBlock);
bool CollisionBlockSide(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, Block **pBlock);

#endif // !_BLOCK_H_