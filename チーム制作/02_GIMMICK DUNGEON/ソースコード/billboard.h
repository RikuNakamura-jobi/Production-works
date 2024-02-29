//========================================  
//
//  �r���{�[�h����(billboard.h)
//  Author : ���v�ԗD�� / �������
//
//========================================  
#ifndef _BILLBOARD_H_ //���̃}�N����`������Ă��Ȃ�������
#define _BILLBOARD_H_ //2�d�C���N���[�h�h�~�̃}�N�����`����

//==========================================
//  �񋓌^�̒�`
//==========================================
//�r���{�[�h�̎��
typedef enum
{
	BILLBOARDTYPE_ICON = 0,
	BILLBOARDTYPE_FIRE,
	BILLBOARDTYPE_ALERT,
	BILLBOARDTYPE_GUIDE,
	BILLBOARDTYPE_MAX
}BILLBOARDTYPE;

//==========================================
//  �\���̂̒�`
//==========================================
//�r���{�[�h�̍\����
typedef struct
{
	D3DXVECTOR3 pos; //�ʒu
	bool bUse; //�g�p���Ă��邩�ǂ���
	int nType; //���
	int nCnterAnim; //�A�j���[�V�����J�E���^�[
	int nPatternAnim; //�A�j���[�V�����p�^�[��
	float fMoveDist;	//�ړ�����
	bool bMoveState;	//�ړ����
	D3DXMATRIX mtxWorld; //���[���h�}�g���b�N�X
}Billboard;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
int SetBillboard(int nType, D3DXVECTOR3 pos);
void SetPositionBillboard(int nIdxBillboard, D3DXVECTOR3 pos);
Billboard *GetBillboard(void);

#endif
