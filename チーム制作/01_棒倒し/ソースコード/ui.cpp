//===========================
//
//ダイレクトX.uiファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "ui.h"
#include "title.h"
#include "result.h"
#include "input.h"
#include <stdio.h>

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureUi[UITEXTURE_MAX] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffUi = NULL;						//頂点情報を格納
UI g_Ui[UITEXTURE_MAX];
float g_fCntTitle = 0.0f;										//タイトルの点滅用変数

//テクスチャファイル名
const char *c_apFilenameUi[UITEXTURE_MAX] =
{
	"data\\TEXTURE\\title.png",
	"data\\TEXTURE\\pressenter.png",
	"data\\TEXTURE\\tutorial_under00.png",
	"data\\TEXTURE\\tutorial_under01.png",
	"data\\TEXTURE\\tutorial_under02.png",
	"data\\TEXTURE\\clear.png",
	"data\\TEXTURE\\GameOver.png"
};

//===========================
//背景の初期化処理
//===========================
void InitUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi;
	char aText[128];
	FILE *pFile = fopen("data\\TEXTURE\\UI.txt", "r");

	if (pFile == NULL)
	{
		return;
	}

	fgets(&aText[0], 128, pFile);

	for (int nCnt = 0; nCnt < UITEXTURE_MAX; nCnt++)
	{
		fscanf(pFile, "%s = %f %f %f %f %f %d", &aText[0], &g_Ui[nCnt].pos.x, &g_Ui[nCnt].pos.y, &g_Ui[nCnt].pos.z, &g_Ui[nCnt].fWidth, &g_Ui[nCnt].fHeight, &g_Ui[nCnt].nType);
	}

	fclose(pFile);

	g_fCntTitle = 0.0f;

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < UITEXTURE_MAX; nCnt++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameUi[nCnt],
			&g_apTextureUi[nCnt]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * UITEXTURE_MAX,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffUi,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffUi->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntUi = 0; nCntUi < UITEXTURE_MAX; nCntUi++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Ui[nCntUi].pos.x - g_Ui[nCntUi].fWidth, g_Ui[nCntUi].pos.y - g_Ui[nCntUi].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Ui[nCntUi].pos.x + g_Ui[nCntUi].fWidth, g_Ui[nCntUi].pos.y - g_Ui[nCntUi].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Ui[nCntUi].pos.x - g_Ui[nCntUi].fWidth, g_Ui[nCntUi].pos.y + g_Ui[nCntUi].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Ui[nCntUi].pos.x + g_Ui[nCntUi].fWidth, g_Ui[nCntUi].pos.y + g_Ui[nCntUi].fHeight, 0.0f);

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
	g_VtxBuffUi->Unlock();
}

//===========================
//背景の終了処理
//===========================
void UninitUi(void)
{
	int nCntUi;

	for (nCntUi = 0; nCntUi < UITEXTURE_MAX; nCntUi++)
	{
		//テクスチャの破棄
		if (g_apTextureUi[nCntUi] != NULL)
		{
			g_apTextureUi[nCntUi]->Release();
			g_apTextureUi[nCntUi] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_VtxBuffUi != NULL)
	{
		g_VtxBuffUi->Release();
		g_VtxBuffUi = NULL;
	}
}

//===========================
//背景の更新処理
//===========================
void UpdateUi(void)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	int nCntUi;

	VERTEX_2D *pVtx;

	if (GetTitle() == false)
	{
		col.a = (sinf(g_fCntTitle) + 1.0f) / 2;

		g_fCntTitle += 0.05f;
	}
	else
	{
		col.a = (sinf(g_fCntTitle) + 1.0f) / 2;

		g_fCntTitle += 0.5f;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffUi->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntUi = 0; nCntUi < UITEXTURE_MAX; nCntUi++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Ui[nCntUi].pos.x - g_Ui[nCntUi].fWidth, g_Ui[nCntUi].pos.y - g_Ui[nCntUi].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Ui[nCntUi].pos.x + g_Ui[nCntUi].fWidth, g_Ui[nCntUi].pos.y - g_Ui[nCntUi].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Ui[nCntUi].pos.x - g_Ui[nCntUi].fWidth, g_Ui[nCntUi].pos.y + g_Ui[nCntUi].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Ui[nCntUi].pos.x + g_Ui[nCntUi].fWidth, g_Ui[nCntUi].pos.y + g_Ui[nCntUi].fHeight, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntUi == UITEXTURE_TITLEBUTTON)
		{
			//頂点カラーの設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;
		}
		else
		{
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_VtxBuffUi->Unlock();
}

//===========================
//背景の描画処理
//===========================
void DrawUi(void)
{
	MODE mode = GetMode();
	RESULT result = GetResult();
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_VtxBuffUi,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntUi = 0; nCntUi < UITEXTURE_MAX; nCntUi++)
	{
		if (g_Ui[nCntUi].nType == mode)
		{
			if (GetResult() == RESULT_CLEAR && mode == MODE_RESULT)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_apTextureUi[UITEXTURE_CLEAR]);
			}
			else if (GetResult() == RESULT_OVER && mode == MODE_RESULT)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_apTextureUi[UITEXTURE_GAMEOVER]);
			}
			else
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_apTextureUi[nCntUi]);
			}

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi * 4, 2);
		}
	}
}