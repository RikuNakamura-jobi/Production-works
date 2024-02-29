//========================================
//
//2Dシューティングゲーム(フェード処理)[fade.cpp]
//Author:佐久間優香
//
//========================================
#include "main.h"
#include "fade.h"

//マクロ定義
#define COL_R	 (0.0f)									//赤
#define COL_G	 (0.0f)									//緑
#define COL_B	 (0.0f)									//青
#define COL_A	 (1.0f)									//透明度
#define TEX_X1 (0.0f)						//テクスチャの座標x
#define TEX_X2 (1.0f)						//テクスチャの座標x
#define TEX_Y1 (0.0f)						//テクスチャの座標y
#define TEX_Y2 (1.0f)						//テクスチャの座標y

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;			//頂点バッファへのポインタ
FADE g_fade;											//フェードの状態
MODE g_modeNext;										//次の画面(モード)
D3DXCOLOR g_colorFade;									//ポリゴン(モード)の色

//========================================
//フェードの初期化処理
//========================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	VERTEX_2D *pVtx;									//頂点情報へのポインタ
	g_fade = FADE_IN;									//フェードイン状態に
	g_modeNext = modeNext;								//次の画面(モード)に設定
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//黒いポリゴン(不透明)にしておく

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		NULL,
		&g_pTextureFade
	);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	//テクスチャの座標の設定
	pVtx[0].tex = D3DXVECTOR2(TEX_X1, TEX_Y1);
	pVtx[1].tex = D3DXVECTOR2(TEX_X2, TEX_Y1);
	pVtx[2].tex = D3DXVECTOR2(TEX_X1, TEX_Y2);
	pVtx[3].tex = D3DXVECTOR2(TEX_X2, TEX_Y2);

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

	//モードの設定
	SetMode(g_modeNext);
}

//========================================
//フェードの終了処理
//========================================
void UninitFade(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//========================================
//フェードの更新処理
//========================================
void UpdateFade(void)
{
	VERTEX_2D *pVtx;									//頂点情報へのポインタ

	if (g_fade != FADE_NONE)
	{
		if (g_fade == FADE_IN)
		{//フェードイン状態
			g_colorFade.a -= 0.02f;		//ポリゴンを透明にしていく
			if (g_colorFade.a <= 0.0f)
			{
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;						//何もしていない状態
			}
		}
		else if (g_fade == FADE_OUT)
		{//フェードアウト状態
			g_colorFade.a += 0.02f;						//ポリゴンを不透明にする
			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;						//フェードイン状態に

				//モードの設定(次の画面に移動)
				SetMode(g_modeNext);
			}
		}
		//頂点バッファをロックし頂点情報へのポインタを取得
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx[0].col = g_colorFade;
		pVtx[1].col = g_colorFade;
		pVtx[2].col = g_colorFade;
		pVtx[3].col = g_colorFade;

		//頂点バッファをアンロックする
		g_pVtxBuffFade->Unlock();
	}
}

//========================================
//フェードの描画処理
//========================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFade);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
//フェードの設定処理
//========================================
void SetFade(MODE modeNext)
{
	if (g_fade == FADE_NONE)
	{
		g_fade = FADE_OUT;									//フェードアウト状態に
		g_modeNext = modeNext;								//次の画面(モード)を設定
		g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//黒いポリゴンを(透明)にしておく
	}
}

//========================================
//フェードの取得処理
//========================================
FADE GetFade(void)
{
	return g_fade;
}