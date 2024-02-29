//==========================================
//
//  メニュー項目(menu_item.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "menu.h"
#include "menu_item.h"
#include "input.h"

//==========================================
//  列挙型定義
//==========================================
typedef enum
{
	ITEM_START = 0, //スタート項目
	ITEM_TUTORIAL, //チュートリアル項目
	ITEM_QUIT, //タイトルに戻る
	ITEM_MAX
}ITEMPATTERN; //メニュー項目

//==========================================
//  マクロ定義
//==========================================
#define TOP_POS (380.0f) //一番上のポリゴンのY座標
#define ITEM_WIDTH (350.0f) //ポリゴン幅の基準値
#define ITEM_HEIGHT (55.0f) //ポリゴン高の基準値
#define ITEM_WIDHT_NONE (ITEM_WIDTH * 0.9) //非選択ポリゴンの幅
#define ITEM_HEIGHT_NONE (ITEM_HEIGHT * 0.9) //非選択ポリゴンの高さ
#define ITEM_WIDHT_SELECT (ITEM_WIDTH * 1.1) //非選択ポリゴンの幅
#define ITEM_HEIGHT_SELECT (ITEM_HEIGHT * 1.1) //非選択ポリゴンの高さ
#define POLYGON_ALPHA (0.01f) //1fで変化するアルファ値
#define ITEM_BITWEEN (0.07f) //ポリゴンの拡縮率
#define ITEM_GRAW (0.05f) //ポリゴンの拡縮率

//メニューのファイル名を保持
const char *c_apFilenameMenu[] =
{
	"data\\TEXTURE\\gamestart000.png",
	"data\\TEXTURE\\tutorial000.png",
	"data\\TEXTURE\\quit001.png",
};

//==========================================
//  構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //中心座標
	float height; //ポリゴンの高さ
	float width; //ポリゴンの幅
	D3DXCOLOR col; //ポリゴンカラー
	bool bSelect; //選択状態
}ITEM;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureMenuItem[ITEM_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMenuItem = NULL;
int g_nState;
ITEM g_Item[ITEM_MAX];
float g_fItemAlpha;

//==========================================
//  初期化処理
//==========================================
void InitMenuItem()
{
	//ローカル変数宣言
	int nState = GetMenuState();
	g_nState = ITEM_START;

	//変数の初期化
	for (int nCntBuff = 0; nCntBuff < ITEM_MAX; nCntBuff++)
	{
		g_Item[nCntBuff].pos = D3DXVECTOR3
		(
			SCREEN_WIDTH * 0.5f,
			TOP_POS + ((ITEM_HEIGHT * 2.0f) * nCntBuff),
			0.0f
		);
		if (nCntBuff == nState)
		{
			g_Item[nCntBuff].width = ITEM_WIDHT_SELECT;
			g_Item[nCntBuff].height = ITEM_HEIGHT_SELECT;
		}
		else
		{
			g_Item[nCntBuff].width = ITEM_WIDHT_NONE;
			g_Item[nCntBuff].height = ITEM_HEIGHT_NONE;
		}
		g_Item[nCntBuff].bSelect = false;
		g_Item[nCntBuff].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	g_fItemAlpha = POLYGON_ALPHA;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * ITEM_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMenuItem,
		NULL
	);

	//テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < ITEM_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile
		(
			pDevice,
			c_apFilenameMenu[nCntTex],
			&g_pTextureMenuItem[nCntTex]
		);
	}

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffMenuItem->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	for (int nCntBuff = 0; nCntBuff < ITEM_MAX; nCntBuff++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_Item[nCntBuff].pos.x - g_Item[nCntBuff].width, g_Item[nCntBuff].pos.y - g_Item[nCntBuff].height, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Item[nCntBuff].pos.x + g_Item[nCntBuff].width, g_Item[nCntBuff].pos.y - g_Item[nCntBuff].height, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Item[nCntBuff].pos.x - g_Item[nCntBuff].width, g_Item[nCntBuff].pos.y + g_Item[nCntBuff].height, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Item[nCntBuff].pos.x + g_Item[nCntBuff].width, g_Item[nCntBuff].pos.y + g_Item[nCntBuff].height, 0.0f);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//rhwの設定
			pVtx[nCnt].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[nCnt].col = g_Item[nCntBuff].col;
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点データを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffMenuItem->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitMenuItem()
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < ITEM_MAX; nCnt++)
	{
		if (g_pTextureMenuItem[nCnt] != NULL)
		{
			g_pTextureMenuItem[nCnt]->Release();
			g_pTextureMenuItem[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMenuItem != NULL)
	{
		g_pVtxBuffMenuItem->Release();
		g_pVtxBuffMenuItem = NULL;
	}

}

//==========================================
//  更新処理
//==========================================
void UpdateMenuItem()
{
	//メニューの選択状態を取得
	int nState = GetMenuState();

	//メニュー項目の点滅
	if (g_Item[nState].col.a < 0.0f || g_Item[nState].col.a > 1.0f)
	{
		g_fItemAlpha *= -1.0f;
	}
	g_Item[nState].col.a += g_fItemAlpha;

	//選択、非選択時の仕様
	if (nState != MENUSTATE_FADE)
	{
		for (int nCnt = 0; nCnt < ITEM_MAX; nCnt++)
		{
			if (nCnt == nState) //選択時
			{
				//対象のポリゴンを拡大する
				if (g_Item[nCnt].width < ITEM_WIDHT_SELECT)
				{
					g_Item[nCnt].width += g_Item[nCnt].width * ITEM_BITWEEN;
				}
				if (g_Item[nCnt].height < ITEM_HEIGHT_SELECT)
				{
					g_Item[nCnt].height += g_Item[nCnt].height * ITEM_BITWEEN;
				}
			}
			else //非選択時
			{
				//対象のポリゴンを縮小する
				if (g_Item[nCnt].width > ITEM_WIDHT_NONE)
				{
					g_Item[nCnt].width -= g_Item[nCnt].width * ITEM_BITWEEN;
				}
				if (g_Item[nCnt].height > ITEM_HEIGHT_NONE)
				{
					g_Item[nCnt].height -= g_Item[nCnt].height * ITEM_BITWEEN;
				}

				//頂点カラーをffffffで固定する
				g_Item[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
	}
	
	//フェード中の挙動
	//if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) || GetJoyPadButtonTrigger(KEY_A, 0) || GetJoyPadButtonTrigger(KEY_START, 0) || GetMouseTrigger(MOUSEBUTTON_LEFT))
	if(GetEnterFrag())
	{
		//選択決定フラグ
		g_Item[g_nState].bSelect = true;
	}

	if(g_Item[g_nState].bSelect)
	{
		//ポリゴンの拡大
		g_Item[g_nState].width += g_Item[g_nState].width * ITEM_GRAW;
		g_Item[g_nState].height += g_Item[g_nState].height * ITEM_GRAW;

		//不透明度の変更
		if (g_Item[g_nState].col.a > 0.0f)
		{
			g_Item[g_nState].col.a -= g_Item[g_nState].col.a * ITEM_GRAW * 3;
		}
		else
		{
			g_Item[g_nState].col.a = 0.0f;
		}
	}
	else
	{
		//選択状態を変更
		if (GetKeyboardTrigger(DIK_W) || GetStickTriggerL(STICK_UP, 0) || GetJoyPadCrossTrigger(CROSS_UP, 0))
		{
			g_nState += ITEM_MAX - 1;
			g_nState %= ITEM_MAX;
		}
		if (GetKeyboardTrigger(DIK_S) || GetStickTriggerL(STICK_DOWN, 0) || GetJoyPadCrossTrigger(CROSS_DOWN, 0))
		{
			g_nState += 1;
			g_nState %= ITEM_MAX;
		}
	}

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffMenuItem->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の設定
	for (int nCntBuff = 0; nCntBuff < ITEM_MAX; nCntBuff++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Item[nCntBuff].pos.x - g_Item[nCntBuff].width, g_Item[nCntBuff].pos.y - g_Item[nCntBuff].height, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Item[nCntBuff].pos.x + g_Item[nCntBuff].width, g_Item[nCntBuff].pos.y - g_Item[nCntBuff].height, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Item[nCntBuff].pos.x - g_Item[nCntBuff].width, g_Item[nCntBuff].pos.y + g_Item[nCntBuff].height, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Item[nCntBuff].pos.x + g_Item[nCntBuff].width, g_Item[nCntBuff].pos.y + g_Item[nCntBuff].height, 0.0f);

		//頂点カラーの設定
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].col = g_Item[nCntBuff].col;
		}

		//頂点データを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffMenuItem->Unlock();
}

//==========================================
//  描画処理
//==========================================
void DrawMenuItem()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMenuItem, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < ITEM_MAX; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMenuItem[nCnt]);

		//メニュー項目の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}
