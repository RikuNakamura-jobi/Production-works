//===========================
//
//�_�C���N�gX.tutorial�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//�Q�[���̏��
typedef enum
{
	TUTORIALSTATE_NONE = 0,				//�������ĂȂ����
	TUTORIALSTATE_NORMAL,				//�ʏ���
	TUTORIALSTATE_END,					//�I�����
	TUTORIALSTATE_MAX
}TUTORIALSTATE;

//�v���g�^�C�v�錾
void InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
void SetTUTORIALSTATE(TUTORIALSTATE state, int nCounter);
TUTORIALSTATE GetTUTORIALSTATE(void);

#endif // !_TUTORIAL_H_