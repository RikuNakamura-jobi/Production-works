//==========================================================
//
//�A�C�e������ [item.h]
//Author:����������
//
//==========================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"	//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include

//�}�N����`
#define MAX_ITEM	(128)		//�A�C�e���̍ő吔

//==========================================================
//�񋓌^�̒�`
//==========================================================
//�A�C�e���̏�ԗ񋓌^
typedef enum
{
	ITEMSTATE_DEL = 0,		//��Q���S�폜
	ITEMSTATE_JET,			//�W�F�b�g�p�b�N
	ITEMSTATE_UP,			//���_�A�b�v
	ITEMSTATE_MAX
}ITEMSTATE;

//==========================================================
//�\���̂̒�`
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 posKeep;	//�ۑ��p�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 moveOld;	//�O��̈ړ���
	float fWidth;			//��
	float fHeight;			//����
	float fMoveRand;		//�����_���Ȉړ���
	bool bUse;				//�g�p���Ă邩
	bool bDisp;				//�`�悷�邩
	bool bJump;				//�W�����v���Ă邩
	int nType;				//�A�C�e���̎��
	int nSwitchLR;			//���E�̐؂�ւ�
}Item;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, int nType);
Item *GetItem(void);

#endif


