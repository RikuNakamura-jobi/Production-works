//==========================================
//
//  ポーズの枠(pause_frame.cpp)
//  Author : 佐久間優香
//
//==========================================
#include"pause_frame.h"

//==========================================
//  マクロ定義
//==========================================
#define PASS_PAUSE_FRAME "data\\TEXTURE\\pause_frame000.png" //テクスチャパス
#define FRAME_POS (D3DXVECTOR3(640.0f,400.0f, 0.0f)) //中心座標
#define FRAME_WIDTH (260.0f) //横幅
#define FRAME_HEIGHT (300.0f) //高さ

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTexturePauseFrame = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPauseFrame = NULL;

//==========================================
//  初期化処理
//==========================================
void InitPauseFrame()
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
		&g_pVtxBuffPauseFrame,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		PASS_PAUSE_FRAME,
		&g_pTexturePauseFrame
	);

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffPauseFrame->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(FRAME_POS.x - FRAME_WIDTH, FRAME_POS.y - FRAME_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(FRAME_POS.x + FRAME_WIDTH, FRAME_POS.y - FRAME_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(FRAME_POS.x - FRAME_WIDTH, FRAME_POS.y + FRAME_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(FRAME_POS.x + FRAME_WIDTH, FRAME_POS.y + FRAME_HEIGHT, 0.0f);

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
	g_pVtxBuffPauseFrame->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitPauseFrame()
{
	//頂点バッファの破棄
	if (g_pVtxBuffPauseFrame != NULL)
	{
		g_pVtxBuffPauseFrame->Release();
		g_pVtxBuffPauseFrame = NULL;
	}

	//テクスチャの破棄
	if (g_pTexturePauseFrame != NULL)
	{
		g_pTexturePauseFrame->Release();
		g_pTexturePauseFrame = NULL;
	}
}

//==========================================
//  更新処理
//==========================================
void UpdatePauseFrame()
{

}

//==========================================
//  描画処理
//==========================================
void DrawPauseFrame()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPauseFrame, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePauseFrame);

	//ロゴの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}