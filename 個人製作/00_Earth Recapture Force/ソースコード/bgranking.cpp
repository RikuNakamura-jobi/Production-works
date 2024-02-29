//===========================
//
//�_�C���N�gX.bgranking�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "bgranking.h"

//�}�N����`
#define NUM_BGRANKING (3)														//�w�i�̐�

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureBgRanking[NUM_BGRANKING] = {};					//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffBgRanking = NULL;								//���_�����i�[
float g_aTexVRanking[NUM_BGRANKING];											//�e�N�X�`�����W�̊J�n�ʒu

//===========================
//�w�i�̏���������
//===========================
void InitBgRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBgRanking;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bgranking000.png",
		&g_apTextureBgRanking[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bgranking000.png",
		&g_apTextureBgRanking[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bgranking000.png",
		&g_apTextureBgRanking[2]);

	//�e�N�X�`�����W�̊J�n�ʒu�̏�����
	for (nCntBgRanking = 0; nCntBgRanking < NUM_BGRANKING; nCntBgRanking++)
	{
		g_aTexVRanking[nCntBgRanking] = 0.9f;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BGRANKING,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffBgRanking,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffBgRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBgRanking = 0; nCntBgRanking < NUM_BGRANKING; nCntBgRanking++)
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
	g_VtxBuffBgRanking->Unlock();
}

//===========================
//�w�i�̏I������
//===========================
void UninitBgRanking(void)
{
	int nCntBgRanking;

	for (nCntBgRanking = 0; nCntBgRanking < NUM_BGRANKING; nCntBgRanking++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureBgRanking[nCntBgRanking] != NULL)
		{
			g_apTextureBgRanking[nCntBgRanking]->Release();
			g_apTextureBgRanking[nCntBgRanking] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffBgRanking != NULL)
	{
		g_VtxBuffBgRanking->Release();
		g_VtxBuffBgRanking = NULL;
	}
}

//===========================
//�w�i�̍X�V����
//===========================
void UpdateBgRanking(void)
{
	int nCntBgRanking;
	
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffBgRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBgRanking = 0; nCntBgRanking < NUM_BGRANKING; nCntBgRanking++)
	{
		//�e�N�X�`�����W�̊J�n�ʒu�̍X�V
		g_aTexVRanking[nCntBgRanking] -= 0.005f * nCntBgRanking + 0.002f;

		if (g_apTextureBgRanking[nCntBgRanking] <= 0)
		{
			g_aTexVRanking[nCntBgRanking] = 0.9f;
		}

		//�e�N�X�`�����W�̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffBgRanking->Unlock();
}

//===========================
//�w�i�̕`�揈��
//===========================
void DrawBgRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBgRanking;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffBgRanking,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBgRanking = 0; nCntBgRanking < NUM_BGRANKING; nCntBgRanking++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureBgRanking[nCntBgRanking]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBgRanking * 4, 2);
	}
}