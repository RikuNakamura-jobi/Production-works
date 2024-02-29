//==========================================================
//
//�t�F�[�h����
//Author ����ɐ�
//
//==========================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"	//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include

//�t�F�[�h��ԗ񋓌^
typedef enum
{
	FADE_NONE = 0,	//�������Ă��Ȃ����
	FADE_IN,		//�t�F�[�h�C�����
	FADE_OUT,		//�t�F�[�h�A�E�g���
	FADE_MAX
}FADE;

typedef enum
{
	FADETYPE_NORMAL = 0,	//�ʏ�t�F�[�h
	FADETYPE_ROTATE,		//��]
	FADETYPE_MAX
}FADETYPE;

//�t�F�[�h�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXCOLOR col;			//�t�F�[�h�J���[
	int nType;				//���
}Fade;

//�v���g�^�C�v�錾
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void SetFade(MODE modeNext);
FADE GetFade(void);
Fade *GetFadeInfo(void);

#endif
