//==============================================================
//
//�p�[�e�B�N������[particle.cpp]
//Author:����������
//
//==============================================================
#include"particle.h"
#include"effect.h"

//�}�N����`
#define MAX_PARTICLE		(1280)		//�p�[�e�B�N���̍ő吔

//�p�[�e�B�N���\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu�i�����ʒu�j
	int nLife;				//�����i�������ԁj
	D3DXCOLOR col;			//�F
	float fRadius;			//���a�i�傫���j
	int nType;				//���
	bool bUse;				//�g�p���Ă��邩�ǂ���
} Particle;

typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXCOLOR col;			//�F
	int nLife;				//����
	int nType;				//���
	float fAngle;			//�p�x
	float fMove;			//�ړ���
	float fRadius;			//���a
} ParticleInfo;

//�v���g�^�C�v�錾
void ParticleGeneration(int nCounter);
void DamageParticle(int nCounter);

//�O���[�o���ϐ�
Particle g_aParticle[MAX_PARTICLE];							//�p�[�e�B�N���̏��
ParticleInfo g_aParticleInfo;
int g_nEffectType = 0;

//==============================================================
//�p�[�e�B�N���̏���������
//==============================================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntParticle;

	//�e�̏��̏�����
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�i�����ʒu�j�̏�����
		g_aParticle[nCntParticle].nLife = 0;								//�����i�������ԁj�̏�����
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F�̏�����
		g_aParticle[nCntParticle].fRadius = 0.0f;							//���a�̏�����
		g_aParticle[nCntParticle].nType = 0;								//��ނ̏�����
		g_aParticle[nCntParticle].bUse = false;								//�g�p���Ă��Ȃ���Ԃɂ���
	}

	g_aParticleInfo.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̏�����
	g_aParticleInfo.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ��ʂ̏�����
	g_aParticleInfo.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F�̏�����
	g_aParticleInfo.nLife = 0;									//�����̏�����
	g_aParticleInfo.nType = 0;									//��ނ̏�����
	g_aParticleInfo.fAngle = 0.0f;								//�p�x�̏�����
	g_aParticleInfo.fMove = 0.0f;								//�v�Z�p�ړ��ʂ�������
	g_aParticleInfo.fRadius = 0.0f;								//���a��������
	g_nEffectType = 0;
}

//==============================================================
//�p�[�e�B�N���̏I������
//==============================================================
void UninitParticle(void)
{
	
}

//==============================================================
//�p�[�e�B�N���̍X�V����
//==============================================================
void UpdateParticle(void)
{

}

//==============================================================
//�p�[�e�B�N���̕`�揈��
//==============================================================
void DrawParticle(void)
{
	
}

//==============================================================
//�p�[�e�B�N���̐ݒ菈��
//==============================================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, int nType, int nLife, float fRadius)
{
	int nCntParticle;

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{//�p�[�e�B�N�����g�p����Ă��Ȃ�
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].col = col;
			g_aParticle[nCntParticle].nType = nType;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].fRadius = fRadius;
			g_nEffectType = nType;

			g_aParticle[nCntParticle].bUse = true;		//�g�p���Ă����Ԃɂ���

			ParticleGeneration(nCntParticle);

			break;
		}
	}
}

//==============================================================
//�p�[�e�B�N������
//==============================================================
void ParticleGeneration(int nCounter)
{
	int nCntParticle = nCounter, nCntAppear, nCntLife;

	for (nCntLife = 0; nCntLife < MAX_PARTICLE; nCntLife++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{//�p�[�e�B�N�����g�p����Ă���

			//�p�[�e�B�N���̐���
			for (nCntAppear = 0; nCntAppear < 5; nCntAppear++)
			{
				//�_���[�W�p�[�e�B�N��
				DamageParticle(nCntParticle);

				//�G�t�F�N�g�̐ݒ�
				SetEffect(g_aParticleInfo.pos,
					g_aParticleInfo.move,
					g_aParticleInfo.col,
					g_aParticleInfo.fRadius,
					g_aParticleInfo.nLife,
					g_aParticleInfo.nType);
			}

			//�����̃J�E���g�_�E��
			g_aParticle[nCntParticle].nLife--;

			if (g_aParticle[nCntParticle].nLife <= 0)
			{//�������s�����ꍇ
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}
}

//==============================================================
//�_���[�W�p�[�e�B�N��
//==============================================================
void DamageParticle(int nCounter)
{
	int nCntParticle = nCounter;

	//�ʒu�̐ݒ�
	g_aParticleInfo.pos = g_aParticle[nCntParticle].pos;

	if (g_nEffectType == 0)
	{
		//�p�x�����߂�
		g_aParticleInfo.fAngle = (float)(rand() % 629 - 314) / 1.0f;

		//�ړ��ʂ����߂�
		g_aParticleInfo.fMove = (float)(rand() % 50) / 10.0f + 1.0f;

		//�ړ��ʂ̐ݒ�
		g_aParticleInfo.move.x = sinf(g_aParticleInfo.fAngle) * g_aParticleInfo.fMove;
		g_aParticleInfo.move.y = cosf(g_aParticleInfo.fAngle) * g_aParticleInfo.fMove;
		g_aParticleInfo.move.z = cosf(g_aParticleInfo.fAngle) * g_aParticleInfo.fMove;

		//�F�̐ݒ�
		g_aParticleInfo.col = g_aParticle[nCntParticle].col;

		//���a�̐ݒ�
		g_aParticleInfo.fRadius = g_aParticle[nCntParticle].fRadius;

		//�����̐ݒ�
		g_aParticleInfo.nLife = g_aParticle[nCntParticle].nLife;

		//�p�[�e�B�N���̎��
		g_aParticleInfo.nType = 0;
	}
	else if (g_nEffectType == 1)
	{
		//�p�x�����߂�
		g_aParticleInfo.fAngle = (float)(rand() % 629 - 314) / 1.0f;

		//�ړ��ʂ����߂�
		g_aParticleInfo.fMove = (float)(rand() % 50) / 10.0f + 1.0f;

		//�ړ��ʂ̐ݒ�
		g_aParticleInfo.move.x = sinf(g_aParticleInfo.fAngle) * g_aParticleInfo.fMove;

		//�p�x�����߂�
		g_aParticleInfo.fAngle = (float)(rand() % 629 - 314) / 1.0f;

		//�ړ��ʂ����߂�
		g_aParticleInfo.fMove = (float)(rand() % 50) / 100.0f + 1.0f;

		g_aParticleInfo.move.y = cosf(g_aParticleInfo.fAngle) * g_aParticleInfo.fMove;

		//�F�̐ݒ�
		g_aParticleInfo.col = g_aParticle[nCntParticle].col;

		//���a�̐ݒ�
		g_aParticleInfo.fRadius = g_aParticle[nCntParticle].fRadius;

		//�����̐ݒ�
		g_aParticleInfo.nLife = g_aParticle[nCntParticle].nLife;

		//�p�[�e�B�N���̎��
		g_aParticleInfo.nType = 0;
	}

	else if (g_nEffectType == 2)
	{
		//�p�x�����߂�
		g_aParticleInfo.fAngle = (float)(rand() % 629 - 314) / 1.0f;

		//�ړ��ʂ����߂�
		g_aParticleInfo.fMove = (float)(rand() % 50) / 10.0f + 1.0f;

		//�ړ��ʂ̐ݒ�
		g_aParticleInfo.move.y = cosf(g_aParticleInfo.fAngle) * g_aParticleInfo.fMove;

		//�p�x�����߂�
		g_aParticleInfo.fAngle = (float)(rand() % 629 - 314) / 1.0f;

		//�ړ��ʂ����߂�
		g_aParticleInfo.fMove = (float)(rand() % 50) / 100.0f + 1.0f;

		g_aParticleInfo.move.x = sinf(g_aParticleInfo.fAngle) * g_aParticleInfo.fMove;

		if (g_aParticleInfo.move.y > 0.0f)
		{
			g_aParticleInfo.move.y *= -1.0f;
		}

		//�F�̐ݒ�
		g_aParticleInfo.col = g_aParticle[nCntParticle].col;

		//���a�̐ݒ�
		g_aParticleInfo.fRadius = g_aParticle[nCntParticle].fRadius;

		//�����̐ݒ�
		g_aParticleInfo.nLife = g_aParticle[nCntParticle].nLife;

		//�p�[�e�B�N���̎��
		g_aParticleInfo.nType = 0;
	}
}