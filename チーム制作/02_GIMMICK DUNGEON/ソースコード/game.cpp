//========================================
//
//2D�A�N�V�����Q�[��(�Q�[������)[game.cpp]
//Author:���v�ԗD��
//
//========================================
#include "game.h"
#include "input.h"
#include "fade.h"
#include "meshfield.h"
#include "object.h"
#include "pause.h"
#include "camera.h"
#include "light.h"
#include "ceiling.h"
#include "player.h"
#include "edit.h"
#include "enemy.h"
#include "rock.h"
#include "wall.h"
#include "rock.h"
#include "wall.h"
#include "billboard.h"
#include "shock.h"
#include "result.h"
#include "time.h"
#include "fog.h"
#include "gauge.h"
#include "score.h"
#include "effect.h"
#include "particle.h"
#include "ranking.h"
#include "shadow.h"
#include "orbit.h"
#include "countdown.h"
#include "sound.h"
#include "point.h"
#include "point_log.h"

//====================
// �}�N����`
//====================
#define NUM_GAME0			(0)
#define NUM_GAME1			(1)
#define NUM_GAME2			(2)
#define COUNT_END			(300)	//�J�ڗP�\
#define TIMESCORE_MLT		(500)	//�������A�c�莞�ԂɊ|����l
#define RESULT_SCORE_POS	(D3DXVECTOR3(300.0f, 450.0f, 0.0f))	//���U���g���X�R�A���W
#define RESULT_POINT_POS	(D3DXVECTOR3(350.0f, 600.0f, 0.0f))	//���U���g���|�C���g���W

//====================
// �O���[�o���ϐ��錾
//====================
GAMESTATE g_gameState;							//�Q�[�����
int g_nCounterGameState;									//��ԊǗ��J�E���^�[
bool g_bPause;											//�|�[�Y��Ԃ�ON/OFF
bool g_bEdit;											//�G�f�B�b�g��Ԃ�ON/OFF
float g_fStart;	//�t�H�O�̊J�n����
float g_fEnd;	//�t�H�O�̏I������
int g_nEndCnter;	//���U���g�܂ł̗P�\
bool g_bStopEnemy;	//�G���~�߂邩�ǂ���
bool g_bPhoto;	//�t�H�g���[�h���ǂ���
bool g_bDebug;	//�f�o�b�O�\�������邩�ǂ���

//====================
// �v���g�^�C�v�錾
//====================
void DebugFOG(void);
void SetGameEnd(RESULTTYPE result);

//========================================
//�Q�[���̏���������
//========================================
void InitGame(void)
{
	//�J�E���g�_�E���̏�����
	InitCountdown();

	//�e�̏�����
	InitShadow();

	//�Q�[�W�̏�����
	InitGauge();

	//�r���{�[�h�̏���������
	InitBillboard();

	//�J�����̏���������
	InitCamera();

	//���C�g�̏���������
	InitLight();

	//�I�u�W�F�N�g�̏�����
	InitObject();

	//���b�V���t�B�[���h�̏�����
	InitMeshField();

	//�V��̏���������
	InitCeiling();

	//�ǂ̏���������
	InitWall();

	//��̏���������
	InitRock();

	//�v���C���[�̏���������
	InitPlayer();

	//�G�l�~�[�̏�����
	InitEnemy();
	if (GetMode() == MODE_GAME)
	{
		SetEnemy(D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
		SetPositionShadow(GetEnemy()->nIdxGauge, GetEnemy()->pos, GetEnemy()->rot, 1.0f);
	}

	//�Ռ��g�̏�����
	InitShock();

	//�O�Ղ̏�����
	InitOrbit();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�p�[�e�B�N���̏���������
	InitParticle();

	//�X�R�A�̏�����
	InitScore();

	//�^�C�}�[�̏�����
	InitTime();

	//�|�[�Y�̏���������
	InitPause();

	//�G�f�B�b�g�̏���������
	InitEdit();
	LoadEdit();

	//�擾�X�R�A�\���̏�����
	InitPoint();

	//�擾�X�R�A���b�Z�[�W�̏�����
	InitPointLog();

	if (GetMode() == MODE_GAME)
	{
		//�T�E���h�Đ� =
		PlaySound(SOUND_LABEL_BGM001);
	}

	//�Q�[���V�[���ȊO�ł̏�Ԃ�ݒ�
	MODE mode = GetMode();

	switch (mode)
	{
	case MODE_TITLE:
	case MODE_MENU:
		g_gameState = GAMESTATE_NONE;
		break;
	case MODE_GAME:
		g_gameState = GAMESTATE_READY;
		break;
	default:
		g_gameState = GAMESTATE_NORMAL;		//�ʏ��Ԃɐݒ�
		break;
	}

	g_nCounterGameState = 0;
	g_bPause = false;
	g_bEdit = false;
	g_nEndCnter = COUNT_END;
	g_bStopEnemy = false;
	g_bPhoto = false;

	g_fStart = 1.0f;
	g_fEnd = 3000.0f;

	//�t�H�O�|�C���^�擾
	FOGDATA *pFog = GetFog();

	switch (GetMode())
	{//�t�H�O�̐ݒ�
	case MODE_TITLE:
		//�F�ݒ�
		pFog->col = FOG_COLOR_BLACK;

		//�I���E�J�n�����ݒ�
		pFog->fEnd = FOG_END_TITLE;
		pFog->fStart = FOG_START_TITLE;

		break;
	case MODE_GAME:
		//�F�ݒ�
		pFog->col = FOG_COLOR_SAND;

		//�I���E�J�n�����ݒ�
		pFog->fEnd = FOG_END_GAME;
		pFog->fStart = FOG_START_GAME;

		break;
	case MODE_TUTORIAL:
		//�F�ݒ�
		pFog->col = FOG_COLOR_SAND;

		//�I���E�J�n�����ݒ�
		pFog->fEnd = FOG_END_GAME;
		pFog->fStart = FOG_START_GAME;

		break;
	default:
		break;
	}
}

//========================================
//�Q�[���̏I������
//========================================
void UninitGame(void)
{
	if (GetMode() == MODE_GAME)
	{
		//�T�E���h�̒�~
		StopSound();
	}

	//�e�̏I������
	UninitShadow();

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();

	//�V��̏I������
	UninitCeiling();

	//�ǂ̏I������
	UninitWall();

	//�r���{�[�h�̏I������
	UninitBillboard();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	//�v���C���[�̏I������
	UninitPlayer();

	//��̏I������
	UninitRock();

	//�I�u�W�F�N�g�̏I������
	UninitObject();

	//�Ռ��g�̏I������
	UninitShock();

	//�G�l�~�[�̏I������
	UninitEnemy();

	if (GetMode() != MODE_TUTORIAL)
	{
		//�J�E���g�_�E���̏I������
		UninitCountdown();
	}

	//�|�[�Y�̏I������
	UninitPause();

	//�^�C�}�[�̏I��
	UninitTime();

	//���U���g�̏I��
	UninitResult();

	//�Q�[�W�̏I������
	UninitGauge();

	//�X�R�A�̏I��
	UninitScore();

	//�O�Ղ̏I��
	UninitOrbit();

	//�擾�X�R�A�̕\���̏I��
	UninitPoint();

	//�擾�X�R�A���b�Z�[�W�̏I��
	UninitPointLog();
}

//========================================
//�Q�[���̍X�V����
//========================================
void UpdateGame(void)
{
	if (g_gameState == GAMESTATE_NORMAL || g_gameState == GAMESTATE_END || g_gameState == GAMESTATE_READY)
	{//�Q�[�����̏���
		if (g_bPause == false)
		{
			if (g_gameState == GAMESTATE_READY)
			{
				//�J�E���g�_�E���̍X�V
				UpdateCountdown();
			}

			//�J�����̍X�V����
			UpdateCamera();

			if (g_bEdit == false && g_bPhoto == false)
			{
				//���C�g�̍X�V����
				UpdateLight();

				//���b�V���t�B�[���h�̍X�V����
				UpdateMeshField();

				//�V��̍X�V����
				UpdateCeiling();

				//�ǂ̍X�V����
				UpdateWall();

				//�r���{�[�h�̍X�V����
				UpdateBillboard();

				//�G�t�F�N�g�̍X�V����
				UpdateEffect();

				//�p�[�e�B�N���̍X�V����
				UpdateParticle();

				//�v���C���[�̍X�V����
				UpdatePlayer();

				//�e�X�V����
				UpdateShadow();

				if (g_gameState != GAMESTATE_READY && g_bStopEnemy == false)
				{
					//�G�l�~�[�̍X�V����
					UpdateEnemy();
				}

				//��̍X�V����
				UpdateRock();

				//�I�u�W�F�N�g�̍X�V����
				UpdateObject();

				//�Ռ��g�̍X�V����
				UpdateShock();

				if (GetMode() != MODE_TUTORIAL)
				{
					if (g_gameState != GAMESTATE_READY)
					{
						//�^�C�}�[�̍X�V����
						UpdateTime();
					}
				}

				//�Q�[�W�̍X�V����
				UpdateGauge();

				//�X�R�A�̍X�V
				UpdateScore();

				//�O�Ղ̍X�V
				UpdateOrbit();

				//�擾�X�R�A�̍X�V
				UpdatePoint();

				//�擾�X�R�A���b�Z�[�W�̍X�V
				UpdatePointLog();
			}
			else
			{
				//�G�f�B�b�g�̍X�V����
				UpdateEdit();
			}
		}

		//�G�f�B�b�g���[�h�̗L��
#ifdef _DEBUG
		if (GetKeyboardTrigger(DIK_F8) == true)
		{
			g_bEdit = (g_bEdit == false) ? true : false;
		}

		if (GetKeyboardTrigger(DIK_BACK) == true)
		{//�G�X�V�؂�ւ�
			g_bStopEnemy = (g_bStopEnemy == false) ? true : false;
		}

		if (GetKeyboardTrigger(DIK_RCONTROL) == true)
		{//�t�H�g���[�h�؂�ւ�
			g_bPhoto = (g_bPhoto == false) ? true : false;
		}
#endif //_DEBUG

		//���s�ݒ菈��
		if (g_nEndCnter == COUNT_END)
		{
			if (GetEnemy()->nLife == 0)
			{//�G���|�ꂽ��
				SetGameEnd(RESULTTYPE_WIN);
				GameEndCamera(RESULTTYPE_WIN);

				//�����̉��Đ�
				PlaySound(SOUND_LABEL_SE_VICTORY);
			}
			if (GetPlayer()->bUse == false)
			{//�v���C���[�����񂾂�
				SetGameEnd(RESULTTYPE_LOSE);
				GameEndCamera(RESULTTYPE_LOSE);
				g_nEndCnter -= 100;
			}
			if (GetTimeUp() == true)
			{//���Ԑ؂�ɂȂ�����
				SetGameEnd(RESULTTYPE_LOSE);
				GameEndCamera(RESULTTYPE_LOSE);
				g_nEndCnter -= 100;
			}
		}

		//���U���g��Ԃֈڍs
		if (g_nEndCnter == 0)
		{//�^�C�}�[���؂ꂽ��
			g_gameState = GAMESTATE_RESULT;

			if (GetResult() == RESULTTYPE_WIN)
			{//���U���g���̃^�C���{�[�i�X�̈ʒu
				SetPointScore(GetTime() * TIMESCORE_MLT, RESULT_POINT_POS, RESULT_POINT_POS, POINTTYPE_STAY, 0.5f, LOGTYPE_TIMEBONUS);
			}

			//���U���g���̍��v�X�R�A�̈ʒu
			SetPointScore(GetScore(), RESULT_SCORE_POS, RESULT_SCORE_POS, POINTTYPE_STAY, 1.0f, LOGTYPE_TOTALSCORE);
		}

		//���U���g�ւ̑J�ڑ҂�
		if (g_nEndCnter > 0 && g_gameState == GAMESTATE_END)
		{
			g_nEndCnter--;
		}
		else
		{
			if (g_bPause == true)
			{
				//�|�[�Y�̍X�V����
				UpdatePause();
			}

			if (GetMode() != MODE_TUTORIAL && g_gameState != GAMESTATE_READY)
			{
				if (GetKeyboardTrigger(DIK_P) == true || GetJoyPadButtonTrigger(KEY_START, 0))
				{//�|�[�Y�؂�ւ�
					g_bPause = (g_bPause == false) ? true : false;
				}
			}
		}
	}

	switch (g_gameState)
	{
	case GAMESTATE_RESULT:
		UpdateResult();

		//�^�C�}�[�̍X�V����
		UpdateTimeResult();

		//�X�R�A�̍X�V
		//UpdateScore();

		//�擾�X�R�A�̍X�V
		UpdatePoint();

		//�擾�X�R�A���b�Z�[�W�̍X�V
		UpdatePointLog();
		break;
	}
}

//========================================
//�Q�[���̕`�揈��
//========================================
void DrawGame(void)
{
	//�J�����̐ݒ菈��
	SetCamera();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���b�V���t�B�[���h�̕`�揈��
	DrawMeshField();

	//�V��̕`�揈��
	DrawCeiling();

	//�ǂ̕`�揈��
	DrawWall();

	//��̕`�揈��
	DrawRock();

	//�I�u�W�F�N�g�̕`�揈��
	DrawObject();

	//�p�[�e�B�N���̕`�揈��
	DrawParticle();

	//�r���{�[�h�̕`�揈��
	DrawBillboard();

	//�Ռ��g�̕`�揈��
	DrawShock();

	if (GetMode() != MODE_TITLE)
	{
		//���f���̕`�揈��
		DrawPlayer();

		//�G�l�~�[�̕`�揈��
		DrawEnemy();
	}

	//�e�`�揈��
	DrawShadow();

	if (g_gameState == GAMESTATE_READY)
	{
		//�J�E���g�_�E���̕`��
		DrawCountdown();
	}

	//�O�Ղ̕`��
	DrawOrbit();

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	if (GetMode() != MODE_TITLE)
	{
		if ((GetMode() == MODE_GAME && g_gameState == GAMESTATE_NORMAL) && GetPhotoMode() == false)
		{
			//�Q�[�W�̕`��
			DrawGauge();

			//�^�C�}�[�̕`�揈��
			DrawTime();

			//�X�R�A�̕`��
			DrawScore();

			//�擾�X�R�A�̕`��
			DrawPoint();

			//�擾�X�R�A���b�Z�[�W�̕`��
			DrawPointLog();
		}
	}

	if (GetMode() == MODE_TUTORIAL)
	{//�`���[�g���A���p�̕`��
		//�Q�[�W�̕`��
		DrawGauge();
	}

	if (g_bPause == true)
	{
		DrawPause();
	}

	//���U���g�̕`��
	if (g_gameState == GAMESTATE_RESULT)
	{
		DrawResult();

		//�^�C�}�[�̕`�揈��
		DrawTime();

		//�擾�X�R�A�̕`��
		DrawPoint();
	
		//�擾�X�R�A���b�Z�[�W�̕`��
		DrawPointLog();
	}

#ifdef _DEBUG
	if (g_bEdit == true)
	{
		//�G�f�B�b�g�̕`�揈��
		DrawEdit();
	}
#endif //_DEBUG
}

//========================================
//�Q�[���̏�Ԑݒ�
//========================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
	g_nCounterGameState = nCounter;
}

//========================================
//�Q�[���̏�Ԃ̎擾
//========================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//========================================
//�|�[�Y�̗L�������ݒ�
//========================================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}

//========================================
//�|�[�Y�̎擾
//========================================
bool GetPause(void)
{
	return g_bPause;
}

//========================================
//�t�H�g���[�h���ǂ����̎擾
//========================================
bool GetPhotoMode(void)
{
	return g_bPhoto;
}

//========================================
//�Q�[���I�����E���s�ݒ�
//========================================
void SetGameEnd(RESULTTYPE result)
{
	//BGM��~
	StopSound();

	//�e��I��
	g_gameState = GAMESTATE_END;
	SetResult(result);

	if (result == RESULTTYPE_WIN)
	{
		AddScore(GetTime() * TIMESCORE_MLT);
		SetRanking(GetScore());
	}

	//���U���g�̏�����
	InitResult();
}

void DebugFOG(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,560,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "START : %.2f\nEND : %.2f", g_fStart, g_fEnd);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
}