//==========================================
//
//  �^�C�g�����b�Z�[�W(title_message.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "title_message.h"
#include "title.h"

//==========================================
//  �}�N����`
//==========================================
#define PASS_TITLE_MESSAGE "data/TEXTURE/PRESS_ANY_KEY000.png" //�e�N�X�`���p�X
#define MESSAGE_POS_LEFT (D3DXVECTOR3(640.0f, 540.0f,0.0f)) //���b�Z�[�W�̒��S���W
#define MESSAGE_ALPHA_SCALE (0.05f) //�A���t�@�l�̌�����
#define SCALE_UP (4.0f) //�|���S���̊g�嗦
#define SIZE_LOOP (0.02f) //�|���S���̊g�嗦
#define SCALE_MAX (2.0f) //�g��̍ő��
#define SCALE_MIN (0.0f) //�g��̍ŏ���
#define MESSAGE_POLYGON_WIDTH (300.0f) //�|���S���̕�
#define MESSAGE_POLYGON_HEIGHT (40.0f) //�|���S���̍���
#define SIZE_RATIO (MESSAGE_POLYGON_WIDTH / MESSAGE_POLYGON_HEIGHT) //�����ƕ��̔䗦

//==========================================
//  �\���̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //���S���W
	D3DXCOLOR col; //�|���S���J���[
	float fScale; //�|���S���̊g�嗦
}MESSAGE;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureTitleMessage = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleMessage = NULL;
MESSAGE g_message;
float g_size;

//==========================================
//  ����������
//==========================================
void InitMessage()
{
	//�ϐ��̏�����
	g_message.pos = MESSAGE_POS_LEFT;
	g_message.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_message.fScale = 0.0f;
	g_size = SIZE_LOOP;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleMessage,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		PASS_TITLE_MESSAGE,
		&g_pTextureTitleMessage
	);

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTitleMessage->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_message.pos.x - (MESSAGE_POLYGON_WIDTH + (g_message.fScale * SIZE_RATIO)), g_message.pos.y - (MESSAGE_POLYGON_HEIGHT + g_message.fScale), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_message.pos.x + (MESSAGE_POLYGON_WIDTH + (g_message.fScale * SIZE_RATIO)), g_message.pos.y - (MESSAGE_POLYGON_HEIGHT + g_message.fScale), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_message.pos.x - (MESSAGE_POLYGON_WIDTH + (g_message.fScale * SIZE_RATIO)), g_message.pos.y + (MESSAGE_POLYGON_HEIGHT + g_message.fScale), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_message.pos.x + (MESSAGE_POLYGON_WIDTH + (g_message.fScale * SIZE_RATIO)), g_message.pos.y + (MESSAGE_POLYGON_HEIGHT + g_message.fScale), 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//rhw�̐ݒ�
		pVtx[nCnt].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[nCnt].col = g_message.col;
	}

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitleMessage->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitMessage()
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitleMessage != NULL)
	{
		g_pVtxBuffTitleMessage->Release();
		g_pVtxBuffTitleMessage = NULL;
	}

	if (g_pTextureTitleMessage != NULL)
	{
		g_pTextureTitleMessage->Release();
		g_pTextureTitleMessage = NULL;
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdateMessage()
{
	//���[�J���ϐ��錾
	int nTitleState = GetTitleState();

	//��ԂɑΉ��������������s����
	switch (nTitleState)
	{
	case TITLESTATE_OUT:
		//�s�����x�̍X�V
		g_message.col.a -= MESSAGE_ALPHA_SCALE;
		//�T�C�Y�̍X�V
		g_message.fScale += SCALE_UP;
		break;
	default:
		//�T�C�Y�̍X�V
		g_message.fScale += g_size;

		//�g�k�̐؂�ւ��␳
		if (g_message.fScale < SCALE_MIN || g_message.fScale > SCALE_MAX)
		{
			g_size *= -1.0f;
		}
		break;
	}

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTitleMessage->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(g_message.pos.x - (MESSAGE_POLYGON_WIDTH + (g_message.fScale * SIZE_RATIO)), g_message.pos.y - (MESSAGE_POLYGON_HEIGHT + g_message.fScale), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_message.pos.x + (MESSAGE_POLYGON_WIDTH + (g_message.fScale * SIZE_RATIO)), g_message.pos.y - (MESSAGE_POLYGON_HEIGHT + g_message.fScale), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_message.pos.x - (MESSAGE_POLYGON_WIDTH + (g_message.fScale * SIZE_RATIO)), g_message.pos.y + (MESSAGE_POLYGON_HEIGHT + g_message.fScale), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_message.pos.x + (MESSAGE_POLYGON_WIDTH + (g_message.fScale * SIZE_RATIO)), g_message.pos.y + (MESSAGE_POLYGON_HEIGHT + g_message.fScale), 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//���_�J���[�̍X�V
		pVtx[nCnt].col = g_message.col;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitleMessage->Unlock();
}

//==========================================
//  �`�揈��
//==========================================
void DrawMessage()
{
	if (GetTitleState() != TITLESTATE_IN)
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffTitleMessage, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitleMessage);

		//���S�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}
