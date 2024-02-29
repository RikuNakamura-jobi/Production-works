//===========================
//
//�_�C���N�gX.tutorial�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "tutorial.h"
#include "sound.h"
#include "input.h"
#include "fade.h"

//�}�N����`
#define NUM_TUTORIAL (9)											//�`���[�g���A����ʂ̐�

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureTutorial[NUM_TUTORIAL] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffTutorial = NULL;					//���_�����i�[
int g_nCounterTutorialState;										//��ԊǗ��J�E���^�[
int g_nPatternTutorial;												//�`���[�g���A���p�^�[��
float g_fCntTutorial;												//�`���[�g���A���̓_�ŗp�ϐ�
bool g_TutorialEnd;													//�I������

//===========================
//�`���[�g���A����ʂ̏���������
//===========================
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTutorial;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\tutorial000.png",
		&g_apTextureTutorial[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Tutorial001.png",
		&g_apTextureTutorial[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Tutorial006.png",
		&g_apTextureTutorial[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\tutorial003.png",
		&g_apTextureTutorial[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Tutorial001.png",
		&g_apTextureTutorial[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Tutorial002.png",
		&g_apTextureTutorial[5]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\tutorial004.png",
		&g_apTextureTutorial[6]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Tutorial005.png",
		&g_apTextureTutorial[7]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Tutorial002.png",
		&g_apTextureTutorial[8]);

	g_nCounterTutorialState = 120;
	g_nPatternTutorial = 0;
	g_fCntTutorial = 0.0f;
	g_TutorialEnd = false;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TUTORIAL,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffTutorial,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTutorial = 0; nCntTutorial < NUM_TUTORIAL; nCntTutorial++)
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
	g_VtxBuffTutorial->Unlock();

	PlaySound(SOUND_LABEL_BGM009);
}

//===========================
//�`���[�g���A����ʂ̏I������
//===========================
void UninitTutorial(void)
{
	int nCntTutorial;

	//�T�E���h�̒�~
	StopSound();

	for (nCntTutorial = 0; nCntTutorial < NUM_TUTORIAL; nCntTutorial++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureTutorial[nCntTutorial] != NULL)
		{
			g_apTextureTutorial[nCntTutorial]->Release();
			g_apTextureTutorial[nCntTutorial] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffTutorial != NULL)
	{
		g_VtxBuffTutorial->Release();
		g_VtxBuffTutorial = NULL;
	}
}

//===========================
//�`���[�g���A����ʂ̍X�V����
//===========================
void UpdateTutorial(void)
{
	D3DXCOLOR col1 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR col2 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	FADE fade = GetFade();
	int nCntTutorial;

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetPadButtonTrigger(0) == true) && g_TutorialEnd == false)
	{
		if (g_nPatternTutorial < 2)
		{
			g_nPatternTutorial++;
		}
		else if (g_nPatternTutorial == 2)
		{
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_DECISION001);

			g_TutorialEnd = true;
		}
	}

	if ((GetKeyboardTrigger(DIK_BACKSPACE) == true || GetPadButtonTrigger(1) == true) && g_TutorialEnd == false)
	{
		if (g_nPatternTutorial > 0)
		{
			g_nPatternTutorial--;
		}
		else if (g_nPatternTutorial == 0)
		{
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_DECISION001);

			g_TutorialEnd = true;
		}
	}

	if (g_TutorialEnd == false)
	{
		col1.a = (sinf(g_fCntTutorial) + 1.0f) / 2;
		col2.a = (sinf(g_fCntTutorial) + 1.0f) / 2;

		g_fCntTutorial += 0.03f;
	}
	else
	{
		if (g_nPatternTutorial == 2)
		{
			col1.a = (sinf(g_fCntTutorial) + 1.0f) / 2;
			col2.a = 0.0f;
		}
		if (g_nPatternTutorial == 0)
		{
			col1.a = 0.0f;
			col2.a = (sinf(g_fCntTutorial) + 1.0f) / 2;
		}

		g_fCntTutorial += 0.5f;
	}

	if (g_TutorialEnd == true)
	{
		g_nCounterTutorialState--;

		if (g_nCounterTutorialState <= 0 && fade == FADE_NONE)
		{
			if (g_nPatternTutorial == 2)
			{
				//���[�h�ݒ�
				SetFade(MODE_GAME);
			}
			if (g_nPatternTutorial == 0)
			{
				//���[�h�ݒ�
				SetFade(MODE_TITLE);
			}
		}
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTutorial = 0; nCntTutorial < NUM_TUTORIAL; nCntTutorial++)
	{
		if ((g_nPatternTutorial * 3) + 1 == nCntTutorial)
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = col1;
			pVtx[1].col = col1;
			pVtx[2].col = col1;
			pVtx[3].col = col1;
		}
		else if ((g_nPatternTutorial * 3) + 2 == nCntTutorial)
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = col2;
			pVtx[1].col = col2;
			pVtx[2].col = col2;
			pVtx[3].col = col2;
		}
		else if (g_nPatternTutorial * 3 == nCntTutorial)
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffTutorial->Unlock();
}

//===========================
//�`���[�g���A����ʂ̕`�揈��
//===========================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTutorial;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffTutorial,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTutorial = 0; nCntTutorial < NUM_TUTORIAL; nCntTutorial++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureTutorial[nCntTutorial]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTutorial * 4, 2);
	}
}