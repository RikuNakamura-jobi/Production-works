//==================================
//
//天井の処理(ceiling.h)
// Author: Kai Takada
//
//==================================
#ifndef _CEILING_H_								//このマクロ定義がされてなかったら
#define _CEILING_H_								//二重インクルード防止のマクロを定義する

//==========================================
//  マクロ定義
//==========================================
#define CEILING_POSY			(500.0f)				//天井の高さ

//==========================================
//  プロトタイプ宣言
//==========================================
void InitCeiling(void);
void UninitCeiling(void);
void UpdateCeiling(void);
void DrawCeiling(void);
void SetPosCeiling(D3DXVECTOR3 pos, float fHeight, float fWidth);
D3DXVECTOR3 GetCeilingPos(void);

#endif
