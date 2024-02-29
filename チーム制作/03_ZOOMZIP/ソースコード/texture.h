//==========================================================
//
//テクスチャ処理 [texture.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"	//main.hで定義しているものが必要なためinclude

//==========================================================
//マクロ定義
//==========================================================
#define MAX_TEXTUREFILE	(256)	//読み込めるテクスチャの最大数

//列挙型の定義
typedef enum
{
	TEXTURETYPE_LOAD = 0,	//地面道路
	TEXTURETYPE_MAX
}TEXTURETYPE;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitTexture(void);
void UninitTexture(void);
void UpdateTexture(void);
LPDIRECT3DTEXTURE9 *SetTexture(int nIdx);

#endif
