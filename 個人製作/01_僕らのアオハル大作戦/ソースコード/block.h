//===========================
//
//ダイレクトX.blockヘッダー
//Author:中村　陸
//
//===========================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//プレイヤーの種類
typedef enum
{
	BLOCKTYPE_0,				//通常状態
	BLOCKTYPE_1,				//通常状態
	BLOCKTYPE_2,				//通常状態
	BLOCKTYPE_3,				//通常状態
	BLOCKTYPE_4,				//通常状態
	BLOCKTYPE_MAX
}BLOCKTYPE;

//敵構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 posOld;				//位置
	D3DXVECTOR3 posInit;			//位置
	D3DXVECTOR3 move;				//移動量
	D3DXCOLOR col;
	BLOCKTYPE blocktype;
	int nType;
	float fHeight;					//高さ
	float fWidth;					//幅
	int nBlockBreak;
	bool bUse;						//使用しているかどうか
	bool bPlayer;
}Block;

//プロトタイプ宣言
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, int nType);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, Block **pBlock);
bool CollisionBlockSide(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, Block **pBlock);

#endif // !_BLOCK_H_