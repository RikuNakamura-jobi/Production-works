//===========================
//
//ダイレクトX.bgstageファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "bgstage.h"
#include "game.h"

//マクロ定義
#define NUM_Bgstage (12)											//背景の数
#define MAX_BGSTAGE (3)												//描画する背景の数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureBgstage[NUM_Bgstage] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffBgstage = NULL;					//頂点情報を格納
float g_aTexVStage[NUM_Bgstage];									//テクスチャ座標の開始位置
float g_aTexUStage[NUM_Bgstage];									//テクスチャ座標の開始位置

//===========================
//背景の初期化処理
//===========================
void InitBgStage(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBgstage;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\sakuramiti.png",
		&g_apTextureBgstage[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\sakurahubuki002.png",
		&g_apTextureBgstage[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\sakurahubuki002.png",
		&g_apTextureBgstage[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\yakei003.png",
		&g_apTextureBgstage[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg100.png",
		&g_apTextureBgstage[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg101.png",
		&g_apTextureBgstage[5]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\umikumo002.png",
		&g_apTextureBgstage[6]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\umikumo003.png",
		&g_apTextureBgstage[7]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\umikumo003.png",
		&g_apTextureBgstage[8]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\EX_BG000.png",
		&g_apTextureBgstage[9]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\EX_BG001.png",
		&g_apTextureBgstage[10]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\EX_BG002.png",
		&g_apTextureBgstage[11]);

	//テクスチャ座標の開始位置の初期化
	for (nCntBgstage = 0; nCntBgstage < NUM_Bgstage; nCntBgstage++)
	{
		g_aTexVStage[nCntBgstage] = 0.9f;
		g_aTexUStage[nCntBgstage] = 0.9f;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_Bgstage,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffBgstage,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffBgstage->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBgstage = 0; nCntBgstage < NUM_Bgstage; nCntBgstage++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(PLAYER_WIDTH_L, PLAYER_HEIGHT_U, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(PLAYER_WIDTH_R, PLAYER_HEIGHT_U, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(PLAYER_WIDTH_L, PLAYER_HEIGHT_D, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(PLAYER_WIDTH_R, PLAYER_HEIGHT_D, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
		pVtx[1].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
		pVtx[2].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
		pVtx[3].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f - (g_aTexUStage[nCntBgstage] * nCntBgstage * 0.3f), g_aTexVStage[nCntBgstage]);
		pVtx[1].tex = D3DXVECTOR2(1.0f - (g_aTexUStage[nCntBgstage] * nCntBgstage * 0.3f), g_aTexVStage[nCntBgstage]);
		pVtx[2].tex = D3DXVECTOR2(0.0f - (g_aTexUStage[nCntBgstage] * nCntBgstage * 0.3f), g_aTexVStage[nCntBgstage] + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f - (g_aTexUStage[nCntBgstage] * nCntBgstage * 0.3f), g_aTexVStage[nCntBgstage] + 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_VtxBuffBgstage->Unlock();
}

//===========================
//背景の終了処理
//===========================
void UninitBgStage(void)
{
	int nCntBgstage;

	for (nCntBgstage = 0; nCntBgstage < NUM_Bgstage; nCntBgstage++)
	{
		//テクスチャの破棄
		if (g_apTextureBgstage[nCntBgstage] != NULL)
		{
			g_apTextureBgstage[nCntBgstage]->Release();
			g_apTextureBgstage[nCntBgstage] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_VtxBuffBgstage != NULL)
	{
		g_VtxBuffBgstage->Release();
		g_VtxBuffBgstage = NULL;
	}
}

//===========================
//背景の更新処理
//===========================
void UpdateBgStage(void)
{
	int nCntBgstage;
	STAGESTATE stageState = GetStageState();
	
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffBgstage->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBgstage = 0; nCntBgstage < NUM_Bgstage; nCntBgstage++)
	{
		if (stageState == STAGESTATE_1 || stageState == STAGESTATE_2)
		{
			//テクスチャ座標の開始位置の更新
			g_aTexVStage[nCntBgstage] -= 0.007f * nCntBgstage + 0.0007f;
			g_aTexUStage[nCntBgstage] -= 0.01f * nCntBgstage + 0.001f;
		}
		else if (stageState == STAGESTATE_3)
		{
			//テクスチャ座標の開始位置の更新
			g_aTexVStage[nCntBgstage] -= 0.01f * (nCntBgstage + 1) + 0.002f;
			g_aTexUStage[nCntBgstage] -= 0.01f * (nCntBgstage + 1) + 0.003f;
		}
		else if (stageState == STAGESTATE_EX)
		{
			//テクスチャ座標の開始位置の更新
			g_aTexVStage[nCntBgstage] -= 0.007f * nCntBgstage + 0.0007f;
			g_aTexUStage[nCntBgstage] = 0.0f;
		}

		if (g_apTextureBgstage[nCntBgstage] <= 0)
		{
			g_aTexVStage[nCntBgstage] = 0.9f;
			g_aTexUStage[nCntBgstage] = 0.9f;
		}

		//テクスチャ座標の更新
		pVtx[0].tex = D3DXVECTOR2(0.0f - (g_aTexUStage[nCntBgstage] * nCntBgstage * 0.3f), g_aTexVStage[nCntBgstage]);
		pVtx[1].tex = D3DXVECTOR2(1.0f - (g_aTexUStage[nCntBgstage] * nCntBgstage * 0.3f), g_aTexVStage[nCntBgstage]);
		pVtx[2].tex = D3DXVECTOR2(0.0f - (g_aTexUStage[nCntBgstage] * nCntBgstage * 0.3f), g_aTexVStage[nCntBgstage] + 0.7f);
		pVtx[3].tex = D3DXVECTOR2(1.0f - (g_aTexUStage[nCntBgstage] * nCntBgstage * 0.3f), g_aTexVStage[nCntBgstage] + 0.7f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_VtxBuffBgstage->Unlock();
}

//===========================
//背景の描画処理
//===========================
void DrawBgStage(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBgstage;
	STAGESTATE stageState = GetStageState();

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_VtxBuffBgstage,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBgstage = 0; nCntBgstage < MAX_BGSTAGE; nCntBgstage++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureBgstage[nCntBgstage - 3 + (3 * (stageState))]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBgstage * 4, 2);
	}
}