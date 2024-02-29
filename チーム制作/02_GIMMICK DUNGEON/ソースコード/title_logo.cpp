//==========================================
//
//  �^�C�g�����S(title_logo.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "title_logo.h"
#include "title.h"

//==========================================
//  �}�N����`
//==========================================
#define PASS_TITLE_LOGO "data/TEXTURE/title002.png" //�e�N�X�`���p�X
#define POS_DEFAULT (D3DXVECTOR3(640.0f, 80.0f ,0.0f)) //�����ʒu
#define END_Y (240.0f) //Y���W�̏��
#define POS_END (D3DXVECTOR3(640.0f, END_Y ,0.0f)) //�ŏI�ʒu
#define LOGO_MOVE ((END_Y - 80.0f) / IN_TIME) //�ړ���
#define LOGO_ALPHA_SCALE (1.0f / IN_TIME) //�A���t�@�l�̏㏸��
#define LOGO_POLYGON_WIDTH (500.0f) //�|���S���̕�
#define LOGO_POLYGON_HEIGHT (180.0f) //�|���S���̍���

//==========================================
//  �\���̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //���S���W
	D3DXCOLOR col; //�|���S���J���[
}LOGO;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureTitleLogo = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;
LOGO g_logo;

//==========================================
//  ����������
//==========================================
void InitLogo()
{
	//�ϐ��̏�����
	g_logo.pos = POS_DEFAULT;
	g_logo.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleLogo,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		PASS_TITLE_LOGO,
		&g_pTextureTitleLogo
	);

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_logo.pos.x - LOGO_POLYGON_WIDTH, g_logo.pos.y - LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_logo.pos.x + LOGO_POLYGON_WIDTH, g_logo.pos.y - LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_logo.pos.x - LOGO_POLYGON_WIDTH, g_logo.pos.y + LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_logo.pos.x + LOGO_POLYGON_WIDTH, g_logo.pos.y + LOGO_POLYGON_HEIGHT, 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//rhw�̐ݒ�
		pVtx[nCnt].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[nCnt].col = g_logo.col;
	}

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitleLogo->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitLogo()
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTextureTitleLogo != NULL)
	{
		g_pTextureTitleLogo->Release();
		g_pTextureTitleLogo = NULL;
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdateLogo()
{
	//���[�J���ϐ��錾
	int nTitleState = GetTitleState();

	//��ԂɑΉ��������������s����
	switch (nTitleState)
	{
	case TITLESTATE_IN:
		//���S�̏����X�V����
		if (g_logo.pos.y < END_Y)
		{
			g_logo.pos.y += LOGO_MOVE;
			g_logo.col.a += LOGO_ALPHA_SCALE;
		}

		//���S�̏���␳����
		if (g_logo.pos.y > END_Y)
		{
			g_logo.pos.y = END_Y;
			g_logo.col.a = 1.0f;
		}
		break;
	case TITLESTATE_NORMAL:
		//�ʒu���Œ肷��
		if (g_logo.pos != POS_END)
		{
			g_logo.pos = POS_END;
		}
		//�F���Œ肷��
		if (g_logo.col.a != 1.0f)
		{
			g_logo.col.a = 1.0f;
		}
		break;
	default:
		break;
	}

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(g_logo.pos.x - LOGO_POLYGON_WIDTH, g_logo.pos.y - LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_logo.pos.x + LOGO_POLYGON_WIDTH, g_logo.pos.y - LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_logo.pos.x - LOGO_POLYGON_WIDTH, g_logo.pos.y + LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_logo.pos.x + LOGO_POLYGON_WIDTH, g_logo.pos.y + LOGO_POLYGON_HEIGHT, 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//���_�J���[�̍X�V
		pVtx[nCnt].col = g_logo.col;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitleLogo->Unlock();
}

//==========================================
//  �`�揈��
//==========================================
void DrawLogo()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTitleLogo);

	//���S�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
