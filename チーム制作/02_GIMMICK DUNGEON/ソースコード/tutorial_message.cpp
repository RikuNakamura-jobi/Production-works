//==========================================
//
//  チュートリアルメッセージ(tutorial_message.cpp)
//  Author : Kai Takada
//
//==========================================
#include "tutorial_message.h"

//==========================================
//  マクロ定義
//==========================================
#define PASS_TUTORIAL_MESSAGE "data/TEXTURE/tutorial_message.png" //テクスチャパス
#define MESSAGE_POS_LEFT (D3DXVECTOR3(1050.0f, 650.0f,0.0f)) //メッセージの中心座標
#define MESSAGE_POLYGON_WIDTH (250.0f) //ポリゴンの幅
#define MESSAGE_POLYGON_HEIGHT (34.0f) //ポリゴンの高さ
#define SIZE_RATIO (MESSAGE_POLYGON_WIDTH / MESSAGE_POLYGON_HEIGHT) //高さと幅の比率

//==========================================
//  構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //中心座標
	D3DXCOLOR col; //ポリゴンカラー
	float fScale; //ポリゴンの拡大率
}MESSAGE;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureTutorialMessage = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialMessage = NULL;
MESSAGE g_tutorialMessage;

//==========================================
//  初期化処理
//==========================================
void InitTutorialMessage()
{
	//変数の初期化
	g_tutorialMessage.pos = MESSAGE_POS_LEFT;
	g_tutorialMessage.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_tutorialMessage.fScale = 1.0f;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialMessage,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		PASS_TUTORIAL_MESSAGE,
		&g_pTextureTutorialMessage
	);

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffTutorialMessage->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_tutorialMessage.pos.x - (MESSAGE_POLYGON_WIDTH + (g_tutorialMessage.fScale * SIZE_RATIO)), g_tutorialMessage.pos.y - (MESSAGE_POLYGON_HEIGHT + g_tutorialMessage.fScale), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_tutorialMessage.pos.x + (MESSAGE_POLYGON_WIDTH + (g_tutorialMessage.fScale * SIZE_RATIO)), g_tutorialMessage.pos.y - (MESSAGE_POLYGON_HEIGHT + g_tutorialMessage.fScale), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_tutorialMessage.pos.x - (MESSAGE_POLYGON_WIDTH + (g_tutorialMessage.fScale * SIZE_RATIO)), g_tutorialMessage.pos.y + (MESSAGE_POLYGON_HEIGHT + g_tutorialMessage.fScale), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_tutorialMessage.pos.x + (MESSAGE_POLYGON_WIDTH + (g_tutorialMessage.fScale * SIZE_RATIO)), g_tutorialMessage.pos.y + (MESSAGE_POLYGON_HEIGHT + g_tutorialMessage.fScale), 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//rhwの設定
		pVtx[nCnt].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[nCnt].col = g_tutorialMessage.col;
	}

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffTutorialMessage->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitTutorialMessage()
{
	//頂点バッファの破棄
	if (g_pVtxBuffTutorialMessage != NULL)
	{
		g_pVtxBuffTutorialMessage->Release();
		g_pVtxBuffTutorialMessage = NULL;
	}

	if (g_pTextureTutorialMessage != NULL)
	{
		g_pTextureTutorialMessage->Release();
		g_pTextureTutorialMessage = NULL;
	}
}

//==========================================
//  更新処理
//==========================================
void UpdateTutorialMessage()
{
}

//==========================================
//  描画処理
//==========================================
void DrawTutorialMessage()
{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffTutorialMessage, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutorialMessage);

		//ロゴの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
