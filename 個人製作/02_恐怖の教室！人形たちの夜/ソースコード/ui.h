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
	UITEXTURE_TITLEPRESS,			//通常状態
	UITEXTURE_TUTORIAL,				//通常状態
	UITEXTURE_TUTORIALDOT,			//通常状態
	UITEXTURE_GAMEDOT,				//ダメージ状態
	UITEXTURE_GAMELIFE,				//ダメージ状態
	UITEXTURE_GAMELIFEBAR,				//ダメージ状態
	UITEXTURE_RESULTCLEAR,			//ダメージ状態
	UITEXTURE_RESULTOVER,			//ダメージ状態
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
void AddLife(float nLife);

#endif // !_UI_H_