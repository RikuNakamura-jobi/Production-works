//==================================================================================================
//
//effect_2D.cpp
//Author:���R����		edit: ���c����
//
//==================================================================================================

#include "main.h"
#include "effect_2D.h"

//==========================================
//  �}�N����`
//==========================================
#define MAX_EFFECT2D		(4096)										//�ő吔
#define LIFE_SPEED		(10)										//�����̌��鑬�x
#define EFFECT2D_TEX_FILE		"data\\TEXTURE\\effect000.png"		//�e�N�X�`���t�@�C����
#define MOVE_FACT		(0.96f)										//�ړ������W��

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureEffect2D = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect2D = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Effect2D g_aEffect2D[MAX_EFFECT2D];						//�G�t�F�N�g�̏��

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void UpdateEffect2DPos(Effect2D *pEffect2D);
void UpdateEffect2DPolygon(Effect2D *pEffect2D,int nCntEffect2D);
void DebugEffect2D(void);

//==================================================================================================
//����������
//==================================================================================================
void InitEffect2D(void)
{
	//�f�o�C�X�|�C���^�ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�|�C���^�錾
	Effect2D *pEffect2D = GetEffect2D();

	//�ϐ��錾
	int nCntEffect2D;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		EFFECT2D_TEX_FILE,
		&g_pTextureEffect2D);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT2D,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect2D,
		NULL);

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect2D->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT2D; nCntEffect2D++,pEffect2D++)
	{//�S�Ă̒e�̏�����
	    //���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pEffect2D->fRadius = 0.0f;
		pEffect2D->col = D3DXCOLOR{ 1.0f,1.0f,1.0f,1.0f };
		pEffect2D->pos = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
		pEffect2D->relPos = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
		pEffect2D->move = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
		pEffect2D->nLife = 0;
		pEffect2D->nMaxLife = 0;
		pEffect2D->bUse = false; //�G�t�F�N�g���g�p���Ă��Ȃ���Ԃɂ���
		pEffect2D->fGrav = 0.0f;

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffEffect2D->Unlock();
}

//==================================================================================================
//�I������
//==================================================================================================
void UninitEffect2D(void)
{
	if (g_pTextureEffect2D != NULL)
	{//�e�N�X�`���ւ̃|�C���^�j��
		g_pTextureEffect2D->Release();
		g_pTextureEffect2D = NULL;
	}

	if (g_pVtxBuffEffect2D != NULL)
	{//���_�o�b�t�@�ւ̃|�C���^�j��
		g_pVtxBuffEffect2D->Release();
		g_pVtxBuffEffect2D = NULL;
	}
}

//==================================================================================================
//�X�V����
//==================================================================================================
void UpdateEffect2D(void)
{
	//�ϐ��錾
	int nCntEffect2D;
	
	//�|�C���^�錾
	Effect2D *pEffect2D = GetEffect2D();

	for (nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT2D; nCntEffect2D++, pEffect2D++)
	{//�S�Ă̍X�V

		if (pEffect2D->bUse)
		{//�g�p����Ă����ԂȂ�

			//�ʒu�X�V����
			UpdateEffect2DPos(pEffect2D);
			
			//���a�̍X�V(�|���S���T�C�Y�̍X�V)
			pEffect2D->fRadius *= ((float)pEffect2D->nLife / (float)pEffect2D->nMaxLife);

			//�����J�E���g�_�E��
			pEffect2D->nLife--;

			if (pEffect2D->fRadius <= 0.3f)
			{
			 //�g�p���Ă��Ȃ���Ԃɂ���
				pEffect2D->bUse = false;
			}

			//�|���S���X�V����
			UpdateEffect2DPolygon(pEffect2D, nCntEffect2D);
		}
	}
}

//==================================================================================================
//�ʒu�X�V����
//==================================================================================================
void UpdateEffect2DPos(Effect2D *pEffect2D)
{
	//�ړ��ʌ���
	pEffect2D->move = pEffect2D->move * MOVE_FACT;

	pEffect2D->move.y += pEffect2D->fGrav;

	if (pEffect2D->pPosOwner != NULL)
	{//���Έʒu�ł̐ݒ�
		pEffect2D->relPos.x += pEffect2D->move.x;
		pEffect2D->relPos.y += pEffect2D->move.y;
		pEffect2D->relPos.z += pEffect2D->move.z;

		pEffect2D->pos.x = pEffect2D->pPosOwner->x + pEffect2D->relPos.x;
		pEffect2D->pos.y = pEffect2D->pPosOwner->y + pEffect2D->relPos.y;
		pEffect2D->pos.z = pEffect2D->pPosOwner->z + pEffect2D->relPos.z;
	}
	else
	{//��΍��W�X�V
		pEffect2D->pos += pEffect2D->move;
	}
}

//==================================================================================================
//�|���S���X�V����
//==================================================================================================
void UpdateEffect2DPolygon(Effect2D *pEffect2D,int nCntEffect2D)
{
	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect2D->Lock(0, 0, (void**)&pVtx, 0);

	//���_�f�[�^�̃|�C���^��4���i�߂�
	pVtx += 4 * nCntEffect2D;

	//���_���W�̐ݒ�
	pVtx[0].pos = { pEffect2D->pos.x - pEffect2D->fRadius, pEffect2D->pos.y - pEffect2D->fRadius, 0.0f };
	pVtx[1].pos = { pEffect2D->pos.x + pEffect2D->fRadius, pEffect2D->pos.y - pEffect2D->fRadius, 0.0f };
	pVtx[2].pos = { pEffect2D->pos.x - pEffect2D->fRadius, pEffect2D->pos.y + pEffect2D->fRadius, 0.0f };
	pVtx[3].pos = { pEffect2D->pos.x + pEffect2D->fRadius, pEffect2D->pos.y + pEffect2D->fRadius, 0.0f };

	//���_�J���[�̐ݒ�
	pVtx[0].col = pEffect2D->col;
	pVtx[1].col = pEffect2D->col;
	pVtx[2].col = pEffect2D->col;
	pVtx[3].col = pEffect2D->col;

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffEffect2D->Unlock();
}

//==================================================================================================
//�`�揈��
//==================================================================================================
void DrawEffect2D(void)
{
	//�f�o�C�X�|�C���^�ݒ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�|�C���^�錾
	Effect2D *pEffect2D = GetEffect2D();

	//�A���t�@�e�X�g�̗L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);

	for (int nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT2D; nCntEffect2D++, pEffect2D++)
	{
		if (pEffect2D->bUse)
		{
			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect2D, 0, sizeof(VERTEX_2D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureEffect2D);

			if (pEffect2D->bAdd == true)
			{
				//���u�����f�B���O�����Z�����ɐݒ�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			//�|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEffect2D, 2);

			if (pEffect2D->bAdd == true)
			{
				//���u�����f�B���O�����ɖ߂�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
		}
	}

	//�A���t�@�e�X�g�̖�����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

#ifdef _DEBUG
	DebugEffect2D();
#endif
}

//==================================================================================================
//�ݒ菈��
//==================================================================================================
void SetEffect2D(D3DXVECTOR3 pos, D3DXVECTOR3 *pPosOwner, D3DXVECTOR3 move, D3DXCOLOR col,float fRadius, int nLife,bool bAdd, float fGrav)
{
	//�ϐ��錾
	int nCntEffect2D;

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//�e�ւ̃|�C���^
	Effect2D *pEffect2D = GetEffect2D();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect2D->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT2D; nCntEffect2D++,pEffect2D++)
	{//�S�Ă̒e���`�F�b�N����
		if (pEffect2D->bUse == false)
		{//�g���Ă��Ȃ���ԂȂ�

			//�ʒu�̐ݒ�
			pEffect2D->pos = pos;

			if (pPosOwner != NULL)
			{
				//������ʒu�̐ݒ�
				pEffect2D->pPosOwner = pPosOwner;

				//���Έʒu�ݒ�
				pEffect2D->relPos = pos - *pPosOwner;
			}

			//�ʒu�̐ݒ�
			pEffect2D->move = move;

			//�F�̐ݒ�
			pEffect2D->col = col;

			//���a�̐ݒ�
			pEffect2D->fRadius = fRadius;

			//�����̐ݒ�
			pEffect2D->nLife = nLife;
			pEffect2D->nMaxLife = nLife;

			//���_���W�̐ݒ�
			pVtx[0].col = D3DXCOLOR{ pEffect2D->col.r,pEffect2D->col.g,pEffect2D->col.b,pEffect2D->col.a };
			pVtx[1].col = D3DXCOLOR{ pEffect2D->col.r,pEffect2D->col.g,pEffect2D->col.b,pEffect2D->col.a };
			pVtx[2].col = D3DXCOLOR{ pEffect2D->col.r,pEffect2D->col.g,pEffect2D->col.b,pEffect2D->col.a };
			pVtx[3].col = D3DXCOLOR{ pEffect2D->col.r,pEffect2D->col.g,pEffect2D->col.b,pEffect2D->col.a };

			//�g�p���Ă���ݒ�
			pEffect2D->bUse = true;

			//���Z�������邩�ǂ���
			pEffect2D->bAdd = bAdd;

			//�d�͂��e�����邩�ǂ���
			pEffect2D->fGrav = fGrav;

			//for���𔲂���
			break;
		}

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffEffect2D->Unlock();
}

//==================================================================================================
//�|�C���^�擾
//==================================================================================================
Effect2D *GetEffect2D(void)
{
	return &g_aEffect2D[0];
}

//==================================================
//�f�o�b�O�\��
//==================================================
void DebugEffect2D(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//�ꏊ
	char aStr[1][256];					//��
	int nNum = 0;
	LPD3DXFONT Font = GetFont();		//���C���̂��

	for (int nCnt = 0; nCnt < MAX_EFFECT2D; nCnt++)
	{
		if (g_aEffect2D[nCnt].bUse == true)
		{
			nNum++;
		}
	}

	//������ɑ��
	sprintf(&aStr[0][0], "2D�G�t�F�N�g�̐��F%d", nNum);

	rect.top += (int)(SCREEN_HEIGHT * 0.4f) + 40;
	Font->DrawText(NULL, &aStr[0][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
