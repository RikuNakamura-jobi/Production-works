//===========================
//
//�_�C���N�gX.particle�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "particle.h"

//�}�N����`
#define MAX_PARTICLE (8192)			//�p�[�e�B�N���̍ő吔

//�p�[�e�B�N���\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXCOLOR col;					//�F
	D3DXVECTOR3 move;				//����
	D3DXVECTOR3 rot;				//����
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	float fRadius;					//���a
	float fAngle;					//�Ίp���̊p�x
	float fRot;						//��]
	int nLife;						//����
	int nType;						//���
	bool bUse;						//�g�p���Ă��邩�ǂ���
}Particle;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureParticle = NULL;						//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;					//���_�����i�[
Particle g_aParticle[MAX_PARTICLE];									//�p�[�e�B�N���̏��

//===========================
//�p�[�e�B�N���̏���������
//===========================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntParticle;
	srand((unsigned int)time(0));

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Effect000.jpg",
		&g_pTextureParticle);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].fRadius = 10.0f;
		g_aParticle[nCntParticle].nLife = 10;
		g_aParticle[nCntParticle].bUse = false;									//�g�p���Ă��Ȃ���Ԃɂ���

		//�Ίp���̒������Z�o����
		g_aParticle[nCntParticle].fRadius = sqrtf((10.0f * 10.0f) + (10.0f * 10.0f)) * 0.5f;

		//�Ίp���̊p�x���Z�o����
		g_aParticle[nCntParticle].fAngle = atan2f(10.0f, 10.0f);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = sinf(g_aParticle[nCntParticle].rot.z - D3DX_PI + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[0].pos.y = -cosf(g_aParticle[nCntParticle].rot.z - D3DX_PI + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = sinf(g_aParticle[nCntParticle].rot.z + D3DX_PI - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[1].pos.y = -cosf(g_aParticle[nCntParticle].rot.z + D3DX_PI - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = sinf(g_aParticle[nCntParticle].rot.z - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[2].pos.y = -cosf(g_aParticle[nCntParticle].rot.z - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = sinf(g_aParticle[nCntParticle].rot.z + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[3].pos.y = -cosf(g_aParticle[nCntParticle].rot.z + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[3].pos.z = 0.0f;

		//���_���W�̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;											//���_���W�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffParticle->Unlock();
}

//===========================
//�p�[�e�B�N���̏I������
//===========================
void UninitParticle(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//===========================
//�p�[�e�B�N���̍X�V����
//===========================
void UpdateParticle(void)
{
	int nCntParticle;
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			g_aParticle[nCntParticle].nLife -= 1;

			if (g_aParticle[nCntParticle].fRadius > 0)
			{
				g_aParticle[nCntParticle].fRadius -= 0.5f;
			}
			else
			{
				g_aParticle[nCntParticle].fRadius = 0;
			}

			if (g_aParticle[nCntParticle].col.a <= 0)
			{
				g_aParticle[nCntParticle].col.a = 0;
			}
			else
			{
				g_aParticle[nCntParticle].col.a -= g_aParticle[nCntParticle].col.a / g_aParticle[nCntParticle].nLife;
			}

			g_aParticle[nCntParticle].move.y -= 0.8f;

			g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
			g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;
			g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z;
			g_aParticle[nCntParticle].rot.z += g_aParticle[nCntParticle].fRot;

			//���_���W�̐ݒ�
			pVtx[0].pos.x = sinf(g_aParticle[nCntParticle].rot.z - D3DX_PI + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[0].pos.y = -cosf(g_aParticle[nCntParticle].rot.z - D3DX_PI + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = sinf(g_aParticle[nCntParticle].rot.z + D3DX_PI - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[1].pos.y = -cosf(g_aParticle[nCntParticle].rot.z + D3DX_PI - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = sinf(g_aParticle[nCntParticle].rot.z - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[2].pos.y = -cosf(g_aParticle[nCntParticle].rot.z - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = sinf(g_aParticle[nCntParticle].rot.z + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[3].pos.y = -cosf(g_aParticle[nCntParticle].rot.z + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[3].pos.z = 0.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;

			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;					//�g�p���ĂȂ���Ԃɂ���
			}
		}

		pVtx += 4;
	}

	g_pVtxBuffParticle->Unlock();
}

//===========================
//�p�[�e�B�N���̕`�揈��
//===========================
void DrawParticle(void)
{
	int nCntParticle;
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxTrans;		//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;			//�r���[�}�g���b�N�X�擾�p

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aParticle[nCntParticle].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aParticle[nCntParticle].mtxWorld, NULL, &mtxView);
			g_aParticle[nCntParticle].mtxWorld._41 = 0.0f;
			g_aParticle[nCntParticle].mtxWorld._42 = 0.0f;
			g_aParticle[nCntParticle].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
			D3DXMatrixMultiply(&g_aParticle[nCntParticle].mtxWorld, &g_aParticle[nCntParticle].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aParticle[nCntParticle].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0,
				g_pVtxBuffParticle,
				0,
				sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureParticle);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntParticle, 2);
		}
	}

	//�e����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//===========================
//�p�[�e�B�N���̐ݒ菈��
//===========================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, int nType, int nRot, D3DXVECTOR3 move, int nMax)
{
	int nCntAppear;

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntAppear = 0; nCntAppear < nMax; nCntAppear++)
	{
		if (g_aParticle[nCntAppear].bUse == false)
		{
			g_aParticle[nCntAppear].pos = pos;
			g_aParticle[nCntAppear].nLife = nLife;
			g_aParticle[nCntAppear].col = col;
			g_aParticle[nCntAppear].fRot = (float)(rand() % 629 - nRot) / 100.0f;
			g_aParticle[nCntAppear].fRadius = sqrtf((fRadius * fRadius) + (fRadius * fRadius)) * 0.5f;
			g_aParticle[nCntAppear].fAngle = atan2f(fRadius, fRadius);

			g_aParticle[nCntAppear].fRot = (float)(rand() % 629 - nRot) / 100.0f;

			if (nType == 1)
			{
				g_aParticle[nCntAppear].fRot = (float)(rand() % 41 - nRot) / 100.0f;
			}
			
			if (g_aParticle[nCntAppear].fRot > D3DX_PI)
			{
				g_aParticle[nCntAppear].fRot -= (D3DX_PI * 2);
			}
			else if (g_aParticle[nCntAppear].fRot < -D3DX_PI)
			{
				g_aParticle[nCntAppear].fRot += (D3DX_PI * 2);
			}

			g_aParticle[nCntAppear].move.x = (float)(rand() % 200 - 100) / (10.0f + move.x);
			g_aParticle[nCntAppear].move.y = (float)(rand() % 200 - 10) / (10.0f + move.y);
			g_aParticle[nCntAppear].move.z = (float)(rand() % 200 - 100) / (10.0f + move.z);

			if (nType == 1)
			{
				g_aParticle[nCntAppear].move.x = (float)(rand() % 51 - 25) / (10.0f + move.x);
				g_aParticle[nCntAppear].move.y = (float)(rand() % 51 - 25) / (10.0f + move.y);
				g_aParticle[nCntAppear].move.z = (float)(rand() % 51 - 25) / (10.0f + move.z);
			}

			g_aParticle[nCntAppear].rot.z += g_aParticle[nCntAppear].fRot;

			//���_���W�̐ݒ�
			pVtx[0].pos.x = sinf(g_aParticle[nCntAppear].rot.z - D3DX_PI + g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[0].pos.y = -cosf(g_aParticle[nCntAppear].rot.z - D3DX_PI + g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = sinf(g_aParticle[nCntAppear].rot.z + D3DX_PI - g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[1].pos.y = -cosf(g_aParticle[nCntAppear].rot.z + D3DX_PI - g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = sinf(g_aParticle[nCntAppear].rot.z - g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[2].pos.y = -cosf(g_aParticle[nCntAppear].rot.z - g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = sinf(g_aParticle[nCntAppear].rot.z + g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[3].pos.y = -cosf(g_aParticle[nCntAppear].rot.z + g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[3].pos.z = 0.0f;

			g_aParticle[nCntAppear].bUse = true;						//�g�p���Ă����Ԃɂ���

			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffParticle->Unlock();
}