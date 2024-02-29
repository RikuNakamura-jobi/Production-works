//===========================
//
//�_�C���N�gX.pause�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "pause.h"
#include "input.h"
#include "fade.h"
#include "game.h"

//�}�N����`
#define NUM_PAUSE (4)												//�|�[�Y���j���[�̐�

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTexturePause[NUM_PAUSE] = {};				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffPause = NULL;						//���_�����i�[
int g_nCounterPause, g_nPatternPause, g_nPauseNumber;				//�|�[�Y�p�ϐ�

//===========================
//�|�[�Y���j���[�̏���������
//===========================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntPause;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\pause000.png",
		&g_apTexturePause[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\pause001.png",
		&g_apTexturePause[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\pause002.png",
		&g_apTexturePause[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\pause003.png",
		&g_apTexturePause[3]);

	g_nCounterPause = 3;
	g_nPatternPause = 0;
	g_nPauseNumber = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PAUSE,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffPause,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		if (nCntPause == 0)
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
		}
		else
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
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffPause->Unlock();
}

//===========================
//�|�[�Y���j���[�̏I������
//===========================
void UninitPause(void)
{
	int nCntPause;

	for (nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		//�e�N�X�`���̔j��
		if (g_apTexturePause[nCntPause] != NULL)
		{
			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffPause != NULL)
	{
		g_VtxBuffPause->Release();
		g_VtxBuffPause = NULL;
	}
}

//===========================
//�|�[�Y���j���[�̍X�V����
//===========================
void UpdatePause(void)
{
	int nCntPause;
	FADE fade = GetFade();

	//���̓J�[�\���ړ�
	if (GetKeyboardTrigger(DIK_S) == true && g_nPauseNumber == 0)
	{
		g_nCounterPause++;
	}

	if (GetKeyboardTrigger(DIK_W) == true && g_nPauseNumber == 0)
	{
		g_nCounterPause += 2;
	}
	
	g_nPatternPause = (g_nCounterPause) % 3;

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetPadButtonTrigger(0) == true) && fade == FADE_NONE)
	{
		switch (g_nPatternPause)
		{
		case 0:
			SetEnablePause(false);
			break;
		case 1:
			SetFade(MODE_GAME);
			SetEnablePause(false);
			break;
		case 2:
			SetFade(MODE_TITLE);
			SetEnablePause(false);
			break;
		default:
			break;
		}
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		if (g_nPatternPause + 1 == nCntPause)
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (nCntPause == 0)
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
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffPause->Unlock();
}

//===========================
//�|�[�Y���j���[�̕`�揈��
//===========================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntPause;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffPause,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTexturePause[nCntPause]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}

//===========================
//�|�[�Y���j���[�̃��Z�b�g����
//===========================
void ResetPause(void)
{
	g_nCounterPause = 3;
	g_nPatternPause = 0;
	g_nPauseNumber = 0;
}