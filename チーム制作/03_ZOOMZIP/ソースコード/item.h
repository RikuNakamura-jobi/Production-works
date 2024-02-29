//==========================================================
//
//アイテム処理 [item.h]
//Author:佐藤根詩音
//
//==========================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"	//main.hで定義しているものが必要なためinclude

//マクロ定義
#define MAX_ITEM	(128)		//アイテムの最大数

//==========================================================
//列挙型の定義
//==========================================================
//アイテムの状態列挙型
typedef enum
{
	ITEMSTATE_DEL = 0,		//障害物全削除
	ITEMSTATE_JET,			//ジェットパック
	ITEMSTATE_UP,			//得点アップ
	ITEMSTATE_MAX
}ITEMSTATE;

//==========================================================
//構造体の定義
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 posKeep;	//保存用位置
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 moveOld;	//前回の移動量
	float fWidth;			//幅
	float fHeight;			//高さ
	float fMoveRand;		//ランダムな移動量
	bool bUse;				//使用してるか
	bool bDisp;				//描画するか
	bool bJump;				//ジャンプしてるか
	int nType;				//アイテムの種類
	int nSwitchLR;			//左右の切り替え
}Item;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, int nType);
Item *GetItem(void);

#endif


