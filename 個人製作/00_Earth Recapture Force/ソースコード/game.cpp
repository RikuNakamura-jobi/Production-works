//===========================
//
//�_�C���N�gX.game�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "game.h"
#include "player.h"
#include "bg.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "bgstage.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "fade.h"
#include "life.h"
#include "remain.h"
#include "effect.h"
#include "particle.h"
#include "warning.h"
#include "boss.h"
#include "pause.h"
#include "stagestart.h"

//�O���[�o���ϐ��錾
bool g_bPause = false;												//�|�[�Y��Ԃ�ON/OFF
GAMESTATE g_gameState = GAMESTATE_NONE;								//�Q�[���̏��
WAVESTATE g_waveState = WAVESTATE_NONE;								//wave�̏��
STAGESTATE g_stageState = STAGESTATE_NONE;							//�X�e�[�W�̏��
int g_nCounterGameState = 0;										//��ԊǗ��J�E���^�[
int g_nCounterWaveState = 0;										//��ԊǗ��J�E���^�[
int g_nCounterStageState = 0;										//��ԊǗ��J�E���^�[
int g_nCounterWave;													//wave���J�E���^�[
int g_nCounterWaveTime;												//wave�J�n�^�C�}�[
int g_nCounterSpawnTime = 30;										//�G�o���^�C�}�[
int g_nCounterWarningTime = 300;									//�x���^�C�}�[
int g_nCounterStagestartTime = 240;									//�X�e�[�W�J�n�^�C�}�[
int g_nCounterHard = 0;												//�n�[�h���[�h�J�E���^�[
int g_nTimeHard = 0;												//�n�[�h���[�h�^�C�}�[
int g_nCounterCommand;												//�R�}���h���͐�
int g_nCounterTimeCommand;											//�R�}���h��������
bool g_bEnemySpawn;													//�S�Ŋm�F
bool g_bHardmode;													//�n�[�h���[�h����
bool g_bCommand;													//�R�}���h����
bool g_bNoContunue = false;											//�R���e�B�j���[����

//===========================
//�Q�[����ʂ̏���������
//===========================
void InitGame(void)
{
	Enemy aENEMY;

	//�e��I�u�W�F�N�g�̏���������
	InitBullet();
	InitExplosion();
	InitPlayer();
	InitBg();
	InitBgStage();
	InitScore();
	SetScore(0);
	InitLife();
	SetLife(5);
	InitRemain();
	SetRemain(3);
	InitEffect();
	InitParticle();
	InitWarning();
	InitBoss();
	InitPause();
	InitStagestart();

	//�G�̏���������
	srand((unsigned int)time(0));

	InitEnemy();

	g_gameState = GAMESTATE_NORMAL;
	g_nCounterGameState = 0;
	g_waveState = WAVESTATE_NORMAL;
	g_nCounterWaveState = 0;
	g_nCounterStageState = 120;
	g_nCounterStagestartTime = 240;
	g_nCounterWave = 0;
	g_nCounterWaveTime = 60;
	g_nCounterWarningTime = 300;
	g_nCounterHard = 0;
	g_nTimeHard = 0;
	g_nCounterCommand = 0;
	g_nCounterTimeCommand = 0;
	g_bPause = false;
	g_bEnemySpawn = false;
	g_bHardmode = false;
	g_bCommand = false;

	switch (g_stageState)
	{
	case STAGESTATE_NONE:
		g_stageState = STAGESTATE_1;
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_BGM003);
		break;
	case STAGESTATE_1:
		g_stageState = STAGESTATE_1;
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_BGM003);
		break;
	case STAGESTATE_2:
		g_stageState = STAGESTATE_2;
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_BGM004);
		break;
	case STAGESTATE_3:
		g_stageState = STAGESTATE_3;
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_BGM013);
		break;
	case STAGESTATE_EX:
		g_stageState = STAGESTATE_EX;
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_BGM006);
		break;
	}
}

//===========================
//�Q�[����ʂ̏I������
//===========================
void UninitGame(void)
{
	//�T�E���h�̒�~
	StopSound();

	//�e��I�u�W�F�N�g�̏I������
	UninitPlayer();
	UninitBg();
	UninitBgStage();
	UninitBullet();
	UninitExplosion();
	UninitEnemy();
	UninitScore();
	UninitLife();
	UninitRemain();
	UninitEffect();
	UninitParticle();
	UninitWarning();
	UninitBoss();
	UninitPause();
	UninitStagestart();
}

//===========================
//�Q�[����ʂ̍X�V����
//===========================
void UpdateGame(void)
{
	Enemy aENEMY;
	Player *pPlayer = GetPlayer();
	FADE fade = GetFade();
	D3DXVECTOR3 posBullet;
	D3DXVECTOR3 moveBullet;
	D3DXVECTOR3 rotBullet;
	int nNumEnemy = GetNumEnemy();

	if (g_bPause == false)
	{
		//�e��I�u�W�F�N�g�̍X�V����
		UpdateBullet();
		UpdateExplosion();
		UpdatePlayer();
		UpdateBg();
		UpdateBgStage();
		UpdateEnemy();
		UpdateScore();
		UpdateLife();
		UpdateRemain();
		UpdateEffect();
		UpdateParticle();
		UpdateWarning();
		UpdateStagestart();
		UpdateBoss();
	}
	else if (g_bPause == true)
	{
		UpdatePause();
		UpdateStagestart();

		//�n�[�h���[�h�R�}���h
		if (GetKeyboardTrigger(DIK_W) == true && g_nCounterCommand == 0)
		{
			g_bCommand = true;
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_UP) == true && g_nCounterCommand == 0)
		{
			g_bCommand = true;
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_W) == true && g_nCounterCommand == 1)
		{
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_UP) == true && g_nCounterCommand == 1)
		{
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_S) == true && g_nCounterCommand == 2)
		{
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_DOWN) == true && g_nCounterCommand == 2)
		{
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_S) == true && g_nCounterCommand == 3)
		{
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_DOWN) == true && g_nCounterCommand == 3)
		{
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_A) == true && g_nCounterCommand == 4)
		{
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_LEFT) == true && g_nCounterCommand == 4)
		{
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_D) == true && g_nCounterCommand == 5)
		{
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_RIGHT) == true && g_nCounterCommand == 5)
		{
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_A) == true && g_nCounterCommand == 6)
		{
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_LEFT) == true && g_nCounterCommand == 6)
		{
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_D) == true && g_nCounterCommand == 7)
		{
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_RIGHT) == true && g_nCounterCommand == 7)
		{
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_A) == true && g_nCounterCommand == 8)
		{
			g_nCounterCommand++;
		}
		if (GetKeyboardTrigger(DIK_B) == true && g_nCounterCommand == 9)
		{
			g_nCounterCommand++;
		}

		//�n�[�h���[�h���Ԑ���
		if (g_bCommand == true)
		{
			g_nCounterTimeCommand++;

			if (g_nCounterTimeCommand >= 180)
			{
				g_nCounterTimeCommand = 0;
				g_nCounterCommand = 0;
				g_bCommand = false;
			}
		}
	}

	//�|�[�Y
	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetPadButtonTrigger(7) == true))
	{
		InitPause();
		g_bPause = g_bPause ? false : true;
	}

	//�n�[�h���[�h�؊�
	if (g_nCounterCommand == 10)
	{
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_DECISION001);

		g_nCounterCommand = 0;

		g_bHardmode = g_bHardmode ? false : true;
	}

	if (nNumEnemy <= 0)
	{
		g_bEnemySpawn = true;
	}

	//�G�S�Ŏ��̏���
	if (nNumEnemy <= 0)
	{
		g_nCounterWaveTime--;

		if (g_nCounterWaveTime <= 0)
		{
			if (g_waveState == WAVESTATE_BOSS)
			{
				g_nCounterStageState--;

				if (g_nCounterStageState == 0)
				{//�X�e�[�W�J��
					if (g_stageState == STAGESTATE_1)
					{
						g_waveState = WAVESTATE_END;
						SetFade(MODE_GAME);
						InitBullet();
						InitBoss();
						InitEnemy();
						InitWarning();
						InitStagestart();
						g_nCounterWave = 0;
						g_nCounterWaveTime = 60;
						g_nCounterWarningTime = 300;
						g_nCounterStagestartTime = 240;
					}
					else if (g_stageState == STAGESTATE_2)
					{
						g_waveState = WAVESTATE_END;
						SetFade(MODE_GAME);
						InitBullet();
						InitBoss();
						InitEnemy();
						InitWarning();
						InitStagestart();
						g_nCounterWave = 0;
						g_nCounterWaveTime = 60;
						g_nCounterWarningTime = 300;
						g_nCounterStagestartTime = 240;
					}
					else if (g_stageState == STAGESTATE_3)
					{
						if (pPlayer->nRemaining >= 3 && g_bNoContunue == false)
						{
							g_waveState = WAVESTATE_END;
							SetFade(MODE_GAME);
							InitBullet();
							InitBoss();
							InitEnemy();
							InitWarning();
							InitStagestart();
							g_nCounterWave = 0;
							g_nCounterWaveTime = 60;
							g_nCounterWarningTime = 300;
							g_nCounterStagestartTime = 240;
						}
						else
						{
							g_nCounterWave = 0;
							InitBullet();
							InitBoss();
							InitEnemy();
							g_waveState = WAVESTATE_END;
							SetGameState(GAMESTATE_END, 60);

							if (g_bHardmode == true)
							{
								AddScore(g_nTimeHard / 10);
							}
							else
							{
								AddScore(3000000);
							}
						}
					}
					else if (g_stageState == STAGESTATE_EX)
					{
						g_nCounterWave = 0;
						InitBullet();
						InitBoss();
						InitEnemy();
						g_waveState = WAVESTATE_END;
						SetGameState(GAMESTATE_END, 60);

						if (g_bHardmode == true)
						{
							AddScore(g_nTimeHard / 10);
						}
						else
						{
							AddScore(3000000);
						}
					}
				}
			}
			else if (g_waveState == WAVESTATE_NORMAL)
			{//�G�o��
				if (g_nCounterWave == 5)
				{//�{�X�o��
					if (g_nCounterWarningTime == 300)
					{
						//�T�E���h�̒�~
						StopSound();

						PlaySound(SOUND_LABEL_SE_WARNING001);

						SetWarning();
						g_nCounterWarningTime--;
					}
					else if (g_nCounterWarningTime == 120)
					{
						SetWarning();
						g_nCounterWarningTime--;

						//�{�X�\���U��
						if (g_stageState == STAGESTATE_1)
						{
							PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

							for (int nCnt = 0; nCnt < 4; nCnt++)
							{
								posBullet.x = pPlayer->pos.x;
								posBullet.y = pPlayer->pos.y;

								rotBullet.z = (float)(rand() % 629 - 314) / 100.0f;

								SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 3.0f, 2000.0f, 100, 0);
							}
						}
						else if (g_stageState == STAGESTATE_2)
						{
							PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

							for (int nCnt = 0; nCnt < 5; nCnt++)
							{
								posBullet.x = pPlayer->pos.x;
								posBullet.y = 0.0f;

								rotBullet.z = (float)((300 + (7 * nCnt)) - 314) / 100.0f;

								SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 30.0f, 2000.0f, 120, 1);
							}
						}
						else if (g_stageState == STAGESTATE_3)
						{
							posBullet.x = pPlayer->pos.x;
							posBullet.y = 0.0f;

							rotBullet.z = 0.0f;

							PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

							SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 0.0f, 2000.0f, 140, 2);
						}
						else if (g_stageState == STAGESTATE_EX)
						{
							PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

							posBullet.x = pPlayer->pos.x;
							posBullet.y = pPlayer->pos.y;

							rotBullet.z = (float)(rand() % 629 - 314) / 100.0f;

							SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 3.0f, 2000.0f, 100, 0);

							for (int nCnt = 0; nCnt < 5; nCnt++)
							{
								posBullet.x = pPlayer->pos.x;
								posBullet.y = 0.0f;

								rotBullet.z = (float)((300 + (7 * nCnt)) - 314) / 100.0f;

								SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 30.0f, 2000.0f, 120, 1);
							}

							posBullet.x = pPlayer->pos.x;
							posBullet.y = 0.0f;

							rotBullet.z = 0.0f;

							SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 0.0f, 2000.0f, 140, 2);
						}
					}
					else if (g_nCounterWarningTime > 0)
					{
						g_nCounterWarningTime--;
					}
					else if (g_nCounterWarningTime == 0)
					{
						if (g_stageState == STAGESTATE_1)
						{
							for (int nCnt = 0; nCnt < 1; nCnt++)
							{
								aENEMY.pos.x = 620.0f;
								aENEMY.pos.y = -100.0f;
								aENEMY.pos.z = 0.0f;
								aENEMY.nType = 3;
								aENEMY.nMoveType = 3;

								SetEnemy(aENEMY.pos, aENEMY.nType, aENEMY.nMoveType, 0);
							}

							PlaySound(SOUND_LABEL_BGM011);
						}
						else if (g_stageState == STAGESTATE_2)
						{
							for (int nCnt = 0; nCnt < 1; nCnt++)
							{
								aENEMY.pos.x = 620.0f;
								aENEMY.pos.y = -100.0f;
								aENEMY.pos.z = 0.0f;
								aENEMY.nType = 7;
								aENEMY.nMoveType = 3;

								SetEnemy(aENEMY.pos, aENEMY.nType, aENEMY.nMoveType, 0);
							}

							PlaySound(SOUND_LABEL_BGM008);
						}
						else if (g_stageState == STAGESTATE_3)
						{
							for (int nCnt = 0; nCnt < 1; nCnt++)
							{
								aENEMY.pos.x = 620.0f;
								aENEMY.pos.y = -100.0f;
								aENEMY.pos.z = 0.0f;
								aENEMY.nType = 11;
								aENEMY.nMoveType = 3;

								SetEnemy(aENEMY.pos, aENEMY.nType, aENEMY.nMoveType, 0);
							}

							PlaySound(SOUND_LABEL_BGM006);
						}
						else if (g_stageState == STAGESTATE_EX)
						{
							for (int nCnt = 0; nCnt < 1; nCnt++)
							{
								aENEMY.pos.x = 620.0f;
								aENEMY.pos.y = -100.0f;
								aENEMY.pos.z = 0.0f;
								aENEMY.nType = 12;
								aENEMY.nMoveType = 7;

								SetEnemy(aENEMY.pos, aENEMY.nType, aENEMY.nMoveType, 0);
							}

							PlaySound(SOUND_LABEL_BGM005);
						}

						g_waveState = WAVESTATE_BOSS;
					}
				}
				else
				{//�ʏ�G�o��
					if (g_nCounterStagestartTime == 240)
					{
						PlaySound(SOUND_LABEL_SE_STAGESTART001);

						SetStagestart();
						g_nCounterStagestartTime--;
					}
					else if (g_nCounterStagestartTime > 0)
					{
						g_nCounterStagestartTime--;
					}
					else if (g_nCounterStagestartTime == 0)
					{
						SetStagestart();

						g_nCounterWaveTime = 60;
						aENEMY.nMoveType = 0;

						if (g_stageState == STAGESTATE_1)
						{
							if (g_nCounterWave == 0)
							{
								for (int nCnt = 0; nCnt < 6; nCnt++)
								{
									aENEMY.pos.x = 450.0f + ((nCnt % 2) * 350.0f);
									aENEMY.pos.y = -350.0f - ((nCnt / 2) * 100.0f);
									aENEMY.pos.z = 0.0f;
									aENEMY.nType = 1;


									SetEnemy(aENEMY.pos, aENEMY.nType, aENEMY.nMoveType, (nCnt / 2) * 20);
								}
							}
							else if (g_nCounterWave == 1)
							{
								for (int nCnt = 0; nCnt < 6; nCnt++)
								{
									aENEMY.pos.x = 380.0f + (nCnt * 100.0f);
									aENEMY.pos.y = -350.0f;
									aENEMY.pos.z = 0.0f;
									aENEMY.nType = 0;

									SetEnemy(aENEMY.pos, aENEMY.nType, aENEMY.nMoveType, (nCnt / 2) * 20);
								}
							}
							else if (g_nCounterWave == 2)
							{
								for (int nCnt = 0; nCnt < 6; nCnt++)
								{
									aENEMY.pos.x = 450.0f + ((nCnt % 2) * 350.0f);
									aENEMY.pos.y = -350.0f - ((nCnt / 2) * 100.0f);
									aENEMY.pos.z = 0.0f;

									if (nCnt == 6)
									{
										aENEMY.nType = 0;
									}
									else if (nCnt == 0 || nCnt == 3 || nCnt == 4)
									{
										aENEMY.nType = 1;
									}
									else if (nCnt == 1 || nCnt == 2 || nCnt == 5)
									{
										aENEMY.nType = 2;
									}

									SetEnemy(aENEMY.pos, aENEMY.nType, aENEMY.nMoveType, (nCnt / 2) * 20);
								}
							}
							else if (g_nCounterWave == 3)
							{
								for (int nCnt = 0; nCnt < 6; nCnt++)
								{
									if (nCnt % 2 == 0 || nCnt % 2 == 2)
									{
										aENEMY.pos.x = 200.0f + ((nCnt % 2) * 880.0f) - ((nCnt / 2) * 50.0f);
										aENEMY.pos.y = 200.0f - ((nCnt / 2) * 50.0f);
										aENEMY.pos.z = 0.0f;
									}
									else if (nCnt % 2 == 1)
									{
										aENEMY.pos.x = 200.0f + ((nCnt % 2) * 880.0f) + ((nCnt / 2) * 50.0f);
										aENEMY.pos.y = 200.0f - ((nCnt / 2) * 50.0f);
										aENEMY.pos.z = 0.0f;
									}

									if (nCnt == 0)
									{
										aENEMY.nType = 0;
										aENEMY.nMoveType = 5;
									}
									else if (nCnt == 1)
									{
										aENEMY.nType = 0;
										aENEMY.nMoveType = 6;
									}
									else if (nCnt == 2)
									{
										aENEMY.nType = 1;
										aENEMY.nMoveType = 5;
									}
									else if (nCnt == 3)
									{
										aENEMY.nType = 1;
										aENEMY.nMoveType = 6;
									}
									else if (nCnt == 4)
									{
										aENEMY.nType = 2;
										aENEMY.nMoveType = 5;
									}
									else if (nCnt == 5)
									{
										aENEMY.nType = 2;
										aENEMY.nMoveType = 6;
									}

									SetEnemy(aENEMY.pos, aENEMY.nType, aENEMY.nMoveType, (nCnt / 2) * 20);
								}
							}
							else if (g_nCounterWave == 4)
							{
								for (int nCnt = 0; nCnt < 6; nCnt++)
								{
									aENEMY.pos.x = 400.0f + ((nCnt % 2) * 450.0f);
									aENEMY.pos.y = -350.0f - ((nCnt / 2) * 100.0f);
									aENEMY.pos.z = 0.0f;

									if (nCnt == 0)
									{
										aENEMY.nType = 2;
										aENEMY.nMoveType = 1;
									}
									else if (nCnt == 1)
									{
										aENEMY.nType = 2;
										aENEMY.nMoveType = 2;
									}
									else if (nCnt == 2)
									{
										aENEMY.nType = 1;
										aENEMY.nMoveType = 1;
									}
									else if (nCnt == 3)
									{
										aENEMY.nType = 1;
										aENEMY.nMoveType = 2;
									}
									else if (nCnt == 4)
									{
										aENEMY.nType = 0;
										aENEMY.nMoveType = 1;
									}
									else if (nCnt == 5)
									{
										aENEMY.nType = 0;
										aENEMY.nMoveType = 2;
									}

									SetEnemy(aENEMY.pos, aENEMY.nType, aENEMY.nMoveType, (nCnt / 2) * 20);
								}
							}
						}
						else if (g_stageState == STAGESTATE_2)
						{

							if (g_nCounterWave == 0)
							{
								for (int nCnt = 0; nCnt < 6; nCnt++)
								{
									aENEMY.pos.x = 450.0f + ((nCnt % 2) * 350.0f);
									aENEMY.pos.y = -350.0f - 100.0f * (nCnt / 2);
									aENEMY.pos.z = 0.0f;
									aENEMY.nType = 4;

									SetEnemy(aENEMY.pos, aENEMY.nType, aENEMY.nMoveType, (nCnt / 2) * 20);
								}
							}
							else if (g_nCounterWave == 1)
							{
								for (int nCnt = 0; nCnt < 6; nCnt++)
								{
									aENEMY.pos.x = 580.0f + ((nCnt % 2) * 100.0f);
									aENEMY.pos.y = -350.0f - 100.0f * (nCnt / 2);
									aENEMY.pos.z = 0.0f;

									if (nCnt == 6)
									{
										aENEMY.nType = 4;
									}
									else if (nCnt == 0 || nCnt == 3 || nCnt == 4)
									{
										aENEMY.nType = 4;
									}
									else if (nCnt == 1 || nCnt == 2 || nCnt == 5)
									{
										aENEMY.nType = 6;
									}

									SetEnemy(aENEMY.pos, aENEMY.nType, aENEMY.nMoveType, (nCnt / 2) * 20);
								}
							}
							else if (g_nCounterWave == 2)
							{
								for (int nCnt = 0; nCnt < 12; nCnt++)
								{
									aENEMY.pos.x = 380.0f + ((nCnt % 6) * 100.0f);
									aENEMY.pos.y = -350.0f - 200.0f * (nCnt / 6);
									aENEMY.pos.z = 0.0f;

									if (nCnt >= 0 && nCnt < 6)
									{
										aENEMY.nType = 4;
									}
									else if (nCnt >= 6 && nCnt < 12)
									{
										aENEMY.nType = 6;
									}

									SetEnemy(aENEMY.pos, aENEMY.nType, aENEMY.nMoveType, (nCnt / 6) * 20);
								}
							}
							else if (g_nCounterWave == 3)
							{
								for (int nCnt = 0; nCnt < 6; nCnt++)
								{
									if (nCnt % 2 == 0 || nCnt % 2 == 2)
									{
										aENEMY.pos.x = 200.0f + ((nCnt % 2) * 880.0f) - ((nCnt / 2) * 50.0f);
										aENEMY.pos.y = 600.0f - ((nCnt / 2) * 50.0f);
										aENEMY.pos.z = 0.0f;
										aENEMY.nMoveType = 5;
									}
									else if (nCnt % 2 == 1)
									{
										aENEMY.pos.x = 200.0f + ((nCnt % 2) * 880.0f) + ((nCnt / 2) * 50.0f);
										aENEMY.pos.y = 600.0f - ((nCnt / 2) * 50.0f);
										aENEMY.pos.z = 0.0f;
										aENEMY.nMoveType = 6;
									}

									if (nCnt == 2 || nCnt == 3 || nCnt == 5)
									{
										aENEMY.nType = 4;
									}
									else if (nCnt == 0 || nCnt == 1 || nCnt == 4)
									{
										aENEMY.nType = 6;
									}

									SetEnemy(aENEMY.pos, aENEMY.nType, aENEMY.nMoveType, (nCnt / 2) * 20);
								}
							}
							else if (g_nCounterWave == 4)
							{
								for (int nCnt = 0; nCnt < 6; nCnt++)
								{
									aENEMY.pos.x = 450.0f + ((nCnt % 2) * 350.0f);
									aENEMY.pos.y = -350.0f - 100.0f * (nCnt / 2);
									aENEMY.pos.z = 0.0f;

									if (nCnt == 2 || nCnt == 3 || nCnt == 5)
									{
										aENEMY.nType = 4;
									}
									else if (nCnt == 0 || nCnt == 1 || nCnt == 4)
									{
										aENEMY.nType = 6;
									}

									SetEnemy(aENEMY.pos, aENEMY.nType, aENEMY.nMoveType, (nCnt / 2) * 20);
								}
							}
						}
						else if (g_stageState == STAGESTATE_3)
						{
							for (int nCnt = 0; nCnt < 72; nCnt++)
							{
								aENEMY.pos.x = 380.0f + ((rand() % 12) * 50.0f);
								aENEMY.pos.y = -350.0f - 200.0f * (nCnt / 4);
								aENEMY.pos.z = 0.0f;
								aENEMY.nType = 8;
								aENEMY.nMoveType = 4;

								SetEnemy(aENEMY.pos, aENEMY.nType, aENEMY.nMoveType, (nCnt / 4) * 20);
							}

							g_nCounterWave += 4;
						}
						else if (g_stageState == STAGESTATE_EX)
						{

						}

						g_nCounterWave++;
					}
				}
			}
			g_nCounterSpawnTime = 30;
		}
	}

	if (g_bHardmode == true && g_bPause == false)
	{//�n�[�h���[�h���菈��
		g_nCounterHard++;
		AddScore(573);

		if (g_nCounterHard % 90 == 0)
		{
			if (g_stageState == STAGESTATE_1)
			{
				PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

				posBullet.x = pPlayer->pos.x;
				posBullet.y = pPlayer->pos.y;

				rotBullet.z = (float)(rand() % 629 - 314) / 100.0f;

				SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 3.0f, 2000.0f, 100, 0);
			}
			else if (g_stageState == STAGESTATE_2)
			{
				PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

				for (int nCnt = 0; nCnt < 5; nCnt++)
				{
					posBullet.x = pPlayer->pos.x;
					posBullet.y = 0.0f;

					rotBullet.z = (float)((300 + (7 * nCnt)) - 314) / 100.0f;

					SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 30.0f, 2000.0f, 120, 1);
				}
			}
			else if (g_stageState == STAGESTATE_3)
			{
				posBullet.x = pPlayer->pos.x;
				posBullet.y = 0.0f;

				rotBullet.z = 0.0f;

				PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

				SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 0.0f, 2000.0f, 140, 2);
			}
			else if (g_stageState == STAGESTATE_EX)
			{
				PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

				posBullet.x = pPlayer->pos.x;
				posBullet.y = pPlayer->pos.y;

				rotBullet.z = (float)(rand() % 629 - 314) / 100.0f;

				SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 3.0f, 2000.0f, 100, 0);

				for (int nCnt = 0; nCnt < 5; nCnt++)
				{
					posBullet.x = pPlayer->pos.x;
					posBullet.y = 0.0f;

					rotBullet.z = (float)((300 + (7 * nCnt)) - 314) / 100.0f;

					SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 30.0f, 2000.0f, 120, 1);
				}

				posBullet.x = pPlayer->pos.x;
				posBullet.y = 0.0f;

				rotBullet.z = 0.0f;

				SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 0.0f, 2000.0f, 140, 2);
			}
		}
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:
		break;
	case GAMESTATE_END:
		g_nCounterGameState--;
		if (g_nCounterGameState <= 0 && fade == FADE_NONE)
		{
			SetFade(MODE_RESULT);
		}
		break;
	}
}

//===========================
//�Q�[����ʂ̕`�揈��
//===========================
void DrawGame(void)
{
	//�e��I�u�W�F�N�g�̕`�揈��
	DrawBgStage();
	DrawEffect();
	DrawParticle();
	DrawBoss();
	DrawBullet();
	DrawEnemy();
	DrawPlayer();
	DrawExplosion();
	DrawBg();
	DrawWarning();
	DrawStagestart();
	DrawScore();
	DrawLife();
	DrawRemain();

	if (g_bPause == true)
	{
		DrawPause();
	}
}

//===========================
//�Q�[����ʏ�Ԏ擾����
//===========================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
	g_nCounterGameState = nCounter;
}

//===========================
//�Q�[����ʏ�Ԓ񋟏���
//===========================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//===========================
//wave��Ԏ擾����
//===========================
void SetWaveState(WAVESTATE state, int nCounter)
{
	g_waveState = state;
	g_nCounterWaveState = nCounter;
}

//===========================
//wave��ʏ�Ԓ񋟏���
//===========================
WAVESTATE GetWaveState(void)
{
	return g_waveState;
}

//===========================
//wave��Ԏ擾����
//===========================
void SetStageState(STAGESTATE state, int nCounter)
{
	g_stageState = state;
	g_nCounterStageState = nCounter;
}

//===========================
//wave��ʏ�Ԓ񋟏���
//===========================
STAGESTATE GetStageState(void)
{
	return g_stageState;
}

//===========================
//�|�[�Y��ԕύX����
//===========================
void SetEnablePause(bool pPause)
{
	g_bPause = pPause;
}

//===========================
//�|�[�Y��ԕύX����
//===========================
bool GetHardMode(void)
{
	return g_bHardmode;
}

//===========================
//�R���e�B�j���[���菈��
//===========================
bool SetNoContinue(bool bNoContinue)
{
	return g_bNoContunue = bNoContinue;
}