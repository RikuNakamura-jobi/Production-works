//========================================
//
//3Dモデルのエディット機能[edit.h]
//Author:Kai Takada
//
//========================================
#ifndef _EDIT_H_								//このマクロ定義がされてなかったら
#define _EDIT_H_								//二重インクルード防止のマクロを定義する

//==========================================
//  プロトタイプ宣言
//==========================================
void InitEdit(void);
void UninitEdit(void);
void UpdateEdit(void);
void DrawEdit(void);
void SetEditIndex(int nIdx);
int GetSelectIdxEdit(void);
void LoadEdit(void);	//エディット内容のロード

#endif
