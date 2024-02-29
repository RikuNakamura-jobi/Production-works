//===========================
//
//�_�C���N�gX.remain�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "remain.h"

//�}�N����`
#define NUM_PLACE_REMAIN (3)											//�c�@�̍ő吔

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureRemain = NULL;								//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRemain = NULL;						//���_�����i�[
D3DXVECTOR3 g_posRemain;												//�ʒu
int g_nRemain;															//�c�@�̒l

//===========================
//�c�@�̏���������
//===========================
void InitRemain(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntRemain;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Player002.png",
		&g_pTextureRemain);

	g_posRemain = D3DXVECTOR3(1000.0f, 240.0f, 0.0f);
	g_nRemain = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE_REMAIN,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffRemain,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRemain->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRemain = 0; nCntRemain < NUM_PLACE_REMAIN; nCntRemain++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_posRemain.x + (80.0f * nCntRemain);
		pVtx[0].pos.y = g_posRemain.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posRemain.x + (80.0f * nCntRemain) + 80.0f;
		pVtx[1].pos.y = g_posRemain.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posRemain.x + (80.0f * nCntRemain);
		pVtx[2].pos.y = g_posRemain.y + 80.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posRemain.x + (80.0f * nCntRemain) + 80.0f;
		pVtx[3].pos.y = g_posRemain.y + 80.0f;
		pVtx[3].pos.z = 0.0f;

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;										//���_���W�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRemain->Unlock();
}

//===========================
//�c�@�̏I������
//===========================
void UninitRemain(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureRemain != NULL)
	{
		g_pTextureRemain->Release();
		g_pTextureRemain = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRemain != NULL)
	{
		g_pVtxBuffRemain->Release();
		g_pVtxBuffRemain = NULL;
	}
}

//===========================
//�c�@�̍X�V����
//===========================
void UpdateRemain(void)
{
	
}

//===========================
//�c�@�̕`�揈��
//===========================
void DrawRemain(void)
{
	int nCntRemain;

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffRemain,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRemain);

	for (nCntRemain = 0; nCntRemain < NUM_PLACE_REMAIN; nCntRemain++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRemain, 2);
	}
}

//===========================
//�c�@�̐ݒ菈��
//===========================
void SetRemain(int nRemain)
{
	int nCntRemain;

	g_nRemain = nRemain;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRemain->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRemain = 0; nCntRemain < NUM_PLACE_REMAIN; nCntRemain++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_posRemain.x + (80.0f * nCntRemain);
		pVtx[0].pos.y = g_posRemain.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posRemain.x + (80.0f * nCntRemain) + 80.0f;
		pVtx[1].pos.y = g_posRemain.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posRemain.x + (80.0f * nCntRemain);
		pVtx[2].pos.y = g_posRemain.y + 80.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posRemain.x + (80.0f * nCntRemain) + 80.0f;
		pVtx[3].pos.y = g_posRemain.y + 80.0f;
		pVtx[3].pos.z = 0.0f;

		pVtx += 4;
	}

	g_pVtxBuffRemain->Unlock();
}

//===========================
//�c�@�̒ǉ�����
//===========================
void AddRemain(int nValue)
{
	int nCntRemain;

	g_nRemain -= nValue;

	if (g_nRemain > 3)
	{
		g_nRemain = 3;
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRemain->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRemain = 0; nCntRemain < NUM_PLACE_REMAIN; nCntRemain++)
	{
		if (nCntRemain < g_nRemain)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_posRemain.x + (80.0f * nCntRemain);
			pVtx[0].pos.y = g_posRemain.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_posRemain.x + (80.0f * nCntRemain) + 80.0f;
			pVtx[1].pos.y = g_posRemain.y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_posRemain.x + (80.0f * nCntRemain);
			pVtx[2].pos.y = g_posRemain.y + 80.0f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_posRemain.x + (80.0f * nCntRemain) + 80.0f;
			pVtx[3].pos.y = g_posRemain.y + 80.0f;
			pVtx[3].pos.z = 0.0f;
		}
		else
		{
			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_posRemain.x;
			pVtx[0].pos.y = g_posRemain.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_posRemain.x;
			pVtx[1].pos.y = g_posRemain.y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_posRemain.x;
			pVtx[2].pos.y = g_posRemain.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_posRemain.x;
			pVtx[3].pos.y = g_posRemain.y;
			pVtx[3].pos.z = 0.0f;
		}
		

		pVtx += 4;
	}

	g_pVtxBuffRemain->Unlock();
}

//===========================
//�c�@�̎擾����
//===========================
int GetRemain(void)
{
	return g_nRemain;
}