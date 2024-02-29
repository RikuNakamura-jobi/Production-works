//==========================================================
//
//�v���C���[���� [player.h]
//Author:����������
//
//==========================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"	//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include

//==========================================================
//�}�N����`
//==========================================================
#define PLAYER_GRAVITY	(0.1f)		//�v���C���[�̏d��
#define WIDTH_PLAYER	(35.0f)		//�v���C���[�̕�
#define HEIGHT_PLAYER	(35.0f)		//�v���C���[�̍���

//==========================================================
//�񋓌^�̒�`
//==========================================================
//�v���C���[�̏�ԗ񋓌^
typedef enum
{
	PLAYER_APPEAR = 0,		//�o�����
	PLAYERSTATE_NORMAL,		//�ʏ���
	PLAYERSTATE_DEATH,		//���S���
	PLAYERSTATE_MAX
}PLAYERSTATE;

//==========================================================
//�\���̂̒�`
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 moveOld;	//�O��̈ړ���
	int nSwitchLR;			//�v���C���[�̍��E�؂�ւ�
	int nCntAnim;			//�A�j���[�V�����J�E���^�[
	int nPatternAnim;		//�A�j���[�V����No.
	int nCntGravity;		//�d�̓J�E���^�[
	float fSpeed;			//�ړ���
	float fJump;			//�W�����v��
	float fLength;			//�Ίp���̒���
	float fAngle;			//�Ίp���̊p�x
	float fWidth;			//��
	float fHeight;			//����
	bool bJump;				//�W�����v���Ă��邩�ǂ���
	bool bUse;				//�g�p���Ă邩
	bool bDisp;				//�`�悷�邩
	PLAYERSTATE state;		//�v���C���[�̏��
}Player;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif

