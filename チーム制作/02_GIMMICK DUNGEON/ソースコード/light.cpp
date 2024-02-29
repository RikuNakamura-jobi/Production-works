//========================================
//
//2Dアクションゲーム(ライト処理)[item.cpp]
//Author:佐久間優香
//
//========================================
#include "main.h"
#include "light.h"
#include "player.h"
#include "input.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_LIGHT (4) //ライトの使用数
#define LIGHT_BRIGHT	(0.9f)	//ライトの明るさ

//グローバル変数宣言
D3DLIGHT9 g_light[MAX_LIGHT];				//ライトの情報

//========================================
//ライトの初期化処理
//========================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得
	D3DXVECTOR3 vecDir;									//設定用方向ベクトル
	int nCntLight;

	for (nCntLight = 0; nCntLight < MAX_LIGHT;nCntLight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		if (nCntLight == 0)
		{
			//ライトの種類設定
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//ライトの拡散光の設定
			g_light[nCntLight].Diffuse = D3DXCOLOR(LIGHT_BRIGHT, LIGHT_BRIGHT, LIGHT_BRIGHT, 1.0f);

			//ライトの方向設定
			vecDir = D3DXVECTOR3(-1.4f, 0.24f, -2.21f);
			D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトル正規化
			g_light[nCntLight].Direction = vecDir;

			//ライト設定
			pDevice->SetLight(nCntLight, &g_light[nCntLight]);

			//ライト有効化
			pDevice->LightEnable(nCntLight, TRUE);
			
		}
		else if (nCntLight == 1)
		{
			//ライトの種類設定
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//ライトの拡散光の設定
			g_light[nCntLight].Diffuse = D3DXCOLOR(LIGHT_BRIGHT, LIGHT_BRIGHT, LIGHT_BRIGHT, 1.0f);

			//ライトの方向設定
			vecDir = D3DXVECTOR3(1.42f, -0.8f, 0.08f);
			D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトル正規化
			g_light[nCntLight].Direction = vecDir;

			//ライト設定
			pDevice->SetLight(nCntLight, &g_light[nCntLight]);

			//ライト有効化
			pDevice->LightEnable(nCntLight, TRUE);
			
		}
		else if (nCntLight == 2)
		{
			//ライトの種類設定
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//ライトの拡散光の設定
			g_light[nCntLight].Diffuse = D3DXCOLOR(LIGHT_BRIGHT, LIGHT_BRIGHT, LIGHT_BRIGHT, 1.0f);

			//ライトの方向設定
			vecDir = D3DXVECTOR3(-0.59f, -0.8f, 1.55f);
			D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトル正規化
			g_light[nCntLight].Direction = vecDir;

			//ライト設定
			pDevice->SetLight(nCntLight, &g_light[nCntLight]);

			//ライト有効化
			pDevice->LightEnable(nCntLight, TRUE);
			
		}
		else if (nCntLight == 3)
		{
			//ライトをポイントライトに設定
			g_light[nCntLight].Type = D3DLIGHT_POINT;

			//ライトの拡散光の設定
			g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			//ライトをプレイヤーに追従させる
			g_light[nCntLight].Position = GetPlayer()->pos;
			g_light[nCntLight].Position.y = 0.0f;

			//ライトのサイズを設定する
			g_light[nCntLight].Range = 500.0f;

			//フォールオフ設定
			g_light[nCntLight].Falloff = 1.0f;

			//ライト設定
			pDevice->SetLight(nCntLight, &g_light[nCntLight]);

			//ライト有効化
			//pDevice->LightEnable(nCntLight, TRUE);
		}
	}

}

//========================================
//ライトの終了処理
//========================================
void UninitLight(void)
{

}

//========================================
//ライトの更新処理
//========================================
void UpdateLight(void)
{
	//ポイントライトをプレイヤーに追従させる
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		if (g_light[nCnt].Type == D3DLIGHT_POINT)
		{
			LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得

			g_light[nCnt].Position.x = GetPlayer()->pos.x;
			g_light[nCnt].Position.z = GetPlayer()->pos.z;

			if (GetKeyboardPress(DIK_UP))
			{
				g_light[nCnt].Range++;
			}
			if (GetKeyboardPress(DIK_DOWN))
			{
				g_light[nCnt].Range--;
			}
			if (GetKeyboardPress(DIK_LEFT))
			{
				g_light[nCnt].Position.y++;
			}
			if (GetKeyboardPress(DIK_RIGHT))
			{
				g_light[nCnt].Position.y--;
			}

			//ライト設定
			pDevice->SetLight(nCnt, &g_light[nCnt]);

			//ライトの使用/不使用
			//if (GetKeyboardTrigger(DIK_RETURN))
			//{
			//	pDevice->LightEnable(nCnt, TRUE);
			//}
			//if (GetKeyboardTrigger(DIK_RSHIFT))
			//{
			//	pDevice->LightEnable(nCnt, FALSE);
			//}
		}
	}
}

//========================================
//プレイヤーのモーション
//========================================
void DebugLIGHT(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,100,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "light.Range : %.2f\nlight.pos : ( %.2f : %.2f : %.2f )", g_light[3].Range, g_light[3].Position.x, g_light[3].Position.y, g_light[3].Position.z);

	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
}
