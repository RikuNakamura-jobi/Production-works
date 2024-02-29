//==========================================
//
//  ���U���g���(result.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _RESULT_H_
#define _RESULT_H_
#include "main.h"

//==========================================
//  ���U���g�񋓌^
//==========================================
typedef enum
{
	RESULTTYPE_WIN = 0,
	RESULTTYPE_LOSE,
	RESULTTYPE_MAX
}RESULTTYPE;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
bool SelectResult(void);
void SetResult(RESULTTYPE type);
RESULTTYPE GetResult(void);

#endif
