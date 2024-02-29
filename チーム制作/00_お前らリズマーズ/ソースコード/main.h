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
#define DEFAULT_NUMBER (0)				//初期化用の数
#define VERTEX_COLOR (255)				//頂点カラー
#define TEXTUER_O_X (0.0f)				//原点のテクスチャのx座標
#define TEXTUER_O_Y (0.0f)				//原点のテクスチャのy座標
#define TEXTUER_TEN_X (0.2f)			//10分割のテクスチャのx座標
#define TEXTUER_TEN_Y (0.5f)			//10分割のテクスチャのy座標
#define FRAME_TEN (10)					//10フレーム
#define PLAYER_WIDTH_R (980.0f)			//playerの動ける幅右
#define PLAYER_WIDTH_L (300.0f)			//playerの動ける幅左
#define PLAYER_HEIGHT_U (0.0f)			//playerの動ける高さ上
#define PLAYER_HEIGHT_D (720.0f)		//playerの動ける高さ下
#define PLAYER_X (211.2f)				//playerのxサイズ
#define PLAYER_Y (352.0f)				//playerのyサイズ
#define PERIOD_PLAYER (3)				//playerの周期
#define SWITCH_PLAYER (5)				//playerの上下切換
#define SPEED_PLAYER (1.5f)				//playerの速度倍率
#define MOVE_ANGLE_F (1.0f)				//前移動の角度倍率
#define MOVE_ANGLE_DIAF (0.75f)			//斜め前移動の角度倍率
#define MOVE_ANGLE_SIDE (0.5f)			//横移動の角度倍率
#define MOVE_ANGLE_DIAB (0.25f)			//斜め後ろ移動の角度倍率
#define MOVE_ANGLE_B (0.0f)				//後ろ移動の角度倍率
#define ROT_PLAYER (0.02f)				//回転の移動量
#define MOVE_SIZE_PLAYER (0.5f)			//サイズの移動量
#define INE_MOVE_PLAYER (0.15f)			//プレイヤーの移動の慣性減速率
#define INE_ROT_PLAYER (0.1f)			//プレイヤーの回転の慣性減速率
#define INE_SIZE_PLAYER (0.1f)			//プレイヤーのサイズの慣性減速率
#define BULLET_SPEED (8.0f)				//弾の速度
#define ENEMY_X (159.5f)				//playerのxサイズ
#define ENEMY_Y (136.4f)				//playerのyサイズ
#define SHOT_INTERVAL_PLAYER (8)		//プレイヤーの弾の発射感覚

//画面モードの種類
typedef enum
{
	MODE_TITLE = 0,		//タイトル画面
	MODE_TUTORIAL,		//チュートリアル画面
	MODE_GAME,			//ゲーム画面
	MODE_RESULT,		//リザルト画面
	MODE_RANKING,		//ランキング画面
	MODE_MAX,
}MODE;

//頂点情報の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;					//頂点座標
	float rhw;							//座標変換用係数
	D3DCOLOR col;						//頂点カラー
	D3DXVECTOR2 tex;					//テクスチャ座標
}VERTEX_2D;

//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif // !_MAIN_H_