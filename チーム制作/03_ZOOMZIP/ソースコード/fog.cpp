//==========================================================
//
//フォグ処理 [fog.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "fog.h"
#include "result.h"

//==========================================================
//グローバル変数
//==========================================================
Fog g_Fog;		//フォグの情報

//==========================================================
//フォグの初期化処理
//==========================================================
void InitFog(void)
{
	//情報初期化
	switch (GetMode())
	{
	case MODE_TUTORIAL:
		g_Fog.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);	//色
		g_Fog.fStart = 1000.0f;	//開始点
		g_Fog.fEnd = 1700.0f;	//終了点
		g_Fog.fDensity = 0.01f;	//フォグ密度
		break;
	case MODE_GAME:
		g_Fog.col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.1f);	//色
		g_Fog.fStart = 2000.0f;	//開始点
		g_Fog.fEnd = 2700.0f;	//終了点
		g_Fog.fDensity = 0.01f;	//フォグ密度
		break;
	case MODE_RESULT:
		//if (*GetResultState() == RESULTSTATE_CLEAR)
		//{//クリア状態
		//	g_Fog.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
		//	g_Fog.fStart = 1000.0f;	//開始点
		//	g_Fog.fEnd = 3000.0f;	//終了点
		//	g_Fog.fDensity = 0.01f;	//フォグ密度
		//}
		//else if(*GetResultState() == RESULTSTATE_GAMEOVER)
		//{//ゲームオーバー状態
			g_Fog.col = D3DXCOLOR(0.05f, 0.05f, 0.05f, 0.5f);	//色
			g_Fog.fStart = 200.0f;	//開始点
			g_Fog.fEnd = 1000.0f;	//終了点
			g_Fog.fDensity = 0.01f;	//フォグ密度
		//}
		break;
	}
}

//==========================================================
//フォグの終了処理
//==========================================================
void UninitFog(void)
{

}

//==========================================================
//フォグの更新処理
//==========================================================
void UpdateFog(void)
{

}

//==========================================================
//フォグの描画処理
//==========================================================
void DrawFog(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタを取得

	Fog fog = g_Fog;	//フォグの情報格納用

	//フォグを有効にする
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	//フォグカラー設定
	pDevice->SetRenderState(D3DRS_FOGCOLOR, fog.col);

	//バーテックスフォグ(線形公式)を使用
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);

	//フォグ範囲設定
	pDevice->SetRenderState(D3DRS_FOGSTART, *((LPDWORD)(&fog.fStart)));
	pDevice->SetRenderState(D3DRS_FOGEND, *((LPDWORD)(&fog.fEnd)));

	//ピクセルフォグ(指数の2乗)を使用
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);

	//フォグ密度設定
	pDevice->SetRenderState(D3DRS_FOGDENSITY, *((LPDWORD)(&fog.fDensity)));

	//範囲ベースのフォグを使用
	pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);

}

//==========================================================
//フォグの状態リセット
//==========================================================
void ResetStateFog(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタを取得

	//フォグを無効にする
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	//範囲ベースのフォグを不使用
	pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
}