//===========================
//
//�_�C���N�gX.player�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "block.h"

//�v���C���[�̎��
typedef enum
{
	PLAYERSTATE_APPEAR = 0,			//�o�����
	PLAYERSTATE_NORMAL,				//�ʏ���
	PLAYERSTATE_DAMAGE,				//�_���[�W���
	PLAYERSTATE_INV,				//�_���[�W���
	PLAYERSTATE_WAIT,				//�ҋ@���
	PLAYERSTATE_GOAL,				//�ҋ@���
	PLAYERSTATE_MOVE,				//�ҋ@���
	PLAYERSTATE_ATTACK,				//�ҋ@���
	PLAYERSTATE_DEATH,				//���S���
	PLAYERSTATE_MAX
}PLAYERSTATE;

//�v���C���[�̃^�C�v
typedef enum
{
	PLAYERTYPE_0 = 0,			//�ʏ���
	PLAYERTYPE_1,				//�ʏ���
	PLAYERTYPE_2,				//�ʏ���
	PLAYERTYPE_3,				//�ʏ���
	PLAYERTYPE_MAX
}PLAYERTYPE;

//�v���C���[�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 posOld;				//�ʒu
	D3DXVECTOR3 move;				//�ړ���
	D3DXVECTOR3 rot;				//����
	int nCounterAnim;				//�A�j���[�V�����J�E���^�[
	int nPatternAnim;				//�A�j���[�V�����p�^�[��No.
	int nLife;						//�A�j���[�V�����p�^�[��No.
	int nCounterState;				//��ԊǗ��J�E���^�[
	float fJumpHigh;
	float fJumpG;
	bool bDisp;						//�g�p���Ă��邩�ǂ���
	bool bJump;
	bool bWall;
	Block *pBlock;
	PLAYERSTATE state;				//���
	PLAYERTYPE playertype;
}Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayer(D3DXVECTOR3 pPos);
void HitPlayer(int nDamage, float move);
Player *GetPlayer(void);
void SetPlayerState(PLAYERSTATE state, int nCounter);

#endif // !_PLAYER_H_