//==========================================
//
//  ゲーム内設定(option.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "option.h"
#include "input.h"

//==========================================
//  マクロ定義
//==========================================
#define MOUSE_SENS_DEFAULT (D3DXVECTOR3(0.001f, 0.001f, 0.0f)) //マウス感度の初期設定
#define MOUSE_SENS_SCALE (0.0001f) //感度設定の基準値

//==========================================
//  構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 temp; //保存用変数
	D3DXVECTOR3 pass; //変数取得用
	bool bReverse_X; //X軸の反転
	bool bReverse_Y; //Y軸の反転
}MOUSE_SENS;

//==========================================
//  グローバル変数
//==========================================
MOUSE_SENS g_MouseSens;

//==========================================
//  初期化処理
//==========================================
void InitOption()
{
	//変数の初期化
	g_MouseSens.temp = MOUSE_SENS_DEFAULT;
	g_MouseSens.pass = g_MouseSens.temp;
	g_MouseSens.bReverse_X = false;
	g_MouseSens.bReverse_Y = false;
}

//==========================================
//  終了処理
//==========================================
void UninitOption()
{

}

//==========================================
//  更新処理
//==========================================
void UpdateOption()
{
	//マウス感度の補正値を設定する
	SetMouseSensCorrection();
}

//==========================================
//  描画処理
//==========================================
void DrawOption()
{

}

//==========================================
//  マウスの感度設定
//==========================================
void SetMouseSensCorrection()
{
	//ローカル変数宣言
	float fWheel = GetMouseMove().z;

	//感度の上下
	if (fWheel > 0.0f)
	{
		g_MouseSens.temp += D3DXVECTOR3(MOUSE_SENS_SCALE, MOUSE_SENS_SCALE, 0.0f);
	}
	if (fWheel < 0.0f)
	{
		g_MouseSens.temp -= D3DXVECTOR3(MOUSE_SENS_SCALE, MOUSE_SENS_SCALE, 0.0f);
	}

	//最低値の補正
	if (g_MouseSens.temp.x < MOUSE_SENS_SCALE)
	{
		g_MouseSens.temp.x = MOUSE_SENS_SCALE;
	}
	if (g_MouseSens.temp.y < MOUSE_SENS_SCALE)
	{
		g_MouseSens.temp.y = MOUSE_SENS_SCALE;
	}

	//設定された感度の取得
	g_MouseSens.pass = g_MouseSens.temp;

	//反転切り替え
	if (GetMouseTrigger(MOUSEBUTTON_LEFT))
	{
		g_MouseSens.bReverse_X = !g_MouseSens.bReverse_X;
	}
	if (GetMouseTrigger(MOUSEBUTTON_RIGHT))
	{
		g_MouseSens.bReverse_Y = !g_MouseSens.bReverse_Y;
	}

	//入力の反転
	if (g_MouseSens.bReverse_X)
	{
		g_MouseSens.pass.x *= -1.0f;
	}
	if (g_MouseSens.bReverse_Y)
	{
		g_MouseSens.pass.y *= -1.0f;
	}
}

//==========================================
//  補正感度の取得
//==========================================
D3DXVECTOR3 GetMouseSensCorrection()
{
	return g_MouseSens.pass;
}
