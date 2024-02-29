//===========================
//
//�_�C���N�gX.Explosion�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "explosion.h"
#include "effect.h"

//�}�N����`
#define MAX_EXPLOSION (256)													//�����̍ő吔

//�����\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;														//�ʒu
	D3DXCOLOR col;															//�ړ���
	int nCntAnim;															//����
	int nCntPatternAnim;													//�A�j���[�V�����p�^�[��
	int nType;																//���
	bool bUse;																//�g�p���Ă��邩�ǂ���
}Explosion;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;								//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;							//���_�����i�[
Explosion g_aExplosion[MAX_EXPLOSION];										//�����̏��

//===========================
//�����̏���������
//===========================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\explosion001.png",
		&g_pTextureExplosion);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aExplosion[nCntExplosion].nCntAnim = 0;
		g_aExplosion[nCntExplosion].nCntPatternAnim = 0;
		g_aExplosion[nCntExplosion].nType = 0;
		g_aExplosion[nCntExplosion].bUse = false;					//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_aExplosion[nCntExplosion].pos.x - 16.0f;
		pVtx[0].pos.y = g_aExplosion[nCntExplosion].pos.y - 16.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aExplosion[nCntExplosion].pos.x + 16.0f;
		pVtx[1].pos.y = g_aExplosion[nCntExplosion].pos.y - 16.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aExplosion[nCntExplosion].pos.x - 16.0f;
		pVtx[2].pos.y = g_aExplosion[nCntExplosion].pos.y + 16.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aExplosion[nCntExplosion].pos.x + 16.0f;
		pVtx[3].pos.y = g_aExplosion[nCntExplosion].pos.y + 16.0f;
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
		pVtx[1].tex = D3DXVECTOR2(0.15f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.15f, 1.0f);

		pVtx += 4;										//���_���W�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//===========================
//�����̏I������
//===========================
void UninitExplosion(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//===========================
//�����̍X�V����
//===========================
void UpdateExplosion(void)
{
	int nCntExplosion;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			g_aExplosion[nCntExplosion].nCntAnim++;

			//�p�^�[��N0.���X�V����
			g_aExplosion[nCntExplosion].nCntPatternAnim = (g_aExplosion[nCntExplosion].nCntAnim / 5) % 8;

			if (g_aExplosion[nCntExplosion].nCntPatternAnim >= 7)
			{
				g_aExplosion[nCntExplosion].bUse = false;
			}

			if (g_aExplosion[nCntExplosion].nType == 0)
			{
				SetEffect(g_aExplosion[nCntExplosion].pos, g_aExplosion[nCntExplosion].col, 64.0f, 6);
			}
			else if (g_aExplosion[nCntExplosion].nType >= 1 && g_aExplosion[nCntExplosion].nType <= 3)
			{
				SetEffect(g_aExplosion[nCntExplosion].pos, g_aExplosion[nCntExplosion].col, 48.0f, 6);
			}
			else if (g_aExplosion[nCntExplosion].nType == 4)
			{
				SetEffect(g_aExplosion[nCntExplosion].pos, g_aExplosion[nCntExplosion].col, 48.0f, 6);
			}
			

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aExplosion[nCntExplosion].pos.x - (32.0f + (g_aExplosion[nCntExplosion].nCntPatternAnim * 3));
			pVtx[0].pos.y = g_aExplosion[nCntExplosion].pos.y - (32.0f + (g_aExplosion[nCntExplosion].nCntPatternAnim * 3));
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aExplosion[nCntExplosion].pos.x + (32.0f + (g_aExplosion[nCntExplosion].nCntPatternAnim * 3));
			pVtx[1].pos.y = g_aExplosion[nCntExplosion].pos.y - (32.0f + (g_aExplosion[nCntExplosion].nCntPatternAnim * 3));
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aExplosion[nCntExplosion].pos.x - (32.0f + (g_aExplosion[nCntExplosion].nCntPatternAnim * 3));
			pVtx[2].pos.y = g_aExplosion[nCntExplosion].pos.y + (32.0f + (g_aExplosion[nCntExplosion].nCntPatternAnim * 3));
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aExplosion[nCntExplosion].pos.x + (32.0f + (g_aExplosion[nCntExplosion].nCntPatternAnim * 3));
			pVtx[3].pos.y = g_aExplosion[nCntExplosion].pos.y + (32.0f + (g_aExplosion[nCntExplosion].nCntPatternAnim * 3));
			pVtx[3].pos.z = 0.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, 1.0f - (0.125f * g_aExplosion[nCntExplosion].nCntPatternAnim));
			pVtx[1].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, 1.0f - (0.125f * g_aExplosion[nCntExplosion].nCntPatternAnim));
			pVtx[2].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, 1.0f - (0.125f * g_aExplosion[nCntExplosion].nCntPatternAnim));
			pVtx[3].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, 1.0f - (0.125f * g_aExplosion[nCntExplosion].nCntPatternAnim));
			
			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((0.125f * g_aExplosion[nCntExplosion].nCntPatternAnim) - 0.125f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f * g_aExplosion[nCntExplosion].nCntPatternAnim, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((0.125f * g_aExplosion[nCntExplosion].nCntPatternAnim) - 0.125f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f * g_aExplosion[nCntExplosion].nCntPatternAnim, 1.0f);
		}
		pVtx += 4;
	}

	g_pVtxBuffExplosion->Unlock();
}

//===========================
//�����̕`�揈��
//===========================
void DrawExplosion(void)
{
	int nCntExplosion;

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffExplosion,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//�������g���Ă���
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntExplosion, 2);
		}
	}
}

//===========================
//�����̐ݒ菈��
//===========================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, int nType)
{
	int nCntExplosion;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].nCntAnim = 0;
			g_aExplosion[nCntExplosion].nCntPatternAnim = 0;
			g_aExplosion[nCntExplosion].nType = nType;
			g_aExplosion[nCntExplosion].bUse = true;						//�g�p���Ă����Ԃɂ���

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aExplosion[nCntExplosion].pos.x - 16.0f;
			pVtx[0].pos.y = g_aExplosion[nCntExplosion].pos.y - 16.0f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aExplosion[nCntExplosion].pos.x + 16.0f;
			pVtx[1].pos.y = g_aExplosion[nCntExplosion].pos.y - 16.0f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aExplosion[nCntExplosion].pos.x - 16.0f;
			pVtx[2].pos.y = g_aExplosion[nCntExplosion].pos.y + 16.0f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aExplosion[nCntExplosion].pos.x + 16.0f;
			pVtx[3].pos.y = g_aExplosion[nCntExplosion].pos.y + 16.0f;
			pVtx[3].pos.z = 0.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.15f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.15f, 1.0f);

			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffExplosion->Unlock();
}