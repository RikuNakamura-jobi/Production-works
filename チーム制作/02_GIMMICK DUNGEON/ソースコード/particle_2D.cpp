//========================================
//
// 2D�p�[�e�B�N���̏���[particle.cpp]
// Author:Kai Takada
//
//========================================
#include "main.h"
#include "particle_2D.h"

//================================
//�}�N����`
//================================
#define MAX_PARTICLE2D (256)		//�p�[�e�B�N���̍ő吔

//================================
//�O���[�o���ϐ��錾
//================================
Particle2D g_aParticle[MAX_PARTICLE2D];		//�p�[�e�B�N�����

//================================
//�p�[�e�B�N���̏���������
//================================
void InitParticle2D(void)
{
	for (int nCntInit = 0;nCntInit < MAX_PARTICLE2D;nCntInit++)
	{
		ZeroMemory(&g_aParticle[nCntInit],sizeof(Particle2D));
	}
}

//================================
//�p�[�e�B�N���̏I������
//================================
void UninitParticle2D(void)
{

}

//================================
//�p�[�e�B�N���̍X�V����
//================================
void UpdateParticle2D(void)
{
	Particle2D *pParticle2D = GetParticle2D();

	for (int nCntUpdate = 0; nCntUpdate < MAX_PARTICLE2D; nCntUpdate++, pParticle2D++)
	{
		if (pParticle2D->bUse == true)
		{
			for (int nCntEf = 0; nCntEf < pParticle2D->nNumEf; nCntEf++)
			{
				//�ړ��ʂ����肷��
				D3DXVec3Normalize(&pParticle2D->move, &D3DXVECTOR3((float)(rand() % (pParticle2D->nSpeed * 2) - pParticle2D->nSpeed), (float)(rand() % (pParticle2D->nSpeed * 2) - pParticle2D->nSpeed), 0.0f));
				pParticle2D->move.x *= rand() % pParticle2D->nSpeed;
				pParticle2D->move.y *= rand() % pParticle2D->nSpeed;

				SetEffect2D(D3DXVECTOR3((float)(rand() % 21) + pParticle2D->effect2D.pos.x,
									  (float)(rand() % 21) + pParticle2D->effect2D.pos.y,
									  0.0f),
							pParticle2D->effect2D.pPosOwner,
							pParticle2D->move,
							pParticle2D->effect2D.col,
							(rand() % 20) * 0.1f + pParticle2D->effect2D.fRadius,
							rand() % 60 + pParticle2D->effect2D.nLife,
							pParticle2D->effect2D.bAdd,
							pParticle2D->effect2D.fGrav);
			}
		
			pParticle2D->nLifePtcl--;

			if (pParticle2D->nLifePtcl <= 0)
			{
				pParticle2D->bUse = false;
			}
		}
	}
}

//================================
//�p�[�e�B�N���̕`�揈��
//================================
void DrawParticle2D(void)
{

}

//================================
//�p�[�e�B�N���̐ݒ菈��
//================================
void SetParticle2D(D3DXVECTOR3 pos,PARTICLETYPE_2D type)
{
	Particle2D *pParticle2D = GetParticle2D();

	for (int nCntSet = 0;nCntSet < MAX_PARTICLE2D;nCntSet++, pParticle2D++)
	{
		if (pParticle2D->bUse == false)
		{
			pParticle2D->Randpos = pos;
			pParticle2D->bUse = true;

			switch (type)
			{
			case PARTICLETYPE_2D_SMOG:
				pParticle2D->nNumEf = 17;
				pParticle2D->nLifePtcl = 1;
				pParticle2D->nSpeed = 60;
				pParticle2D->effect2D.pos = pos;
				pParticle2D->effect2D.pPosOwner = NULL;
				pParticle2D->effect2D.col = D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.3f);
				pParticle2D->effect2D.fRadius = 80.0f;
				pParticle2D->effect2D.nLife = 2000;
				pParticle2D->effect2D.fGrav = 0.0f;
				pParticle2D->effect2D.bAdd = false;
				break;
			case PARTICLETYPE_2D_RESULT:
				pParticle2D->nNumEf = 70;
				pParticle2D->nLifePtcl = 1;
				pParticle2D->nSpeed = 25;
				pParticle2D->effect2D.pos = pos;
				pParticle2D->effect2D.pPosOwner = NULL;
				pParticle2D->effect2D.col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pParticle2D->effect2D.fRadius = 50.0f;
				pParticle2D->effect2D.nLife = 1500;
				pParticle2D->effect2D.fGrav = 0.0f;
				pParticle2D->effect2D.bAdd = true;
				break;
			}
			break;
		}
	}
}

//================================
//�p�[�e�B�N���̎擾����
//================================
Particle2D *GetParticle2D()
{
	return &g_aParticle[0];
}
