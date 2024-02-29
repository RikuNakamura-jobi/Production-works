//===========================
//
//ダイレクトX.uiヘッダー
//Author:中村　陸
//
//===========================
#ifndef _UI_H_
#define _UI_H_

//敵の種類
typedef enum
{
	UISTATE_TITLE = 0,			//通常状態
	UISTATE_GAME,				//ダメージ状態
	UISTATE_RESULT,				//ダメージ状態
	UISTATE_MAX
}UISTATE;

//敵の種類
typedef enum
{
	UITEXTURE_TITLE = 0,			//通常状態
	UITEXTURE_TITLEBUTTON,		//通常状態
	UITEXTURE_TUTORIAL_0,			//通常状態
	UITEXTURE_TUTORIAL_1,			//通常状態
	UITEXTURE_TUTORIAL_2,			//通常状態
	UITEXTURE_CLEAR,			//通常状態
	UITEXTURE_GAMEOVER,			//通常状態
	UITEXTURE_MAX
}UITEXTURE;

//プレイヤーの構造体
typedef struct
{
	D3DXVECTOR3 pos;
	float fWidth;
	float fHeight;
	int nType;
}UI;

//プロトタイプ宣言
void InitUi(void);
void UninitUi(void);
void UpdateUi(void);
void DrawUi(void);

#endif // !_UI_H_