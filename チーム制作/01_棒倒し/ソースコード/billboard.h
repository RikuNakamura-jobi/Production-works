//===========================
//
//ダイレクトX.billboardヘッダー
//Author:中村　陸
//
//===========================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//プロトタイプ宣言
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
void SetBillboard(D3DXVECTOR3 pos);

#endif // !_BILLBOARD_H_