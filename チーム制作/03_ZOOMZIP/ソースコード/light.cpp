//==========================================================
//
//ライト処理 [light.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "light.h"

//==========================================================
//グローバル変数
//==========================================================
D3DLIGHT9 g_alight[NUM_LIGHT];	//ライトの情報
int g_nSelectLight;		//選択中ライト

//==========================================================
//ライトの初期化処理
//==========================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタを取得
	D3DXVECTOR3 vecDir;		//設定変更用ベクトル

	g_nSelectLight = 0;

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&g_alight[nCntLight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		g_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		switch (nCntLight)
		{
		case 0:
			g_alight[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		case 1:
			g_alight[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
			break;
		case 2:
			g_alight[nCntLight].Diffuse = D3DXCOLOR(0.45f, 0.45f, 0.45f, 1.0f);
			break;
		}

		//ライトの方向を設定
		switch (nCntLight)
		{
		case 0:
			vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
			break;
		case 1:
			vecDir = D3DXVECTOR3(0.8f, 0.23f, -0.55f);
			break;
		case 2:
			vecDir = D3DXVECTOR3(-0.8f, -0.21f, -0.44f);
			break;
		}

		D3DXVec3Normalize(&vecDir, &vecDir);	//ベクトルを正規化する
		g_alight[nCntLight].Direction = vecDir;

		//ライトを設定する
		pDevice->SetLight(nCntLight, &g_alight[nCntLight]);

		//ライトを有効化する
		pDevice->LightEnable(nCntLight, TRUE);
	}

}

//==========================================================
//ライトの終了処理
//==========================================================
void UninitLight(void)
{

}

//==========================================================
//ライトの更新処理
//==========================================================
void UpdateLight(void)
{



}

//==========================================================
//ライトの色変更
//==========================================================
void ChangeColLight(void)
{


}


//==========================================================
//ライトの取得
//==========================================================
D3DLIGHT9 *GetLight(void)
{
	return &g_alight[0];
}

//==========================================================
//選択中ライト番号取得
//==========================================================
int GetSelectLight(void)
{
	return g_nSelectLight;
}