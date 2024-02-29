//==========================================
//
//  �Q�[�����ݒ菈��(option.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _OPTION_H_
#define _OPTION_H_
#include "main.h"

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitOption(void);
void UninitOption(void);
void UpdateOption(void);
void DrawOption(void);
void SetMouseSensCorrection(void);
D3DXVECTOR3 GetMouseSensCorrection(void);

#endif
