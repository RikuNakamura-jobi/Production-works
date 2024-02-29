//==========================================
//
//  �X�R�A�\��(score.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "score.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "point.h"

//==========================================
//  �}�N����`
//==========================================
#define MAX_SCORE (999999) //�X�R�A�̍ő�l
#define MIN_SCORE (000000) //�X�R�A�̍ŏ��l
#define SCORE_NUM (5) //�X�R�A�̌���
#define SCORE_WIDTH (50.0f) //�X�R�A�̉���
#define SCORE_HEIGHT (75.0f) //�X�R�A�̍���
#define SCORE_FPA (40) //�X�R�A�̃t���[�� % ���Z�x
#define SCORE_FPA_BRC (1000) //�X�R�A�̉��Z�x����
#define SCORE_SOUND_BRC (1000) //�X�R�A�̌��ʉ�����

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;
int g_nScore; //���݂̃X�R�A
int g_nScoreOld; //�O��̃X�R�A
D3DXVECTOR3 g_posScore;	//���W

//==========================================
//  ����������
//==========================================
void InitScore()
{
	//�ϐ��̏�����
	g_nScore = 0;
	g_nScoreOld = 0;
	g_posScore = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * SCORE_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data/TEXTURE/number001.png",
		&g_pTextureScore
	);

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < SCORE_NUM; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x - (SCORE_WIDTH * (SCORE_NUM - nCnt)), 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x - (SCORE_WIDTH * (SCORE_NUM - (nCnt + 1))), 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x - (SCORE_WIDTH * (SCORE_NUM - nCnt)), SCORE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x - (SCORE_WIDTH * (SCORE_NUM - (nCnt + 1))), SCORE_HEIGHT, 0.0f);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//rhw�̐ݒ�
			pVtx[nCnt].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		//���_�f�[�^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitScore()
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdateScore()
{
	int nFpa = SCORE_FPA;	//���Z��

	if (GetGameState() == GAMESTATE_RESULT)
	{
		nFpa *= 2;
	}

	if (g_nScoreOld < g_nScore)
	{//�v���X����
		
		if (g_nScore - g_nScoreOld >= SCORE_FPA_BRC)
		{//�}�N���ȏ㗣��Ă�����
			DiffAddScore(nFpa + 1);
		}
		else
		{
			DiffAddScore(nFpa);
		}
	}
	else if (g_nScoreOld > g_nScore)
	{//�}�C�i�X����

		if (g_nScoreOld - g_nScore >= SCORE_FPA_BRC)
		{//�}�N���ȏ㗣��Ă�����
			DiffAddScore(-nFpa - 1);
		}
		else
		{
			DiffAddScore(-nFpa);
		}
	}

#ifdef _DEBUG

	//�f�o�b�O����
	if (GetKeyboardTrigger(DIK_F7))
	{//�X�R�A���Z
		AddScore(200);
	}
	if (GetKeyboardTrigger(DIK_F6))
	{//���Z
		AddScore(-200);
	}

#endif
}

//==========================================
//  �`�揈��
//==========================================
void DrawScore()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < SCORE_NUM; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureScore);

		//�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
	}
}

//==========================================
//  �ݒ菈��
//==========================================
void SetScore(int nScore)
{
	//���[�J���ϐ��錾
	int aTex[SCORE_NUM];
	int nCntScore;

	//�X�R�A�̎擾
	if (nScore > MAX_SCORE)
	{
		g_nScoreOld = MAX_SCORE;
	}
	else if (nScore < MIN_SCORE)
	{
		g_nScoreOld = MIN_SCORE;
	}
	else
	{
		g_nScoreOld = nScore;
	}

	//�v�Z�p�ϐ�
	int nCalc = g_nScoreOld;

	//�e�N�X�`�����W�̌v��
	for (int nCntScore = SCORE_NUM - 1; nCntScore >= 0; nCntScore--)
	{
		aTex[nCntScore] = nCalc % 10;
		nCalc /= 10;
	}

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//�ݒ菈��
	for (nCntScore = 0; nCntScore < SCORE_NUM; nCntScore++)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aTex[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((aTex[nCntScore] * 0.1f) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTex[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((aTex[nCntScore] * 0.1f) + 0.1f, 1.0f);

		pVtx += 4; //���_�f�[�^�̃|�C���^��4�i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();
}

//==========================================
//  ���Z����
//==========================================
void DiffAddScore(int nValue)
{
	if (nValue > 0 && g_nScoreOld + nValue > g_nScore)
	{//���ʗ\�肪�v���X
		nValue = (g_nScore - g_nScoreOld);
	}
	else if (nValue < 0 && g_nScoreOld + nValue < g_nScore)
	{//���ʗ\�肪�}�C�i�X
		nValue = (g_nScoreOld - g_nScore);
	}

	//�X�R�A�̉��Z
	SetScore(g_nScoreOld + nValue);
}

//==========================================
//  ���Z���󂯎�菈��
//==========================================
void AddScore(int nValue)
{
	//�X�R�A�̉��Z
	if (g_nScore + nValue > MAX_SCORE)
	{//���
		g_nScore = MAX_SCORE;
	}
	else if (g_nScore + nValue < MIN_SCORE)
	{//����
		g_nScore = MIN_SCORE;
	}
	else
	{
		g_nScore += nValue;

		if (nValue >= SCORE_SOUND_BRC)
		{
			PlaySound(SOUND_LABEL_SE_ADDSCORE_001);
		}
		else if (nValue >= 0)
		{
			PlaySound(SOUND_LABEL_SE_ADDSCORE_000);
		}
	}
}

//==========================================
//  �X�R�A�̍��W�E�{���ݒ菈��
//==========================================
void SetAdjScore(D3DXVECTOR3 pos, float fMgn)
{
	g_posScore = pos;

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < SCORE_NUM; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x - ((SCORE_WIDTH * fMgn) * (SCORE_NUM - nCnt)), g_posScore.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x - ((SCORE_WIDTH * fMgn) * (SCORE_NUM - (nCnt + 1))), g_posScore.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x - ((SCORE_WIDTH * fMgn) * (SCORE_NUM - nCnt)), g_posScore.y + SCORE_HEIGHT * fMgn, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x - ((SCORE_WIDTH * fMgn) * (SCORE_NUM - (nCnt + 1))),	g_posScore.y + SCORE_HEIGHT * fMgn, 0.0f);

		//���_�f�[�^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();
}

//==========================================
//  �X�R�A�擾����
//==========================================
int GetScore()
{
	return g_nScore;
}

//==========================================
//  �X�R�A�擾����
//==========================================
void DebugScore()
{
	LPD3DXFONT pFont = GetFont();
	RECT rect = { 0,580,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "�����Z<H/J>\n���̃X�R�A; %d\n�O�̃X�R�A: %d",g_nScore,g_nScoreOld);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 200, 200, 255));
}
