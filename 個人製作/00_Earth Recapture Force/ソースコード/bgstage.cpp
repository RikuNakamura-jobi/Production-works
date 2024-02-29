//===========================
//
//�_�C���N�gX.bgstage�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "bgstage.h"
#include "game.h"

//�}�N����`
#define NUM_Bgstage (12)											//�w�i�̐�
#define MAX_BGSTAGE (3)												//�`�悷��w�i�̐�

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureBgstage[NUM_Bgstage] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffBgstage = NULL;					//���_�����i�[
float g_aTexVStage[NUM_Bgstage];									//�e�N�X�`�����W�̊J�n�ʒu
float g_aTexUStage[NUM_Bgstage];									//�e�N�X�`�����W�̊J�n�ʒu

//===========================
//�w�i�̏���������
//===========================
void InitBgStage(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBgstage;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\sakuramiti.png",
		&g_apTextureBgstage[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\sakurahubuki002.png",
		&g_apTextureBgstage[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\sakurahubuki002.png",
		&g_apTextureBgstage[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\yakei003.png",
		&g_apTextureBgstage[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg100.png",
		&g_apTextureBgstage[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg101.png",
		&g_apTextureBgstage[5]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\umikumo002.png",
		&g_apTextureBgstage[6]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\umikumo003.png",
		&g_apTextureBgstage[7]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\umikumo003.png",
		&g_apTextureBgstage[8]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\EX_BG000.png",
		&g_apTextureBgstage[9]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\EX_BG001.png",
		&g_apTextureBgstage[10]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\EX_BG002.png",
		&g_apTextureBgstage[11]);

	//�e�N�X�`�����W�̊J�n�ʒu�̏�����
	for (nCntBgstage = 0; nCntBgstage < NUM_Bgstage; nCntBgstage++)
	{
		g_aTexVStage[nCntBgstage] = 0.9f;
		g_aTexUStage[nCntBgstage] = 0.9f;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_Bgstage,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffBgstage,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffBgstage->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBgstage = 0; nCntBgstage < NUM_Bgstage; nCntBgstage++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(PLAYER_WIDTH_L, PLAYER_HEIGHT_U, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(PLAYER_WIDTH_R, PLAYER_HEIGHT_U, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(PLAYER_WIDTH_L, PLAYER_HEIGHT_D, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(PLAYER_WIDTH_R, PLAYER_HEIGHT_D, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f - (g_aTexUStage[nCntBgstage] * nCntBgstage * 0.3f), g_aTexVStage[nCntBgstage]);
		pVtx[1].tex = D3DXVECTOR2(1.0f - (g_aTexUStage[nCntBgstage] * nCntBgstage * 0.3f), g_aTexVStage[nCntBgstage]);
		pVtx[2].tex = D3DXVECTOR2(0.0f - (g_aTexUStage[nCntBgstage] * nCntBgstage * 0.3f), g_aTexVStage[nCntBgstage] + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f - (g_aTexUStage[nCntBgstage] * nCntBgstage * 0.3f), g_aTexVStage[nCntBgstage] + 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffBgstage->Unlock();
}

//===========================
//�w�i�̏I������
//===========================
void UninitBgStage(void)
{
	int nCntBgstage;

	for (nCntBgstage = 0; nCntBgstage < NUM_Bgstage; nCntBgstage++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureBgstage[nCntBgstage] != NULL)
		{
			g_apTextureBgstage[nCntBgstage]->Release();
			g_apTextureBgstage[nCntBgstage] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffBgstage != NULL)
	{
		g_VtxBuffBgstage->Release();
		g_VtxBuffBgstage = NULL;
	}
}

//===========================
//�w�i�̍X�V����
//===========================
void UpdateBgStage(void)
{
	int nCntBgstage;
	STAGESTATE stageState = GetStageState();
	
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffBgstage->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBgstage = 0; nCntBgstage < NUM_Bgstage; nCntBgstage++)
	{
		if (stageState == STAGESTATE_1 || stageState == STAGESTATE_2)
		{
			//�e�N�X�`�����W�̊J�n�ʒu�̍X�V
			g_aTexVStage[nCntBgstage] -= 0.007f * nCntBgstage + 0.0007f;
			g_aTexUStage[nCntBgstage] -= 0.01f * nCntBgstage + 0.001f;
		}
		else if (stageState == STAGESTATE_3)
		{
			//�e�N�X�`�����W�̊J�n�ʒu�̍X�V
			g_aTexVStage[nCntBgstage] -= 0.01f * (nCntBgstage + 1) + 0.002f;
			g_aTexUStage[nCntBgstage] -= 0.01f * (nCntBgstage + 1) + 0.003f;
		}
		else if (stageState == STAGESTATE_EX)
		{
			//�e�N�X�`�����W�̊J�n�ʒu�̍X�V
			g_aTexVStage[nCntBgstage] -= 0.007f * nCntBgstage + 0.0007f;
			g_aTexUStage[nCntBgstage] = 0.0f;
		}

		if (g_apTextureBgstage[nCntBgstage] <= 0)
		{
			g_aTexVStage[nCntBgstage] = 0.9f;
			g_aTexUStage[nCntBgstage] = 0.9f;
		}

		//�e�N�X�`�����W�̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f - (g_aTexUStage[nCntBgstage] * nCntBgstage * 0.3f), g_aTexVStage[nCntBgstage]);
		pVtx[1].tex = D3DXVECTOR2(1.0f - (g_aTexUStage[nCntBgstage] * nCntBgstage * 0.3f), g_aTexVStage[nCntBgstage]);
		pVtx[2].tex = D3DXVECTOR2(0.0f - (g_aTexUStage[nCntBgstage] * nCntBgstage * 0.3f), g_aTexVStage[nCntBgstage] + 0.7f);
		pVtx[3].tex = D3DXVECTOR2(1.0f - (g_aTexUStage[nCntBgstage] * nCntBgstage * 0.3f), g_aTexVStage[nCntBgstage] + 0.7f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffBgstage->Unlock();
}

//===========================
//�w�i�̕`�揈��
//===========================
void DrawBgStage(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBgstage;
	STAGESTATE stageState = GetStageState();

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffBgstage,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBgstage = 0; nCntBgstage < MAX_BGSTAGE; nCntBgstage++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureBgstage[nCntBgstage - 3 + (3 * (stageState))]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBgstage * 4, 2);
	}
}