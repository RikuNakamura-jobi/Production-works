//==========================================================
//
//ハイスコア処理 [high_score.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef HIGHSCORE_H_	//このマクロ定義が定義されていなかったら
#define HIGHSCORE_H_    //2連インクルード防止のマクロを定義する

#include "main.h"    //作成したmain.hをインクルードする

//==========================================================
//列挙型の定義
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
	D3DXCOLOR col;		//色
	float fAngle;		//角度
	float fLength;		//対角線の長さ
	float fHeight;		//高さ
	float fWidth;		//幅
	bool bUse;			//使用しているかどうか
}HighScore;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitHighScore(void);
void UninitHighScore(void);
void UpdateHighScore(void);
void DrawHighScore(void);
HighScore *GetHighScore(void);
void AddHighScore(int nAddHighScore);
int GetNowHighScore(void);

#endif
