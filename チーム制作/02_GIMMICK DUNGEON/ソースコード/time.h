//==================================================================================================
//
//Time.h
//Author:髙山桃也
//
//==================================================================================================

#ifndef _TIME_H_
#define _TIME_H_

#include"main.h"

//プロトタイプ宣言
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void UpdateTimeResult(void);
void DrawTime(void);
void SetTime(int nTime);
void AddTime(int nValue);
int GetTime(void);
bool GetTimeUp(void);
#endif