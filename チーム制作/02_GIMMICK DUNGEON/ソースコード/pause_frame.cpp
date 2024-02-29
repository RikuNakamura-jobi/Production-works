//==========================================
//
//  �|�[�Y�̘g(pause_frame.cpp)
//  Author : ���v�ԗD��
//
//==========================================
#include"pause_frame.h"

//==========================================
//  �}�N����`
//==========================================
#define PASS_PAUSE_FRAME "data\\TEXTURE\\pause_frame000.png" //�e�N�X�`���p�X
#define FRAME_POS (D3DXVECTOR3(640.0f,400.0f, 0.0f)) //���S���W
#define FRAME_WIDTH (260.0f) //����
#define FRAME_HEIGHT (300.0f) //����

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTexturePauseFrame = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPauseFrame = NULL;

//==========================================
//  ����������
//==========================================
void InitPauseFrame()
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
		&g_pVtxBuffPauseFrame,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		PASS_PAUSE_FRAME,
		&g_pTexturePauseFrame
	);

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPauseFrame->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(FRAME_POS.x - FRAME_WIDTH, FRAME_POS.y - FRAME_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(FRAME_POS.x + FRAME_WIDTH, FRAME_POS.y - FRAME_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(FRAME_POS.x - FRAME_WIDTH, FRAME_POS.y + FRAME_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(FRAME_POS.x + FRAME_WIDTH, FRAME_POS.y + FRAME_HEIGHT, 0.0f);

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
	g_pVtxBuffPauseFrame->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitPauseFrame()
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPauseFrame != NULL)
	{
		g_pVtxBuffPauseFrame->Release();
		g_pVtxBuffPauseFrame = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTexturePauseFrame != NULL)
	{
		g_pTexturePauseFrame->Release();
		g_pTexturePauseFrame = NULL;
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdatePauseFrame()
{

}

//==========================================
//  �`�揈��
//==========================================
void DrawPauseFrame()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPauseFrame, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePauseFrame);

	//���S�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}