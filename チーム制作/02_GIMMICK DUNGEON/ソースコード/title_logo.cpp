//==========================================
//
//  タイトルロゴ(title_logo.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "title_logo.h"
#include "title.h"

//==========================================
//  マクロ定義
//==========================================
#define PASS_TITLE_LOGO "data/TEXTURE/title002.png" //テクスチャパス
#define POS_DEFAULT (D3DXVECTOR3(640.0f, 80.0f ,0.0f)) //初期位置
#define END_Y (240.0f) //Y座標の上限
#define POS_END (D3DXVECTOR3(640.0f, END_Y ,0.0f)) //最終位置
#define LOGO_MOVE ((END_Y - 80.0f) / IN_TIME) //移動量
#define LOGO_ALPHA_SCALE (1.0f / IN_TIME) //アルファ値の上昇量
#define LOGO_POLYGON_WIDTH (500.0f) //ポリゴンの幅
#define LOGO_POLYGON_HEIGHT (180.0f) //ポリゴンの高さ

//==========================================
//  構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //中心座標
	D3DXCOLOR col; //ポリゴンカラー
}LOGO;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureTitleLogo = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;
LOGO g_logo;

//==========================================
//  初期化処理
//==========================================
void InitLogo()
{
	//変数の初期化
	g_logo.pos = POS_DEFAULT;
	g_logo.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleLogo,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		PASS_TITLE_LOGO,
		&g_pTextureTitleLogo
	);

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_logo.pos.x - LOGO_POLYGON_WIDTH, g_logo.pos.y - LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_logo.pos.x + LOGO_POLYGON_WIDTH, g_logo.pos.y - LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_logo.pos.x - LOGO_POLYGON_WIDTH, g_logo.pos.y + LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_logo.pos.x + LOGO_POLYGON_WIDTH, g_logo.pos.y + LOGO_POLYGON_HEIGHT, 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//rhwの設定
		pVtx[nCnt].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[nCnt].col = g_logo.col;
	}

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffTitleLogo->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitLogo()
{
	//頂点バッファの破棄
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureTitleLogo != NULL)
	{
		g_pTextureTitleLogo->Release();
		g_pTextureTitleLogo = NULL;
	}
}

//==========================================
//  更新処理
//==========================================
void UpdateLogo()
{
	//ローカル変数宣言
	int nTitleState = GetTitleState();

	//状態に対応した処理を実行する
	switch (nTitleState)
	{
	case TITLESTATE_IN:
		//ロゴの情報を更新する
		if (g_logo.pos.y < END_Y)
		{
			g_logo.pos.y += LOGO_MOVE;
			g_logo.col.a += LOGO_ALPHA_SCALE;
		}

		//ロゴの情報を補正する
		if (g_logo.pos.y > END_Y)
		{
			g_logo.pos.y = END_Y;
			g_logo.col.a = 1.0f;
		}
		break;
	case TITLESTATE_NORMAL:
		//位置を固定する
		if (g_logo.pos != POS_END)
		{
			g_logo.pos = POS_END;
		}
		//色を固定する
		if (g_logo.col.a != 1.0f)
		{
			g_logo.col.a = 1.0f;
		}
		break;
	default:
		break;
	}

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(g_logo.pos.x - LOGO_POLYGON_WIDTH, g_logo.pos.y - LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_logo.pos.x + LOGO_POLYGON_WIDTH, g_logo.pos.y - LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_logo.pos.x - LOGO_POLYGON_WIDTH, g_logo.pos.y + LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_logo.pos.x + LOGO_POLYGON_WIDTH, g_logo.pos.y + LOGO_POLYGON_HEIGHT, 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//頂点カラーの更新
		pVtx[nCnt].col = g_logo.col;
	}

	//頂点バッファをアンロック
	g_pVtxBuffTitleLogo->Unlock();
}

//==========================================
//  描画処理
//==========================================
void DrawLogo()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitleLogo);

	//ロゴの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
