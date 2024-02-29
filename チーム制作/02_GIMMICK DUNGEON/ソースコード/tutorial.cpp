//==========================================
//
//  �`���[�g���A���̏���(tutorial.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "tutorial.h"
#include "tutorial_message.h"
#include "game.h"
#include "input.h"
#include "fade.h"
#include "polygon.h"
#include "sound.h"
#include "enemy.h"
#include "shadow.h"
#include "gauge.h"
#include "option.h"
#include "count.h"
#include "book.h"

//==========================================
//  �}�N����`
//==========================================
#define TUTORIAL_POS_DEFAULT (D3DXVECTOR3(320.0f, 120.0, 0.0f)) //�`���[�g���A���̈ʒu
#define TUTORIAL_POS_BACK (D3DXVECTOR3(640.0f, 120.0, 0.0f)) //�w�i�|���S���̈ʒu

#define SKIP_GAUGE_POS	(D3DXVECTOR3(1050.0f,700.0f,0.0f))	//�X�L�b�v�Q�[�W�̈ʒu

#define TUTORIAL_POS_ENTER (D3DXVECTOR3(1000.0f, 270.0, 0.0f)) //�w�i�|���S���̈ʒu
#define TUTORIAL_WIDTH (200.0f) //�|���S���̍���
#define TUTORIAL_HEIGHT (100.0f) //�|���S���̕�
#define BACK_WIDTH (550.0f) //�w�i�|���S���̕�
#define BACK_HEIGHT (110.0f) //�w�i�|���S���̍���
#define ENTER_WIDTH (160.0f) //����L�[�\���|���S���̕�
#define ENTER_HEIGHT (40.0f) //����L�[�\���|���S���̍���
#define HOLDFRAME_COUNTER (60) //�������J�ڂ̃t���[����

//==========================================
//  �e�N�X�`���p�X��ݒ�
//==========================================
const char *c_pFilePassTutorial[TUTORIAL_TEX_MAX] =
{
	"data/TEXTURE/tutorial_move.png", //�ړ�����
	"data/TEXTURE/tutorial_move_000.png", //�ړ��̐���
	"data/TEXTURE/tutorial_point.png", //�J����������
	"data/TEXTURE/tutorial_point_000.png", //�J�����̐���
	"data/TEXTURE/tutorial_reflector.png", //���˔𑀍삵��
	"data/TEXTURE/tutorial_reflector_000.png", //���˔̐���
	"data/TEXTURE/tutorial_stopper.png", //�{�^���𑀍삵��
	"data/TEXTURE/tutorial_stopper_000.png", //�{�^���̐���
	"data/TEXTURE/tutorial_jump.png", //�W�����v����
	"data/TEXTURE/tutorial_jump_000.png", //�W�����v�̐���
	"data/TEXTURE/tutorial_enemy.png", //�G��|��
	"data/TEXTURE/tutorial_enemy_001.png", //�G�̐���
	"data/TEXTURE/tutorial_frame.png", //�w�i�|���S��
	"data/TEXTURE/tutorial_decisionr.png", //����L�[�\��
};

//==========================================
//  �`���[�g���A���\���̂̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //�`���[�g���A���|���S���̏ꏊ
	D3DXVECTOR3 size; //�`���[�g���A���|���S���̑傫��
	D3DXCOLOR col; //�`���[�g���A���|���S���̐F
	TUTORIALTYPE type; //�ݒ肳���e�N�X�`���̎��
	TUTORIALPOS side; //�ݒ肳���ʒu
	bool bUse; //�`��̗L��
	bool bClear; //�i�s
}TUTORIAL;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureTutorial[TUTORIAL_TEX_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;
TUTORIALSTATE g_TutorialState; //�`���[�g���A�����
TUTORIAL g_Tutorial[TUTORIAL_TEX_MAX]; //�`���[�g���A���|���S���̃X�e�[�^�X
bool g_bEnemy; //�G�̏o�����
int g_nHoldCtr;	//�������J�E���g
int g_nIdxHoldGauge;	//�X�L�b�v�Q�[�W
bool g_bFade;	//�����t�F�[�h�h�~

//==========================================
//  ����������
//==========================================
void InitTutorial()
{
	//�ϐ��̏�����
	g_TutorialState = TUTORIALSTATE_MOVE;
	g_bEnemy = false;
	g_bFade = false;
	g_nHoldCtr = 0;

	//�ϐ��̐ݒ�
	for (int nCnt = 0; nCnt < TUTORIAL_TEX_MAX; nCnt++)
	{
		//�s�g�p�ŏ�����
		g_Tutorial[nCnt].bUse = false;
		g_Tutorial[nCnt].bClear = false;

		//�|���S���f�[�^��ݒ�
		if (nCnt == TUTORIAL_BG)
		{
			//��ނ�ݒ�
			g_Tutorial[nCnt].type = TUTORIALTYPE_BG;

			//�ʒu��ݒ�
			g_Tutorial[nCnt].pos = TUTORIAL_POS_BACK;

			//�T�C�Y��ݒ�
			g_Tutorial[nCnt].size = D3DXVECTOR3(BACK_WIDTH, BACK_HEIGHT, 0.0f);

			//�F��ݒ�
			g_Tutorial[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

			//�g�p�ݒ�
			g_Tutorial[nCnt].bUse = true;
		}
		else if (nCnt == TUTORIAL_ENTER)
		{
			//��ނ�ݒ�
			g_Tutorial[nCnt].type = TUTORIALTYPE_ENTER;

			//�ʒu��ݒ�
			g_Tutorial[nCnt].pos = TUTORIAL_POS_ENTER;

			//�T�C�Y��ݒ�
			g_Tutorial[nCnt].size = D3DXVECTOR3(ENTER_WIDTH, ENTER_HEIGHT, 0.0f);

			//�F��ݒ�
			g_Tutorial[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

			//�g�p�ݒ�
			g_Tutorial[nCnt].bUse = false;
		}
		else
		{
			if (nCnt % 2 != 0)
			{
				//��ނ�ݒ�
				g_Tutorial[nCnt].type = TUTORIALTYPE_EX;

				//�ʒu��ݒ�
				g_Tutorial[nCnt].side = RIGHTSIDE;

				//�ʒu��ݒ�
				g_Tutorial[nCnt].pos = TUTORIAL_POS_DEFAULT;
				g_Tutorial[nCnt].pos.x *= 3.0f;

				//�T�C�Y��ݒ�
				g_Tutorial[nCnt].size = D3DXVECTOR3(TUTORIAL_WIDTH, TUTORIAL_HEIGHT, 0.0f);

				//�F��ݒ�
				g_Tutorial[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

				//�g�p�ݒ�
				g_Tutorial[nCnt].bUse = false;
			}
			else
			{
				//��ނ�ݒ�
				g_Tutorial[nCnt].type = TUTORIALTYPE_BASE;

				//�ʒu��ݒ�
				g_Tutorial[nCnt].side = LEFTSIDE;

				//�ʒu��ݒ�
				g_Tutorial[nCnt].pos = TUTORIAL_POS_DEFAULT;

				//�T�C�Y��ݒ�
				g_Tutorial[nCnt].size = D3DXVECTOR3(TUTORIAL_WIDTH, TUTORIAL_HEIGHT, 0.0f);

				//�F��ݒ�
				g_Tutorial[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

				//�g�p�ݒ�
				g_Tutorial[nCnt].bUse = false;
			}
		}
	}

	//�Q�[���̏�����
	InitGame();

	//�{�̏�����
	InitBook();

	//�Q�[�W�̐ݒ�
	g_nIdxHoldGauge = SetGauge(SKIP_GAUGE_POS, HOLDFRAME_COUNTER, 30.0f, 200.0f, GAUGETYPE_HORIZON,SKIPGAUGE_CR);

	//�|���S���̏�����
	g_pVtxBuffTutorial = Init_2D_Polygon(TUTORIAL_TEX_MAX);

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	for (int nCnt = 0; nCnt < TUTORIAL_TEX_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile
		(
			pDevice,
			c_pFilePassTutorial[nCnt],
			&g_pTextureTutorial[nCnt]
		);
	}

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < TUTORIAL_TEX_MAX; nCntPolygon++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Tutorial[nCntPolygon].pos.x - g_Tutorial[nCntPolygon].size.x, g_Tutorial[nCntPolygon].pos.y - g_Tutorial[nCntPolygon].size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Tutorial[nCntPolygon].pos.x + g_Tutorial[nCntPolygon].size.x, g_Tutorial[nCntPolygon].pos.y - g_Tutorial[nCntPolygon].size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Tutorial[nCntPolygon].pos.x - g_Tutorial[nCntPolygon].size.x, g_Tutorial[nCntPolygon].pos.y + g_Tutorial[nCntPolygon].size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Tutorial[nCntPolygon].pos.x + g_Tutorial[nCntPolygon].size.x, g_Tutorial[nCntPolygon].pos.y + g_Tutorial[nCntPolygon].size.y, 0.0f);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//���_�J���[�̐ݒ�
			pVtx[nCnt].col = g_Tutorial[nCntPolygon].col;
		}

		if (g_Tutorial[nCntPolygon].type == TUTORIALTYPE_BASE)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
		}

		else
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		//���_�f�[�^�̃|�C���^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTutorial->Unlock();

	//�`���[�g���A�����b�Z�[�W�̏�����
	InitTutorialMessage();

	//�J�E���g�̏�����
	InitCount();

	PlaySound(SOUND_LABEL_BGM001);
}

//==========================================
//  �I������
//==========================================
void UninitTutorial()
{
	//�T�E���h�̒�~
	StopSound();

	//�Q�[���̏I��
	UninitGame();

	//�{�̏I��
	UninitBook();

	//�`���[�g���A�����b�Z�[�W�̏I��
	UninitTutorialMessage();

	//�J�E���g�̏I��
	UninitCount();

	//�|���S���̔j��
	UninitPolygon(&g_pVtxBuffTutorial);

	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < TUTORIAL_TEX_MAX; nCnt++)
	{
		if (g_pTextureTutorial[nCnt] != NULL)
		{
			g_pTextureTutorial[nCnt]->Release();
			g_pTextureTutorial[nCnt] = NULL;
		}
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdateTutorial()
{
	for (int nCnt = 0; nCnt < TUTORIAL_TEX_MAX; nCnt++)
	{
		//�i�s��Ԃ�ۑ�����
		if (nCnt < g_TutorialState * 2)
		{
			g_Tutorial[nCnt].bClear = true;
		}

		//�i�s�ɓY���ă|���S����`�悷��
		if (nCnt != TUTORIAL_BG)
		{
			if (nCnt == g_TutorialState * 2 || nCnt == g_TutorialState * 2 + 1)
			{
				g_Tutorial[nCnt].bUse = true;
			}
			if (nCnt == TUTORIAL_ENTER)
			{
				if (g_TutorialState == TUTORIALSTATE_CAMERA)
				{
					g_Tutorial[nCnt].bUse = true;
				}
			}

			//�|���S������������
			if (g_Tutorial[nCnt].col.a < 0.0f)
			{
				g_Tutorial[nCnt].bUse = false;
			}
			if (g_Tutorial[nCnt].bClear && nCnt != TUTORIAL_CAMERA)
			{
				g_Tutorial[nCnt].col.a -= 0.02f;
			}
			if (nCnt == TUTORIAL_CAMERA)
			{
				if (g_Tutorial[nCnt].bClear && g_Tutorial[TUTORIAL_ENTER].bClear)
				{
					g_Tutorial[nCnt].col.a -= 0.02f;
				}
			}

			//�|���S����`�悷��
			if (g_Tutorial[nCnt].bUse)
			{
				if (nCnt < 2)
				{
					if (g_Tutorial[nCnt].col.a < 1.0f)
					{
						g_Tutorial[nCnt].col.a += 0.01f;
					}
				}
				else if(nCnt == TUTORIAL_ENTER)
				{
					if (g_Tutorial[nCnt].col.a < 1.0f)
					{
						g_Tutorial[nCnt].col.a += 0.01f;
					}
				}
				else
				{
					if (g_Tutorial[nCnt - 2].bUse == false)
					{
						if (g_Tutorial[nCnt].col.a < 1.0f)
						{
							g_Tutorial[nCnt].col.a += 0.01f;
						}
					}
				}
			}
		}
	}

	//�J�����`���[�g���A���̂݃G���^�[�Ői�߂�
	if (g_TutorialState == TUTORIALSTATE_CAMERA && (GetKeyboardTrigger(DIK_RETURN) || GetJoyPadButtonTrigger(KEY_START, 0)))
	{
		g_TutorialState = TUTORIALSTATE_REFLECTOR;
		g_Tutorial[TUTORIAL_ENTER].bClear = true;

		PlaySound(SOUND_LABEL_SE_BUTTON_004);
	}

	//�J�����`���[�g���A���̂݊��x��������������
	if (g_TutorialState == TUTORIALSTATE_CAMERA)
	{
		UpdateOption();

		if (GetKeyboardPress(DIK_DOWN) || GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) || GetMouseControll() || GetStickR(0) != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
			g_Tutorial[TUTORIAL_CAMERA].bClear = true;
		}
	}

	//�`���[�g���A���̍Ō�ɓG���Ăяo��
	if (g_TutorialState == TUTORIALSTATE_JUMP && g_bEnemy == false)
	{
		SetEnemy(D3DXVECTOR3(310.76f, 0.0f, 300.57f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
		SetPositionShadow(GetEnemy()->nIdxGauge, GetEnemy()->pos, GetEnemy()->rot, 1.0f);
		g_bEnemy = true;
	}

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < TUTORIAL_TEX_MAX; nCntPolygon++)
	{
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//���_�J���[�̐ݒ�
			pVtx[nCnt].col = g_Tutorial[nCntPolygon].col;
		}

		//�e�N�X�`�����W�̐ݒ�
		if (g_Tutorial[nCntPolygon].bClear && g_Tutorial[nCntPolygon].type == TUTORIALTYPE_BASE)
		{
			pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		//���_�f�[�^�̃|�C���^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTutorial->Unlock();

	//�`���[�g���A���̏I��
	if (GetPause() == false)
	{
		if (g_nHoldCtr < HOLDFRAME_COUNTER)
		{
			if ((GetKeyboardPress(DIK_RETURN) || GetJoyPadButtonPress(KEY_START, 0)) && *GetBookState() == BOOKSTATE_END)
			{//�J�E���^�[���Z
				g_nHoldCtr++;
			}
			else
			{//�J�E���^�[���Z�b�g
				g_nHoldCtr = 0;
			}
		}

		//��ʑJ��
		if ((g_nHoldCtr >= HOLDFRAME_COUNTER || g_TutorialState == TUTORIALSTATE_END) &&
			g_bFade == false)
		{
			SetFade(MODE_GAME);

			g_bFade = true;
		}
	}

	//�Q�[�W���X�V
	SetInfoGauge(g_nIdxHoldGauge, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), (float)g_nHoldCtr);

	//�{�̍X�V
	UpdateBook();

	//�Q�[���̍X�V
	UpdateGame();

	//�`���[�g���A�����b�Z�[�W�̍X�V
	UpdateTutorialMessage();

	//�J�E���g�̍X�V
	UpdateCount();
}

//==========================================
//  �`�揈��
//==========================================
void DrawTutorial()
{
	//���[�J���ϐ��錾
	int nCntPolygon;

	//�Q�[���̕`��
	DrawGame();

	//�{�̕`��
	DrawBook();

	if (*GetBookState() == BOOKSTATE_END && GetFade() == FADE_NONE)
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�`���[�g���A�����e�̕`��
		for (nCntPolygon = TUTORIAL_TEX_MAX - 1; nCntPolygon >= 0; nCntPolygon--)
		{
			if (g_Tutorial[nCntPolygon].bUse)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureTutorial[nCntPolygon]);

				//�`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPolygon * 4, 2);
			}
		}

		//�`���[�g���A�����b�Z�[�W�̕`��
		DrawTutorialMessage();

		//�J�E���g�̕`��
		if (g_TutorialState == TUTORIALSTATE_JUMP || g_TutorialState == TUTORIALSTATE_ENEMY)
		{
			DrawCount();
		}
	}
}

//==========================================
//  �`���[�g���A����Ԃ̃|�C���^���擾
//==========================================
TUTORIALSTATE *GetTutorialState()
{
	return &g_TutorialState;
}
