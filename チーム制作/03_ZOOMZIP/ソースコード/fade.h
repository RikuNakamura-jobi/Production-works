//==========================================================
//
//フェード処理
//Author 奥定伊吹
//
//==========================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"	//main.hで定義しているものが必要なためinclude

//フェード状態列挙型
typedef enum
{
	FADE_NONE = 0,	//何もしていない状態
	FADE_IN,		//フェードイン状態
	FADE_OUT,		//フェードアウト状態
	FADE_MAX
}FADE;

typedef enum
{
	FADETYPE_NORMAL = 0,	//通常フェード
	FADETYPE_ROTATE,		//回転
	FADETYPE_MAX
}FADETYPE;

//フェードの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXCOLOR col;			//フェードカラー
	int nType;				//種類
}Fade;

//プロトタイプ宣言
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void SetFade(MODE modeNext);
FADE GetFade(void);
Fade *GetFadeInfo(void);

#endif
