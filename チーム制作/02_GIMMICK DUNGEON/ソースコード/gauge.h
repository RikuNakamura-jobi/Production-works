//==================================================================================================
//
//gauge.h
//Author:髙山桃也
//
//==================================================================================================

#ifndef _GAUGE_H_
#define _GAUGE_H_

//===============================
// マクロ定義
//===============================
#define ENEMY_LIFE_CR	(0.006f)	//敵体力ゲージの変化倍率
#define PLAYER_LIFE_CR	(0.006f)	//プレイヤー体力ゲージの変化倍率
#define SKIPGAUGE_CR	(0.05f)	//スキップゲージの変化倍率

//===============================
// 列挙型定義
//===============================
typedef enum
{//ゲージの向き
	GAUGETYPE_HORIZON = 0,				//水平
	GAUGETYPE_VERTICAL,					//垂直
	GAUGETYPE_MAX
}GAUGETYPE;

//===============================
// 構造体定義
//===============================
typedef struct
{//ゲージの構造体
	D3DXVECTOR3 pos;	//根っこのところの位置
	D3DXCOLOR col;	//色
	D3DXCOLOR colFrame;	//枠色
	D3DXCOLOR colBack;	//背景色
	GAUGETYPE type;	//種類
	float fParamMax;	//パラメーターの最大値
	float fParam;	//パラメーターの現在の値
	float fParamOld;	//パラメーターの前回の値
	float fParamDest;	//目的のパラメーター値
	float fRateChange;	//変化する倍率
	float height;	//高さ
	float width;	//幅
	bool bUse;	//使用しているかどうか
}Gauge;

//===============================
// プロトタイプ宣言
//===============================
// 基本処理================
void InitGauge(void);
void UninitGauge(void);
void UpdateGauge(void);
void DrawGauge(void);

// 情報取得================
Gauge *GetGauge(void);

// 設定処理================
int SetGauge(D3DXVECTOR3 pos,float fParamMax,float height,float width,GAUGETYPE type,float fRateChange);
void SetInfoGauge(int nIdx,D3DXCOLOR col,float fParam);

#endif
