//==========================================
//
//  カウントの処理(count.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "count.h"
#include "polygon.h"
#include "tutorial.h"
#include "sound.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_DIGIT (1) //最大の桁数
#define MAX_POLYGON (1 + MAX_DIGIT * 2) //スラッシュのポリゴン + 最大桁数の数値ポリゴン * 2

//==========================================
//  テクスチャラベル
//==========================================
typedef enum
{
	TEX_SLASH = 0, 
	TEX_NUM, 
	TEX_MAX
}TEXTURE_COUNT_LABEL;

//==========================================
//  構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //位置
	float fSize; //サイズ
	int nMax; //カウントの最大値
	int nCountCurrent; //現在のカウント
	bool bClear; //達成状態
}COUNT;

//==========================================
//  テクスチャ一覧
//==========================================
const char *c_apTextureCount[TEX_MAX] =
{
	"data/TEXTURE/slash_000.png", //スラッシュ
	"data/TEXTURE/number001.png" //数値
};

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureCount[TEX_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCount = NULL;
COUNT g_Count;

//==========================================
//  初期化処理
//==========================================
void InitCount()
{
	//変数の初期化
	ZeroMemory(&g_Count, sizeof(COUNT));

	//頂点バッファの初期化
	g_pVtxBuffCount = Init_2D_Polygon(MAX_POLYGON);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの設定
	for (int nCnt = 0; nCnt < TEX_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apTextureCount[nCnt],
			&g_pTextureCount[nCnt]);
	}
}

//==========================================
//  終了処理
//==========================================
void UninitCount()
{
	//頂点バッファの破棄
	UninitPolygon(&g_pVtxBuffCount);

	//テクスチャの破棄
	for (int nCnt = 0; nCnt < TEX_MAX; nCnt++)
	{
		if (g_pTextureCount[nCnt] != NULL)
		{
			g_pTextureCount[nCnt]->Release();
			g_pTextureCount[nCnt] = NULL;
		}
	}
}

//==========================================
//  更新処理
//==========================================
void UpdateCount()
{
	//ローカル変数宣言
	int aTex[MAX_DIGIT];
	int nCntCount, nCalc;

	//最大数の取得
	nCalc = g_Count.nCountCurrent;

	//テクスチャ座標の計測
	for (nCntCount = 0; nCntCount < MAX_DIGIT; nCntCount++)
	{
		aTex[nCntCount] = nCalc % 10;
		nCalc /= 10;
	}

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffCount->Lock(0, 0, (void**)&pVtx, 0);

	//固定の値を設定から省く
	pVtx += 4 * (MAX_DIGIT + 1);

	//設定処理
	for (nCntCount = 0; nCntCount < MAX_DIGIT; nCntCount++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Count.pos.x - ((float)(nCntCount + 1.0f) * (g_Count.fSize * 0.6f)), g_Count.pos.y - g_Count.fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Count.pos.x - ((float)nCntCount * (g_Count.fSize * 0.6f)), g_Count.pos.y - g_Count.fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Count.pos.x - ((float)(nCntCount + 1.0f) * (g_Count.fSize * 0.6f)), g_Count.pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Count.pos.x - ((float)nCntCount * (g_Count.fSize * 0.6f)), g_Count.pos.y, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTex[nCntCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((aTex[nCntCount] * 0.1f) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTex[nCntCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((aTex[nCntCount] * 0.1f) + 0.1f, 1.0f);

		//頂点データのポインタを4つ進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffCount->Unlock();

	//カウントの達成判定
	if (g_Count.nMax <= g_Count.nCountCurrent)
	{
		g_Count.bClear = true;
	}

	//チュートリアルを進める
	TUTORIALSTATE *tutorial = GetTutorialState();
	if (*tutorial == TUTORIALSTATE_JUMP && g_Count.bClear && GetMode() == MODE_TUTORIAL)
	{
		*tutorial = TUTORIALSTATE_ENEMY;

		PlaySound(SOUND_LABEL_SE_BUTTON_004);

		//カウンタを設定
		SetCount(D3DXVECTOR3(640.0f, 120.0, 0.0f), 100.0f, 3);
	}
	else if (*tutorial == TUTORIALSTATE_ENEMY && g_Count.bClear)
	{
		if (GetCount())
		{
			*tutorial = TUTORIALSTATE_END;

			PlaySound(SOUND_LABEL_SE_BUTTON_004);
		}
	}
}

//==========================================
//  描画処理
//==========================================
void DrawCount()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCount, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_POLYGON; nCnt++)
	{
		//テクスチャの設定
		if (nCnt == 0)
		{
			pDevice->SetTexture(0, g_pTextureCount[TEX_SLASH]);
		}
		else
		{
			pDevice->SetTexture(0, g_pTextureCount[TEX_NUM]);
		}

		//メニュー項目の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}

//==========================================
//  設定処理
//==========================================
void SetCount(D3DXVECTOR3 pos, float size, int max)
{
	//ローカル変数宣言
	int aTex[MAX_DIGIT];
	int nCntCount, nCalc;

	//引き数の保存
	g_Count.pos = pos;
	g_Count.fSize = size;
	g_Count.nMax = max;

	//現在カウントの初期化
	g_Count.nCountCurrent = 0;
	g_Count.bClear = false;

	//最大数の取得
	nCalc = g_Count.nMax;

	//テクスチャ座標の計測
	for (nCntCount = MAX_DIGIT - 1; nCntCount >= 0; nCntCount--)
	{
		aTex[nCntCount] = nCalc % 10;
		nCalc /= 10;
	}

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffCount->Lock(0, 0, (void**)&pVtx, 0);

	//設定処理
	for (nCntCount = 0; nCntCount <= MAX_DIGIT; nCntCount++)
	{
		if (nCntCount == 0)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_Count.pos.x - g_Count.fSize * 0.3f, g_Count.pos.y - g_Count.fSize, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Count.pos.x + g_Count.fSize * 0.3f, g_Count.pos.y - g_Count.fSize, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Count.pos.x - g_Count.fSize * 0.3f, g_Count.pos.y + g_Count.fSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Count.pos.x + g_Count.fSize * 0.3f, g_Count.pos.y + g_Count.fSize, 0.0f);
		}
		else
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_Count.pos.x + ((float)(nCntCount - 1) * (g_Count.fSize * 0.6f)), g_Count.pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Count.pos.x + ((float)((nCntCount - 1) + 1.0f) * (g_Count.fSize * 0.6f)), g_Count.pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Count.pos.x + ((float)(nCntCount - 1) * (g_Count.fSize * 0.6f)), g_Count.pos.y + g_Count.fSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Count.pos.x + ((float)((nCntCount - 1) + 1.0f) * (g_Count.fSize * 0.6f)), g_Count.pos.y + g_Count.fSize, 0.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(aTex[nCntCount - 1] * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((aTex[nCntCount - 1] * 0.1f) + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(aTex[nCntCount - 1] * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((aTex[nCntCount - 1] * 0.1f) + 0.1f, 1.0f);
		}

		//頂点データのポインタを4つ進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
 	g_pVtxBuffCount->Unlock();
}

//==========================================
//  加算処理
//==========================================
void AddCount(int add)
{
	g_Count.nCountCurrent += add;

	PlaySound(SOUND_LABEL_SE_BUTTON_004);
}

//==========================================
//  カウントの状態を取得
//==========================================
bool GetCount()
{
	return g_Count.bClear;
}
