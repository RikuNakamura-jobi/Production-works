//===========================
//
//�_�C���N�gX.title�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "title.h"
#include "sound.h"
#include "input.h"
#include "fade.h"

//�}�N����`
#define NUM_TITLE (3)											//�^�C�g����ʂ̐�

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureTitle[NUM_TITLE] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffTitle = NULL;					//���_�����i�[
int g_nCounterTitleState;										//��ԊǗ��J�E���^�[
int nTimerRanking;												//�^�C�g���̃����L���O�J�ڃ^�C�}�[
float g_fCntTitle = 0.0f;										//�^�C�g���̓_�ŗp�ϐ�
bool g_TitleEnd;												//�I������

//===========================
//�^�C�g����ʂ̏���������
//===========================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTitle;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\title002.png",
		&g_apTextureTitle[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\title003.png",
		&g_apTextureTitle[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\title004.png",
		&g_apTextureTitle[2]);

	g_nCounterTitleState = 120;
	g_fCntTitle = 0.0f;
	nTimerRanking = 0;
	g_TitleEnd = false;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TITLE,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffTitle,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffTitle->Unlock();

	PlaySound(SOUND_LABEL_BGM010);
}

//===========================
//�^�C�g����ʂ̏I������
//===========================
void UninitTitle(void)
{
	int nCntTitle;

	//�T�E���h�̒�~
	StopSound();

	for (nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureTitle[nCntTitle] != NULL)
		{
			g_apTextureTitle[nCntTitle]->Release();
			g_apTextureTitle[nCntTitle] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffTitle != NULL)
	{
		g_VtxBuffTitle->Release();
		g_VtxBuffTitle = NULL;
	}
}

//===========================
//�^�C�g����ʂ̍X�V����
//===========================
void UpdateTitle(void)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	FADE fade = GetFade();
	int nCntTitle;

	nTimerRanking++;

	if (g_TitleEnd == false)
	{
		col.a = (sinf(g_fCntTitle) + 1.0f) / 2;

		g_fCntTitle += 0.05f;
	}
	else
	{
		col.a = (sinf(g_fCntTitle) + 1.0f) / 2;

		g_fCntTitle += 0.5f;
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		if (nCntTitle == 2)
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffTitle->Unlock();

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetPadButtonTrigger(0) == true)  && g_TitleEnd == false)
	{
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_DECISION001);

		g_TitleEnd = true;
	}

	if (g_TitleEnd == true)
	{
		g_nCounterTitleState--;

		if (g_nCounterTitleState <= 0 && fade == FADE_NONE)
		{
			//���[�h�ݒ�
			SetFade(MODE_TUTORIAL);
		}
	}

	if (nTimerRanking == 1200 && g_TitleEnd == false)
	{
		//���[�h�ݒ�
		SetFade(MODE_RANKING);
	}
}

//===========================
//�^�C�g����ʂ̕`�揈��
//===========================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTitle;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffTitle,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTitle = 0; nCntTitle < NUM_TITLE; nCntTitle++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureTitle[nCntTitle]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
	}
}