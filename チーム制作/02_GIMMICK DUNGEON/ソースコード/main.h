
//========================================
//
//2Dシューティングゲーム[main.h]
//Author:佐久間優香
//
//========================================
#ifndef _MAIN_H_						//このマクロが定義されていなかったら
#define _MAIN_H_						//2重インクルード防止のマクロを定義する

#include<windows.h>
#include <time.h>
#include "d3dx9.h"						//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告対処用マクロ
#include "dinput.h"						//入力処理に必要
#include "xaudio2.h"					//サウンド処理に必要
#include <stdio.h>



//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")			//描画処理に必要
#pragma comment(lib,"d3dx9.lib")		//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")		//DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"winmm.lib")		//システム時刻の取得に必要
#pragma comment(lib,"dinput8.lib")		//入力処理に必要

//マクロ定義
#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)



//頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;					//頂点座標
	float rhw;							//座標変換用係数(1.0fで固定)
	D3DCOLOR col;						//頂点カラー
	D3DXVECTOR2 tex;					//テクスチャの座標
}VERTEX_2D;

//頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;			//頂点座標
	D3DXVECTOR3 nor;			//法線ベクトル
	D3DCOLOR col;				//頂点カラー
	D3DXVECTOR2 tex;			//テクスチャ座標
}VERTEX_3D;

//画面(モード)の種類
typedef enum
{
	MODE_LOGO = 0,						//スタート画面
	MODE_TITLE,							//タイトル画面
	MODE_MENU,							//メニュー画面
	MODE_TUTORIAL,						//ポーズ画面
	MODE_GAME,							//ゲーム画面
	MODE_RANKING,						//ランキング画面
	MODE_MAX							
}MODE;

//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);
LPD3DXFONT GetFont(void);

#endif // !_MAIN_H_