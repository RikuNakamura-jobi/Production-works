//========================================
//
// 2D�p�[�e�B�N���̏���[particle_2D.h]
// Author:Kai Takada
//
//========================================
#ifndef _PARTICLE_2D_H_
#define _PARTICLE_2D_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "effect_2D.h"

//===========================
//�^�C�v�񋓌^��`
//===========================
typedef enum
{
	PARTICLETYPE_2D_SMOG = 0,
	PARTICLETYPE_2D_RESULT,
	PARTICLETYPE_2D_MAX
}PARTICLETYPE_2D;

//===========================
//�\���̒�`
//===========================
typedef struct
{
	Effect2D effect2D;
	D3DXVECTOR3 Randpos;	//�����_�����W
	D3DXVECTOR3 move; //�ړ���
	int nSpeed;		//�X�s�[�h��10�{
	int nLife;		//�G�t�F�N�g����
	int nLifePtcl;		//�p�[�e�B�N������
	int nNumEf;		//�o���G�t�F�N�g��
	bool bUse;
}Particle2D;

//===========================
//�v���g�^�C�v�錾
//===========================
void InitParticle2D(void);
void UninitParticle2D(void);
void UpdateParticle2D(void);
void DrawParticle2D(void);
void SetParticle2D(D3DXVECTOR3 pos, PARTICLETYPE_2D type);
Particle2D *GetParticle2D(void);

#endif