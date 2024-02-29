//==========================================
//
//  メニューメッセージ(menu_message.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "menu_message.h"

//==========================================
//  マクロ定義
//==========================================
#define PASS_MENU_MESSAGE "data/TEXTURE/menu000.png" //テクスチャパス
#define MESSAGE_POS_LEFT (D3DXVECTOR3(640.0f, 90.0f, 0.0f)) //中心座標
#define MESSAGE_WIDTH (200.0f) //横幅
#define MESSAGE_HEIGHT (70.0f) //高さ

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureMenuMessage = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMenuMessage = NULL;

//==========================================
//  初期化処理
//==========================================
void InitMenuMessage()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMenuMessage,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		PASS_MENU_MESSAGE,
		&g_pTextureMenuMessage
	);

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffMenuMessage->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(MESSAGE_POS_LEFT.x - MESSAGE_WIDTH, MESSAGE_POS_LEFT.y - MESSAGE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(MESSAGE_POS_LEFT.x + MESSAGE_WIDTH, MESSAGE_POS_LEFT.y - MESSAGE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(MESSAGE_POS_LEFT.x - MESSAGE_WIDTH, MESSAGE_POS_LEFT.y + MESSAGE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(MESSAGE_POS_LEFT.x + MESSAGE_WIDTH, MESSAGE_POS_LEFT.y + MESSAGE_HEIGHT, 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//rhwの設定
		pVtx[nCnt].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffMenuMessage->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitMenuMessage()
{
	//頂点バッファの破棄
	if (g_pVtxBuffMenuMessage != NULL)
	{
		g_pVtxBuffMenuMessage->Release();
		g_pVtxBuffMenuMessage = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureMenuMessage != NULL)
	{
		g_pTextureMenuMessage->Release();
		g_pTextureMenuMessage = NULL;
	}
}

//==========================================
//  更新処理
//==========================================
void UpdateMenuMessage()
{

}

//==========================================
//  描画処理
//==========================================
void DrawMenuMessage()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMenuMessage, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMenuMessage);

	//メッセージの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}