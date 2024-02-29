//===========================
//
//ダイレクトX.boostヘッダー
//Author:中村　陸
//
//===========================
#ifndef _BOOST_H_
#define _BOOST_H_

#include "main.h"

//プロトタイプ宣言
void InitBoost(void);
void UninitBoost(void);
void UpdateBoost(void);
void DrawBoost(void);
void SetBoost(int nBoost);
void AddBoost(int nValue);
int GetBoost(void);

#endif // !_BOOST_H_