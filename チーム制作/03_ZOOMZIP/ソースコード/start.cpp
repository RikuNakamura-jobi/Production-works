//==========================================================
//
//ポリゴン処理 [Start.h]
//Author Ibuki Okusada
//
//==========================================================
#include "start.h"
#include "texture.h"
#include "fileload.h"
#include "texture.h"
#include "player.h"
#include "input.h"
#include "game.h"
#include "sound.h"

//==========================================================
//マクロ定義
//==========================================================
#define MAX_POLYGON		(1)

//==========================================================
//グローバル変数
//==========================================================
LPDIRECT3DTEXTURE9 g_pTextureStart[MAX_POLYGON] = {};			//テクスチャ1へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStart = NULL;	//頂点バッファへのポインタ
Start g_aStart[MAX_POLYGON];

//==========================================================
//ポリゴンの初期化処理
//==========================================================
void InitStart(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	for (int nCntStart = 0; nCntStart < MAX_POLYGON; nCntStart++)
	{
		if (GetMode() == MODE_GAME && nCntStart == MAX_POLYGON - 1)
		{
			g_pTextureStart[nCntStart] = *SetTexture(3);
		}

		g_aStart[nCntStart].pos = {};
		g_aStart[nCntStart].rot = {};
		g_aStart[nCntStart].fWidth = 0.0f;
		g_aStart[nCntStart].fHeight = 0.0f;
		g_aStart[nCntStart].fTexU = 0.0f;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStart,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStart = 0; nCntStart < MAX_POLYGON; nCntStart++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.15f, 200.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.85f, 200.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.15f, 390.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.85f, 390.0f, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(g_aStart[nCntStart].fTexU, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aStart[nCntStart].fTexU + 1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aStart[nCntStart].fTexU, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aStart[nCntStart].fTexU + 1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffStart->Unlock();
}

//==========================================================
//ポリゴンの終了処理
//==========================================================
void UninitStart(void)
{

	//テクスチャの破棄
	for (int nCntStart = 0; nCntStart < MAX_POLYGON; nCntStart++)
	{
		if (g_pTextureStart[nCntStart] != NULL)
		{
			g_pTextureStart[nCntStart]->Release();
			g_pTextureStart[nCntStart] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffStart != NULL)
	{
		g_pVtxBuffStart->Release();
		g_pVtxBuffStart = NULL;
	}
}

//==========================================================
//ポリゴンの更新処理
//==========================================================
void UpdateStart(void)
{
	if (GetKeyboardTrigger(DIK_SPACE) ||
		GetGamepadTrigger(BUTTON_A, 0))
	{
		SetGameState(GAMESTATE_NORMAL, 0);
		PlaySound(SOUND_LABEL_SEBUTTON);
	}
}

//==========================================================
//ポリゴンの描画処理
//==========================================================
void DrawStart(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得

	for (int nCntStart = 0; nCntStart < MAX_POLYGON; nCntStart++)
	{

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffStart, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureStart[nCntStart]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
			4 * nCntStart,								//プリミティブ（ポリゴンの数）
			2);												//描画するプリミティブ数
	}
}