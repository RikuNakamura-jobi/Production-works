//==========================================
//
//�����L���O�̏���(ranking.cpp)
//Author Kai Takada
//
//==========================================
#include "main.h"
#include "ranking.h"
#include "ranking_rank.h"
#include "ranking_bg.h"
#include "input.h"
#include "fade.h"
#include "score.h"
#include "sound.h"
#include "player.h"
#include <stdio.h>

//========================
//�}�N����`
//========================
#define NUM_DIGITS (5)		//�\�����錅��
#define RANKING_FILE "data\\SAVE\\RANKING\\ranking.bin"		//�����L���O�ɏo���X�R�A�̃t�@�C��
#define RANKING_FONTFILE "data\\Texture\\number001.png"		//�����t�H���g�t�@�C��
#define RANKING_ALPHAINTERVAL (25)		//�A���t�@�l�̍X�V�p�x
#define RANKING_FRAME (2700)		//�����L���O�ێ��t���[��
#define RANK_SLD_OK (20.0f)		//�X���C�h�����덷
#define RANK_SLIDE_PER (0.05f)		//�X���C�h��������
#define RANK_RGBA_R (160)		//�X�V�X�R�A��r�F
#define RANK_RGBA_B (0)		//�X�V�X�R�A��b�F

//========================
//�v���g�^�C�v�錾
//========================
void MoveRanking(void);

//========================
//�����L���O�\����
//========================
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posDest;	//�\��ʒu
	int nScore = 0;		//�X�R�A
	bool bArvl;		//�����I�������
}Ranking;

//========================
//�f�o�b�O�p�o�̓X�R�A�z��
//========================
const int c_aDebugScore[NUM_RANKING] =
{
	12400,
	8600,
	7100,
	5200,
	1800
};

//========================
//�O���[�o���ϐ��錾
//========================
LPDIRECT3DTEXTURE9 g_apTextureRanking = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Ranking g_aRanking[NUM_RANKING];		//�����L���O���
int g_nGetScore1;		//�X�R�A�̕ۑ�
int g_nRankUpdate = -1;		//�X�V�����NNo.
int g_nTimerRanking;		//�����L���O��ʕ\���^�C�}�[
bool g_bFadeRanking;		//�����t�F�[�h�h�~
bool g_bMove;		//�X�R�A�ړ�����
int g_nFadeCounterRank;			//�t�F�[�h�J�E���^�[
int g_nRankR, g_nRankB;			//�ΏۃX�R�A�̃A���t�@�l
int nRankAlphaCnter;		//�A���t�@�l�̃J�E���^�[
int g_nNewScoreCnt;		//�X�V�����X�R�A�̏��ʂ̕ۑ�

//===========================
//�����L���O�̏���������
//===========================
void InitRanking(void)
{
	//���̑������L���O�n�̏�����
	InitRankingRank();
	InitRankingBg();

	g_bMove = true;		//�����L���O�X�R�A�ړ����I����ĂȂ�
	g_nFadeCounterRank = 0;		//�t�F�[�h�J�E���^�[�̏�����
	nRankAlphaCnter = 0;		//�A���t�@�J�E���^�[�̏�����
	g_nRankR = 255;			//r�l�̏�����
	g_nRankB = 255;			//b�l�̏�����
	D3DXVECTOR3 *pRankpos = GetRankPos();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		RANKING_FONTFILE,							//�e�N�X�`���̃t�@�C����
		&g_apTextureRanking);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_RANKING * NUM_DIGITS,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	g_bFadeRanking = false;		//������

	VERTEX_2D*pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntY = 0; nCntY < NUM_RANKING; nCntY++)
	{
		g_aRanking[nCntY].bArvl = false;

		for (int nCntX = 0; nCntX < NUM_DIGITS; nCntX++)
		{
			g_aRanking[nCntY].posDest = D3DXVECTOR3((SCREEN_WIDTH * 0.4f) + (nCntX * RANK_WIDTH * 2),
				pRankpos[nCntY].y,
				0.0f);

			g_aRanking[nCntY].pos = g_aRanking[nCntY].posDest;
			g_aRanking[nCntY].pos.x = SCREEN_WIDTH + RANK_WIDTH;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aRanking[nCntY].pos.x - RANK_WIDTH, g_aRanking[nCntY].pos.y - RANK_HEIGHT, 0.0f);	//(x,y,z)
			pVtx[1].pos = D3DXVECTOR3(g_aRanking[nCntY].pos.x + RANK_WIDTH, g_aRanking[nCntY].pos.y - RANK_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aRanking[nCntY].pos.x - RANK_WIDTH, g_aRanking[nCntY].pos.y + RANK_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aRanking[nCntY].pos.x + RANK_WIDTH, g_aRanking[nCntY].pos.y + RANK_HEIGHT, 0.0f);

			//rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = pVtx[0].col;
			pVtx[2].col = pVtx[0].col;
			pVtx[3].col = pVtx[0].col;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			if (g_nRankUpdate != -1)
			{//�����L���O���X�V���ꂽ

			}
			pVtx += 4;
		}
		g_aRanking[nCntY].posDest.x = (SCREEN_WIDTH * 0.5f);
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();

	TexRanking();

	int aRankScore[NUM_RANKING];

#ifdef _DEBUG
	for (int nCntSave = 0; nCntSave < NUM_RANKING; nCntSave++)
	{
		aRankScore[nCntSave] = c_aDebugScore[nCntSave];
	}
#else
	for (int nCntSave = 0; nCntSave < NUM_RANKING; nCntSave++)
	{
		aRankScore[nCntSave] = g_aRanking[nCntSave].nScore;
	}
#endif

	FILE *pFile;									//�t�@�C���|�C���g��錾	

	pFile = fopen(RANKING_FILE, "wb");

	if (pFile != NULL)
	{//�O���t�@�C���Ƀ����L���O���(score)��ۑ�
		fwrite(&aRankScore[0], sizeof(int), NUM_RANKING, pFile);

		fclose(pFile);
	}

	//�T�E���h�Đ� =
	PlaySound(SOUND_LABEL_BGM003);
}

//===========================
//�����L���O�̏I������
//===========================
void UninitRanking(void)
{
	//�T�E���h�̒�~
	StopSound();

	//���Z�b�g
	g_nRankUpdate = -1;

	if (g_apTextureRanking != NULL)
	{
		g_apTextureRanking->Release();
		g_apTextureRanking = NULL;					//�e�N�X�`���j��
	}

	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;					//�o�b�t�@�̔j��
	}

	//���̑������L���O�n�̏I��
	UninitRankingRank();
	UninitRankingBg();
}

//===========================
//�����L���O�̍X�V����
//===========================
void UpdateRanking(void)
{
	D3DXVECTOR3 Rankpos;

	VERTEX_2D*pVtx;

	if (g_nRankUpdate != -1)
	{//�����L���O���X�V���ꂽ��
		if (nRankAlphaCnter % RANKING_ALPHAINTERVAL == 0)
		{//0.5�b��������
			g_nRankR == 255 ? (g_nRankR = RANK_RGBA_R) : (g_nRankR = 255);
			g_nRankB = g_nRankB ? RANK_RGBA_B : 255;
			nRankAlphaCnter = 0;
		}

		nRankAlphaCnter++;
	}

	//�X�R�A�|���S���ړ�����
	MoveRanking();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	//�ʒu�X�V
	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{
		Rankpos = g_aRanking[nCntRank].pos;
		
		for (int nCntAlpha = 0; nCntAlpha < NUM_DIGITS; nCntAlpha++)
		{
			Rankpos.x += RANK_WIDTH * 2;

			if (nCntRank == g_nNewScoreCnt)
			{
				//���_�J���[�̐ݒ�
				for (int nCntCol = 0;nCntCol < 4;nCntCol++)
				{
					pVtx[nCntCol].col = D3DCOLOR_RGBA(g_nRankR, 255, g_nRankB, 255);
				}
			}

			pVtx[0].pos = D3DXVECTOR3(Rankpos.x - RANK_WIDTH, Rankpos.y - RANK_HEIGHT + (RANK_HEI * nCntRank), 0.0f);	//(x,y,z)
			pVtx[1].pos = D3DXVECTOR3(Rankpos.x + RANK_WIDTH, Rankpos.y - RANK_HEIGHT + (RANK_HEI * nCntRank), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(Rankpos.x - RANK_WIDTH, Rankpos.y + RANK_HEIGHT + (RANK_HEI * nCntRank), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(Rankpos.x + RANK_WIDTH, Rankpos.y + RANK_HEIGHT + (RANK_HEI * nCntRank), 0.0f);

			pVtx += 4;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();

	//�X�R�A�ړ��̊���
	if (GetFade() == FADE_NONE)
	{
		//���[�h�ݒ�
		if ((GetKeyboardTrigger(DIK_RETURN) ||
			GetKeyboardTrigger(DIK_SPACE) ||
			g_nFadeCounterRank >= RANKING_FRAME ||
			GetJoyPadButtonTrigger(KEY_A,0) ||
			GetJoyPadButtonTrigger(KEY_B, 0) ||
			GetJoyPadButtonTrigger(KEY_START, 0))
			&& g_bFadeRanking == false
			&& g_bMove == false)
		{//ENTER�L�[���������Ƃ�or�w��t���[�����ȏ�X�V�����Ƃ�
			SetFade(MODE_TITLE);
			g_bFadeRanking = true;
		}
		else if ((GetKeyboardTrigger(DIK_RETURN)||
			GetKeyboardTrigger(DIK_SPACE) ||
			GetJoyPadButtonTrigger(KEY_A, 0) ||
			GetJoyPadButtonTrigger(KEY_B, 0) ||
			GetJoyPadButtonTrigger(KEY_START, 0))
			&& g_bMove == true)
		{
			g_bMove = false;
		}

		if (g_bMove == false)
		{
			if (GetPlayer()->bEaster == true)
			{
				g_nFadeCounterRank++;
			}
			else
			{
				g_nFadeCounterRank += 3;
			}
		}
	}

	//���̑������L���O�n�̍X�V
	UpdateRankingRank();
	UpdateRankingBg();
}

//===========================
//�����L���O�̕`�揈��
//===========================
void DrawRanking(void)
{
	//���̑������L���O�n�̕`��
	DrawRankingBg();
	DrawRankingRank();

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureRanking);
	
	for (int nCntDraw = 0; nCntDraw < NUM_DIGITS * NUM_RANKING; nCntDraw++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
			nCntDraw * 4,									//�`�悷��ŏ��̃C���f�b�N�X
			2);												//�`�悷��v���~�e�B�u��
	}
}

//==========================
//�����L���O�̐ݒ菈��(sort)
//==========================
void SetRanking(int nScore)
{
	g_nGetScore1 = nScore;

	if (g_nGetScore1 > g_aRanking[NUM_RANKING - 1].nScore)
	{
		g_aRanking[NUM_RANKING - 1].nScore = g_nGetScore1;

		for (int nCntMax = 0; nCntMax < NUM_RANKING; nCntMax++)
		{
			int nHidata = nCntMax;

			for (int nCntSort = nCntMax + 1; nCntSort < NUM_RANKING; nCntSort++)
			{
				if (g_aRanking[nHidata].nScore < g_aRanking[nCntSort].nScore)
				{
					nHidata = nCntSort;
				}
			}

			if (nHidata != nCntMax)
			{//�I�������ꏊ���ς���Ă�����
				int nTemp = g_aRanking[nCntMax].nScore;
				g_aRanking[nCntMax].nScore = g_aRanking[nHidata].nScore;
				g_aRanking[nHidata].nScore = nTemp;
			}
		}

		for (int nCntComp = 0; nCntComp < NUM_RANKING; nCntComp++)
		{
			if (g_aRanking[nCntComp].nScore == g_nGetScore1)
			{
				g_nRankUpdate = 0;
				g_nNewScoreCnt = nCntComp;
			}
		}
	}
}

//==========================
//�����L���O�̃��Z�b�g����(load)
//==========================
void ResetRanking(void)
{
	D3DXVECTOR3 *pRankpos = GetRankPos();
	int aRankScore[5] = {0,0,0,0,0};

	FILE *pFile;									//�t�@�C���|�C���g��錾

	pFile = fopen(RANKING_FILE, "rb");

	if (pFile != NULL)
	{//�t�@�C�����J������
		fread(&aRankScore[0], sizeof(int), 5, pFile);
		
		for (int nCnt = 0; nCnt < NUM_RANKING; nCnt++)
		{
			g_aRanking[nCnt].nScore = aRankScore[nCnt];
		}

		fclose(pFile);
	}
	else
	{//�J���Ȃ�������
		for (int nCnt = 0;nCnt < NUM_RANKING;nCnt++)
		{
			g_aRanking[nCnt].nScore = 0;
		}
	}
	
	for (int nCnt = 0; nCnt < NUM_RANKING; nCnt++)
	{
		g_aRanking[nCnt].pos = D3DXVECTOR3(SCREEN_WIDTH, pRankpos[nCnt].y, 0.0f);
	}
}

//==========================
//�����L���O�̃e�N�X�`���̐ݒ菈��
//==========================
void TexRanking(void)
{
	int aTexU[NUM_DIGITS];				//�e���̐������i�[
	int nCalc = 0;						//�v�Z�p
	VERTEX_2D*pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < NUM_RANKING; nCntRank++)
	{
		//�e�N�X�`�����W�̌v��
		nCalc = g_aRanking[nCntRank].nScore;

		for (int nCntScore = NUM_DIGITS - 1; nCntScore >= 0; nCntScore--)
		{
			aTexU[nCntScore] = nCalc % 10;
			nCalc /= 10;
		}

		for (int nCntPlace = 0; nCntPlace < NUM_DIGITS; nCntPlace++)
		{
			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + (aTexU[nCntPlace] * 0.1f), 0.0f);					//(x,y)
			pVtx[1].tex = D3DXVECTOR2(0.1f + (aTexU[nCntPlace] * 0.1f), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (aTexU[nCntPlace] * 0.1f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (aTexU[nCntPlace] * 0.1f), 1.0f);

			pVtx += 4;		//���_�f�[�^�̃|�C���^��4���i�߂�
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();
}

//==========================
//�����L���O�̈ړ�����
//==========================
void MoveRanking(void)
{
	float fDiff = 0.0f;

	for (int nCntArvl = NUM_RANKING - 1; nCntArvl >= 0; nCntArvl--)
	{
		if (g_bMove == false)
		{//�ړ��������A�Œ�l��������
			g_aRanking[nCntArvl].bArvl = true;
			g_aRanking[nCntArvl].pos.x = g_aRanking[nCntArvl].posDest.x + RANK_SLD_OK;
		}

		if (g_aRanking[nCntArvl].bArvl == false)
		{//�܂��ł����
			//�ʒu�̕␳
			fDiff = g_aRanking[nCntArvl].pos.x - g_aRanking[nCntArvl].posDest.x;

			if (fDiff <= RANK_SLD_OK)
			{//���l�܂Ō����Ă����犮����
				g_aRanking[nCntArvl].bArvl = true;
			}
			else
			{//�����ňړ����I��
				g_aRanking[nCntArvl].pos.x -= fDiff * RANK_SLIDE_PER;
				break;
			}
		}
	}

	if (g_aRanking[0].bArvl
		&& g_aRanking[1].bArvl
		&& g_aRanking[2].bArvl
		&& g_aRanking[3].bArvl
		&& g_aRanking[4].bArvl == true)
	{//�S���ړ�����
		g_bMove = false;
	}
}
