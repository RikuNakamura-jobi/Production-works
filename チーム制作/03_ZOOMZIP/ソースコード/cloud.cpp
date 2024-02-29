//==========================================================
//
//�|���S������ [Cloud.h]
//Author Ibuki Okusada
//
//==========================================================
#include "cloud.h"
#include "texture.h"
#include "fileload.h"
#include "texture.h"
#include "player.h"

//==========================================================
//�}�N����`
//==========================================================
#define MAX_POLYGON		(1)

//==========================================================
//�O���[�o���ϐ�
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureCloud[MAX_POLYGON] = {};			//�e�N�X�`��1�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCloud = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Cloud g_aCloud[MAX_POLYGON];

//==========================================================
//�|���S���̏���������
//==========================================================
void InitCloud(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	for (int nCntCloud = 0; nCntCloud < MAX_POLYGON; nCntCloud++)
	{
		if (GetMode() == MODE_GAME && nCntCloud == MAX_POLYGON - 1)
		{
			g_pTextureCloud[nCntCloud] = *SetTexture(2);
		}

		g_aCloud[nCntCloud].pos = {};
		g_aCloud[nCntCloud].rot = {};
		g_aCloud[nCntCloud].fWidth = 0.0f;
		g_aCloud[nCntCloud].fHeight = 0.0f;
		g_aCloud[nCntCloud].fTexU = 0.0f;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCloud,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCloud->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCloud = 0; nCntCloud < MAX_POLYGON; nCntCloud++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 700.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1280.0f, 700.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 920.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1280.0f, 920.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(g_aCloud[nCntCloud].fTexU, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aCloud[nCntCloud].fTexU + 1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aCloud[nCntCloud].fTexU, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aCloud[nCntCloud].fTexU + 1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCloud->Unlock();
}

//==========================================================
//�|���S���̏I������
//==========================================================
void UninitCloud(void)
{

	//�e�N�X�`���̔j��
	for (int nCntCloud = 0; nCntCloud < MAX_POLYGON; nCntCloud++)
	{
		if (g_pTextureCloud[nCntCloud] != NULL)
		{
			g_pTextureCloud[nCntCloud]->Release();
			g_pTextureCloud[nCntCloud] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCloud != NULL)
	{
		g_pVtxBuffCloud->Release();
		g_pVtxBuffCloud = NULL;
	}
}

//==========================================================
//�|���S���̍X�V����
//==========================================================
void UpdateCloud(void)
{
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	switch (GetMode())
	{
	case MODE_TITLE:	//�^�C�g�����
		g_aCloud[0].fTexU += 0.0003f;

		if (g_aCloud[0].fTexU > 1.0f)
		{
			g_aCloud[0].fTexU = 0.0f;
		}
		break;
	case MODE_TUTORIAL:	//�`���[�g���A�����
		break;
	case MODE_RESULT:	//���U���g���
		g_aCloud[0].fTexU += 0.0003f;

		if (g_aCloud[0].fTexU > 1.0f)
		{
			g_aCloud[0].fTexU = 0.0f;
		}
		break;
	case MODE_RANKING:	//�����L���O���
		break;
	default:	//�Q�[�����
		if (GetPlayer()->move.x < 0.0f)
		{
			g_aCloud[0].fTexU += 0.0003f;
		}
		else
		{
			g_aCloud[0].fTexU += 0.0001f;
		}

		if (g_aCloud[0].fTexU > 1.0f)
		{
			g_aCloud[0].fTexU = 0.0f;
		}

		break;
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCloud->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCloud = 0; nCntCloud < MAX_POLYGON; nCntCloud++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 680.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1280.0f, 680.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 850.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1280.0f, 850.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(g_aCloud[0].fTexU, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aCloud[0].fTexU + 0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aCloud[0].fTexU, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aCloud[0].fTexU + 0.5f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCloud->Unlock();
}

//==========================================================
//�|���S���̕`�揈��
//==========================================================
void DrawCloud(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾

	for (int nCntCloud = 0; nCntCloud < MAX_POLYGON; nCntCloud++)
	{

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffCloud, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureCloud[nCntCloud]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
			4 * nCntCloud,								//�v���~�e�B�u�i�|���S���̐��j
			2);												//�`�悷��v���~�e�B�u��
	}
}