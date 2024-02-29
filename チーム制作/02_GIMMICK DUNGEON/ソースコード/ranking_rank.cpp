//============================
//
//�����L���O���ʂ̏���(ranking_rank.cpp)
//
//Author ���c ����
//
//============================
#include "main.h"
#include "ranking.h"
#include "ranking_rank.h"
#include <stdio.h>

//===========================
//�}�N����`
//===========================
#define RANK_W (80)						//���ʂ̃T�C�Y�i���j
#define RANK_H (50)						//���ʂ̃T�C�Y�i�c�j
#define PASS_RANKING_RANK "data\\Texture\\rank000.png"	//�e�N�X�`���p�X

//===========================
//�O���[�o���ϐ��錾
//===========================
LPDIRECT3DTEXTURE9 g_pTextureRankingRank = NULL;				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingRank = NULL;			//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_Rankpos[NUM_RANKING];

//===========================
//�����L���O�̏���������
//===========================
void InitRankingRank(void)
{
	for (int nCnt = 0; nCnt < NUM_RANKING; nCnt++)
	{
		g_Rankpos[nCnt] = D3DXVECTOR3(SCREEN_WIDTH * 0.40f, (SCREEN_HEIGHT / 8.0f) *  (nCnt + 2), 0.0f);
	}

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		PASS_RANKING_RANK,							//�e�N�X�`���̃t�@�C����
		&g_pTextureRankingRank);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_RANKING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingRank,
		NULL);

	VERTEX_2D*pVtx;
	
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRankingRank->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntInit = 0; nCntInit < NUM_RANKING; nCntInit++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Rankpos[nCntInit].x - RANK_W, g_Rankpos[nCntInit].y - RANK_H + (RANK_HEI * nCntInit), 0.0f);	//(x,y,z)
		pVtx[1].pos = D3DXVECTOR3(g_Rankpos[nCntInit].x + RANK_W, g_Rankpos[nCntInit].y - RANK_H + (RANK_HEI * nCntInit), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Rankpos[nCntInit].x - RANK_W, g_Rankpos[nCntInit].y + RANK_H + (RANK_HEI * nCntInit), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Rankpos[nCntInit].x + RANK_W, g_Rankpos[nCntInit].y + RANK_H + (RANK_HEI * nCntInit), 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(nCntInit * 0.2f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((nCntInit * 0.2f) + 0.2f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nCntInit * 0.2f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((nCntInit * 0.2f) + 0.2f, 1.0f);

		pVtx += 4;
	}
	
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRankingRank->Unlock();
}

//===========================
//�����L���O�̏I������
//===========================
void UninitRankingRank(void)
{
	if (g_pTextureRankingRank != NULL)
	{
		g_pTextureRankingRank->Release();
		g_pTextureRankingRank = NULL;					//�e�N�X�`���j��
	}

	if (g_pVtxBuffRankingRank != NULL)
	{
		g_pVtxBuffRankingRank->Release();
		g_pVtxBuffRankingRank = NULL;					//�o�b�t�@�̔j��
	}
}

//===========================
//�����L���O�̍X�V����
//===========================
void UpdateRankingRank(void)
{

}

//===========================
//�����L���O�̕`�揈��
//===========================
void DrawRankingRank(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRankingRank, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRankingRank);

	for (int nCntDraw = 0; nCntDraw < NUM_RANKING; nCntDraw++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
			nCntDraw * 4,									//�`�悷��ŏ��̃C���f�b�N�X
			2);												//�`�悷��v���~�e�B�u��
	}
}

//===========================
//�����L���O�̕`�揈��
//===========================
D3DXVECTOR3 *GetRankPos()
{
	return &g_Rankpos[0];
}
