//===========================
//
//�_�C���N�gX.stagestart�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "stagestart.h"
#include "game.h"

//�}�N����`
#define NUM_STAGESTART (4)												//�{�X�x���̐�

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureStagestart[NUM_STAGESTART] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffStagestart = NULL;						//���_�����i�[
float g_aTexVStagestart[NUM_STAGESTART];								//�e�N�X�`�����W�̊J�n�ʒu
float g_fCntStagestart;													//�X�e�[�W�J�n�J�E���g
int g_nTimerStagestart;													//�X�e�[�W�J�n�^�C�}�[
bool g_bUseStagestart;													//�X�e�[�W�J�n�`�攻��

//===========================
//�{�X�x���̏���������
//===========================
void InitStagestart(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntStagestart;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\stagestart000.png",
		&g_apTextureStagestart[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\stagestart001.png",
		&g_apTextureStagestart[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\stagestart002.png",
		&g_apTextureStagestart[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\stagestart003.png",
		&g_apTextureStagestart[3]);

	//�e�N�X�`�����W�̊J�n�ʒu�̏�����
	for (nCntStagestart = 0; nCntStagestart < NUM_STAGESTART; nCntStagestart++)
	{
		g_aTexVStagestart[nCntStagestart] = 0.9f;
	}

	g_bUseStagestart = false;
	g_fCntStagestart = 0.0f;
	g_nTimerStagestart = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_STAGESTART,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffStagestart,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffStagestart->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStagestart = 0; nCntStagestart < NUM_STAGESTART; nCntStagestart++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(300.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(980.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(300.0f, 720.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(980.0f, 720.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffStagestart->Unlock();
}

//===========================
//�{�X�x���̏I������
//===========================
void UninitStagestart(void)
{
	int nCntStagestart;

	for (nCntStagestart = 0; nCntStagestart < NUM_STAGESTART; nCntStagestart++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureStagestart[nCntStagestart] != NULL)
		{
			g_apTextureStagestart[nCntStagestart]->Release();
			g_apTextureStagestart[nCntStagestart] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffStagestart != NULL)
	{
		g_VtxBuffStagestart->Release();
		g_VtxBuffStagestart = NULL;
	}
}

//===========================
//�{�X�x���̍X�V����
//===========================
void UpdateStagestart(void)
{
	int nCntStagestart;
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	if (g_bUseStagestart == true)
	{
		if (g_nTimerStagestart < 180.0f)
		{
			if (g_fCntStagestart < 1.0f)
			{
				col.a = g_fCntStagestart;
				g_fCntStagestart += 0.05f;
			}
			else if (g_fCntStagestart >= 1.0f)
			{
				col.a = 1.0f;
				g_fCntStagestart = 1.0f;
			}
		}
		else if (g_nTimerStagestart >= 180.0f)
		{
			if (g_fCntStagestart > 0.0f)
			{
				col.a = g_fCntStagestart;
				g_fCntStagestart -= 0.05f;
			}
			else if (g_fCntStagestart <= 0.0f)
			{
				col.a = 0.0f;
				g_fCntStagestart = 0.0f;
			}
		}

		g_nTimerStagestart++;
	}
	
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffStagestart->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStagestart = 0; nCntStagestart < NUM_STAGESTART; nCntStagestart++)
	{
		if (g_bUseStagestart == true)
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffStagestart->Unlock();
}

//===========================
//�{�X�x���̕`�揈��
//===========================
void DrawStagestart(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	STAGESTATE stageState = GetStageState();

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffStagestart,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_bUseStagestart == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureStagestart[stageState - 1]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (stageState - 1) * 4, 2);
	}
}

//===========================
//�{�X�x���̕`�揈��
//===========================
void SetStagestart(void)
{
	g_bUseStagestart = g_bUseStagestart ? false : true;
}