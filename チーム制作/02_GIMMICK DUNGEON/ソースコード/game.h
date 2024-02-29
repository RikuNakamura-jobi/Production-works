//========================================
//
//2D�A�N�V�����Q�[��(�Q�[������)[game.h]
//Author:���v�ԗD��
//
//========================================
#ifndef _GAME_H_							//���̃}�N����`������Ă��Ȃ�������
#define _GAME_H_							//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�Q�[���̏��
typedef enum
{
	GAMESTATE_NONE = 0,						//�������Ă��Ȃ����
	GAMESTATE_READY,					//�J�E���g�_�E�����
	GAMESTATE_NORMAL,						//�ʏ���
	GAMESTATE_END,							//�I�����
	GAMESTATE_RESULT,						//���U���g���
	GAMESTATE_MAX
}GAMESTATE;

//=============================
//�v���g�^�C�v�錾
//=============================
//��{����
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

//�ݒ菈��
void SetGameState(GAMESTATE state, int nCounter);
void SetEnablePause(bool bPause);

//���擾����
bool GetPause(void);
GAMESTATE GetGameState(void);
bool GetPhotoMode(void);

#endif
