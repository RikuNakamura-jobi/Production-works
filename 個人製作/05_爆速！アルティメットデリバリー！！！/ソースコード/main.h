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
#include <string.h> 
#include <stdio.h>
#include "d3dx9.h"						//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告用マクロ
#include "dinput.h"						//入力処理に必要
#include "xaudio2.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifdef true

#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define new new
#endif

#endif


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

//頂点情報の構造体を定義
struct VERTEX_2D
{
	D3DXVECTOR3 pos;					//頂点座標
	float rhw;							//座標変換用係数
	D3DCOLOR col;						//頂点カラー
	D3DXVECTOR2 tex;					//テクスチャ座標
};

//頂点情報[3D]の構造体を定義
struct VERTEX_3D
{
	D3DXVECTOR3 pos;					//頂点座標
	D3DXVECTOR3 nor;					//法線ベクトル
	D3DCOLOR col;						//頂点カラー
	D3DXVECTOR2 tex;					//テクスチャ座標
};

#endif // !_MAIN_H_