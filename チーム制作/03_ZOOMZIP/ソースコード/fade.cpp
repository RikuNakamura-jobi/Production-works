//==========================================================
//
//フェード処理
//Author 佐藤根詩音
//
//==========================================================
#include "fade.h"

//マクロ定義
#define NUM_TEX		(2)		//テクスチャの数
#define WIDTH_FADE	(1280.0f * 10.0f)	//横の長さ
#define HEIGHT_FADE	(720.0f * 10.0f)	//縦の長さ
#define ROTATE_LENGTH	(1500.0f)		//回転フェードの対角線の長さ

//プロトタイプ宣言
void NormalFade(void);
void RotateFade(void);

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureFade[NUM_TEX] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	//頂点バッファへのポインタ
FADE g_fade;									//フェードの状態
Fade g_aFade;								//フェードの情報
MODE g_modeNext;								//次の画面(モード)
float g_fLengthFade;		//対角線の長さ
float g_fAngleFade;			//対角線の角度
float g_fOldLengthFade;		//対角線の長さ保存用

//==========================================================
//フェードの初期化処理
//==========================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&g_pTextureFade[FADETYPE_NORMAL]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\fade_rotate.png",
		&g_pTextureFade[FADETYPE_ROTATE]);

	g_aFade.pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);	//位置
	g_aFade.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	g_aFade.nType = FADETYPE_ROTATE;
	g_aFade.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//黒いポリゴンにしておく

	g_fade = FADE_IN;	//フェードイン状態に
	g_modeNext = modeNext;	//次の画面を設定

	//対角線の長さを算出する
	g_fLengthFade = sqrtf(WIDTH_FADE * WIDTH_FADE + HEIGHT_FADE * HEIGHT_FADE) * 0.5f;
	g_fOldLengthFade = g_fLengthFade;

	//対角線の角度を算出する
	g_fAngleFade = atan2f(WIDTH_FADE, HEIGHT_FADE);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	if (g_aFade.nType == FADETYPE_NORMAL)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	}
	else if (g_aFade.nType == FADETYPE_ROTATE)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_aFade.pos.x + sinf(g_aFade.rot.z + -D3DX_PI * 0.75f) * g_fLengthFade;
		pVtx[0].pos.y = g_aFade.pos.y + cosf(g_aFade.rot.z + -D3DX_PI * 0.75f) * g_fLengthFade;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aFade.pos.x + sinf(g_aFade.rot.z + D3DX_PI * 0.75f) * g_fLengthFade;
		pVtx[1].pos.y = g_aFade.pos.y + cosf(g_aFade.rot.z + D3DX_PI * 0.75f) * g_fLengthFade;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aFade.pos.x + sinf(g_aFade.rot.z + -D3DX_PI * 0.25f) * g_fLengthFade;
		pVtx[2].pos.y = g_aFade.pos.y + cosf(g_aFade.rot.z + -D3DX_PI * 0.25f) * g_fLengthFade;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aFade.pos.x + sinf(g_aFade.rot.z + D3DX_PI * 0.25f) * g_fLengthFade;
		pVtx[3].pos.y = g_aFade.pos.y + cosf(g_aFade.rot.z + D3DX_PI * 0.25f) * g_fLengthFade;
		pVtx[3].pos.z = 0.0f;
	}

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[1].col = D3DXCOLOR(g_aFade.col.r, g_aFade.col.g, g_aFade.col.b, g_aFade.col.a);
	pVtx[2].col = D3DXCOLOR(g_aFade.col.r, g_aFade.col.g, g_aFade.col.b, g_aFade.col.a);
	pVtx[3].col = D3DXCOLOR(g_aFade.col.r, g_aFade.col.g, g_aFade.col.b, g_aFade.col.a);
	pVtx[0].col = D3DXCOLOR(g_aFade.col.r, g_aFade.col.g, g_aFade.col.b, g_aFade.col.a);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

	//モードの設定
	SetMode(g_modeNext);
}

//==========================================================
//フェードの終了処理
//==========================================================
void UninitFade(void)
{
	for (int nCntFade = 0; nCntFade < NUM_TEX; nCntFade++)
	{
		//テクスチャの破棄
		if (g_pTextureFade[nCntFade] != NULL)
		{
			g_pTextureFade[nCntFade]->Release();
			g_pTextureFade[nCntFade] = NULL;
		}
	}

	//頂点バッファの廃棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//==========================================================
//フェードの更新処理
//==========================================================
void UpdateFade(void)
{
	if (g_aFade.nType == FADETYPE_NORMAL)
	{
		//通常フェード
		NormalFade();
	}
	else if (g_aFade.nType == FADETYPE_ROTATE)
	{
		//回転フェード
		RotateFade();
	}

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	if (g_aFade.nType == FADETYPE_NORMAL)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	}
	else if (g_aFade.nType == FADETYPE_ROTATE)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_aFade.pos.x + sinf(g_aFade.rot.z + -D3DX_PI * 0.75f) * g_fLengthFade;
		pVtx[0].pos.y = g_aFade.pos.y + cosf(g_aFade.rot.z + -D3DX_PI * 0.75f) * g_fLengthFade;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aFade.pos.x + sinf(g_aFade.rot.z + D3DX_PI * 0.75f) * g_fLengthFade;
		pVtx[1].pos.y = g_aFade.pos.y + cosf(g_aFade.rot.z + D3DX_PI * 0.75f) * g_fLengthFade;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aFade.pos.x + sinf(g_aFade.rot.z + -D3DX_PI * 0.25f) * g_fLengthFade;
		pVtx[2].pos.y = g_aFade.pos.y + cosf(g_aFade.rot.z + -D3DX_PI * 0.25f) * g_fLengthFade;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aFade.pos.x + sinf(g_aFade.rot.z + D3DX_PI * 0.25f) * g_fLengthFade;
		pVtx[3].pos.y = g_aFade.pos.y + cosf(g_aFade.rot.z + D3DX_PI * 0.25f) * g_fLengthFade;
		pVtx[3].pos.z = 0.0f;
	}

	//頂点カラーの設定
	pVtx[1].col = g_aFade.col;
	pVtx[2].col = g_aFade.col;
	pVtx[3].col = g_aFade.col;
	pVtx[0].col = g_aFade.col;

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();
}

//==========================================================
//通常フェード処理
//==========================================================
void NormalFade(void)
{
	if (g_fade != FADE_NONE)
	{//何もしていない状態以外
		if (g_fade == FADE_IN)
		{//フェードイン状態
			g_aFade.col.a -= 1.0f / 30;	//透明に近づけていく

			if (g_aFade.col.a <= 0.0f)
			{//完全に透明になった場合
				g_aFade.col.a = 0.0f;	//透明度を透明に
				g_fade = FADE_NONE;	//何もしていない状態に
			}
		}
		else if (g_fade == FADE_OUT)
		{//フェードアウト状態

			g_aFade.col.a += 1.0f / 30;	//不透明に近づけていく

			if (g_aFade.col.a >= 1.0f)
			{//完全に不透明になった場合
				g_aFade.col.a = 1.0f;	//透明度を不透明に

				//モードの設定(次の画面に移行)
				SetMode(g_modeNext);

				g_fade = FADE_IN;	//何もしていない状態に
				g_aFade.nType = FADETYPE_ROTATE;


			}
		}
	}
}

//==========================================================
//回転フェード処理
//==========================================================
void RotateFade(void)
{
	if (g_fade != FADE_NONE)
	{//何もしていない状態以外
		if (g_fade == FADE_IN)
		{//フェードイン状態

			g_fLengthFade += ROTATE_LENGTH;

			if (g_fLengthFade >= 14000.0f && g_aFade.col.a > 0.0f)
			{
				g_aFade.col.a -= 1.0f / 25;	//透明に近づけていく
			}

			if (g_fLengthFade >= 46000.0f)
			{//完全に透明になった場合
				g_aFade.col.a = 0.0f;	//透明度を透明に
				g_fade = FADE_NONE;		//何もしていない状態に
			}

			if (g_fLengthFade + ROTATE_LENGTH >= 46000.0f)
			{
				g_fLengthFade = 46000.0f;
			}
		}
		else if (g_fade == FADE_OUT)
		{//フェードアウト状態

			g_fLengthFade -= ROTATE_LENGTH;

			if (g_fLengthFade <= 901.0f)
			{//完全に不透明になった場合
				g_aFade.col.a = 1.0f;	//透明度を不透明に

				g_fLengthFade = g_fOldLengthFade;		//元の長さ

				//モードの設定(次の画面に移行)
				SetMode(g_modeNext);
				g_aFade.nType = FADETYPE_ROTATE;
				g_fade = FADE_IN;	//何もしていない状態に
			}

			if (g_fLengthFade - ROTATE_LENGTH <= 901.0f)
			{//完全に不透明になった場合
				g_fLengthFade = 901.0f;
				g_aFade.nType = FADETYPE_NORMAL;
			}
		}
	}
}

//==========================================================
//フェードの描画処理
//==========================================================
void DrawFade(void)
{
	if (g_fade != FADE_NONE)
	{//何もしていない状態以外の場合
		PDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(
			0,
			g_pVtxBuffFade,
			0,
			sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		if (g_aFade.nType == FADETYPE_NORMAL)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, NULL);
		}
		else if (g_aFade.nType == FADETYPE_ROTATE)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureFade[1]);
		}

		//フェードの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			0,
			2	//頂点情報構造体のサイズ
		);
	}
}

//==========================================================
//フェードの透明度取得
//==========================================================
FADE GetFade(void)
{
	//透明度を返す
	return g_fade;
}

//==========================================================
//フェードの状態設定
//==========================================================
void SetFade(MODE modeNext)
{
	if (g_fade == FADE_NONE)
	{
		g_fade = FADE_OUT;	//フェードアウト状態に
		g_modeNext = modeNext;	//次の画面(モード)に

		g_aFade.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//透明に設定
	}
}

//==========================================================
//フェードの情報
//==========================================================
Fade *GetFadeInfo(void)
{
	return &g_aFade;
}