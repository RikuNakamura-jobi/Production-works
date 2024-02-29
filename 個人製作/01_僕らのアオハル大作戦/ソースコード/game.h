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

//�v���g�^�C�v�錾
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state, int nCounter);
void SetEnablePause(bool pPause);

#endif // !_GAME_H_