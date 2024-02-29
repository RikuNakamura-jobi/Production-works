//==========================================
//
//�����L���O�̏���(ranking.h)
//Author:Kai Takada
//
//==========================================
#ifndef _RANKING_H_							//���̃}�N����`������ĂȂ�������
#define _RANKING_H_							//��d�C���N���[�h�h�~�̃}�N�����`����

//===============================
//�}�N����`
//===============================
#define RANK_HEI (25)						//�����L���O�̊Ԃ̕�
#define RANK_HEIGHT (45)					//�^�C�g�����̃T�C�Y�i�c�j
#define RANK_WIDTH (30)						//�^�C�g�����̃T�C�Y�i���j
#define NUM_RANKING (5)						//�����L���O�ɏo���X�R�A�̐�

//===============================
//�v���g�^�C�v�錾
//===============================
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void SetRanking(int nScore);
void ResetRanking(void);
void TexRanking(void);

#endif
