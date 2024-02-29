//==========================================
//
//  �`���[�g���A���̏���(tutorial.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_
#include "main.h"

//==========================================
//  �񋓌^��`
//==========================================
typedef enum
{
	TUTORIALSTATE_MOVE = 0, //�ړ�����
	TUTORIALSTATE_CAMERA, //�J����������
	TUTORIALSTATE_REFLECTOR, //���˔���
	TUTORIALSTATE_BUTTON, //�{�^������
	TUTORIALSTATE_JUMP, //�W�����v����
	TUTORIALSTATE_ENEMY, //�G��|��
	TUTORIALSTATE_END, //�`���[�g���A���̏I��
	TUTORIAL_MAX
}TUTORIALSTATE; //�`���[�g���A�����

typedef enum
{
	TUTORIAL_MOVE = 0,
	MOVE_EX,
	TUTORIAL_CAMERA,
	CAMERA_EX,
	TUTORIAL_REFLECTOR,
	REFLECTOR_EX,
	TUTORIAL_BUTTON,
	BUTTON_EX,
	TUTORIAL_JUMP,
	JUMP_EX,
	TUTORIAL_ENEMY,
	ENEMY_EX,
	TUTORIAL_BG,
	TUTORIAL_ENTER,
	TUTORIAL_TEX_MAX
}TUTORIALTEX; //�`���[�g���A���e�N�X�`��

typedef enum
{
	TUTORIALTYPE_BASE = 0,
	TUTORIALTYPE_EX, 
	TUTORIALTYPE_BG,
	TUTORIALTYPE_ENTER,
	TUTORIALTYPE_MAX
}TUTORIALTYPE; //�|���S���̎��

typedef enum
{
	LEFTSIDE = 0,
	RIGHTSIDE,
	UNDERSIDE
}TUTORIALPOS; //�|���S���̈ʒu

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
TUTORIALSTATE *GetTutorialState(void);

#endif
