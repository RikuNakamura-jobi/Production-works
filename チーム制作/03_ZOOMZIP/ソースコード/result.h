//
//
//リザルト[result.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _RESULT_H_     //このマクロ定義が定義されていなかったら
#define _RESULT_H_      //2連インクルード防止のマクロを定義する

#include "main.h"    //作成したmain.hをインクルードする

//=======================================
//リザルト状態
//=======================================
typedef enum
{
	RESULTSTATE_NONE = 0,	//何もない状態
	RESULTSTATE_CLEAR,		//ゲームクリア
	RESULTSTATE_GAMEOVER,	//ゲームオーバー
	RESULTSTATE_MAX
}RESULTSTATE;

//=======================================
//リザルト構造体の定義
//=======================================
typedef struct
{
	RESULTSTATE State;
}RESULT;

//=======================================
//プロトタイプ宣言
//=======================================
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetResultMode(RESULTSTATE Result);
void ResultJudge(void);
RESULTSTATE *GetResultState(void);

#endif