//===========================
//
//ダイレクトX.mainヘッダー
//Author:中村　陸
//
//===========================
#ifndef _MAIN_H_
#define _MAIN_H_

#include <Windows.h>
#include <time.h>
#include "d3dx9.h"						//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告用マクロ
#include "dinput.h"						//入力処理に必要
#include "xaudio2.h"

//ライブラリのリンク
#pragma comment(lib, "d3d9.lib")		//描画処理に必要
#pragma comment(lib, "d3dx9.lib")		//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib, "dxguid.lib")		//ダイレクトXコンポーネント(部品)使用に必要
#pragma comment(lib, "winmm.lib")		//システム時刻の取得に必要
#pragma comment(lib, "dinput8.lib")		//入力処理に必要

//マクロ定義
#define SCREEN_WIDTH (1280)				//ウィンドウの幅
#define SCREEN_HEIGHT (720)				//ウィンドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL| D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define DEFAULT_NUMBER (0)				//初期化用の数
#define VERTEX_COLOR (255)				//頂点カラー
#define TEXTUER_O_X (0.0f)				//原点のテクスチャのx座標
#define TEXTUER_O_Y (0.0f)				//原点のテクスチャのy座標
#define FRAME_TEN (10)					//10フレーム
#define PLAYER_X (64.0f)				//playerのxサイズ
#define PLAYER_Y (64.0f)				//playerのyサイズ
#define PERIOD_PLAYER (5)				//playerの周期
#define SWITCH_PLAYER (5)				//playerの上下切換
#define SPEED_PLAYER (1.0f)				//playerの速度倍率
#define MOVE_ANGLE_F (1.0f)				//前移動の角度倍率
#define MOVE_ANGLE_DIAF (0.75f)			//斜め前移動の角度倍率
#define MOVE_ANGLE_SIDE (0.5f)			//横移動の角度倍率
#define MOVE_ANGLE_DIAB (0.25f)			//斜め後ろ移動の角度倍率
#define MOVE_ANGLE_B (0.0f)				//後ろ移動の角度倍率
#define INE_MOVE_PLAYER (0.15f)			//プレイヤーの移動の慣性減速率
#define INE_ROT_PLAYER (0.1f)			//プレイヤーの回転の慣性減速率
#define INE_SIZE_PLAYER (0.1f)			//プレイヤーのサイズの慣性減速率
#define SHOT_INTERVAL_PLAYER (8)		//プレイヤーの弾の発射感覚

//頂点情報の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;					//頂点座標
	float rhw;							//座標変換用係数
	D3DCOLOR col;						//頂点カラー
	D3DXVECTOR2 tex;					//テクスチャ座標
}VERTEX_2D;

//頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;					//頂点座標
	D3DXVECTOR3 nor;					//法線ベクトル
	D3DCOLOR col;						//頂点カラー
	D3DXVECTOR2 tex;					//テクスチャ座標
}VERTEX_3D;

//画面モードの種類
typedef enum
{
	MODE_TITLE = 0,		//タイトル画面
	MODE_TUTORIAL,		//チュートリアル画面
	MODE_GAME,			//ゲーム画面
	MODE_RESULT,		//リザルト画面
	MODE_MAX,
}MODE;

//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
LPD3DXFONT GetFont(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif // !_MAIN_H_