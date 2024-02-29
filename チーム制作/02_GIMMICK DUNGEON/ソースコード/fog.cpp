//==========================================
//
//  砂煙の制御(fog.cpp)
//  Autnor : Tomoya Kanazaki
//
//==========================================
#include "fog.h"
#include "player.h"
#include "input.h"

//==========================================
//  グローバル変数宣言
//==========================================
FOGDATA g_fog;

//==========================================
//  初期化処理
//==========================================
void InitFog()
{
	//変数の初期化
	ZeroMemory(&g_fog, sizeof(FOGDATA));
	g_fog.bUse = false;
	g_fog.col = FOG_COLOR_SAND;
	g_fog.fStart = FOG_START_TITLE;
	g_fog.fEnd = FOG_END_TITLE;
}

//==========================================
//  終了処理
//==========================================
void UninitFog()
{

}

//==========================================
//  更新処理
//==========================================
void UpdateFog()
{
	//使用状態の更新
	if (GetEnvironment() == PLAYERENVIRONMENT_CLOUD)
	{
		g_fog.bUse = true;
	}
	else
	{
		//g_fog.bUse = false;
	}

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_3))
	{
		g_fog.fStart += 0.5f;
	}
	if (GetKeyboardPress(DIK_4))
	{
		g_fog.fStart -= 0.5f;
	}
	if (GetKeyboardPress(DIK_5))
	{
		g_fog.fEnd += 0.5f;
	}
	if (GetKeyboardPress(DIK_6))
	{
		g_fog.fEnd -= 0.5f;
	}
#endif
}

//==========================================
//  設定処理
//==========================================
void SetFog()
{
	if (g_fog.bUse)
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//霧を有効化
		pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

		//霧の色を設定
		pDevice->SetRenderState(D3DRS_FOGCOLOR, g_fog.col);

		//霧の状態を設定
		pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
		pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

		//霧の発生範囲を設定
		pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&g_fog.fStart));
		pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&g_fog.fEnd));
	}
}

//==========================================
//  オンオフ切り替え処理 takayama
//==========================================
void EnableFog(bool bUse)
{
	g_fog.bUse = bUse;
}

//==========================================
//  ポインタ取得処理 takayama
//==========================================
FOGDATA *GetFog(void)
{
	return &g_fog;
}