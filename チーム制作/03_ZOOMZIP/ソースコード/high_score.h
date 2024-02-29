//==========================================================
//
//�n�C�X�R�A���� [high_score.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef HIGHSCORE_H_	//���̃}�N����`����`����Ă��Ȃ�������
#define HIGHSCORE_H_    //2�A�C���N���[�h�h�~�̃}�N�����`����

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
}HighScore;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitHighScore(void);
void UninitHighScore(void);
void UpdateHighScore(void);
void DrawHighScore(void);
HighScore *GetHighScore(void);
void AddHighScore(int nAddHighScore);
int GetNowHighScore(void);

#endif
