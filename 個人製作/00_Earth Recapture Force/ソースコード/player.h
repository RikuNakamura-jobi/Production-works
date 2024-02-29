//===========================
//
//�_�C���N�gX.player�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//�v���C���[�̎��
typedef enum
{
	PLAYERSTATE_APPEAR = 0,			//�o�����
	PLAYERSTATE_NORMAL,				//�ʏ���
	PLAYERSTATE_DAMAGE,				//�_���[�W���
	PLAYERSTATE_WAIT,				//�ҋ@���
	PLAYERSTATE_DEATH,				//���S���
	PLAYERSTATE_MAX
}PLAYERSTATE;

//�v���C���[�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 move;				//�ړ���
	D3DXVECTOR3 rot;				//����
	int nCounterAnim;				//�A�j���[�V�����J�E���^�[
	int nPatternAnim;				//�A�j���[�V�����p�^�[��No.
	int nLife;						//�̗�
	int nRemaining;					//�c�@
	PLAYERSTATE state;				//���
	int nCounterState;				//��ԊǗ��J�E���^�[
	bool bDisp;						//�g�p���Ă��邩�ǂ���
}Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void HitPlayer(int nDamage);
void AddPlayer(int nLife, int nRemain);

#endif // !_PLAYER_H_