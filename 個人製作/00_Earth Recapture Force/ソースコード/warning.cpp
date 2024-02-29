//===========================
//
//�_�C���N�gX.warning�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "warning.h"
#include "game.h"

//�}�N����`
#define NUM_WARNING (4)													//�{�X�x���̐�

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureWarning[NUM_WARNING] = {};				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffWarning = NULL;						//���_�����i�[
float g_aTexVWarning[NUM_WARNING];										//�e�N�X�`�����W�̊J�n�ʒu
float g_fCntWarning;													//�x���_�ŗp�ϐ�
bool g_bUseWarning;														//�x���`�攻��

//===========================
//�{�X�x���̏���������
//===========================
void InitWarning(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntWarning;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\warning.png",
		&g_apTextureWarning[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\warning.png",
		&g_apTextureWarning[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\warning.png",
		&g_apTextureWarning[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\warning.png",
		&g_apTextureWarning[3]);

	//�e�N�X�`�����W�̊J�n�ʒu�̏�����
	for (nCntWarning = 0; nCntWarning < NUM_WARNING; nCntWarning++)
	{
		g_aTexVWarning[nCntWarning] = 0.9f;
	}

	g_bUseWarning = false;
	g_fCntWarning = 0.0f;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_WARNING,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffWarning,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffWarning->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWarning = 0; nCntWarning < NUM_WARNING; nCntWarning++)
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
	g_VtxBuffWarning->Unlock();
}

//===========================
//�{�X�x���̏I������
//===========================
void UninitWarning(void)
{
	int nCntWarning;

	for (nCntWarning = 0; nCntWarning < NUM_WARNING; nCntWarning++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureWarning[nCntWarning] != NULL)
		{
			g_apTextureWarning[nCntWarning]->Release();
			g_apTextureWarning[nCntWarning] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffWarning != NULL)
	{
		g_VtxBuffWarning->Release();
		g_VtxBuffWarning = NULL;
	}
}

//===========================
//�{�X�x���̍X�V����
//===========================
void UpdateWarning(void)
{
	int nCntWarning;
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	col.a = (sinf(g_fCntWarning) + 1.0f) / 2;

	g_fCntWarning += 0.1f;
	
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffWarning->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWarning = 0; nCntWarning < NUM_WARNING; nCntWarning++)
	{
		if (g_bUseWarning == true)
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
	g_VtxBuffWarning->Unlock();
}

//===========================
//�{�X�x���̕`�揈��
//===========================
void DrawWarning(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	STAGESTATE stageState = GetStageState();

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffWarning,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_bUseWarning == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureWarning[stageState - 1]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (stageState - 1) * 4, 2);
	}
}

//===========================
//�{�X�x���̕`�揈��
//===========================
void SetWarning(void)
{
	g_bUseWarning = g_bUseWarning ? false : true;
}