//==========================================================
//
//�^�C�g����ʏ��� [title.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "polygon.h"

//==================================================
//�}�N���錾
//==================================================
#define MAX_TITLE	(2)

//==================================================
//�\���̂̒�`
//==================================================

//==================================================
//�O���[�o���ϐ��錾
//==================================================

//==================================================
//�v���g�^�C�v�錾
//==================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE] = {};			//�e�N�X�`��1�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;	//���_�o�b�t�@�ւ̃|�C���^
polygon g_aTitlePolygon[MAX_TITLE];

//==================================================
//�t�@�C����
//==================================================
const char* c_apFilenameTitle[MAX_TITLE] =
{
	"data\\TEXTURE\\title.png",		//���S��
	"data\\TEXTURE\\pressenter000.png",		//���S�̃R�s�[
	//"data\\TEXTURE\\enemy001.png",		//���S�̃R�s�[
};

//==================================================
//�^�C�g����ʂ̏���������
//==================================================
void InitTitle(void)
{
	//�|���S���̏�����
	InitPolygon();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	for (int nCntPolygon = 0; nCntPolygon < MAX_TITLE; nCntPolygon++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameTitle[nCntPolygon],
			&g_pTextureTitle[nCntPolygon]);

		g_aTitlePolygon[nCntPolygon].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.3f, 0.0f);
		g_aTitlePolygon[nCntPolygon].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTitlePolygon[nCntPolygon].fWidth = 300.0f;
		g_aTitlePolygon[nCntPolygon].fHeight = 200.0f;
	}

	g_aTitlePolygon[1].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.8f, 0.0f);
	g_aTitlePolygon[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aTitlePolygon[1].fWidth = 280.0f;
	g_aTitlePolygon[1].fHeight = 60.0f;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TITLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_TITLE; nCntPolygon++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aTitlePolygon[nCntPolygon].pos.x - g_aTitlePolygon[nCntPolygon].fWidth, g_aTitlePolygon[nCntPolygon].pos.y - g_aTitlePolygon[nCntPolygon].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTitlePolygon[nCntPolygon].pos.x + g_aTitlePolygon[nCntPolygon].fWidth, g_aTitlePolygon[nCntPolygon].pos.y - g_aTitlePolygon[nCntPolygon].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTitlePolygon[nCntPolygon].pos.x - g_aTitlePolygon[nCntPolygon].fWidth, g_aTitlePolygon[nCntPolygon].pos.y + g_aTitlePolygon[nCntPolygon].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTitlePolygon[nCntPolygon].pos.x + g_aTitlePolygon[nCntPolygon].fWidth, g_aTitlePolygon[nCntPolygon].pos.y + g_aTitlePolygon[nCntPolygon].fHeight, 0.0f);

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

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();

	PlaySound(SOUND_LABEL_TITLE);	//BGM�̍Đ�
}

//==================================================
//�^�C�g����ʂ̏I������
//==================================================
void UninitTitle(void)
{
	StopSound();	//BGM�̒�~

	//�|���S���̏I������
	UninitPolygon();

	//�e�N�X�`���̔j��
	for (int nCntPolygon = 0; nCntPolygon < MAX_TITLE; nCntPolygon++)
	{
		if (g_pTextureTitle[nCntPolygon] != NULL)
		{
			g_pTextureTitle[nCntPolygon]->Release();
			g_pTextureTitle[nCntPolygon] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//==================================================
//�^�C�g����ʂ̍X�V����
//==================================================
void UpdateTitle(void)
{
	//�|���S���̍X�V����
	UpdatePolygon();

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamepadTrigger(BUTTON_A, 0))
	{//�J�ڃL�[�������ꂽ
		SetFade(MODE_TUTORIAL);
		PlaySound(SOUND_LABEL_SEBUTTON);
	}
}

//==================================================
//�^�C�g����ʂ̕`�揈��
//==================================================
void DrawTitle(void)
{
	//�|���S���̕`�揈��
	DrawPolygon();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾

	for (int nCntPolygon = 0; nCntPolygon < MAX_TITLE; nCntPolygon++)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[nCntPolygon]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
			4 * nCntPolygon,								//�v���~�e�B�u�i�|���S���̐��j
			2);												//�`�悷��v���~�e�B�u��
	}
}
