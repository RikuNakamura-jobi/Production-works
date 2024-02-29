//===========================
//
//�_�C���N�gX.area�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "area.h"
#include "player.h"

//�}�N����`
#define NUM_AREA (3)												//�w�i�̐�

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureArea[NUM_AREA] = {};					//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffArea = NULL;						//���_�����i�[

//===========================
//�w�i�̏���������
//===========================
void InitArea(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	Player *pPlayer = GetPlayer();
	int nCntArea;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\hitarea2 (2).png",
		&g_apTextureArea[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\hitarea2 (2).png",
		&g_apTextureArea[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\hitarea2 (2).png",
		&g_apTextureArea[2]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_AREA,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffArea,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffArea->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntArea = 0; nCntArea < NUM_AREA; nCntArea++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = pPlayer->pos.x - 87.0f;
		pVtx[0].pos.y = pPlayer->pos.y - 17.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pPlayer->pos.x - 25.0f;
		pVtx[1].pos.y = pPlayer->pos.y - 17.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pPlayer->pos.x - 87.0f;
		pVtx[2].pos.y = pPlayer->pos.y + 57.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pPlayer->pos.x - 25.0f;
		pVtx[3].pos.y = pPlayer->pos.y + 57.0f;
		pVtx[3].pos.z = 0.0f;

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
	g_VtxBuffArea->Unlock();
}

//===========================
//�w�i�̏I������
//===========================
void UninitArea(void)
{
	int nCntArea;

	for (nCntArea = 0; nCntArea < NUM_AREA; nCntArea++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureArea[nCntArea] != NULL)
		{
			g_apTextureArea[nCntArea]->Release();
			g_apTextureArea[nCntArea] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffArea != NULL)
	{
		g_VtxBuffArea->Release();
		g_VtxBuffArea = NULL;
	}
}

//===========================
//�w�i�̍X�V����
//===========================
void UpdateArea(void)
{
	
}

//===========================
//�w�i�̕`�揈��
//===========================
void DrawArea(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntArea;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffArea,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntArea = 0; nCntArea < NUM_AREA; nCntArea++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureArea[nCntArea]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntArea * 4, 2);
	}
}