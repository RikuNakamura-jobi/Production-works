//===========================
//
//ダイレクトX.lifeヘッダー
//Author:中村　陸
//
//===========================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"

//プロトタイプ宣言
void InitLife(void);
void UninitLife(void);
void UpdateLife(void);
void DrawLife(void);
void SetLife(int nLife);
void AddLife(int nValue);
int GetLife(void);

#endif // !_LIFE_H_