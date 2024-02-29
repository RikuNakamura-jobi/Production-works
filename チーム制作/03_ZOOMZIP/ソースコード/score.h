//==========================================================
//
//�X�R�A���� [score.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _SCORE_H_	//���̃}�N����`����`����Ă��Ȃ�������
#define _SCORE_H_    //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"    //�쐬����main.h���C���N���[�h����

//==========================================================
//�񋓌^�̒�`
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
	D3DXCOLOR col;		//�F
	float fAngle;		//�p�x
	float fLength;		//�Ίp���̒���
	float fHeight;		//����
	float fWidth;		//��
	bool bUse;			//�g�p���Ă��邩�ǂ���
}Score;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
Score *GetScore(void);
void AddScore(int nAddScore);
int GetNowScore(void);

#endif
