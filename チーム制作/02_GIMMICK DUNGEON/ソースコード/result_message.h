//==========================================
//
//  リザルトメッセージ(result_message.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _RESULT_MESSAGE_H_
#define _RESULT_MESSAGE_H_
#include "main.h"

//==========================================
//  プロトタイプ宣言
//==========================================
void InitResultMessage(void);
void UninitResultMessage(void);
void UpdateResultMessage(void);
void DrawResultMessage(void);
bool GetResultState(void);

#endif
