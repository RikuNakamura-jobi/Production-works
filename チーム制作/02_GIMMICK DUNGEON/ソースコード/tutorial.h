//==========================================
//
//  チュートリアルの処理(tutorial.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_
#include "main.h"

//==========================================
//  列挙型定義
//==========================================
typedef enum
{
	TUTORIALSTATE_MOVE = 0, //移動しろ
	TUTORIALSTATE_CAMERA, //カメラ動かせ
	TUTORIALSTATE_REFLECTOR, //反射板押せ
	TUTORIALSTATE_BUTTON, //ボタン押せ
	TUTORIALSTATE_JUMP, //ジャンプしろ
	TUTORIALSTATE_ENEMY, //敵を倒せ
	TUTORIALSTATE_END, //チュートリアルの終了
	TUTORIAL_MAX
}TUTORIALSTATE; //チュートリアル状態

typedef enum
{
	TUTORIAL_MOVE = 0,
	MOVE_EX,
	TUTORIAL_CAMERA,
	CAMERA_EX,
	TUTORIAL_REFLECTOR,
	REFLECTOR_EX,
	TUTORIAL_BUTTON,
	BUTTON_EX,
	TUTORIAL_JUMP,
	JUMP_EX,
	TUTORIAL_ENEMY,
	ENEMY_EX,
	TUTORIAL_BG,
	TUTORIAL_ENTER,
	TUTORIAL_TEX_MAX
}TUTORIALTEX; //チュートリアルテクスチャ

typedef enum
{
	TUTORIALTYPE_BASE = 0,
	TUTORIALTYPE_EX, 
	TUTORIALTYPE_BG,
	TUTORIALTYPE_ENTER,
	TUTORIALTYPE_MAX
}TUTORIALTYPE; //ポリゴンの種類

typedef enum
{
	LEFTSIDE = 0,
	RIGHTSIDE,
	UNDERSIDE
}TUTORIALPOS; //ポリゴンの位置

//==========================================
//  プロトタイプ宣言
//==========================================
void InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
TUTORIALSTATE *GetTutorialState(void);

#endif
