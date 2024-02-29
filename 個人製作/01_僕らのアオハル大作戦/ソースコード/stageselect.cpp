//===========================
//
//�_�C���N�gX.stageselect�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "stageselect.h"
#include "input.h"
#include "fade.h"
#include "stage.h"
#include "sound.h"

//�}�N����`
#define NUM_STAGESELECT (11)											//�^�C�g����ʂ̐�

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureStageselect[NUM_STAGESELECT] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffStageselect = NULL;					//���_�����i�[
int g_nCounterStageselectState;										//��ԊǗ��J�E���^�[
int g_nStageselectSelect;
float g_fCntStageselect = 0.0f;										//�^�C�g���̓_�ŗp�ϐ�
bool g_StageselectEnd;												//�I������

																	//�e�N�X�`���t�@�C����
const char *c_apFilenameStageselect[NUM_STAGESELECT] =
{
	"data\\TEXTURE\\aoharuBG003.png",
	"data\\TEXTURE\\stagenumber001.png",
	"data\\TEXTURE\\stagenumber002.png",
	"data\\TEXTURE\\stagenumber003.png",
	"data\\TEXTURE\\stagenumber004.png",
	"data\\TEXTURE\\stagenumber005.png",
	"data\\TEXTURE\\stagenumber006.png",
	"data\\TEXTURE\\stagenumber007.png",
	"data\\TEXTURE\\stagenumber008.png",
	"data\\TEXTURE\\stagenumber009.png",
	"data\\TEXTURE\\stagenumber010.png"
};

//===========================
//�^�C�g����ʂ̏���������
//===========================
void InitStageselect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntStageselect;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	for (int nCnt = 0; nCnt < NUM_STAGESELECT; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameStageselect[nCnt],
			&g_apTextureStageselect[nCnt]);
	}

	g_nCounterStageselectState = 240;
	g_fCntStageselect = 0.0f;
	g_nStageselectSelect = 0;
	g_StageselectEnd = false;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_STAGESELECT,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffStageselect,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffStageselect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStageselect = 0; nCntStageselect < NUM_STAGESELECT; nCntStageselect++)
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

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffStageselect->Unlock();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM016);
}

//===========================
//�^�C�g����ʂ̏I������
//===========================
void UninitStageselect(void)
{
	int nCntStageselect;

	//�T�E���h�̒�~
	StopSound();

	for (nCntStageselect = 0; nCntStageselect < NUM_STAGESELECT; nCntStageselect++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureStageselect[nCntStageselect] != NULL)
		{
			g_apTextureStageselect[nCntStageselect]->Release();
			g_apTextureStageselect[nCntStageselect] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffStageselect != NULL)
	{
		g_VtxBuffStageselect->Release();
		g_VtxBuffStageselect = NULL;
	}
}

//===========================
//�^�C�g����ʂ̍X�V����
//===========================
void UpdateStageselect(void)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	FADE fade = GetFade();
	int nCntStageselect;

	if (g_StageselectEnd == false)
	{
		col.a = (sinf(g_fCntStageselect) + 1.0f) / 2;

		g_fCntStageselect += 0.05f;
	}
	else
	{
		col.a = (sinf(g_fCntStageselect) + 1.0f) / 2;

		g_fCntStageselect += 0.5f;
	}

	//���̓J�[�\���ړ�
	if (GetKeyboardTrigger(DIK_S) == true && g_StageselectEnd == false)
	{
		g_nStageselectSelect += 5;
		col.a = 1.0f;
		g_fCntStageselect = 0.0f;
	}

	if (GetKeyboardTrigger(DIK_W) == true && g_StageselectEnd == false)
	{
		g_nStageselectSelect += 5;
		col.a = 1.0f;
		g_fCntStageselect = 0.0f;
	}

	//���̓J�[�\���ړ�
	if (GetKeyboardTrigger(DIK_D) == true && g_StageselectEnd == false)
	{
		g_nStageselectSelect++;
		col.a = 1.0f;
		g_fCntStageselect = 0.0f;
	}

	if (GetKeyboardTrigger(DIK_A) == true && g_StageselectEnd == false)
	{
		g_nStageselectSelect += 9;
		col.a = 1.0f;
		g_fCntStageselect = 0.0f;
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffStageselect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStageselect = 0; nCntStageselect < NUM_STAGESELECT; nCntStageselect++)
	{
		if (nCntStageselect == (g_nStageselectSelect % 10) + 1)
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, col.a);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, col.a);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, col.a);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, col.a);
		}
		else
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffStageselect->Unlock();

	if (GetKeyboardTrigger(DIK_RETURN) == true && g_StageselectEnd == false)
	{
		g_StageselectEnd = true;

		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_SCHOOL);
	}

	if (GetKeyboardTrigger(DIK_BACKSPACE) == true && g_StageselectEnd == false)
	{
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_DECISION002);

		SetFade(MODE_MENU);
	}

	if (g_StageselectEnd == true)
	{
		g_nCounterStageselectState--;

		if (g_nCounterStageselectState <= 0 && fade == FADE_NONE)
		{
			switch ((g_nStageselectSelect % 10) + 1)
			{
			case 1:
				SetStageState(STAGESTATE_1, 60);
				break;
			case 2:
				SetStageState(STAGESTATE_2, 60);
				break;
			case 3:
				SetStageState(STAGESTATE_3, 60);
				break;
			case 4:
				SetStageState(STAGESTATE_4, 60);
				break;
			case 5:
				SetStageState(STAGESTATE_5, 60);
				break;
			case 6:
				SetStageState(STAGESTATE_6, 60);
				break;
			case 7:
				SetStageState(STAGESTATE_7, 60);
				break;
			case 8:
				SetStageState(STAGESTATE_8, 60);
				break;
			case 9:
				SetStageState(STAGESTATE_9, 60);
				break;
			case 10:
				SetStageState(STAGESTATE_10, 60);
				break;
			}

			SetFade(MODE_GAME);
		}
	}
}

//===========================
//�^�C�g����ʂ̕`�揈��
//===========================
void DrawStageselect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntStageselect;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffStageselect,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntStageselect = 0; nCntStageselect < NUM_STAGESELECT; nCntStageselect++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureStageselect[nCntStageselect]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntStageselect * 4, 2);
	}
}