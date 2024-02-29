//===========================
//
//ダイレクトX.billboardファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "billboard.h"
#include "camera.h"
#include "player.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;							//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;					//頂点情報を格納
D3DXVECTOR3 g_posBillboard;
D3DXMATRIX g_mtxWorldBillboard;		//ワールドマトリックス
bool g_bBillboard;
int nBillboardCount;

//テクスチャファイル名
const char *c_apFilenameBoard[2] =
{
	"data\\TEXTURE\\life001.png",
	"data\\TEXTURE\\life001.png",
};

//===========================
//プレイヤーの初期化処理
//===========================
void InitBillboard(void)
{
	MODE mode = GetMode();

	g_bBillboard = true;
	nBillboardCount = 0;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		c_apFilenameBoard[1],
		&g_pTextureBillboard);

	g_posBillboard = D3DXVECTOR3(-2200.0f, 35.0f, -300.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = -25.0f;
	pVtx[0].pos.y = 25.0f;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = 25.0f;
	pVtx[1].pos.y = 25.0f;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = -25.0f;
	pVtx[2].pos.y = -25.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = 25.0f;
	pVtx[3].pos.y = -25.0f;
	pVtx[3].pos.z = 0.0f;

	//頂点座標の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//===========================
//プレイヤーの終了処理
//===========================
void UninitBillboard(void)
{
	//テクスチャの破棄
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//===========================
//プレイヤーの更新処理
//===========================
void UpdateBillboard(void)
{
	if (g_bBillboard == true)
	{
		if (CollisionPlayer(&g_posBillboard, &g_posBillboard, &D3DXVECTOR3(0.0f, 0.0f, -0.0f), 3) == true)
		{
			g_bBillboard = false;
		}
	}
	else
	{
		nBillboardCount++;

		if (nBillboardCount >= 300)
		{
			g_bBillboard = true;
			nBillboardCount = 0;
		}
	}
}

//===========================
//プレイヤーの描画処理
//===========================
void DrawBillboard(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTrans;		//計算用マトリックス
	D3DXMATRIX mtxView;			//ビューマトリックス取得用

	//影消し
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

	if (g_bBillboard == true)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldBillboard);

		//ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&g_mtxWorldBillboard, NULL, &mtxView);
		g_mtxWorldBillboard._41 = 0.0f;
		g_mtxWorldBillboard._42 = 0.0f;
		g_mtxWorldBillboard._43 = 0.0f;

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			g_posBillboard.x, g_posBillboard.y, g_posBillboard.z);
		D3DXMatrixMultiply(&g_mtxWorldBillboard, &g_mtxWorldBillboard, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillboard);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0,
			g_pVtxBuffBillboard,
			0,
			sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBillboard);


		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	//影消し
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}