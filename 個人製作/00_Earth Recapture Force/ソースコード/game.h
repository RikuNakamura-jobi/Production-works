//===========================
//
//�_�C���N�gX.game�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _GAME_H_
#define _GAME_H_

//�Q�[���̏��
typedef enum
{
	GAMESTATE_NONE = 0,				//�������ĂȂ����
	GAMESTATE_NORMAL,				//�ʏ���
	GAMESTATE_END,					//�I�����
	GAMESTATE_MAX
}GAMESTATE;

//wave�̏��
typedef enum
{
	WAVESTATE_NONE = 0,				//�������ĂȂ����
	WAVESTATE_NORMAL,				//wave1���
	WAVESTATE_BOSS,					//boss���
	WAVESTATE_END,					//boss���
	WAVESTATE_MAX
}WAVESTATE;

//�X�e�[�W�̏��
typedef enum
{
	STAGESTATE_NONE = 0,			//�������ĂȂ����
	STAGESTATE_1,					//STAGE1���
	STAGESTATE_2,					//STAGE2���
	STAGESTATE_3,					//STAGE3���
	STAGESTATE_EX,					//STAGE3���
	STAGESTATE_MAX
}STAGESTATE;

//�v���g�^�C�v�錾
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state, int nCounter);
GAMESTATE GetGameState(void);
void SetWaveState(WAVESTATE state, int nCounter);
WAVESTATE GetWaveState(void);
void SetStageState(STAGESTATE state, int nCounter);
STAGESTATE GetStageState(void);
void SetEnablePause(bool pPause);
bool GetHardMode(void);
bool SetNoContinue(bool bNoContinue);

#endif // !_GAME_H_