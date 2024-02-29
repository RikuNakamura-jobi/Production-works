//===========================
//
//ダイレクトX.remainヘッダー
//Author:中村　陸
//
//===========================
#ifndef _REMAIN_H_
#define _REMAIN_H_

#include "main.h"

//プロトタイプ宣言
void InitRemain(void);
void UninitRemain(void);
void UpdateRemain(void);
void DrawRemain(void);
void SetRemain(int nRemain);
void AddRemain(int nValue);
int GetRemain(void);

#endif // !_REMAIN_H_