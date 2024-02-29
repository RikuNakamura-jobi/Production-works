//==========================================
//
//  �`�[�����S���(team_logo.h)
//  Author : Kai Takada
//
//==========================================
#include "team_logo.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//==========================================
//  �}�N����`
//==========================================
#define IN_TIME (240)	//�^�C�g����Ԃ��؂�ւ��܂ł̃^�C�}�[
#define RANKING_FROM_TEAM_LOGO (420)	//�����L���O�ɑJ�ڂ���܂ł̎���

#define PASS_TITLE_LOGO "data/TEXTURE/Tk_software_01.png"	 //�e�N�X�`���p�X
#define POS_DEFAULT (D3DXVECTOR3(640.0f, 360.0f ,0.0f))	 //�����ʒu
#define LOGO_POLYGON_HEIGHT (50.0f)		//�|���S���̍���
#define LOGO_POLYGON_WIDTH (LOGO_POLYGON_HEIGHT * 6.86f)	 //�|���S���̕�
#define LOGO_ALPHA_SCALE (1.0f / IN_TIME)	//�A���t�@�l�̏㏸��

//==========================================
//  �\���̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //���S���W
	D3DXCOLOR col; //�|���S���J���[
}LOGO;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureTeamLogo = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTeamLogo = NULL;
LOGO g_Teamlogo; 
int g_Team_logoState; //�^�C�g���̏��
int g_nCntTitle; //�����L���O�ɑJ�ڂ���J�E���^

//==========================================
//  ����������
//==========================================
void InitTeam_logo()
{
	//�ϐ��̏�����
	g_Team_logoState = TEAM_LOGOSTATE_IN;
	g_nCntTitle = 0;

	//�ϐ��̏�����
	g_Teamlogo.pos = POS_DEFAULT;
	g_Teamlogo.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTeamLogo,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		PASS_TITLE_LOGO,
		&g_pTextureTeamLogo
	);

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTeamLogo->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Teamlogo.pos.x - LOGO_POLYGON_WIDTH, g_Teamlogo.pos.y - LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Teamlogo.pos.x + LOGO_POLYGON_WIDTH, g_Teamlogo.pos.y - LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Teamlogo.pos.x - LOGO_POLYGON_WIDTH, g_Teamlogo.pos.y + LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Teamlogo.pos.x + LOGO_POLYGON_WIDTH, g_Teamlogo.pos.y + LOGO_POLYGON_HEIGHT, 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//rhw�̐ݒ�
		pVtx[nCnt].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[nCnt].col = g_Teamlogo.col;
	}

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTeamLogo->Unlock();

	//BGM�Đ�
	//PlaySound(SOUND_LABEL_BGM000);
}

//==========================================
//  �I������
//==========================================
void UninitTeam_logo()
{
	//�T�E���h�̒�~
	//StopSound();

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTeamLogo != NULL)
	{
		g_pVtxBuffTeamLogo->Release();
		g_pVtxBuffTeamLogo = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTextureTeamLogo != NULL)
	{
		g_pTextureTeamLogo->Release();
		g_pTextureTeamLogo = NULL;
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdateTeam_logo()
{
	//�����J�ڃJ�E���^�̍X�V
	if (g_Team_logoState == TEAM_LOGOSTATE_NORMAL)
	{
		g_nCntTitle++;
		if (g_nCntTitle >= RANKING_FROM_TEAM_LOGO)
		{
			SetFade(MODE_TITLE);
		}
	}

	for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
	{//�L�[�{�[�h AnyKey
		if (GetKeyboardTrigger(nCnt) && nCnt != DIK_ESCAPE)
		{
			if (g_Team_logoState == TEAM_LOGOSTATE_NORMAL)
			{
				g_nCntTitle = RANKING_FROM_TEAM_LOGO;
				break;
			}
			else if (g_Team_logoState == TEAM_LOGOSTATE_IN)
			{
				g_Team_logoState = TEAM_LOGOSTATE_NORMAL;
				break;
			}
		}
	}
	for (int nCnt = 0; nCnt <= KEY_START; nCnt++)
	{//Pad AnyKey
		if (GetJoyPadButtonTrigger((JOYKEY_BUTTON)nCnt, 0))
		{
			if (g_Team_logoState == TEAM_LOGOSTATE_NORMAL)
			{
				g_nCntTitle = RANKING_FROM_TEAM_LOGO;
				break;
			}
			else if (g_Team_logoState == TEAM_LOGOSTATE_IN)
			{
				g_Team_logoState = TEAM_LOGOSTATE_NORMAL;
				break;
			}
		}
	}
	if (GetMouseTrigger(MOUSEBUTTON_LEFT))
	{//���N���b�N
		if (g_Team_logoState == TEAM_LOGOSTATE_NORMAL)
		{
			g_nCntTitle = RANKING_FROM_TEAM_LOGO;
		}
		else if (g_Team_logoState == TEAM_LOGOSTATE_IN)
		{
			g_Team_logoState = TEAM_LOGOSTATE_NORMAL;
		}
	}


	//��ԂɑΉ��������������s����
	switch (g_Team_logoState)
	{
	case TEAM_LOGOSTATE_IN:
		//���S�̏����X�V����
		if (g_Teamlogo.col.a <= 1.0f)
		{
			g_Teamlogo.col.a += LOGO_ALPHA_SCALE;
		}

		//���S�̏���␳����
		if (g_Teamlogo.col.a > 1.0f)
		{
			g_Teamlogo.col.a = 1.0f;
			g_Team_logoState = TEAM_LOGOSTATE_NORMAL;
		}
		break;
	case TEAM_LOGOSTATE_NORMAL:
		//�F���Œ肷��
		if (g_Teamlogo.col.a != 1.0f)
		{
			g_Teamlogo.col.a = 1.0f;
		}
		break;
	default:
		break;
	}

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTeamLogo->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//���_�J���[�̍X�V
		pVtx[nCnt].col = g_Teamlogo.col;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTeamLogo->Unlock();
}

//==========================================
//  �`�揈��
//==========================================
void DrawTeam_logo()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTeamLogo, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTeamLogo);

	//���S�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
