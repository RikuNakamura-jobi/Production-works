//===========================
//
//�_�C���N�gX.start�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "start.h"
#include "player.h"

//�}�N����`
#define MAX_START (256)

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureStart = NULL;				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffStart = NULL;			//���_�����i�[
Start g_aStart[MAX_START];								//�G�̏��

//�e�N�X�`���t�@�C����
const char *c_apFilenameStart[1] =
{
	"data\\TEXTURE\\start000.png"
};

//===========================
//�G�̏���������
//===========================
void InitStart(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntStart;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\start000.png",
		&g_pTextureStart);

	for (nCntStart = 0; nCntStart < MAX_START; nCntStart++)
	{
		g_aStart[nCntStart].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aStart[nCntStart].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aStart[nCntStart].fHeight = 80.0f;
		g_aStart[nCntStart].fWidth = 80.0f;
		g_aStart[nCntStart].bUse = false;					//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_START,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffStart,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStart = 0; nCntStart < MAX_START; nCntStart++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_aStart[nCntStart].pos.x;
		pVtx[0].pos.y = g_aStart[nCntStart].pos.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aStart[nCntStart].pos.x + g_aStart[nCntStart].fWidth;
		pVtx[1].pos.y = g_aStart[nCntStart].pos.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aStart[nCntStart].pos.x;
		pVtx[2].pos.y = g_aStart[nCntStart].pos.y + g_aStart[nCntStart].fHeight;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aStart[nCntStart].pos.x + g_aStart[nCntStart].fWidth;
		pVtx[3].pos.y = g_aStart[nCntStart].pos.y + g_aStart[nCntStart].fHeight;
		pVtx[3].pos.z = 0.0f;

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
	g_VtxBuffStart->Unlock();
}

//===========================
//�G�̏I������
//===========================
void UninitStart(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureStart != NULL)
	{
		g_pTextureStart->Release();
		g_pTextureStart = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffStart != NULL)
	{
		g_VtxBuffStart->Release();
		g_VtxBuffStart = NULL;
	}
}

//===========================
//�G�̍X�V����
//===========================
void UpdateStart(void)
{

}

//===========================
//�G�̕`�揈��
//===========================
void DrawStart(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntStart;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffStart,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntStart = 0; nCntStart < MAX_START; nCntStart++)
	{
		if (g_aStart[nCntStart].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureStart);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntStart * 4, 2);
		}
	}
}

//===========================
//�G�̐ݒ菈��
//===========================
void SetStart(D3DXVECTOR3 pos, int nType)
{
	int nCntStart;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStart = 0; nCntStart < MAX_START; nCntStart++)
	{
		if (g_aStart[nCntStart].bUse == false)
		{
			g_aStart[nCntStart].pos = pos;
			g_aStart[nCntStart].nType = nType;
			g_aStart[nCntStart].bUse = true;

			SetPlayer(D3DXVECTOR3 (pos.x + 40.0f, pos.y + 40.0f, 0.0f));

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aStart[nCntStart].pos.x;
			pVtx[0].pos.y = g_aStart[nCntStart].pos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aStart[nCntStart].pos.x + g_aStart[nCntStart].fWidth;
			pVtx[1].pos.y = g_aStart[nCntStart].pos.y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aStart[nCntStart].pos.x;
			pVtx[2].pos.y = g_aStart[nCntStart].pos.y + g_aStart[nCntStart].fHeight;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aStart[nCntStart].pos.x + g_aStart[nCntStart].fWidth;
			pVtx[3].pos.y = g_aStart[nCntStart].pos.y + g_aStart[nCntStart].fHeight;
			pVtx[3].pos.z = 0.0f;

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

			break;
		}

		pVtx += 4;
	}

	g_VtxBuffStart->Unlock();
}

//===========================
//�v���C���[�Ƃ̓����蔻��
//===========================
bool CollisionStart(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fAngle, float fLength, float fRot, Start **pStart)
{
	bool bLand = false;

	for (int nCntStart = 0; nCntStart < MAX_START; nCntStart++)
	{
		if (g_aStart[nCntStart].bUse == true)
		{
			if ((pPos->x + 24.0f > g_aStart[nCntStart].pos.x && pPos->x - 24.0f < g_aStart[nCntStart].pos.x + g_aStart[nCntStart].fWidth) && (pPos->y + 32.0f > g_aStart[nCntStart].pos.y && pPosOld->y + 32.0f <= g_aStart[nCntStart].pos.y) && pPos->y + 32.0f < g_aStart[nCntStart].pos.y + g_aStart[nCntStart].fHeight)
			{
				bLand = true;
			}
			else if ((pPos->x + 24.0f > g_aStart[nCntStart].pos.x && pPos->x - 24.0f < g_aStart[nCntStart].pos.x + g_aStart[nCntStart].fWidth) && (pPos->y - 32.0f < g_aStart[nCntStart].pos.y + g_aStart[nCntStart].fHeight && pPosOld->y - 32.0f >= g_aStart[nCntStart].pos.y + g_aStart[nCntStart].fHeight) && pPos->y - 32.0f > g_aStart[nCntStart].pos.y)
			{
				bLand = true;
			}
		}
	}

	return bLand;
}