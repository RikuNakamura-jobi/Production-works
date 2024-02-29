//===========================
//
//ダイレクトX.lightファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "light.h"

//マクロ定義
#define NUM_LIGHT (3)

//グローバル変数宣言
D3DLIGHT9 g_aLight[NUM_LIGHT];		//ライトの情報

//===========================
//プレイヤーの初期化処理
//===========================
void InitLight(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;		//設定用方向ベクトル

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&g_aLight[nCntLight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		g_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		if (nCntLight == 0)
		{
			//ライトの拡散光を設定
			g_aLight[nCntLight].Diffuse = D3DXCOLOR(0.7f, 0.5f, 0.5f, 1.0f);

			//ライトの方向を設定
			vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
		}
		else if (nCntLight == 1)
		{
			//ライトの拡散光を設定
			g_aLight[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.0f, 0.0f, 0.65f);

			//ライトの方向を設定
			vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
		}
		else if (nCntLight == 2)
		{
			//ライトの拡散光を設定
			g_aLight[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 0.15f);

			//ライトの方向を設定
			vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
		}

		D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトルを正規化する
		g_aLight[nCntLight].Direction = vecDir;

		//ライトを設定する
		pDevice->SetLight(nCntLight, &g_aLight[nCntLight]);
	}

	//ライトを有効にする
	pDevice->LightEnable(0, TRUE);
}

//===========================
//プレイヤーの終了処理
//===========================
void UninitLight(void)
{
	
}

//===========================
//プレイヤーの更新処理
//===========================
void UpdateLight(void)
{

}