//===========================
//
//ダイレクトX.tutorialヘッダー
//Author:中村　陸
//
//===========================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//ゲームの状態
typedef enum
{
	TUTORIALSTATE_NONE = 0,				//何もしてない状態
	TUTORIALSTATE_NORMAL,				//通常状態
	TUTORIALSTATE_END,					//終了状態
	TUTORIALSTATE_MAX
}TUTORIALSTATE;

//プロトタイプ宣言
void InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
void SetTUTORIALSTATE(TUTORIALSTATE state, int nCounter);
TUTORIALSTATE GetTUTORIALSTATE(void);

#endif // !_TUTORIAL_H_