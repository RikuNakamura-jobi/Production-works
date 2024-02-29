//===========================
//
//�_�C���N�gX.bg�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "bg.h"

//�}�N����`
#define NUM_BG (3)												//�w�i�̐�

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureBg[NUM_BG] = {};					//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffBg = NULL;						//���_�����i�[
float g_aTexV[NUM_BG];											//�e�N�X�`�����W�̊J�n�ʒu

//===========================
//�w�i�̏���������
//===========================
void InitBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBg;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\aoharuBG000.png",
		&g_apTextureBg[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\aoharuBG001.png",
		&g_apTextureBg[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\aoharuBG000.png",
		&g_apTextureBg[2]);

	//�e�N�X�`�����W�̊J�n�ʒu�̏�����
	for (nCntBg = 0; nCntBg < NUM_BG; nCntBg++)
	{
		g_aTexV[nCntBg] = 0.9f;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffBg,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBg = 0; nCntBg < NUM_BG; nCntBg++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
		pVtx[1].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
		pVtx[2].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
		pVtx[3].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffBg->Unlock();
}

//===========================
//�w�i�̏I������
//===========================
void UninitBg(void)
{
	int nCntBg;

	for (nCntBg = 0; nCntBg < NUM_BG; nCntBg++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureBg[nCntBg] != NULL)
		{
			g_apTextureBg[nCntBg]->Release();
			g_apTextureBg[nCntBg] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffBg != NULL)
	{
		g_VtxBuffBg->Release();
		g_VtxBuffBg = NULL;
	}
}

//===========================
//�w�i�̍X�V����
//===========================
void UpdateBg(void)
{
	int nCntBg;
	
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBg = 0; nCntBg < NUM_BG; nCntBg++)
	{
		//�e�N�X�`�����W�̊J�n�ʒu�̍X�V
		g_aTexV[nCntBg] += 0.0002f * nCntBg + 0.0002f;

		if (g_apTextureBg[nCntBg] <= 0)
		{
			g_aTexV[nCntBg] = 0.9f;
		}

		if (nCntBg == 1)
		{
			//�e�N�X�`�����W�̍X�V
			pVtx[0].tex = D3DXVECTOR2(g_aTexV[nCntBg], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aTexV[nCntBg] + 1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_aTexV[nCntBg], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_aTexV[nCntBg] + 1.0f, 1.0f);
		}
		else
		{
			//�e�N�X�`�����W�̍X�V
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffBg->Unlock();
}

//===========================
//�w�i�̕`�揈��
//===========================
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBg;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffBg,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBg = 0; nCntBg < NUM_BG; nCntBg++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureBg[nCntBg]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBg * 4, 2);
	}
}