//===========================
//
//�_�C���N�gX.ui�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "ui.h"
#include "result.h"
#include "input.h"
#include <stdio.h>

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureUi[UITEXTURE_MAX] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffUi = NULL;						//���_�����i�[
UI g_Ui[UITEXTURE_MAX];
int nTutolialCount;

//�e�N�X�`���t�@�C����
const char *c_apFilenameUi[UITEXTURE_MAX] =
{
	"data\\TEXTURE\\title002.png",
	"data\\TEXTURE\\title003.png",
	"data\\TEXTURE\\tutorial000.png",
	"data\\TEXTURE\\block002.png",
	"data\\TEXTURE\\block002.png",
	"data\\TEXTURE\\life000.png",
	"data\\TEXTURE\\boss001.png",
	"data\\TEXTURE\\clear000.png",
	"data\\TEXTURE\\gameover000.png"
};

//===========================
//�w�i�̏���������
//===========================
void InitUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi;
	char aText[128];
	FILE *pFile = fopen("data\\TEXTURE\\UI.txt", "r");

	if (pFile == NULL)
	{
		return;
	}

	fgets(&aText[0], 128, pFile);

	for (int nCnt = 0; nCnt < UITEXTURE_MAX; nCnt++)
	{
		fscanf(pFile, "%s = %f %f %f %f %f %d", &aText[0], &g_Ui[nCnt].pos.x, &g_Ui[nCnt].pos.y, &g_Ui[nCnt].pos.z, &g_Ui[nCnt].fWidth, &g_Ui[nCnt].fHeight, &g_Ui[nCnt].nType);
	}

	fclose(pFile);

	nTutolialCount = 0;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < UITEXTURE_MAX; nCnt++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameUi[nCnt],
			&g_apTextureUi[nCnt]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * UITEXTURE_MAX,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffUi,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffUi->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntUi = 0; nCntUi < UITEXTURE_MAX; nCntUi++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Ui[nCntUi].pos.x - g_Ui[nCntUi].fWidth, g_Ui[nCntUi].pos.y - g_Ui[nCntUi].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Ui[nCntUi].pos.x + g_Ui[nCntUi].fWidth, g_Ui[nCntUi].pos.y - g_Ui[nCntUi].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Ui[nCntUi].pos.x - g_Ui[nCntUi].fWidth, g_Ui[nCntUi].pos.y + g_Ui[nCntUi].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Ui[nCntUi].pos.x + g_Ui[nCntUi].fWidth, g_Ui[nCntUi].pos.y + g_Ui[nCntUi].fHeight, 0.0f);

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
	g_VtxBuffUi->Unlock();
}

//===========================
//�w�i�̏I������
//===========================
void UninitUi(void)
{
	int nCntUi;

	for (nCntUi = 0; nCntUi < UITEXTURE_MAX; nCntUi++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureUi[nCntUi] != NULL)
		{
			g_apTextureUi[nCntUi]->Release();
			g_apTextureUi[nCntUi] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffUi != NULL)
	{
		g_VtxBuffUi->Release();
		g_VtxBuffUi = NULL;
	}
}

//===========================
//�w�i�̍X�V����
//===========================
void UpdateUi(void)
{
	int nCntUi;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffUi->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntUi = 0; nCntUi < UITEXTURE_MAX; nCntUi++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Ui[nCntUi].pos.x - g_Ui[nCntUi].fWidth, g_Ui[nCntUi].pos.y - g_Ui[nCntUi].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Ui[nCntUi].pos.x + g_Ui[nCntUi].fWidth, g_Ui[nCntUi].pos.y - g_Ui[nCntUi].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Ui[nCntUi].pos.x - g_Ui[nCntUi].fWidth, g_Ui[nCntUi].pos.y + g_Ui[nCntUi].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Ui[nCntUi].pos.x + g_Ui[nCntUi].fWidth, g_Ui[nCntUi].pos.y + g_Ui[nCntUi].fHeight, 0.0f);

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
	g_VtxBuffUi->Unlock();
}

//===========================
//�w�i�̕`�揈��
//===========================
void DrawUi(void)
{
	MODE mode = GetMode();
	RESULT result = GetResult();
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffUi,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntUi = 0; nCntUi < UITEXTURE_MAX; nCntUi++)
	{
		if (g_Ui[nCntUi].nType == mode)
		{
			if (mode == MODE_RESULT && result == RESULT_CLEAR)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureUi[UITEXTURE_RESULTCLEAR]);

				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UITEXTURE_RESULTCLEAR * 4, 2);
			}
			else if (mode == MODE_RESULT && result == RESULT_OVER)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureUi[UITEXTURE_RESULTOVER]);

				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, UITEXTURE_RESULTOVER * 4, 2);
			}
			else
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureUi[nCntUi]);

				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi * 4, 2);
			}
		}
	}
}

//===========================
//�w�i�̕`�揈��
//===========================
void AddLife(float nLife)
{
	g_Ui[UITEXTURE_GAMELIFEBAR].fWidth -= nLife;
	g_Ui[UITEXTURE_GAMELIFEBAR].pos.x -= nLife;

	if (g_Ui[UITEXTURE_GAMELIFEBAR].fWidth > 300.0f)
	{
		g_Ui[UITEXTURE_GAMELIFEBAR].fWidth = 300.0f;
		g_Ui[UITEXTURE_GAMELIFEBAR].pos.x = 300.0f;
	}
}