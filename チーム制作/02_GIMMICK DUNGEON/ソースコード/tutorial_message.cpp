//==========================================
//
//  �`���[�g���A�����b�Z�[�W(tutorial_message.cpp)
//  Author : Kai Takada
//
//==========================================
#include "tutorial_message.h"

//==========================================
//  �}�N����`
//==========================================
#define PASS_TUTORIAL_MESSAGE "data/TEXTURE/tutorial_message.png" //�e�N�X�`���p�X
#define MESSAGE_POS_LEFT (D3DXVECTOR3(1050.0f, 650.0f,0.0f)) //���b�Z�[�W�̒��S���W
#define MESSAGE_POLYGON_WIDTH (250.0f) //�|���S���̕�
#define MESSAGE_POLYGON_HEIGHT (34.0f) //�|���S���̍���
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
LPDIRECT3DTEXTURE9 g_pTextureTutorialMessage = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialMessage = NULL;
MESSAGE g_tutorialMessage;

//==========================================
//  ����������
//==========================================
void InitTutorialMessage()
{
	//�ϐ��̏�����
	g_tutorialMessage.pos = MESSAGE_POS_LEFT;
	g_tutorialMessage.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_tutorialMessage.fScale = 1.0f;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialMessage,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		PASS_TUTORIAL_MESSAGE,
		&g_pTextureTutorialMessage
	);

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTutorialMessage->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_tutorialMessage.pos.x - (MESSAGE_POLYGON_WIDTH + (g_tutorialMessage.fScale * SIZE_RATIO)), g_tutorialMessage.pos.y - (MESSAGE_POLYGON_HEIGHT + g_tutorialMessage.fScale), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_tutorialMessage.pos.x + (MESSAGE_POLYGON_WIDTH + (g_tutorialMessage.fScale * SIZE_RATIO)), g_tutorialMessage.pos.y - (MESSAGE_POLYGON_HEIGHT + g_tutorialMessage.fScale), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_tutorialMessage.pos.x - (MESSAGE_POLYGON_WIDTH + (g_tutorialMessage.fScale * SIZE_RATIO)), g_tutorialMessage.pos.y + (MESSAGE_POLYGON_HEIGHT + g_tutorialMessage.fScale), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_tutorialMessage.pos.x + (MESSAGE_POLYGON_WIDTH + (g_tutorialMessage.fScale * SIZE_RATIO)), g_tutorialMessage.pos.y + (MESSAGE_POLYGON_HEIGHT + g_tutorialMessage.fScale), 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//rhw�̐ݒ�
		pVtx[nCnt].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[nCnt].col = g_tutorialMessage.col;
	}

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTutorialMessage->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitTutorialMessage()
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorialMessage != NULL)
	{
		g_pVtxBuffTutorialMessage->Release();
		g_pVtxBuffTutorialMessage = NULL;
	}

	if (g_pTextureTutorialMessage != NULL)
	{
		g_pTextureTutorialMessage->Release();
		g_pTextureTutorialMessage = NULL;
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdateTutorialMessage()
{
}

//==========================================
//  �`�揈��
//==========================================
void DrawTutorialMessage()
{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffTutorialMessage, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTutorialMessage);

		//���S�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
