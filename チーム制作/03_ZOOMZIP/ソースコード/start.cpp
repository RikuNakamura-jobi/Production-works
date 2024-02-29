//==========================================================
//
//�|���S������ [Start.h]
//Author Ibuki Okusada
//
//==========================================================
#include "start.h"
#include "texture.h"
#include "fileload.h"
#include "texture.h"
#include "player.h"
#include "input.h"
#include "game.h"
#include "sound.h"

//==========================================================
//�}�N����`
//==========================================================
#define MAX_POLYGON		(1)

//==========================================================
//�O���[�o���ϐ�
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureStart[MAX_POLYGON] = {};			//�e�N�X�`��1�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStart = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Start g_aStart[MAX_POLYGON];

//==========================================================
//�|���S���̏���������
//==========================================================
void InitStart(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	for (int nCntStart = 0; nCntStart < MAX_POLYGON; nCntStart++)
	{
		if (GetMode() == MODE_GAME && nCntStart == MAX_POLYGON - 1)
		{
			g_pTextureStart[nCntStart] = *SetTexture(3);
		}

		g_aStart[nCntStart].pos = {};
		g_aStart[nCntStart].rot = {};
		g_aStart[nCntStart].fWidth = 0.0f;
		g_aStart[nCntStart].fHeight = 0.0f;
		g_aStart[nCntStart].fTexU = 0.0f;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStart,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStart = 0; nCntStart < MAX_POLYGON; nCntStart++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.15f, 200.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.85f, 200.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.15f, 390.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.85f, 390.0f, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(g_aStart[nCntStart].fTexU, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aStart[nCntStart].fTexU + 1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aStart[nCntStart].fTexU, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aStart[nCntStart].fTexU + 1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStart->Unlock();
}

//==========================================================
//�|���S���̏I������
//==========================================================
void UninitStart(void)
{

	//�e�N�X�`���̔j��
	for (int nCntStart = 0; nCntStart < MAX_POLYGON; nCntStart++)
	{
		if (g_pTextureStart[nCntStart] != NULL)
		{
			g_pTextureStart[nCntStart]->Release();
			g_pTextureStart[nCntStart] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffStart != NULL)
	{
		g_pVtxBuffStart->Release();
		g_pVtxBuffStart = NULL;
	}
}

//==========================================================
//�|���S���̍X�V����
//==========================================================
void UpdateStart(void)
{
	if (GetKeyboardTrigger(DIK_SPACE) ||
		GetGamepadTrigger(BUTTON_A, 0))
	{
		SetGameState(GAMESTATE_NORMAL, 0);
		PlaySound(SOUND_LABEL_SEBUTTON);
	}
}

//==========================================================
//�|���S���̕`�揈��
//==========================================================
void DrawStart(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾

	for (int nCntStart = 0; nCntStart < MAX_POLYGON; nCntStart++)
	{

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffStart, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureStart[nCntStart]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
			4 * nCntStart,								//�v���~�e�B�u�i�|���S���̐��j
			2);												//�`�悷��v���~�e�B�u��
	}
}