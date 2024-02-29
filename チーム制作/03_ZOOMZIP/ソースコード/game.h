//===================================================
//
//�Q�[����ʏ��� [game.h]
//Author Ibuki Okusda
//
//===================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//===================================================
//�}�N����`
//===================================================
#define GAMESTOP_COUNT	(100)		//�Q�[���I���܂ł̃J�E���g

//===================================================
//�񋓌^���`
//===================================================
//�Q�[���̏��
typedef enum
{
	GAMESTATE_NONE = 0,			//�����Ȃ����
	GAMESTATE_NORMAL,			//�ʏ���
	GAMESTATE_END,				//�I�����
	GAMESTATE_MAX
}GAMESTATE;

//===================================================
//�v���g�^�C�v�錾
//===================================================
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state, int nCounter);
GAMESTATE GetGameState(void);
void SetEnablePause(bool bPause);

#endif
