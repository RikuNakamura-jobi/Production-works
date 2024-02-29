//===========================
//
//�_�C���N�gX.Boost�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "boost.h"
#include "player.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureBoost = NULL;								//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoost = NULL;							//���_�����i�[
D3DXVECTOR3 g_posBoost;													//�ʒu
D3DXCOLOR g_colBoost;
int g_nBoost;															//�̗͂̒l

//===========================
//�̗͂̏���������
//===========================
void InitBoost(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\boss001.png",
		&g_pTextureBoost);

	g_posBoost = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_colBoost = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	g_nBoost = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoost,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBoost->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_posBoost.x;
	pVtx[0].pos.y = g_posBoost.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posBoost.x + 64.0f;
	pVtx[1].pos.y = g_posBoost.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posBoost.x;
	pVtx[2].pos.y = g_posBoost.y + 5.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posBoost.x + 64.0f;
	pVtx[3].pos.y = g_posBoost.y + 5.0f;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = g_colBoost;
	pVtx[1].col = g_colBoost;
	pVtx[2].col = g_colBoost;
	pVtx[3].col = g_colBoost;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBoost->Unlock();
}

//===========================
//�̗͂̏I������
//===========================
void UninitBoost(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBoost != NULL)
	{
		g_pTextureBoost->Release();
		g_pTextureBoost = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBoost != NULL)
	{
		g_pVtxBuffBoost->Release();
		g_pVtxBuffBoost = NULL;
	}
}

//===========================
//�̗͂̍X�V����
//===========================
void UpdateBoost(void)
{
	Player *pPlayer = GetPlayer();

	g_posBoost.x = pPlayer->pos.x - 32.0f;
	g_posBoost.y = pPlayer->pos.y + 32.0f;
	g_posBoost.z = pPlayer->pos.z;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBoost->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_posBoost.x;
	pVtx[0].pos.y = g_posBoost.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posBoost.x + (0.4266666666666667f * g_nBoost);
	pVtx[1].pos.y = g_posBoost.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posBoost.x;
	pVtx[2].pos.y = g_posBoost.y + 5.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posBoost.x + +(0.4266666666666667f * g_nBoost);
	pVtx[3].pos.y = g_posBoost.y + 5.0f;
	pVtx[3].pos.z = 0.0f;

	if (pPlayer->playertype == PLAYERTYPE_3)
	{
		if (g_nBoost >= 150)
		{
			g_colBoost.a -= 0.1f;

			if (g_colBoost.a < 0.0f)
			{
				g_colBoost.a = 0.0f;
			}
		}
		else if (g_nBoost < 150)
		{
			g_colBoost.a = 1.0f;
		}
	}
	else
	{
		g_colBoost.a = 0.0f;
	}

	//���_�J���[�̐ݒ�
	pVtx[0].col = g_colBoost;
	pVtx[1].col = g_colBoost;
	pVtx[2].col = g_colBoost;
	pVtx[3].col = g_colBoost;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.001f * g_nBoost, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.001f * g_nBoost, 1.0f);

	g_pVtxBuffBoost->Unlock();
}

//===========================
//�̗͂̕`�揈��
//===========================
void DrawBoost(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffBoost,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBoost);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0 , 2);
}

//===========================
//�̗͂̐ݒ菈��
//===========================
void SetBoost(int nBoost)
{
	g_nBoost = nBoost;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBoost->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_posBoost.x;
	pVtx[0].pos.y = g_posBoost.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posBoost.x + (0.4266666666666667f * g_nBoost);
	pVtx[1].pos.y = g_posBoost.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posBoost.x;
	pVtx[2].pos.y = g_posBoost.y + 5.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posBoost.x + +(0.4266666666666667f * g_nBoost);
	pVtx[3].pos.y = g_posBoost.y + 5.0f;
	pVtx[3].pos.z = 0.0f;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.015625f * g_nBoost, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.015625f * g_nBoost, 1.0f);

	g_pVtxBuffBoost->Unlock();
}

//===========================
//�̗͂̒ǉ�����
//===========================
void AddBoost(int nValue)
{
	Player *pPlayer = GetPlayer();
	g_nBoost -= nValue;

	g_posBoost.x = pPlayer->pos.x - 32.0f;
	g_posBoost.y = pPlayer->pos.y + 32.0f;
	g_posBoost.z = pPlayer->pos.z;

	if (g_nBoost >= 150)
	{
		g_nBoost = 150;
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBoost->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_posBoost.x;
	pVtx[0].pos.y = g_posBoost.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posBoost.x + (0.4266666666666667f * g_nBoost);
	pVtx[1].pos.y = g_posBoost.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posBoost.x;
	pVtx[2].pos.y = g_posBoost.y + 5.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posBoost.x + +(0.4266666666666667f * g_nBoost);
	pVtx[3].pos.y = g_posBoost.y + 5.0f;
	pVtx[3].pos.z = 0.0f;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.015625f * g_nBoost, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.015625f * g_nBoost, 1.0f);

	g_pVtxBuffBoost->Unlock();
}

//===========================
//�̗͂̎擾����
//===========================
int GetBoost(void)
{
	return g_nBoost;
}