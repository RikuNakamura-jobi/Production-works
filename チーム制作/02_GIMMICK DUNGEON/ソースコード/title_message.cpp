//==========================================
//
//  タイトルメッセージ(title_message.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "title_message.h"
#include "title.h"

//==========================================
//  マクロ定義
//==========================================
#define PASS_TITLE_MESSAGE "data/TEXTURE/PRESS_ANY_KEY000.png" //テクスチャパス
#define MESSAGE_POS_LEFT (D3DXVECTOR3(640.0f, 540.0f,0.0f)) //メッセージの中心座標
#define MESSAGE_ALPHA_SCALE (0.05f) //アルファ値の減少量
#define SCALE_UP (4.0f) //ポリゴンの拡大率
#define SIZE_LOOP (0.02f) //ポリゴンの拡大率
#define SCALE_MAX (2.0f) //拡大の最大量
#define SCALE_MIN (0.0f) //拡大の最小量
#define MESSAGE_POLYGON_WIDTH (300.0f) //ポリゴンの幅
#define MESSAGE_POLYGON_HEIGHT (40.0f) //ポリゴンの高さ
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
LPDIRECT3DTEXTURE9 g_pTextureTitleMessage = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleMessage = NULL;
MESSAGE g_message;
float g_size;

//==========================================
//  初期化処理
//==========================================
void InitMessage()
{
	//変数の初期化
	g_message.pos = MESSAGE_POS_LEFT;
	g_message.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_message.fScale = 0.0f;
	g_size = SIZE_LOOP;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleMessage,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		PASS_TITLE_MESSAGE,
		&g_pTextureTitleMessage
	);

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffTitleMessage->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_message.pos.x - (MESSAGE_POLYGON_WIDTH + (g_message.fScale * SIZE_RATIO)), g_message.pos.y - (MESSAGE_POLYGON_HEIGHT + g_message.fScale), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_message.pos.x + (MESSAGE_POLYGON_WIDTH + (g_message.fScale * SIZE_RATIO)), g_message.pos.y - (MESSAGE_POLYGON_HEIGHT + g_message.fScale), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_message.pos.x - (MESSAGE_POLYGON_WIDTH + (g_message.fScale * SIZE_RATIO)), g_message.pos.y + (MESSAGE_POLYGON_HEIGHT + g_message.fScale), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_message.pos.x + (MESSAGE_POLYGON_WIDTH + (g_message.fScale * SIZE_RATIO)), g_message.pos.y + (MESSAGE_POLYGON_HEIGHT + g_message.fScale), 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//rhwの設定
		pVtx[nCnt].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[nCnt].col = g_message.col;
	}

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffTitleMessage->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitMessage()
{
	//頂点バッファの破棄
	if (g_pVtxBuffTitleMessage != NULL)
	{
		g_pVtxBuffTitleMessage->Release();
		g_pVtxBuffTitleMessage = NULL;
	}

	if (g_pTextureTitleMessage != NULL)
	{
		g_pTextureTitleMessage->Release();
		g_pTextureTitleMessage = NULL;
	}
}

//==========================================
//  更新処理
//==========================================
void UpdateMessage()
{
	//ローカル変数宣言
	int nTitleState = GetTitleState();

	//状態に対応した処理を実行する
	switch (nTitleState)
	{
	case TITLESTATE_OUT:
		//不透明度の更新
		g_message.col.a -= MESSAGE_ALPHA_SCALE;
		//サイズの更新
		g_message.fScale += SCALE_UP;
		break;
	default:
		//サイズの更新
		g_message.fScale += g_size;

		//拡縮の切り替え補正
		if (g_message.fScale < SCALE_MIN || g_message.fScale > SCALE_MAX)
		{
			g_size *= -1.0f;
		}
		break;
	}

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffTitleMessage->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(g_message.pos.x - (MESSAGE_POLYGON_WIDTH + (g_message.fScale * SIZE_RATIO)), g_message.pos.y - (MESSAGE_POLYGON_HEIGHT + g_message.fScale), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_message.pos.x + (MESSAGE_POLYGON_WIDTH + (g_message.fScale * SIZE_RATIO)), g_message.pos.y - (MESSAGE_POLYGON_HEIGHT + g_message.fScale), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_message.pos.x - (MESSAGE_POLYGON_WIDTH + (g_message.fScale * SIZE_RATIO)), g_message.pos.y + (MESSAGE_POLYGON_HEIGHT + g_message.fScale), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_message.pos.x + (MESSAGE_POLYGON_WIDTH + (g_message.fScale * SIZE_RATIO)), g_message.pos.y + (MESSAGE_POLYGON_HEIGHT + g_message.fScale), 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//頂点カラーの更新
		pVtx[nCnt].col = g_message.col;
	}

	//頂点バッファをアンロック
	g_pVtxBuffTitleMessage->Unlock();
}

//==========================================
//  描画処理
//==========================================
void DrawMessage()
{
	if (GetTitleState() != TITLESTATE_IN)
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffTitleMessage, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitleMessage);

		//ロゴの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}
