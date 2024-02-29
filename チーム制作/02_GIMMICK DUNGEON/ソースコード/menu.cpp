//==========================================
//
//  ���j���[���(menu.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "menu.h"
#include "menu_item.h"
#include "menu_message.h"
#include "input.h"
#include "fade.h"
#include "game.h"
#include "object.h"
#include "billboard.h"
#include "sound.h"

//==========================================
//  �}�N����`
//==========================================
#define TITLE_FROM_MENU (1800) //���u�Ń^�C�g���ɑJ�ڂ��鎞��

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
int g_nMenu; //�I�����
int g_nCntTimer; //���u�^�C�}�[
bool g_bEnter; //����t���O

//==========================================
//  ����������
//==========================================
void InitMenu()
{
	//�ϐ��̏�����
	g_nMenu = MENUSTATE_START;
	g_nCntTimer = 0;
	g_bEnter = false;

	//���j���[���ڂ̏�����
	InitMenuItem();

	//���j���[���b�Z�[�W�̏�����
	InitMenuMessage();
}

//==========================================
//  �I������
//==========================================
void UninitMenu()
{
	//���j���[���ڂ̏I��
	UninitMenuItem();

	//���j���[���b�Z�[�W�̏I��
	UninitMenuMessage();
}

//==========================================
//  �X�V����
//==========================================
void UpdateMenu()
{
	//�^�C�}�[�̍X�V
	g_nCntTimer++;

	//���j���[�̌���
	if (GetFade() == FADE_NONE)
	{
		if (g_bEnter != true)
		{
			//�I����Ԃ̍X�V
			if (GetKeyboardTrigger(DIK_W) || GetStickTriggerL(STICK_UP, 0) || GetJoyPadCrossTrigger(CROSS_UP, 0))
			{
				PlaySound(SOUND_LABEL_SE_BUTTON_000);

				g_nMenu += (MENUSTATE_FADE - 1);

				//���u�^�C�}�[�̃��Z�b�g
				g_nCntTimer = 0;

				//�I����Ԃ̕␳
				g_nMenu %= MENUSTATE_FADE;
			}
			if (GetKeyboardTrigger(DIK_S) || GetStickTriggerL(STICK_DOWN, 0) || GetJoyPadCrossTrigger(CROSS_DOWN, 0))
			{
				PlaySound(SOUND_LABEL_SE_BUTTON_000);

				g_nMenu++;

				//���u�^�C�}�[�̃��Z�b�g
				g_nCntTimer = 0;

				//�I����Ԃ̕␳
				g_nMenu %= MENUSTATE_FADE;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) || (GetJoyPadButtonTrigger(KEY_A, 0) || GetJoyPadButtonTrigger(KEY_START, 0)) || GetMouseTrigger(MOUSEBUTTON_LEFT))
		{
			if (g_nMenu != MENUSTATE_FADE)
			{
				PlaySound(SOUND_LABEL_SE_BUTTON_001);
			}

			switch (g_nMenu)
			{
			case MENUSTATE_START: //�Q�[���X�^�[�g
				SetFade(MODE_GAME);
				g_nMenu = MENUSTATE_FADE;
				break;
			case MENUSTATE_TUTORIAL: //�`���[�g���A��
				SetFade(MODE_TUTORIAL);
				break;
			case MENUSTATE_QUIT: //�Q�[���I��
				SetFade(MODE_TITLE);
				break;
			default:
				break;
			}

			//�t���O���ւ��܂�
			g_bEnter = true;
		}
	}

	//���j���[���ڂ̍X�V
	UpdateMenuItem();

	//���j���[���b�Z�[�W�̍X�V
	UpdateMenuMessage();

	//�^�C�g���ɋ�������
	if (g_nCntTimer >= TITLE_FROM_MENU)
	{
		SetFade(MODE_TITLE);
	}
}

//==========================================
//  �`�揈��
//==========================================
void DrawMenu()
{
	//���j���[���ڂ̕`��
	DrawMenuItem();

	//���j���[���b�Z�[�W�̕`��
	DrawMenuMessage();
}

//==========================================
//  ���j���[��Ԃ̎擾
//==========================================
int GetMenuState()
{
	return g_nMenu;
}

//==========================================
//  ����t���O���擾
//==========================================
bool GetEnterFrag()
{
	return g_bEnter;
}
