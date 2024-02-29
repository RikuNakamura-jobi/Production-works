//===========================
//
//ダイレクトX.shadowヘッダー
//Author:中村　陸
//
//===========================
#ifndef _SHADOW_H_
#define _SHADOW_H_

//影の構造体
typedef struct
{
	D3DXVECTOR3 posShadow;
	D3DXVECTOR3 rotShadow;
	D3DXMATRIX mtxWorldShadow;		//ワールドマトリックス
	bool bTrue;
}Shadow;

//プロトタイプ宣言
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(float rad);
void SetShadowMat(void);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);
void SetShadowFalse(int nIdxShadow);

#endif // !_SHADOW_H_