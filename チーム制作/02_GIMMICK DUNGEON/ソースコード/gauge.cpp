//==================================================================================================
//
//gauge.cpp
//Author:髙山桃也
//
//==================================================================================================

//===============================
// インクルード
//===============================
#include "main.h"
#include "gauge.h"
#include "player.h"

//===============================
// マクロ定義
//===============================
#define NUM_TEX		(2)	//テクスチャの数
#define MAX_GAUGE	(16)	//ゲージの最大数(ゲージと枠があるから二倍する)

#define PARAM_FACT	(0.1f)	//パラメーターの慣性の係数
#define MULTIPLE_POLYGON	(4)	//ポリゴン数の倍数　ゲージ、枠、背景、差分の色違いの分
#define DIFF_RANGE	(0.001f)	//差分ゲージが動き出すまでの差分範囲
#define RATE_FRAME	(2.0f)	//枠のゲージの割合

//===============================
// グローバル変数宣言
//===============================
LPDIRECT3DTEXTURE9 g_pTextureGauge[NUM_TEX] = {};								//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;									//頂点バッファへのポインタ
const char *c_apFilenameGauge[NUM_TEX] =
{
	"data\\TEXTURE\\life000.png",
	"data\\TEXTURE\\gauge_frame.png"
};																				//テクスチャファイル名
Gauge g_aGauge[MAX_GAUGE];	//ゲージの最大数

//==================================================================================================
//初期化処理
//==================================================================================================
void InitGauge(void)
{
	//デバイスポインタ設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//情報取得
	Gauge *pGauge = GetGauge();

	for (int nCntTex = 0;nCntTex < NUM_TEX;nCntTex++)
	{
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameGauge[nCntTex],
			&g_pTextureGauge[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GAUGE * MULTIPLE_POLYGON,	
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL);

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	//変数初期化
	ZeroMemory(pGauge, sizeof(Gauge) * MAX_GAUGE);

	for (int nCntGauge = 0;nCntGauge < MAX_GAUGE * MULTIPLE_POLYGON;nCntGauge++,pGauge++)
	{
		//頂点情報初期化=====================
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		pVtx[1].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		pVtx[2].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		pVtx[3].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//ポインタを進める
		pVtx += 4;
		//頂点情報初期化=====================
	}

	//頂点バッファをアンロック
	g_pVtxBuffGauge->Unlock();
}

//==================================================================================================
//終了処理
//==================================================================================================
void UninitGauge(void)
{
	for (int nCntGauge = 0;nCntGauge < NUM_TEX;nCntGauge++)
	{
		if (g_pTextureGauge[nCntGauge] != NULL)
		{//テクスチャへのポインタの破棄
			g_pTextureGauge[nCntGauge]->Release();
			g_pTextureGauge[nCntGauge] = NULL;
		}
	}

	if (g_pVtxBuffGauge != NULL)
	{//頂点バッファへのポインタの破棄
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}

//==================================================================================================
//更新処理
//==================================================================================================
void UpdateGauge(void)
{
	//変数宣言
	float fLength;
	float fGaugeDiff;

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	//情報取得
	Gauge *pGauge = GetGauge();

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++, pGauge++)
	{
		if (pGauge->bUse)
		{//使用されていたら
			//パラメーターを目的の値に寄せる
			fGaugeDiff = pGauge->fParamDest - pGauge->fParam;

			if (fGaugeDiff > 0.0f)
			{//差分がプラスの場合
				pGauge->fParam += pGauge->fParamMax * pGauge->fRateChange;

				if (pGauge->fParam > pGauge->fParamDest)
				{//目標の値に補正
					pGauge->fParam = pGauge->fParamDest;
				}
			}
			else if(fGaugeDiff < 0.0f)
			{//差分がマイナスの場合
				pGauge->fParam -= pGauge->fParamMax * pGauge->fRateChange;

				if (pGauge->fParam < pGauge->fParamDest)
				{//目標の値に補正
					pGauge->fParam = pGauge->fParamDest;
				}
			}

			//pGauge->fParam += fGaugeDiff * PARAM_FACT;

			//パラメーターの割合を算出
			fLength = pGauge->fParam / pGauge->fParamMax;

			//頂点カラーの設定
			pVtx[0].col = pGauge->col;
			pVtx[1].col = pGauge->col;
			pVtx[2].col = pGauge->col;
			pVtx[3].col = pGauge->col;

			//種類ごとの頂点情報の設定
			switch (pGauge->type)
			{
			case GAUGETYPE_VERTICAL:
				//垂直タイプ=======================
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pGauge->pos.x - pGauge->width * 0.5f, pGauge->pos.y - pGauge->height * fLength, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * 0.5f, pGauge->pos.y - pGauge->height * fLength, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pGauge->pos.x - pGauge->width * 0.5f, pGauge->pos.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * 0.5f, pGauge->pos.y, 0.0f);

				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f * fLength);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f * fLength);

				//ゲージ差分までポインタを進める
				pVtx += 4 * MAX_GAUGE * 3;

				//ゲージ差分の頂点情報設定
				//頂点座標の設定
				pVtx[2].pos = D3DXVECTOR3(pGauge->pos.x - pGauge->width * 0.5f, pGauge->pos.y - pGauge->height * fLength, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * 0.5f, pGauge->pos.y - pGauge->height * fLength, 0.0f);

				//差分ゲージの収束
				if (fGaugeDiff * fGaugeDiff < DIFF_RANGE * DIFF_RANGE)
				{//差分が一定以下のときのみ更新
					pVtx[0].pos += (pVtx[2].pos - pVtx[0].pos) * PARAM_FACT;
					pVtx[1].pos += (pVtx[3].pos - pVtx[1].pos) * PARAM_FACT;
				}
				//垂直タイプ=======================
				break;

			case GAUGETYPE_HORIZON:
				//水平タイプ=======================
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pGauge->pos.x, pGauge->pos.y - pGauge->height * 0.5f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * fLength, pGauge->pos.y - pGauge->height * 0.5f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pGauge->pos.x, pGauge->pos.y + pGauge->height * 0.5f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * fLength, pGauge->pos.y + pGauge->height * 0.5f, 0.0f);

				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(1.0f - fLength, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f - fLength, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				//ゲージ差分までポインタを進める
				pVtx += 4 * MAX_GAUGE * 3;

				//ゲージ差分の頂点情報設定
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * fLength, pGauge->pos.y - pGauge->height * 0.5f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * fLength, pGauge->pos.y + pGauge->height * 0.5f, 0.0f);

				//差分ゲージの収束
				if (fGaugeDiff * fGaugeDiff < DIFF_RANGE * DIFF_RANGE)
				{//差分が一定以下のときのみ更新
					pVtx[1].pos += (pVtx[0].pos - pVtx[1].pos) * PARAM_FACT;
					pVtx[3].pos += (pVtx[2].pos - pVtx[3].pos) * PARAM_FACT;
				}
				
				//水平タイプ=======================
				break;
			default:
				break;
			}

			//ポインタを戻す
			pVtx -= 4 * MAX_GAUGE * 3;
		}

		//ポインタを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffGauge->Unlock();
}

//==================================================================================================
//描画処理
//==================================================================================================
void DrawGauge(void)
{
	//デバイスポインタ設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//情報取得
	Gauge *pGauge = GetGauge();

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++, pGauge++)
	{
		if (pGauge->bUse)
		{//使用されていたら
			//背景の描画=========================
			//テクスチャ設定
			pDevice->SetTexture(0, NULL);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4 + MAX_GAUGE * 4 * 2, 2);

			//差分ゲージの描画=========================
			//テクスチャ設定
			pDevice->SetTexture(0, NULL);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4 + MAX_GAUGE * 4 * 3, 2);

			//ゲージの描画=========================
			//テクスチャ設定
			pDevice->SetTexture(0, g_pTextureGauge[0]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4, 2);

			//枠の描画=========================
			//テクスチャ設定
			pDevice->SetTexture(0, g_pTextureGauge[1]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4 + MAX_GAUGE * 4, 2);
		}
	}
}

//==================================================================================================
// 設定処理
//==================================================================================================
int SetGauge(D3DXVECTOR3 pos,float fParamMax, float height, float width, GAUGETYPE type, float fRateChange)
{
	//変数宣言
	int nCntGauge = -1;

	//情報取得
	Gauge *pGauge = GetGauge();

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_GAUGE; nCount++, nCntGauge++, pGauge++)
	{
		if (pGauge->bUse == false)
		{//使用していない状態なら

			//引数の受け取り
			pGauge->fParamMax = fParamMax;
			pGauge->fParam = fParamMax;
			pGauge->fParamDest = fParamMax;
			pGauge->fRateChange = fRateChange;
			pGauge->height = height;
			pGauge->width = width;
			pGauge->pos = pos;
			pGauge->type = type;

			//ゲージの設定========================================
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - width * 0.5f, pos.y - height, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + width * 0.5f, pos.y - height, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - width * 0.5f, pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + width * 0.5f, pos.y, 0.0f);

			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//ゲージの設定========================================

			//枠までポインタを進める
			pVtx += 4 * MAX_GAUGE;

			//枠の設定========================================
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y - height * 0.5f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + height * RATE_FRAME, pos.y - height * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x, pos.y + height * 0.5f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + height * RATE_FRAME, pos.y + height * 0.5f, 0.0f);

			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//枠の設定========================================

			//背景までポインタを進める
			pVtx += 4 * MAX_GAUGE;

			//背景の設定========================================
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y - height * 0.5f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + width, pos.y - height * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x, pos.y + height * 0.5f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + width, pos.y + height * 0.5f, 0.0f);

			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(200, 200, 200, 200);
			pVtx[1].col = D3DCOLOR_RGBA(200, 200, 200, 200);
			pVtx[2].col = D3DCOLOR_RGBA(200, 200, 200, 200);
			pVtx[3].col = D3DCOLOR_RGBA(200, 200, 200, 200);

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//背景の設定========================================

			//差分ゲージまでポインタを進める
			pVtx += 4 * MAX_GAUGE;

			//差分ゲージの設定========================================
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - width * 0.5f, pos.y - height, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x, pos.y - height * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - width * 0.5f, pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x, pos.y + height * 0.5f, 0.0f);

			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 215, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 215, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 215, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 215, 0, 255);

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//差分ゲージの設定========================================

			nCntGauge = nCount;

			//使用状態にする
			pGauge->bUse = true;

			//for文を抜ける
			break;
		}

		//ポインタを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffGauge->Unlock();

	return nCntGauge;
}

//==================================================================================================
// 情報設定処理
//==================================================================================================
void SetInfoGauge(int nIdx, D3DXCOLOR col, float fParam)
{
	if (nIdx == -1)
	{//メモリ破壊防止
		return;
	}

	//情報取得
	Gauge *pGauge = GetGauge();

	//色の設定
	pGauge[nIdx].col = col;

	//パラメーターの設定
	pGauge[nIdx].fParamDest = fParam;
}

//==================================================================================================
// 情報取得処理
//==================================================================================================
Gauge *GetGauge(void)
{
	return &g_aGauge[0];
}