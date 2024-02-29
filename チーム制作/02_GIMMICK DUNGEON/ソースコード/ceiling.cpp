//==================================
//
//天井の処理(ceiling.cpp)
// Author: Kai Takada
//
//==================================
#include "stdio.h"
#include "input.h"
#include "main.h"
#include "ceiling.h"
#include "meshfield.h"

//==========================================
//マクロ定義
//==========================================
#define CEILING_TEX_U (300.0f)	//テクスチャの長さx
#define CEILING_TEX_V (300.0f)	//テクスチャの長さy

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureCeiling = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCeiling = NULL;	//頂点バッファへのポインタ
D3DXVECTOR3 g_posCeiling;							//位置
D3DXVECTOR3 g_rotCeiling;							//移動量
D3DXMATRIX g_mtxWorldCeiling;						//ワールドマトリックス
float g_fHeightCeiling,g_fWidthCeiling;	//縦横

//==================================
//天井の初期化処理
//==================================
void InitCeiling(void)
{
	//テクスチャ分割数
	float fDivU, fDivV;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\wall000.jpg",				//テクスチャのファイル名
		&g_pTextureCeiling);

	//変数の初期化
	g_posCeiling = D3DXVECTOR3(0.0f, CEILING_POSY, 0.0f);
	g_rotCeiling = D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f);
	g_fHeightCeiling = FIELD_RANGE * MESH_V;
	g_fWidthCeiling = FIELD_RANGE * MESH_U;
	fDivU = g_fWidthCeiling / CEILING_TEX_U;
	fDivV = g_fHeightCeiling / CEILING_TEX_V;
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCeiling,
		NULL);

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCeiling->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-g_fWidthCeiling, 0.0f, g_fHeightCeiling);		//(x,y,z)
	pVtx[1].pos = D3DXVECTOR3(g_fWidthCeiling, 0.0f, g_fHeightCeiling);
	pVtx[2].pos = D3DXVECTOR3(-g_fWidthCeiling, 0.0f, -g_fHeightCeiling);
	pVtx[3].pos = D3DXVECTOR3(g_fWidthCeiling, 0.0f, -g_fHeightCeiling);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);	//(x,y,z)
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				//(x,y)
	pVtx[1].tex = D3DXVECTOR2(fDivU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, fDivV);
	pVtx[3].tex = D3DXVECTOR2(fDivU, fDivV);

	//頂点バッファをアンロックする
	g_pVtxBuffCeiling->Unlock();
}

//==================================
//天井の終了処理
//==================================
void UninitCeiling(void)
{
	if (g_pTextureCeiling != NULL)
	{
		g_pTextureCeiling->Release();
		g_pTextureCeiling = NULL;					//テクスチャ破棄
	}
	if (g_pVtxBuffCeiling != NULL)
	{
		g_pVtxBuffCeiling->Release();
		g_pVtxBuffCeiling = NULL;					//バッファの破棄
	}
}

//==================================
//天井の更新処理
//==================================
void UpdateCeiling(void)
{

}

//==================================
//天井の描画処理
//==================================
void DrawCeiling(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldCeiling);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_rotCeiling.y, g_rotCeiling.x, g_rotCeiling.z);

	D3DXMatrixMultiply(&g_mtxWorldCeiling, &g_mtxWorldCeiling, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_posCeiling.x, g_posCeiling.y, g_posCeiling.z);

	D3DXMatrixMultiply(&g_mtxWorldCeiling, &g_mtxWorldCeiling, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldCeiling);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCeiling, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureCeiling);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
		0,												//描画する最初のインデックス
		2);												//描画するプリミティブ数
}

//==================================
//天井の設置処理
//==================================
void SetPosCeiling(D3DXVECTOR3 pos, float fHeight, float fWidth)
{
	//テクスチャ分割数
	float fDivU, fDivV;

	g_posCeiling = pos;
	g_fWidthCeiling = fWidth;
	g_fHeightCeiling = fHeight;

	fDivU = fWidth / CEILING_TEX_U;
	fDivV = fHeight / CEILING_TEX_V;

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCeiling->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-g_fWidthCeiling, 0.0f, g_fHeightCeiling);		//(x,y,z)
	pVtx[1].pos = D3DXVECTOR3(g_fWidthCeiling, 0.0f, g_fHeightCeiling);
	pVtx[2].pos = D3DXVECTOR3(-g_fWidthCeiling, 0.0f, -g_fHeightCeiling);
	pVtx[3].pos = D3DXVECTOR3(g_fWidthCeiling, 0.0f, -g_fHeightCeiling);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				//(x,y)
	pVtx[1].tex = D3DXVECTOR2(fDivU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, fDivV);
	pVtx[3].tex = D3DXVECTOR2(fDivU, fDivV);

	//頂点バッファをアンロックする
	g_pVtxBuffCeiling->Unlock();
}

//==================================
//天井の位置取得処理
//==================================
D3DXVECTOR3 GetCeilingPos(void)
{
	return g_posCeiling;
}
