//===========================
//
//�_�C���N�gX.goal�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "goal.h"
#include "player.h"

//�}�N����`
#define MAX_GOAL (256)

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureGoal = NULL;				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffGoal = NULL;			//���_�����i�[
Goal g_aGoal[MAX_GOAL];								//�G�̏��

//�e�N�X�`���t�@�C����
const char *c_apFilenameGoal[1] =
{
	"data\\TEXTURE\\goal000.png"
};

//===========================
//�G�̏���������
//===========================
void InitGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntGoal;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\goal000.png",
		&g_pTextureGoal);

	for (nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		g_aGoal[nCntGoal].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aGoal[nCntGoal].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aGoal[nCntGoal].fHeight = 80.0f;
		g_aGoal[nCntGoal].fWidth = 80.0f;
		g_aGoal[nCntGoal].bUse = false;					//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GOAL,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffGoal,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_aGoal[nCntGoal].pos.x;
		pVtx[0].pos.y = g_aGoal[nCntGoal].pos.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aGoal[nCntGoal].pos.x + g_aGoal[nCntGoal].fWidth;
		pVtx[1].pos.y = g_aGoal[nCntGoal].pos.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aGoal[nCntGoal].pos.x;
		pVtx[2].pos.y = g_aGoal[nCntGoal].pos.y + g_aGoal[nCntGoal].fHeight;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aGoal[nCntGoal].pos.x + g_aGoal[nCntGoal].fWidth;
		pVtx[3].pos.y = g_aGoal[nCntGoal].pos.y + g_aGoal[nCntGoal].fHeight;
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
	g_VtxBuffGoal->Unlock();
}

//===========================
//�G�̏I������
//===========================
void UninitGoal(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureGoal != NULL)
	{
		g_pTextureGoal->Release();
		g_pTextureGoal = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffGoal != NULL)
	{
		g_VtxBuffGoal->Release();
		g_VtxBuffGoal = NULL;
	}
}

//===========================
//�G�̍X�V����
//===========================
void UpdateGoal(void)
{
	
}

//===========================
//�G�̕`�揈��
//===========================
void DrawGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntGoal;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffGoal,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		if (g_aGoal[nCntGoal].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureGoal);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGoal * 4, 2);
		}
	}
}

//===========================
//�G�̐ݒ菈��
//===========================
void SetGoal(D3DXVECTOR3 pos, int nType)
{
	int nCntGoal;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		if (g_aGoal[nCntGoal].bUse == false)
		{
			g_aGoal[nCntGoal].pos = pos;
			g_aGoal[nCntGoal].nType = nType;
			g_aGoal[nCntGoal].bUse = true;

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aGoal[nCntGoal].pos.x;
			pVtx[0].pos.y = g_aGoal[nCntGoal].pos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aGoal[nCntGoal].pos.x + g_aGoal[nCntGoal].fWidth;
			pVtx[1].pos.y = g_aGoal[nCntGoal].pos.y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aGoal[nCntGoal].pos.x;
			pVtx[2].pos.y = g_aGoal[nCntGoal].pos.y + g_aGoal[nCntGoal].fHeight;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aGoal[nCntGoal].pos.x + g_aGoal[nCntGoal].fWidth;
			pVtx[3].pos.y = g_aGoal[nCntGoal].pos.y + g_aGoal[nCntGoal].fHeight;
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

			break;
		}

		pVtx += 4;
	}

	g_VtxBuffGoal->Unlock();
}

//===========================
//�v���C���[�Ƃ̓����蔻��
//===========================
bool CollisionGoal(D3DXVECTOR3 *pPos)
{
	bool bGoal = false;

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		if (g_aGoal[nCntGoal].bUse == true)
		{
			if (((pPos->x - (g_aGoal[nCntGoal].pos.x + 40.0f)) * (pPos->x - (g_aGoal[nCntGoal].pos.x + 40.0f))) + ((pPos->y - (g_aGoal[nCntGoal].pos.y + 40.0f)) * (pPos->y - (g_aGoal[nCntGoal].pos.y + 40.0f))) < (32.0f * 32.0f))
			{
				bGoal = true;
			}
		}
	}

	return bGoal;
}