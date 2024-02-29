//===========================
//
//ダイレクトX.fadeファイル
//Author:中村　陸
//
//===========================
#include "fade.h"
#include "game.h"
#include "main.h"
#include "camera.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;							//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;					//頂点情報を格納
FADE g_fade;													//フェード
MODE g_modeNext;												//次の画面
D3DXCOLOR g_colorFade;											//フェードの色
D3DXVECTOR3 g_posFade;
D3DXMATRIX g_mtxWorldFade;		//ワールドマトリックス

//===========================
//フェードの初期化処理
//===========================
void InitFade(MODE modeNext)
{
	g_fade = FADE_IN;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_posFade = D3DXVECTOR3(0.0f, 200.0f, 580.0f);

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\jouheki.jpg",
		&g_pTextureFade);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = -2900.0f;
	pVtx[0].pos.y = 1500.0f;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = 2900.0f;
	pVtx[1].pos.y = 1500.0f;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = -2900.0f;
	pVtx[2].pos.y = -1500.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = 2900.0f;
	pVtx[3].pos.y = -1500.0f;
	pVtx[3].pos.z = 0.0f;

	//頂点座標の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

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

//===========================
//フェードの終了処理
//===========================
void UninitFade(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//===========================
//フェードの更新処理
//===========================
void UpdateFade(void)
{
	Camera pCamera = GetCamera();

	g_posFade = pCamera.posR;

	if (g_fade != FADE_NONE)
	{
		if (g_fade == FADE_IN)
		{//フェードイン
			g_colorFade.a -= 0.05f;

			if (g_colorFade.a <= 0.0f)
			{
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;
			}
		}
		else if (g_fade == FADE_OUT)
		{//フェードアウト
			g_colorFade.a += 0.03f;

			if (g_colorFade.a > 1.0f)
			{
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;

				//モードの設定
				SetMode(g_modeNext);
			}
		}

		VERTEX_3D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);

		//頂点バッファをアンロックする
		g_pVtxBuffFade->Unlock();
	}
}

//===========================
//フェードの描画処理
//===========================
void DrawFade(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTrans;		//計算用マトリックス
	D3DXMATRIX mtxView;			//ビューマトリックス取得用

	//影消し
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldFade);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&g_mtxWorldFade, NULL, &mtxView);
	g_mtxWorldFade._41 = 0.0f;
	g_mtxWorldFade._42 = 0.0f;
	g_mtxWorldFade._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_posFade.x, g_posFade.y, g_posFade.z);
	D3DXMatrixMultiply(&g_mtxWorldFade, &g_mtxWorldFade, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldFade);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffFade,
		0,
		sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFade);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//影消し
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//===========================
//フェードの設定処理
//===========================
void SetFade(MODE modeNext)
{
	g_fade = FADE_OUT;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//===========================
//フェードの取得処理
//===========================
FADE GetFade(void)
{
	return g_fade;
}