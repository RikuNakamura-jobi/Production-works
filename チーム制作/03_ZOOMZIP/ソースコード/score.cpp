//==========================================================
//
//�X�R�A���� [score.h]
//Author Ibuki Okusada
//
//==========================================================
#include "main.h"    //�쐬����main.h���C���N���[�h����
#include "score.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "enemy.h"
#include <math.h>

//==========================================================
//�}�N����`
//==========================================================
#define SCORE_X			(SCREEN_WIDTH * 0.45f)	//���SX���W
#define SCORE_Y			(SCREEN_HEIGHT * 0.92f)	//���SY���W
#define SCORE_SPACEX	(SCREEN_WIDTH * 0.05f)	//���S�X�y�[�X
#define SCORE_WIDTH		(SCREEN_WIDTH * 0.08f)	//���S��
#define SCORE_HEIGHT	(SCREEN_HEIGHT * 0.14f)	//���S����
#define SCORE_FADECNT	(120)					//�J�ڃJ�E���g
#define SCORE_ROTATECNT	(80)					//��]�J�E���^�[
#define SCORE_ROTATESPD	(D3DX_PI * 0.1f)		//��]���x
#define MAX_LENGTH		(500.0f)				//�ő咷��
#define SCORE_LENGSPD	(4.0f)					//�����������x
#define SCORE_TEXFILENAME	"data\\TEXTURE\\number000.png"	//
#define NUM_SCORE		(3)						//�X�R�A����

//==========================================================
//�\���̂̒�`
//==========================================================

//==========================================================
//�O���[�o���ϐ�
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureScore = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Score g_aScore[NUM_SCORE];	//���S�̏��z��
int g_nScore = 0;			//�X�R�A

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void SetpVtxScore(void);

//==========================================================
//�e�N�X�`���t�@�C����
//==========================================================

//==========================================================
//�`�[�����S�̏���������
//==========================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	Score *pScore = GetScore();	//���S�̃|�C���^

	//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			SCORE_TEXFILENAME,
			&g_pTextureScore);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	g_nScore = 0;

	//��񏉊���
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++, pScore++)
	{
		pScore->pos = D3DXVECTOR3(SCORE_X + nCnt * SCORE_SPACEX, SCORE_Y, 0.0f);
		pScore->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pScore->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pScore->fHeight = SCORE_HEIGHT;
		pScore->fWidth = SCORE_WIDTH;
		pScore->fAngle = 0.0f;

		//�Ίp���̒������Z�o����
		pScore->fLength = sqrtf(SCORE_WIDTH * SCORE_WIDTH + SCORE_HEIGHT * SCORE_HEIGHT) * 0.5f;

		//�Ίp���̊p�x���Z�o����
		pScore->fAngle = atan2f(SCORE_WIDTH, SCORE_HEIGHT);
	}

	//���_���ݒ�
	SetpVtxScore();
}

//==========================================================
//�`�[�����S�̏I������
//==========================================================
void UninitScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//==========================================================
//�`�[�����S�̍X�V����
//==========================================================
void UpdateScore(void)
{
	
}

//==========================================================
//�`�[�����S�̕`�揈��
//==========================================================
void DrawScore(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		if (g_aScore[nCnt].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureScore);

			//�|���S���̕`��(�J�n���_�ړ�)
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}
}

//==========================================================
//�X�R�A�擾
//==========================================================
Score *GetScore(void)
{
	return &g_aScore[0];
}

//==========================================================
//�`�[�����S�̕`�揈��
//==========================================================
void SetpVtxScore(void)
{
	Score *pScore = GetScore();	//���S�̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++, pScore++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = pScore->pos.x + sinf(pScore->rot.z + (-D3DX_PI + pScore->fAngle)) * pScore->fLength;
		pVtx[0].pos.y = pScore->pos.y + cosf(pScore->rot.z + (-D3DX_PI + pScore->fAngle)) * pScore->fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = pScore->pos.x + sinf(pScore->rot.z + (D3DX_PI - pScore->fAngle)) * pScore->fLength;
		pVtx[1].pos.y = pScore->pos.y + cosf(pScore->rot.z + (D3DX_PI - pScore->fAngle)) * pScore->fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = pScore->pos.x + sinf(pScore->rot.z + -pScore->fAngle) * pScore->fLength;
		pVtx[2].pos.y = pScore->pos.y + cosf(pScore->rot.z + -pScore->fAngle) * pScore->fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = pScore->pos.x + sinf(pScore->rot.z + pScore->fAngle) * pScore->fLength;
		pVtx[3].pos.y = pScore->pos.y + cosf(pScore->rot.z + pScore->fAngle) * pScore->fLength;
		pVtx[3].pos.z = 0.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = pScore->col;
		pVtx[1].col = pScore->col;
		pVtx[2].col = pScore->col;
		pVtx[3].col = pScore->col;

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
	g_pVtxBuffScore->Unlock();
}

//==========================================================
//�X�R�A�X�V����
//==========================================================
void AddScore(int nAddScore)
{
	int aTexU[NUM_SCORE] = {};

	g_nScore += nAddScore;
	PlaySound(SOUND_LABEL_SCORE);
	SetNearMiss();

	//�^�C�����e�z��Ɋi�[
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		aTexU[nCnt] = g_nScore % (int)pow(10, (NUM_SCORE - nCnt)) / (int)pow(10, (NUM_SCORE - nCnt) - 1);

		if (g_nScore * 10 >= (int)pow(10, (NUM_SCORE - nCnt)))
		{//�X�R�A����������̏ꍇ
			g_aScore[nCnt].bUse = true;
		}
		else
		{
			g_aScore[nCnt].bUse = false;
		}
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffScore->Unlock();
}

//==========================================================
//���݂̃X�R�A
//==========================================================
int GetNowScore(void)
{
	return g_nScore;
}