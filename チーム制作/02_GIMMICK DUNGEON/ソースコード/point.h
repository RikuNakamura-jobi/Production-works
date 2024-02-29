//==========================================
//
//  取得スコア表示処理
//  Author : 髙山桃也
//
//==========================================

#ifndef _POINT_H_
#define _POINT_H_

#include "main.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_POINT_NUM (12) //取得スコア表示の最大数

//==========================================
//  列挙型定義
//==========================================
typedef enum
{//取得スコアの状態
	POINTSTATE_IN = 0,	//スライドイン状態
	POINTSTATE_WAIT,	//待機状態
	POINTSTATE_OUT,	//スライドアウト状態
	POINTSTATE_MAX
}POINTSTATE;

typedef enum
{//取得スコアの種類
	POINTTYPE_SLIDE = 0,	//スライドインしてくるスコア表示
	POINTTYPE_STAY,	//表示し続けるスコア表示
	POINTTYPE_MAX
}POINTTYPE;

typedef enum
{//メッセージの種類
	LOGTYPE_DAMAGE = 0,	//ダメージ
	LOGTYPE_LOST,	//追跡振り切り
	LOGTYPE_HITROCK,	//岩命中
	LOGTYPE_HITSANDPOLE,	//砂の柱命中
	LOGTYPE_DODGE_SHOCK,	//衝撃波回避
	LOGTYPE_HITSTONE,	//石柱命中
	LOGTYPE_TIMEBONUS,	//タイムボーナス
	LOGTYPE_TOTALSCORE,	//総スコア
	LOGTYPE_MAX
}LOGTYPE;

//==========================================
//  構造体定義
//==========================================
//取得スコア数字の構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posDest;	//目標位置
	D3DXCOLOR col;	//色
	POINTSTATE state;	//状態
	POINTTYPE type;	//種類
	int nScore;	//スコア値
	int nCntState;	//状態遷移カウンター
	int nNumPlace;	//桁数
	float fScale;	//スケール
	bool bUse;	//使用しているかどうか
}Point;

typedef struct
{//メッセージ
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;	//色
	LOGTYPE type;	//種類
	Point *pPoint;	//スコア表示へのポインタ
	float fScale;	//スケール
	bool bUse;	//使用されているかどうか
}PointLog;

//==========================================
//  プロトタイプ宣言
//==========================================
void InitPoint(void);
void UninitPoint(void);
void UpdatePoint(void);
void DrawPoint(void);
void SetPointScore(int nPoint,D3DXVECTOR3 pos, D3DXVECTOR3 posDest, POINTTYPE type, float fScale, LOGTYPE logType);
void PushUpScore(int nCntpoint);
Point *GetPoint(void);

//種類ごとの更新分け
void UpdatePointSlide(Point *pPoint);
void UpdatePointStay(Point *pPoint);

#endif