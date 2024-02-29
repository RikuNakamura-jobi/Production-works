//==========================================================
//
//�|���S������ [polygon.h]
//Author Ibuki Okusada
//
//==========================================================
#include "polygon.h"
#include "texture.h"
#include "fileload.h"
#include "texture.h"
#include "player.h"

//==========================================================
//�}�N����`
//==========================================================
#define MAX_POLYGON		(3)

//==========================================================
//�O���[�o���ϐ�
//==========================================================
LPDIRECT3DTEXTURE9 g_pTexturePolygon[MAX_POLYGON] = {};			//�e�N�X�`��1�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;	//���_�o�b�t�@�ւ̃|�C���^
polygon g_aPolygon[MAX_POLYGON];
float g_fPolyTexU = 0.0f;

//==========================================================
//�|���S���̏���������
//==========================================================
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^
	g_fPolyTexU = 0.0f;

	//�t�@�C���̏�����
	InitFileLoad();
	GametxtFileLoad();

	//�e�N�X�`���̏�����
	InitTexture();

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (GetMode() == MODE_GAME && nCntPolygon < MAX_POLYGON - 1)
		{
			g_pTexturePolygon[nCntPolygon] = *SetTexture(nCntPolygon);
		}
		else
		{
			g_pTexturePolygon[nCntPolygon] = *SetTexture(0);
		}

		g_aPolygon[nCntPolygon].pos = {};
		g_aPolygon[nCntPolygon].rot = {};
		g_aPolygon[nCntPolygon].fWidth = 0.0f;
		g_aPolygon[nCntPolygon].fHeight = 0.0f;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(g_fPolyTexU, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_fPolyTexU + 1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_fPolyTexU, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_fPolyTexU + 1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}

//==========================================================
//�|���S���̏I������
//==========================================================
void UninitPolygon(void)
{
	//�e�N�X�`���̏I������
	UninitTexture();

	//�e�N�X�`���̔j��
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (g_pTexturePolygon[nCntPolygon] != NULL)
		{
			g_pTexturePolygon[nCntPolygon]->Release();
			g_pTexturePolygon[nCntPolygon] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//==========================================================
//�|���S���̍X�V����
//==========================================================
void UpdatePolygon(void)
{
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	switch (GetMode())
	{
	case MODE_TITLE:	//�^�C�g�����
		g_fPolyTexU += 0.0003f;

		if (g_fPolyTexU > 1.0f)
		{
			g_fPolyTexU = 0.0f;
		}
		break;
	case MODE_TUTORIAL:	//�`���[�g���A�����
		break;
	case MODE_RESULT:	//���U���g���
		g_fPolyTexU += 0.0003f;

		if (g_fPolyTexU > 1.0f)
		{
			g_fPolyTexU = 0.0f;
		}
		break;
	case MODE_RANKING:	//�����L���O���
		break;
	default:	//�Q�[�����
		if (GetPlayer()->move.x < 0.0f)
		{
			g_fPolyTexU += 0.0003f;
		}
		else
		{
			g_fPolyTexU += 0.0001f;
		}

		if (g_fPolyTexU > 1.0f)
		{
			g_fPolyTexU = 0.0f;
		}

		break;
	}

	if (MODE_TUTORIAL != GetMode())
	{
		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
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
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(g_fPolyTexU, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_fPolyTexU + 0.5f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_fPolyTexU, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_fPolyTexU + 0.5f, 1.0f);

			pVtx += 4;
		}

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffPolygon->Unlock();
	}
}

//==========================================================
//�|���S���̕`�揈��
//==========================================================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (GetMode() == MODE_GAME && nCntPolygon >= MAX_POLYGON - 1)
		{
			break;
		}

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygon[nCntPolygon]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
			4 * nCntPolygon,								//�v���~�e�B�u�i�|���S���̐��j
			2);												//�`�悷��v���~�e�B�u��
	}
}