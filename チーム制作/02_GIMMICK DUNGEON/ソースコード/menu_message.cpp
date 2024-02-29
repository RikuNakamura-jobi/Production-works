//==========================================
//
//  ���j���[���b�Z�[�W(menu_message.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "menu_message.h"

//==========================================
//  �}�N����`
//==========================================
#define PASS_MENU_MESSAGE "data/TEXTURE/menu000.png" //�e�N�X�`���p�X
#define MESSAGE_POS_LEFT (D3DXVECTOR3(640.0f, 90.0f, 0.0f)) //���S���W
#define MESSAGE_WIDTH (200.0f) //����
#define MESSAGE_HEIGHT (70.0f) //����

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureMenuMessage = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMenuMessage = NULL;

//==========================================
//  ����������
//==========================================
void InitMenuMessage()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMenuMessage,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		PASS_MENU_MESSAGE,
		&g_pTextureMenuMessage
	);

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMenuMessage->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(MESSAGE_POS_LEFT.x - MESSAGE_WIDTH, MESSAGE_POS_LEFT.y - MESSAGE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(MESSAGE_POS_LEFT.x + MESSAGE_WIDTH, MESSAGE_POS_LEFT.y - MESSAGE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(MESSAGE_POS_LEFT.x - MESSAGE_WIDTH, MESSAGE_POS_LEFT.y + MESSAGE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(MESSAGE_POS_LEFT.x + MESSAGE_WIDTH, MESSAGE_POS_LEFT.y + MESSAGE_HEIGHT, 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//rhw�̐ݒ�
		pVtx[nCnt].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMenuMessage->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitMenuMessage()
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMenuMessage != NULL)
	{
		g_pVtxBuffMenuMessage->Release();
		g_pVtxBuffMenuMessage = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTextureMenuMessage != NULL)
	{
		g_pTextureMenuMessage->Release();
		g_pTextureMenuMessage = NULL;
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdateMenuMessage()
{

}

//==========================================
//  �`�揈��
//==========================================
void DrawMenuMessage()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMenuMessage, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMenuMessage);

	//���b�Z�[�W�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}