//========================================
//
// 3D�p�[�e�B�N���̏���[particle.h]
// Author:Kai Takada
//
//========================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "effect.h"

//===========================
//�^�C�v�񋓌^��`
//===========================
typedef enum
{
	PARTICLETYPE_SMOG = 0,
	PARTICLETYPE_SPARK,
	PARTICLETYPE_SANDPOLE,
	PARTICLETYPE_SANDDUST,
	PARTICLETYPE_LANDED,
	PARTICLETYPE_MAX
}PARTICLETYPE;

//===========================
//�\���̒�`
//===========================
typedef struct
{
	Effect effect;
	D3DXVECTOR3 Randpos;	//�����_�����W
	int nSpeed;		//�X�s�[�h��10�{
	int nLife;		//�G�t�F�N�g����
	int nLifePtcl;		//�p�[�e�B�N������
	int nNumEf;		//�o���G�t�F�N�g��
	bool bUse;
}Particle;

//===========================
//�v���g�^�C�v�錾
//===========================
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, PARTICLETYPE type);
Particle *GetParticle(void);

#endif