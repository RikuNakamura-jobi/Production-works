//==============================================================
//
//�G�t�F�N�g����[effect.cpp]
//Author:����������
//
//==============================================================
#include"effect.h"

//�}�N����`
#define MAX_EFFECT		(7200)		//�G�t�F�N�g�̍ő吔
#define NUM_TEX			(1)			//�e�N�X�`���̐�
#define WIDTH			(50.0f)		//����
#define HEIGHT			(50.0f)		//�c��

//�G�t�F�N�g�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXCOLOR col;			//�F
	float fRadius;			//���a�i�傫���j
	int nLife;				//�����i�\�����ԁj
	int nType;				//���
	bool bUse;				//�g�p���Ă��邩�ǂ���
	D3DXMATRIX mtxWorldEffect;
} Effect;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureEffect[NUM_TEX] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;		//���_�o�b�t�@�̃|�C���^
Effect g_aEffect[MAX_EFFECT];							//�G�t�F�N�g�̏��

//==============================================================
//�G�t�F�N�g�̏���������
//==============================================================
void InitEffect(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEffect;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.png",
		&g_pTextureEffect[0]);

	//�e�̏��̏�����
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̏�����
		g_aEffect[nCntEffect].move = D3DXVECTOR3(1.0f, 1.0f, 0.0f);		//�ړ��ʂ̏�����
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F�̏�����
		g_aEffect[nCntEffect].fRadius = 0.0f;							//���a�i�傫���j�̏�����
		g_aEffect[nCntEffect].nLife = 0;								//�����i�\�����ԁj�̏�����
		g_aEffect[nCntEffect].nType = 0;								//��ނ̏�����
		g_aEffect[nCntEffect].bUse = false;								//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + -WIDTH, g_aEffect[nCntEffect].pos.y + -HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + WIDTH,  g_aEffect[nCntEffect].pos.y + -HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + -WIDTH, g_aEffect[nCntEffect].pos.y + HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + WIDTH,  g_aEffect[nCntEffect].pos.y + HEIGHT, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw =1.0f;
		pVtx[2].rhw =1.0f;
		pVtx[3].rhw =1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			//���_�f�[�^�̃|�C���^���S���W�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//==============================================================
//�G�t�F�N�g�̏I������
//==============================================================
void UninitEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < NUM_TEX; nCntEffect++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureEffect[nCntEffect] != NULL)
		{
			g_pTextureEffect[nCntEffect]->Release();
			g_pTextureEffect[nCntEffect] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//==============================================================
//�G�t�F�N�g�̍X�V����
//==============================================================
void UpdateEffect(void)
{
	int nCntEffect;
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���
			if (g_aEffect[nCntEffect].nType == 0)
			{//�ʏ�̃G�t�F�N�g�̏ꍇ
				g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;
				g_aEffect[nCntEffect].fRadius *= 0.95f;
				g_aEffect[nCntEffect].col.a *= 0.95f;

				g_aEffect[nCntEffect].nLife--;
			}

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[1].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[2].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[3].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + -g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].pos.y + -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + -g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);


			if (g_aEffect[nCntEffect].nLife <= 0)
			{
 				g_aEffect[nCntEffect].bUse = false;
			}
		}
		pVtx += 4;											//���_�f�[�^�̃|�C���^���S���W�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//==============================================================
//�G�t�F�N�g�̕`�揈��
//==============================================================
void DrawEffect(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEffect;

	D3DXMATRIX mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;				//�r���[�}�g���b�N�X�擾�p

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorldEffect);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			if (g_aEffect[nCntEffect].nType == 0)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureEffect[0]);
			}
			else if (g_aEffect[nCntEffect].nType == 1)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureEffect[1]);
			}

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}
	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//==============================================================
//�G�t�F�N�g�̐ݒ菈��
//==============================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nType)
{
	int nCntEffect;

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//�G�t�F�N�g���g�p����Ă��Ȃ�
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nType = nType;
			g_aEffect[nCntEffect].nLife = nLife;

			g_aEffect[nCntEffect].bUse = true;		//�g�p���Ă����Ԃɂ���
			VERTEX_2D *pVtx;

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += nCntEffect * 4;				//���_�f�[�^�̃|�C���^���S���W�߂�

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[1].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[2].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[3].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + -g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + -g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);


			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffEffect->Unlock();
			break;
		}
	}
}