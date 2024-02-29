//
//
//���U���g[result.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _RESULT_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _RESULT_H_      //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"    //�쐬����main.h���C���N���[�h����

//=======================================
//���U���g���
//=======================================
typedef enum
{
	RESULTSTATE_NONE = 0,	//�����Ȃ����
	RESULTSTATE_CLEAR,		//�Q�[���N���A
	RESULTSTATE_GAMEOVER,	//�Q�[���I�[�o�[
	RESULTSTATE_MAX
}RESULTSTATE;

//=======================================
//���U���g�\���̂̒�`
//=======================================
typedef struct
{
	RESULTSTATE State;
}RESULT;

//=======================================
//�v���g�^�C�v�錾
//=======================================
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetResultMode(RESULTSTATE Result);
void ResultJudge(void);
RESULTSTATE *GetResultState(void);

#endif