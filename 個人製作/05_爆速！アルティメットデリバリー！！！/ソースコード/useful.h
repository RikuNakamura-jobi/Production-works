//=====================================
//
// bgヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _USEFUL_H_
#define _USEFUL_H_

#include "main.h"
#include "object2D.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//名前空間宣言---------------------
namespace useful
{
	D3DXCOLOR HSLtoRGB(float Hue);

	D3DXVECTOR3 PosRelativeMtx(D3DXVECTOR3 posO, D3DXVECTOR3 rot, D3DXVECTOR3 offset);
}

#endif // !_USEFUL_H_