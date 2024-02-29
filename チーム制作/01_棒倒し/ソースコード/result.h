//===========================
//
//ダイレクトX.resultヘッダー
//Author:中村　陸
//
//===========================
#ifndef _RESULT_H_
#define _RESULT_H_

//画面モードの種類
typedef enum
{
	RESULT_CLEAR = 0,		//タイトル画面
	RESULT_OVER,			//ゲーム画面
	RESULT_MAX,
}RESULT;

//プロトタイプ宣言
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
RESULT GetResult(void);
void SetResult(RESULT Result);

#endif // !_RESULT_H_