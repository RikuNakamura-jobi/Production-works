//==========================================================
//
//タイトル画面処理 [title.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "polygon.h"

//==================================================
//マクロ宣言
//==================================================
#define MAX_TITLE	(2)

//==================================================
//構造体の定義
//==================================================

//==================================================
//グローバル変数宣言
//==================================================

//==================================================
//プロトタイプ宣言
//==================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE] = {};			//テクスチャ1へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;	//頂点バッファへのポインタ
polygon g_aTitlePolygon[MAX_TITLE];

//==================================================
//ファイル名
//==================================================
const char* c_apFilenameTitle[MAX_TITLE] =
{
	"data\\TEXTURE\\title.png",		//ロゴ元
	"data\\TEXTURE\\pressenter000.png",		//ロゴのコピー
	//"data\\TEXTURE\\enemy001.png",		//ロゴのコピー
};

//==================================================
//タイトル画面の初期化処理
//==================================================
void InitTitle(void)
{
	//ポリゴンの初期化
	InitPolygon();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	for (int nCntPolygon = 0; nCntPolygon < MAX_TITLE; nCntPolygon++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameTitle[nCntPolygon],
			&g_pTextureTitle[nCntPolygon]);

		g_aTitlePolygon[nCntPolygon].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.3f, 0.0f);
		g_aTitlePolygon[nCntPolygon].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTitlePolygon[nCntPolygon].fWidth = 300.0f;
		g_aTitlePolygon[nCntPolygon].fHeight = 200.0f;
	}

	g_aTitlePolygon[1].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.8f, 0.0f);
	g_aTitlePolygon[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aTitlePolygon[1].fWidth = 280.0f;
	g_aTitlePolygon[1].fHeight = 60.0f;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TITLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_TITLE; nCntPolygon++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aTitlePolygon[nCntPolygon].pos.x - g_aTitlePolygon[nCntPolygon].fWidth, g_aTitlePolygon[nCntPolygon].pos.y - g_aTitlePolygon[nCntPolygon].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTitlePolygon[nCntPolygon].pos.x + g_aTitlePolygon[nCntPolygon].fWidth, g_aTitlePolygon[nCntPolygon].pos.y - g_aTitlePolygon[nCntPolygon].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTitlePolygon[nCntPolygon].pos.x - g_aTitlePolygon[nCntPolygon].fWidth, g_aTitlePolygon[nCntPolygon].pos.y + g_aTitlePolygon[nCntPolygon].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTitlePolygon[nCntPolygon].pos.x + g_aTitlePolygon[nCntPolygon].fWidth, g_aTitlePolygon[nCntPolygon].pos.y + g_aTitlePolygon[nCntPolygon].fHeight, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

	PlaySound(SOUND_LABEL_TITLE);	//BGMの再生
}

//==================================================
//タイトル画面の終了処理
//==================================================
void UninitTitle(void)
{
	StopSound();	//BGMの停止

	//ポリゴンの終了処理
	UninitPolygon();

	//テクスチャの破棄
	for (int nCntPolygon = 0; nCntPolygon < MAX_TITLE; nCntPolygon++)
	{
		if (g_pTextureTitle[nCntPolygon] != NULL)
		{
			g_pTextureTitle[nCntPolygon]->Release();
			g_pTextureTitle[nCntPolygon] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//==================================================
//タイトル画面の更新処理
//==================================================
void UpdateTitle(void)
{
	//ポリゴンの更新処理
	UpdatePolygon();

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamepadTrigger(BUTTON_A, 0))
	{//遷移キーが押された
		SetFade(MODE_TUTORIAL);
		PlaySound(SOUND_LABEL_SEBUTTON);
	}
}

//==================================================
//タイトル画面の描画処理
//==================================================
void DrawTitle(void)
{
	//ポリゴンの描画処理
	DrawPolygon();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得

	for (int nCntPolygon = 0; nCntPolygon < MAX_TITLE; nCntPolygon++)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nCntPolygon]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
			4 * nCntPolygon,								//プリミティブ（ポリゴンの数）
			2);												//描画するプリミティブ数
	}
}
