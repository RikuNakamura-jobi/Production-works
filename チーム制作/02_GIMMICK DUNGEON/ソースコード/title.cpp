
//==========================================
//
//  �^�C�g�����(title.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "title_logo.h"
#include "title_message.h"
#include "camera.h"
#include "game.h"
#include "object.h"
#include "billboard.h"
#include "menu.h"
#include "field.h"
#include "sound.h"
#include "fog.h"

//==========================================
//  �}�N����`
//==========================================
#define MENU_COUNT (30) //���j���[�ɑJ�ڂ���܂ł̎���
#define RANKING_FROM_TITLE (1800) //�����L���O�ɑJ�ڂ���܂ł̎���

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
int g_TitleState; //�^�C�g���̏��
int g_nCntFadeIn; //�^�C�g����Ԃ̃J�E���^
int g_nCntMenu; //���j���[�֑J�ڂ��鎞�Ԃ̃J�E���^
int g_nCntRanking; //�����L���O�ɑJ�ڂ���J�E���^

//==========================================
//  ����������
//==========================================
void InitTitle()
{
	//�ϐ��̏�����
	g_TitleState = TITLESTATE_IN;
	g_nCntFadeIn = 0;
	g_nCntMenu = 0;
	g_nCntRanking = 0;

	//�Q�[���̏�����
	InitGame();

	//�^�C�g�����S�̏�����
	InitLogo();

	//�^�C�g�����b�Z�[�W�̏�����
	InitMessage();

	//���j���[�̏�����
	InitMenu();

	//���̏�����
	InitField();

	//�t�H�O�̐ݒ�
	EnableFog(true);
	FOGDATA *pFog = GetFog();

	//�t�H�O�F�ݒ�
	pFog->col = FOG_COLOR_BLACK;

	//BGM�Đ�
	PlaySound(SOUND_LABEL_BGM000);
}

//==========================================
//  �I������
//==========================================
void UninitTitle()
{
	//�T�E���h�̒�~
	StopSound();

	//�J�����̏I��
	UninitGame();

	//�^�C�g�����S�̏I��
	UninitLogo();

	//�^�C�g�����b�Z�[�W�̏I��
	UninitMessage();

	//���j���[�̏I��
	UninitMenu();

	//�t�B�[���h�̏I��
	UninitField();
}

//==========================================
//  �X�V����
//==========================================
void UpdateTitle()
{
	//�����J�ڃJ�E���^�̍X�V
	if (g_TitleState == TITLESTATE_NORMAL)
	{
		g_nCntRanking++;
		if (g_nCntRanking >= RANKING_FROM_TITLE)
		{
			SetFade(MODE_RANKING);
		}
		for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
		{
			if (GetKeyboardPress(nCnt))
			{
				g_nCntRanking = 0;
			}
		}
		for (int nCnt = 0; nCnt <= KEY_START; nCnt++)
		{
			if (GetJoyPadButtonPress((JOYKEY_BUTTON)nCnt, 0))
			{
				g_nCntRanking = 0;
			}
		}
		if (GetStickL(0) != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
			g_nCntRanking = 0;
		}
		if (GetMousePress(MOUSEBUTTON_LEFT))
		{
			g_nCntRanking = 0;
		}
		if (GetJoyPadCrossNone(0) == false)
		{
			g_nCntRanking = 0;
		}
	}
	else
	{
		g_nCntRanking = 0;
	}

	//�J�����̍X�V
	UpdateCamera();

	if (g_TitleState != TITLESTATE_MENU)
	{
		//�J�E���^�̉��Z
		if (g_TitleState == TITLESTATE_IN)
		{
			g_nCntFadeIn++;
			if (g_nCntFadeIn == IN_TIME)
			{
				g_TitleState = TITLESTATE_NORMAL;
			}
		}

		//�^�C�g����Ԃ̍X�V
		for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
		{
			if (nCnt != DIK_ESCAPE)
			{
				if (GetKeyboardTrigger(nCnt))
				{
					if (g_TitleState == TITLESTATE_NORMAL)
					{
						PlaySound(SOUND_LABEL_SE_BUTTON_002);
					}

					if (g_TitleState != TITLESTATE_OUT)
					{
						g_TitleState++;
						break;
					}
				}
			}
		}
		for (int nCnt = 0; nCnt <= KEY_START; nCnt++)
		{
			if (GetJoyPadButtonTrigger((JOYKEY_BUTTON)nCnt,0))
			{
				if (g_TitleState == TITLESTATE_NORMAL)
				{
					PlaySound(SOUND_LABEL_SE_BUTTON_002);
				}

				if (g_TitleState != TITLESTATE_OUT)
				{
					g_TitleState++;
					break;
				}
			}
		}
		if (GetMousePress(MOUSEBUTTON_LEFT))
		{
			if (g_TitleState == TITLESTATE_NORMAL)
			{
				PlaySound(SOUND_LABEL_SE_BUTTON_002);
			}

			if (g_TitleState != TITLESTATE_OUT)
			{
				g_TitleState++;
			}
		}

		//�t�F�[�h�̊J�n
		if (g_TitleState == TITLESTATE_OUT)
		{
			g_nCntMenu++;
			if (g_nCntMenu >= MENU_COUNT)
			{
				g_TitleState = TITLESTATE_MENU;
			}
		}

		//�^�C�g�����S�̍X�V
		UpdateLogo();

		//�^�C�g�����b�Z�[�W�̍X�V
		UpdateMessage();
	}
	else
	{
		//���j���[�̍X�V
		UpdateMenu();
	}

	//�r���{�[�h�̍X�V����
	UpdateBillboard();

	//�I�u�W�F�N�g�̍X�V
	UpdateObject();

	//�t�B�[���h�̍X�V
	UpdateField();

	//�Q�[���X�V
	UpdateGame();
}

//==========================================
//  �`�揈��
//==========================================
void DrawTitle()
{
	//�J�����̐ݒ�
	SetCamera();
	DrawGame();

	//���̕`��
	DrawField();

	if (g_TitleState != TITLESTATE_MENU)
	{
		//�^�C�g�����S�̕`��
		DrawLogo();

		//�^�C�g�����b�Z�[�W�̕`��
		DrawMessage();
	}
	else
	{
		//���j���[�̕`��
		DrawMenu();
	}
}

//==========================================
//  �^�C�g����Ԃ̎擾
//==========================================
int GetTitleState()
{
	return g_TitleState;
}
