//===========================
//
//�_�C���N�gX.ui�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _UI_H_
#define _UI_H_

//�G�̎��
typedef enum
{
	UISTATE_TITLE = 0,			//�ʏ���
	UISTATE_GAME,				//�_���[�W���
	UISTATE_RESULT,				//�_���[�W���
	UISTATE_MAX
}UISTATE;

//�G�̎��
typedef enum
{
	UITEXTURE_TITLE = 0,			//�ʏ���
	UITEXTURE_TITLEPRESS,			//�ʏ���
	UITEXTURE_TUTORIAL,				//�ʏ���
	UITEXTURE_TUTORIALDOT,			//�ʏ���
	UITEXTURE_GAMEDOT,				//�_���[�W���
	UITEXTURE_GAMELIFE,				//�_���[�W���
	UITEXTURE_GAMELIFEBAR,				//�_���[�W���
	UITEXTURE_RESULTCLEAR,			//�_���[�W���
	UITEXTURE_RESULTOVER,			//�_���[�W���
	UITEXTURE_MAX
}UITEXTURE;

//�v���C���[�̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	float fWidth;
	float fHeight;
	int nType;
}UI;

//�v���g�^�C�v�錾
void InitUi(void);
void UninitUi(void);
void UpdateUi(void);
void DrawUi(void);
void AddLife(float nLife);

#endif // !_UI_H_