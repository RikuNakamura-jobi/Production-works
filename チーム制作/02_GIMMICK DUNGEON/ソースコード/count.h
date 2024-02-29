//==========================================
//
//  �J�E���g�̏���(count.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _COUNT_H_
#define _COUNT_H_
#include "main.h"

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitCount(void);
void UninitCount(void);
void UpdateCount(void);
void DrawCount(void);
void SetCount(D3DXVECTOR3 pos, float size, int max);
void AddCount(int add);
bool GetCount(void);

#endif
