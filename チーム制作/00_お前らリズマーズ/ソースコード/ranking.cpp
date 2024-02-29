//===========================
//
//�_�C���N�gX.ranking�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "Ranking.h"
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "score.h"
#include <stdio.h>

//�}�N����`
#define NUM_PLACE_RANKING (8)											//�X�R�A�̍ő吔
#define MAX_RANKING (5)													//�X�R�A�̃����N��

//�����L���O�\����
typedef struct
{
	D3DXVECTOR3 pos;													//�ʒu
	int nScore;															//�X�R�A
}RANKING;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureRankingScore = NULL;						//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingScore = NULL;					//���_�����i�[
RANKING g_aRanking[MAX_RANKING];										//�����L���O���
D3DXVECTOR3 g_PosRanking;												//�����L���O�̈ʒu
int g_nRankUpdate = -1;													//�����L���O�̍X�V����
int g_nTimerRanking;													//�����L���O�̃^�C�g���J�ڃ^�C�}�[

//===========================
//�X�R�A�̏���������
//===========================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntRanking;
	int nScore = GetScore();

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number001.png",
		&g_pTextureRankingScore);

	g_PosRanking = D3DXVECTOR3(500.0f, 135.0f, 0.0f);
	g_nTimerRanking = 0;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE_RANKING * MAX_RANKING,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingScore,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		for (int nCntRankingScore = 0; nCntRankingScore < NUM_PLACE_RANKING; nCntRankingScore++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_PosRanking.x + (20.0f * nCntRankingScore);
			pVtx[0].pos.y = g_PosRanking.y + (40.0f * nCntRanking) + (44.0f * nCntRanking);
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_PosRanking.x + (20.0f * nCntRankingScore) + 20.0f;
			pVtx[1].pos.y = g_PosRanking.y + (40.0f * nCntRanking) + (44.0f * nCntRanking);
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_PosRanking.x + (20.0f * nCntRankingScore);
			pVtx[2].pos.y = g_PosRanking.y + (40.0f * nCntRanking) + (44.0f * nCntRanking) + 40.0f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_PosRanking.x + (20.0f * nCntRankingScore) + 20.0f;
			pVtx[3].pos.y = g_PosRanking.y + (40.0f * nCntRanking) + (44.0f * nCntRanking) + 40.0f;
			pVtx[3].pos.z = 0.0f;

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
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;										//���_���W�̃|�C���^��4���i�߂�
		}
	}

	if (g_nRankUpdate != -1)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRankingScore->Unlock();

	SetRanking(nScore);

	PlaySound(SOUND_LABEL_BGM007);
}

//===========================
//�X�R�A�̏I������
//===========================
void UninitRanking(void)
{
	g_nRankUpdate = -1;

	//�T�E���h�̒�~
	StopSound();

	//�e�N�X�`���̔j��
	if (g_pTextureRankingScore != NULL)
	{
		g_pTextureRankingScore->Release();
		g_pTextureRankingScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRankingScore != NULL)
	{
		g_pVtxBuffRankingScore->Release();
		g_pVtxBuffRankingScore = NULL;
	}
}

//===========================
//�X�R�A�̍X�V����
//===========================
void UpdateRanking(void)
{
	FADE fade = GetFade();

	g_nTimerRanking++;

	int aTexU[MAX_RANKING][NUM_PLACE_RANKING];

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		aTexU[nCntRanking][0] = g_aRanking[nCntRanking].nScore % 100000000 / 10000000;
		aTexU[nCntRanking][1] = g_aRanking[nCntRanking].nScore % 10000000 / 1000000;
		aTexU[nCntRanking][2] = g_aRanking[nCntRanking].nScore % 1000000 / 100000;
		aTexU[nCntRanking][3] = g_aRanking[nCntRanking].nScore % 100000 / 10000;
		aTexU[nCntRanking][4] = g_aRanking[nCntRanking].nScore % 10000 / 1000;
		aTexU[nCntRanking][5] = g_aRanking[nCntRanking].nScore % 1000 / 100;
		aTexU[nCntRanking][6] = g_aRanking[nCntRanking].nScore % 100 / 10;
		aTexU[nCntRanking][7] = g_aRanking[nCntRanking].nScore % 10 / 1;
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		for (int nCntRankingScore = 0; nCntRankingScore < NUM_PLACE_RANKING; nCntRankingScore++)
		{
			if (g_nRankUpdate != -1)
			{
				if (nCntRanking == g_nRankUpdate)
				{
					//���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				}
			}

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.1f * aTexU[nCntRanking][nCntRankingScore], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[nCntRanking][nCntRankingScore], 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aTexU[nCntRanking][nCntRankingScore], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[nCntRanking][nCntRankingScore], 1.0f);

			pVtx += 4;
		}
	}

	g_pVtxBuffRankingScore->Unlock();

	if (g_nTimerRanking == 1500 || (GetKeyboardTrigger(DIK_RETURN) == true && fade == FADE_NONE))
	{
		SetFade(MODE_TITLE);
	}
}

//===========================
//�X�R�A�̕`�揈��
//===========================
void DrawRanking(void)
{
	int nCntRanking;

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�X�R�A
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffRankingScore,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRankingScore);

	for (nCntRanking = 0; nCntRanking < NUM_PLACE_RANKING * MAX_RANKING; nCntRanking++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRanking, 2);
	}
}

//===========================
//�X�R�A�̐ݒ菈��
//===========================
void SetRanking(int nRanking)
{
	int nMaxNumber, nTemp;

	//�X�R�A�̃��[�h����
	LoadData();

	if (g_aRanking[MAX_RANKING - 1].nScore <= nRanking)
	{//�����L���O����ւ�
		g_aRanking[MAX_RANKING - 1].nScore = nRanking;
	}

	//�~���}���\�[�g
	for (int nCnt1 = 1; nCnt1 < MAX_RANKING; nCnt1++)
	{
		nMaxNumber = nCnt1;

		while (nMaxNumber > 0 && g_aRanking[nMaxNumber - 1].nScore < g_aRanking[nMaxNumber].nScore)
		{
			nTemp = g_aRanking[nMaxNumber - 1].nScore;
			g_aRanking[nMaxNumber - 1].nScore = g_aRanking[nMaxNumber].nScore;
			g_aRanking[nMaxNumber].nScore = nTemp;

			nMaxNumber--;
		}
	}

	//�����L���O�X�V�m�F
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		if (g_aRanking[nCnt].nScore == nRanking)
		{
			g_nRankUpdate = nCnt;
			break;
		}
	}

	//�X�R�A�̃Z�[�u����
	SaveData();
}

//===========================
//�X�R�A�̃��Z�b�g����
//===========================
void ResetRanking(void)
{
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		g_aRanking[nCntRanking].pos.x = 460.0f;
		g_aRanking[nCntRanking].pos.y = 100.0f;
		g_aRanking[nCntRanking].pos.z = 0.0f;

		g_aRanking[nCntRanking].nScore = 0;
	}
}

//===========================
//�X�R�A�̃��[�h����
//===========================
void LoadData(void)
{
	FILE *pFile;

	//�t�@�C���������ǂݍ���
	pFile = fopen("ranking.bin", "rb");

	if (pFile != NULL)
	{//�J�����ꍇ

		fread(&g_aRanking[0].nScore, sizeof(RANKING), MAX_RANKING, pFile);
		fclose(pFile);
	}
	else
	{//�J���Ȃ������ꍇ

	 //�����L���O������
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
		{
			g_aRanking[nCnt].nScore = 0;
		}
	}
}

//===========================
//�X�R�A�̃Z�[�u����
//===========================
void SaveData(void)
{
	FILE *pFile;

	//�t�@�C���������ǂݍ���
	pFile = fopen("ranking.bin", "wb");

	if (pFile != NULL)
	{//�J�����ꍇ

		fwrite(&g_aRanking[0].nScore, sizeof(RANKING), MAX_RANKING, pFile);
		fclose(pFile);
	}
	else
	{//�J���Ȃ������ꍇ

	}
}