//===========================
//
//�_�C���N�gX.result�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "result.h"
#include "sound.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "score.h"
#include "fade.h"
#include "bullet.h"

//�}�N����`
#define NUM_RESULT (3)												//���U���g��ʂ̐�

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureResult[NUM_RESULT] = {};				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffResult = NULL;						//���_�����i�[
int g_nCounterTime;													//���U���g��ʂ̌p������

//===========================
//���U���g��ʂ̏���������
//===========================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	Player *pPlayer = GetPlayer();
	int nCntResult;
	int nBulletScore = GetBulletScore();

	InitScore();

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Result.png",
		&g_apTextureResult[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Ranking�Z.png",
		&g_apTextureResult[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Ranking�~.png",
		&g_apTextureResult[2]);

	g_nCounterTime = 900;
	nBulletScore = nBulletScore % 10 / 1;

	SetScore(nBulletScore);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_RESULT,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffResult,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
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
	g_VtxBuffResult->Unlock();

	if (pPlayer->state == PLAYERSTATE_DEATH)
	{
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_BGM014);
	}
	else
	{
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_BGM012);
	}
}

//===========================
//���U���g��ʂ̏I������
//===========================
void UninitResult(void)
{
	int nCntResult;

	//�T�E���h�̒�~
	StopSound();

	for (nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureResult[nCntResult] != NULL)
		{
			g_apTextureResult[nCntResult]->Release();
			g_apTextureResult[nCntResult] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffResult != NULL)
	{
		g_VtxBuffResult->Release();
		g_VtxBuffResult = NULL;
	}

	UninitScore();
}

//===========================
//���U���g��ʂ̍X�V����
//===========================
void UpdateResult(void)
{
	FADE fade = GetFade();

	UpdateScore();

	g_nCounterTime--;

	if ((GetKeyboardTrigger(DIK_RETURN) == true && fade == FADE_NONE) || (g_nCounterTime <= 0 && fade == FADE_NONE))
	{
		//���[�h�ݒ�
		SetFade(MODE_TITLE);
	}
}

//===========================
//���U���g��ʂ̕`�揈��
//===========================
void DrawResult(void)
{
	Player *pPlayer = GetPlayer();
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffResult,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < NUM_RESULT; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureResult[0]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}

	DrawScore();
}