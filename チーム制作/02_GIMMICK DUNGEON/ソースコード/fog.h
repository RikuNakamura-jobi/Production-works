//==========================================
//
//  �����̐���(fog.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _FOG_H_
#define _FOG_H_
#include "main.h"

//==========================================
//  �}�N����`
//==========================================
#define FOG_START_GAME (1000.0f) //���̔����ʒu
#define FOG_END_GAME (5000.0f) //���̏I���ʒu

#define FOG_START_TITLE (400.0f) //���̔����ʒu
#define FOG_END_TITLE (800.0f) //���̏I���ʒu

#define FOG_COLOR_SAND (D3DXCOLOR(0.65f, 0.5f, 0.0f, 1.0f)) //���̖�
#define FOG_COLOR_BLACK (D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)) //������

//==========================================
//  ���̍\���̒�`
//==========================================
typedef struct
{
	bool bUse; //�g�p���
	float fStart; //�����ʒu
	float fEnd; //�I���ʒu
	D3DXCOLOR col; //�F
}FOGDATA;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitFog(void);
void UninitFog(void);
void UpdateFog(void);
void SetFog(void);
void EnableFog(bool bUse);
FOGDATA *GetFog(void);

#endif
