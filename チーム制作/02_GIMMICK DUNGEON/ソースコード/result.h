//==========================================
//
//  リザルト画面(result.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _RESULT_H_
#define _RESULT_H_
#include "main.h"

//==========================================
//  リザルト列挙型
//==========================================
typedef enum
{
	RESULTTYPE_WIN = 0,
	RESULTTYPE_LOSE,
	RESULTTYPE_MAX
}RESULTTYPE;

//==========================================
//  プロトタイプ宣言
//==========================================
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
bool SelectResult(void);
void SetResult(RESULTTYPE type);
RESULTTYPE GetResult(void);

#endif
