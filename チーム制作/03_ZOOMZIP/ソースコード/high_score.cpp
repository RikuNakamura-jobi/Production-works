//==========================================================
//
//�n�C�X�R�A���� [high_score.h]
//Author Ibuki Okusada
//
//==========================================================
#include "main.h"    //�쐬����main.h���C���N���[�h����
#include "high_score.h"
#include "fade.h"
#include "input.h"
#include <math.h>
#include "score.h"
#include <stdio.h>
#include "player.h"
#include "game.h"
#include "sound.h"

//==========================================================
//�}�N����`
//==========================================================
#define SCORE_X			(SCREEN_WIDTH * 0.4f)	//���SX���W
#define SCORE_Y			(SCREEN_HEIGHT * 0.4f)	//���SY���W
#define SCORE_SPACEX	(SCREEN_WIDTH * 0.08f)	//���S�X�y�[�X
#define SCORE_WIDTH		(SCREEN_WIDTH * 0.15f)	//���S��
#define SCORE_HEIGHT	(SCREEN_HEIGHT * 0.25f)	//���S����
#define SCORE_FADECNT	(120)					//�J�ڃJ�E���g
#define SCORE_ROTATECNT	(80)					//��]�J�E���^�[
#define SCORE_ROTATESPD	(D3DX_PI * 0.1f)		//��]���x
#define MAX_LENGTH		(500.0f)				//�ő咷��
#define SCORE_LENGSPD	(4.0f)					//�����������x
#define SCORE_TEXFILENAME		"data\\TEXTURE\\number000.png"	//
#define SCOREFONT_TEXFILENAME	"data\\TEXTURE\\highscore000.png"	//
#define SCOREINPUT_TEXFILENAME	"data\\TEXTURE\\highscore001.png"	//
#define NUM_SCORE		(3)						//�X�R�A����
#define HIGHSCORE_FILE	"data\\FILE\\highscore.bin"	//
#define SCOREFONT_X			(SCREEN_WIDTH * 0.5f)	//���SX���W
#define SCOREFONT_Y			(SCREEN_HEIGHT * 0.15f)	//���SY���W
#define SCOREFONT_WIDTH		(SCREEN_WIDTH * 0.15f)	//���S��
#define SCOREFONT_HEIGHT	(SCREEN_HEIGHT * 0.1f)	//���S����
#define SCOREINPUT_X		(SCREEN_WIDTH * 0.85f)	//���SX���W
#define SCOREINPUT_Y		(SCREEN_HEIGHT * 0.85f)	//���SY���W
#define SCOREINPUT_WIDTH	(SCREEN_WIDTH * 0.15f)	//���S��
#define SCOREINPUT_HEIGHT	(SCREEN_HEIGHT * 0.13f)	//���S����

//==========================================================
//�\���̂̒�`
//==========================================================

//==========================================================
//�O���[�o���ϐ�
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureHighScore = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHighScore = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureHighScoreFont = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHighScoreFont = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureHighScoreInput = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHighScoreInput = NULL;	//���_�o�b�t�@�ւ̃|�C���^
HighScore g_aHighScore[NUM_SCORE];	//���S�̏��z��
int g_nHighScore = 0;			//�X�R�A
bool g_bNewRecord = false;		//�X�V�������ǂ���

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void SetpVtxHighScore(void);
void SaveRankingScore(void);
void LoadRankingScore(void);
void SetpVtxFont(void);
void SetpVtxInput(void);

//==========================================================
//�e�N�X�`���t�@�C����
//==========================================================

//==========================================================
//�n�C�X�R�A�̏���������
//==========================================================
void InitHighScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	HighScore *pHighScore = GetHighScore();	//���S�̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		SCORE_TEXFILENAME,
		&g_pTextureHighScore);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHighScore,
		NULL);

	g_nHighScore = 0;
	g_bNewRecord = false;
	

	//��񏉊���
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++, pHighScore++)
	{
		pHighScore->pos = D3DXVECTOR3(SCORE_X + nCnt * SCORE_SPACEX, SCORE_Y, 0.0f);
		pHighScore->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pHighScore->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pHighScore->fHeight = SCORE_HEIGHT;
		pHighScore->fWidth = SCORE_WIDTH;
		pHighScore->fAngle = 0.0f;

		//�Ίp���̒������Z�o����
		pHighScore->fLength = sqrtf(SCORE_WIDTH * SCORE_WIDTH + SCORE_HEIGHT * SCORE_HEIGHT) * 0.5f;

		//�Ίp���̊p�x���Z�o����
		pHighScore->fAngle = atan2f(SCORE_WIDTH, SCORE_HEIGHT);
	}

	//���_���ݒ�
	SetpVtxHighScore();

	LoadRankingScore();

	AddHighScore(0);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		SCOREFONT_TEXFILENAME,
		&g_pTextureHighScoreFont);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHighScoreFont,
		NULL);

	//���_�ݒ�
	SetpVtxFont();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		SCOREINPUT_TEXFILENAME,
		&g_pTextureHighScoreInput);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHighScoreInput,
		NULL);

	//
	SetpVtxInput();
}

//==========================================================
//�n�C�X�R�A�̏I������
//==========================================================
void UninitHighScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureHighScore != NULL)
	{
		g_pTextureHighScore->Release();
		g_pTextureHighScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffHighScore != NULL)
	{
		g_pVtxBuffHighScore->Release();
		g_pVtxBuffHighScore = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTextureHighScoreFont != NULL)
	{
		g_pTextureHighScoreFont->Release();
		g_pTextureHighScoreFont = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffHighScoreFont != NULL)
	{
		g_pVtxBuffHighScoreFont->Release();
		g_pVtxBuffHighScoreFont = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTextureHighScoreInput != NULL)
	{
		g_pTextureHighScoreInput->Release();
		g_pTextureHighScoreInput = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffHighScoreInput != NULL)
	{
		g_pVtxBuffHighScoreInput->Release();
		g_pVtxBuffHighScoreInput = NULL;
	}
}

//==========================================================
//�n�C�X�R�A�̍X�V����
//==========================================================
void UpdateHighScore(void)
{
	if (GetNowScore() > g_nHighScore)
	{//�n�C�X�R�A�𒴂����ꍇ
		AddHighScore(GetNowScore() - g_nHighScore);
	}


	if (GetGameState() == GAMESTATE_END)
	{
		//�f�[�^�̕ۑ�
		if (g_bNewRecord == false)
		{
			SaveRankingScore();
			g_bNewRecord = true;
		}
		if (GetKeyboardTrigger(DIK_RETURN) || GetGamepadTrigger(BUTTON_B, 0))
		{
			SetFade(MODE_TEAMLOGO);
			PlaySound(SOUND_LABEL_SECURSOR);
		}
		else if (GetKeyboardTrigger(DIK_SPACE) || GetGamepadTrigger(BUTTON_A, 0))
		{
			SetFade(MODE_GAME);
			PlaySound(SOUND_LABEL_SEBUTTON);
		}
	}
	else
	{
		if (GetKeyboardTrigger(DIK_RETURN) || GetGamepadTrigger(BUTTON_B, 0))
		{
			SetGameState(GAMESTATE_END, 0);
		}
	}
}

//==========================================================
//�n�C�X�R�A�̕`�揈��
//==========================================================
void DrawHighScore(void)
{

	if (GetPlayer()->bUse == false)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffHighScore, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
		{
			if (g_aHighScore[nCnt].bUse == true)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureHighScore);

				//�|���S���̕`��(�J�n���_�ړ�)
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
			}
		}

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffHighScoreFont, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureHighScoreFont);

		//�|���S���̕`��(�J�n���_�ړ�)
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffHighScoreInput, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureHighScoreInput);

		//�|���S���̕`��(�J�n���_�ړ�)
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//==========================================================
//�X�R�A�擾
//==========================================================
HighScore *GetHighScore(void)
{
	return &g_aHighScore[0];
}

//==========================================================
//�n�C�X�R�A�̕`�揈��
//==========================================================
void SetpVtxHighScore(void)
{
	HighScore *pHighScore = GetHighScore();	//���S�̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffHighScore->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++, pHighScore++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = pHighScore->pos.x + sinf(pHighScore->rot.z + (-D3DX_PI + pHighScore->fAngle)) * pHighScore->fLength;
		pVtx[0].pos.y = pHighScore->pos.y + cosf(pHighScore->rot.z + (-D3DX_PI + pHighScore->fAngle)) * pHighScore->fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = pHighScore->pos.x + sinf(pHighScore->rot.z + (D3DX_PI - pHighScore->fAngle)) * pHighScore->fLength;
		pVtx[1].pos.y = pHighScore->pos.y + cosf(pHighScore->rot.z + (D3DX_PI - pHighScore->fAngle)) * pHighScore->fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = pHighScore->pos.x + sinf(pHighScore->rot.z + -pHighScore->fAngle) * pHighScore->fLength;
		pVtx[2].pos.y = pHighScore->pos.y + cosf(pHighScore->rot.z + -pHighScore->fAngle) * pHighScore->fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = pHighScore->pos.x + sinf(pHighScore->rot.z + pHighScore->fAngle) * pHighScore->fLength;
		pVtx[3].pos.y = pHighScore->pos.y + cosf(pHighScore->rot.z + pHighScore->fAngle) * pHighScore->fLength;
		pVtx[3].pos.z = 0.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = pHighScore->col;
		pVtx[1].col = pHighScore->col;
		pVtx[2].col = pHighScore->col;
		pVtx[3].col = pHighScore->col;

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHighScore->Unlock();
}

//==========================================================
//�X�R�A�X�V����
//==========================================================
void AddHighScore(int nAddHighScore)
{
	int aTexU[NUM_SCORE] = {};

	g_nHighScore += nAddHighScore;

	//�^�C�����e�z��Ɋi�[
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		aTexU[nCnt] = g_nHighScore % (int)pow(10, (NUM_SCORE - nCnt)) / (int)pow(10, (NUM_SCORE - nCnt) - 1);

		if (g_nHighScore * 10 >= (int)pow(10, (NUM_SCORE - nCnt)) || nCnt == NUM_SCORE - 1)
		{//�X�R�A����������̏ꍇ
			g_aHighScore[nCnt].bUse = true;
		}
		else
		{
			g_aHighScore[nCnt].bUse = false;
		}
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffHighScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_SCORE; nCntTime++)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f * aTexU[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * aTexU[nCntTime] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aTexU[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * aTexU[nCntTime] + 0.1f, 1.0f);

		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffHighScore->Unlock();
}

//==========================================================
//���݂̃X�R�A
//==========================================================
int GetNowHighScore(void)
{
	return g_nHighScore;
}

//========================================
//�����L���O�̕ۑ�����
//========================================
void SaveRankingScore(void)
{
	FILE *pFile;	//�t�@�C���ւ̃|�C���^

	pFile = fopen(HIGHSCORE_FILE, "wb");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

	 //�f�[�^��ۑ�
		fwrite(&g_nHighScore, sizeof(int), 1, pFile);

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ

	}
}

//========================================
//�����L���O�̓ǂݍ��ݏ���
//========================================
void LoadRankingScore(void)
{
	FILE *pFile;	//�t�@�C���ւ̃|�C���^

	pFile = fopen(HIGHSCORE_FILE, "rb");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		//�f�[�^��ǂݍ���
		fread(&g_nHighScore, sizeof(int), 1, pFile);

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
	 //�Œ�̒l����
		g_nHighScore = 0;
	}
}

//========================================
//�n�C�X�R�A�����̒��_�ݒ�
//========================================
void SetpVtxFont(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffHighScoreFont->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);
	//���_���W�̐ݒ�
	pVtx[0].pos.x = SCOREFONT_X - SCOREFONT_WIDTH;
	pVtx[0].pos.y = SCOREFONT_Y - SCOREFONT_HEIGHT;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = SCOREFONT_X + SCOREFONT_WIDTH;
	pVtx[1].pos.y = SCOREFONT_Y - SCOREFONT_HEIGHT;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = SCOREFONT_X - SCOREFONT_WIDTH;
	pVtx[2].pos.y = SCOREFONT_Y + SCOREFONT_HEIGHT;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = SCOREFONT_X + SCOREFONT_WIDTH;
	pVtx[3].pos.y = SCOREFONT_Y + SCOREFONT_HEIGHT;
	pVtx[3].pos.z = 0.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHighScoreFont->Unlock();
}

void SetpVtxInput(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffHighScoreInput->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);
	//���_���W�̐ݒ�
	pVtx[0].pos.x = SCOREINPUT_X - SCOREINPUT_WIDTH;
	pVtx[0].pos.y = SCOREINPUT_Y - SCOREINPUT_HEIGHT;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = SCOREINPUT_X + SCOREINPUT_WIDTH;
	pVtx[1].pos.y = SCOREINPUT_Y - SCOREINPUT_HEIGHT;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = SCOREINPUT_X - SCOREINPUT_WIDTH;
	pVtx[2].pos.y = SCOREINPUT_Y + SCOREINPUT_HEIGHT;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = SCOREINPUT_X + SCOREINPUT_WIDTH;
	pVtx[3].pos.y = SCOREINPUT_Y + SCOREINPUT_HEIGHT;
	pVtx[3].pos.z = 0.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHighScoreInput->Unlock();
}