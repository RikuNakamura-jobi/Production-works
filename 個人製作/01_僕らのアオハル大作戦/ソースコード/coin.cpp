//===========================
//
//�_�C���N�gX.coin�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "coin.h"
#include "player.h"

//�}�N����`
#define MAX_COIN (256)

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureCoin = NULL;				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffCoin = NULL;			//���_�����i�[
Coin g_aCoin[MAX_COIN];								//�G�̏��
int g_nNumCoin;

//�e�N�X�`���t�@�C����
const char *c_apFilenameCoin[1] =
{
	"data\\TEXTURE\\coin000.png"
};

//===========================
//�G�̏���������
//===========================
void InitCoin(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntCoin;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Coin000.png",
		&g_pTextureCoin);

	for (nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++)
	{
		g_aCoin[nCntCoin].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCntCoin].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCoin[nCntCoin].fHeight = 80.0f;
		g_aCoin[nCntCoin].fWidth = 80.0f;
		g_aCoin[nCntCoin].bUse = false;					//�g�p���Ă��Ȃ���Ԃɂ���
	}

	g_nNumCoin = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_COIN,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffCoin,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffCoin->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_aCoin[nCntCoin].pos.x;
		pVtx[0].pos.y = g_aCoin[nCntCoin].pos.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aCoin[nCntCoin].pos.x + g_aCoin[nCntCoin].fWidth;
		pVtx[1].pos.y = g_aCoin[nCntCoin].pos.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aCoin[nCntCoin].pos.x;
		pVtx[2].pos.y = g_aCoin[nCntCoin].pos.y + g_aCoin[nCntCoin].fHeight;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aCoin[nCntCoin].pos.x + g_aCoin[nCntCoin].fWidth;
		pVtx[3].pos.y = g_aCoin[nCntCoin].pos.y + g_aCoin[nCntCoin].fHeight;
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
	g_VtxBuffCoin->Unlock();
}

//===========================
//�G�̏I������
//===========================
void UninitCoin(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureCoin != NULL)
	{
		g_pTextureCoin->Release();
		g_pTextureCoin = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffCoin != NULL)
	{
		g_VtxBuffCoin->Release();
		g_VtxBuffCoin = NULL;
	}
}

//===========================
//�G�̍X�V����
//===========================
void UpdateCoin(void)
{

}

//===========================
//�G�̕`�揈��
//===========================
void DrawCoin(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntCoin;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffCoin,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++)
	{
		if (g_aCoin[nCntCoin].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureCoin);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCoin * 4, 2);
		}
	}
}

//===========================
//�G�̐ݒ菈��
//===========================
void SetCoin(D3DXVECTOR3 pos, int nType)
{
	int nCntCoin;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffCoin->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++)
	{
		if (g_aCoin[nCntCoin].bUse == false)
		{
			g_aCoin[nCntCoin].pos = pos;
			g_aCoin[nCntCoin].nType = nType;
			g_aCoin[nCntCoin].bUse = true;

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aCoin[nCntCoin].pos.x;
			pVtx[0].pos.y = g_aCoin[nCntCoin].pos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aCoin[nCntCoin].pos.x + g_aCoin[nCntCoin].fWidth;
			pVtx[1].pos.y = g_aCoin[nCntCoin].pos.y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aCoin[nCntCoin].pos.x;
			pVtx[2].pos.y = g_aCoin[nCntCoin].pos.y + g_aCoin[nCntCoin].fHeight;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aCoin[nCntCoin].pos.x + g_aCoin[nCntCoin].fWidth;
			pVtx[3].pos.y = g_aCoin[nCntCoin].pos.y + g_aCoin[nCntCoin].fHeight;
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

			g_nNumCoin++;
			break;
		}

		pVtx += 4;
	}

	g_VtxBuffCoin->Unlock();
}

//===========================
//�v���C���[�Ƃ̓����蔻��
//===========================
bool CollisionCoin(D3DXVECTOR3 *pPos)
{
	bool bCoin = false;

	for (int nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++)
	{
		if (g_aCoin[nCntCoin].bUse == true)
		{
			if (((pPos->x - (g_aCoin[nCntCoin].pos.x + 40.0f)) * (pPos->x - (g_aCoin[nCntCoin].pos.x + 40.0f))) + ((pPos->y - (g_aCoin[nCntCoin].pos.y + 60.0f)) * (pPos->y - (g_aCoin[nCntCoin].pos.y + 60.0f))) < (64.0f * 64.0f))
			{
				bCoin = true;
				g_aCoin[nCntCoin].bUse = false;
				g_nNumCoin--;
			}
		}
	}

	return bCoin;
}

//===========================
//�G�̑S�Ŕ��菈��
//===========================
int GetNumCoin(void)
{
	return g_nNumCoin;
}