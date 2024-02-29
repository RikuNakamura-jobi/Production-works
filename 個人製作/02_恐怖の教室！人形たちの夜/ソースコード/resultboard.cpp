//===========================
//
//ダイレクトX.resultboardファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "resultboard.h"
#include "camera.h"
#include "result.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureResultBoard = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultBoard = NULL;		//頂点情報を格納
D3DXVECTOR3 g_posResultBoard;
D3DXMATRIX g_mtxWorldResultBoard;							//ワールドマトリックス

//テクスチャファイル名
const char *c_apFilenameResultBoard[2] =
{
	"data\\TEXTURE\\clear000.png",
	"data\\TEXTURE\\gameover000.png",
};

//===========================
//プレイヤーの初期化処理
//===========================
void InitResultBoard(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	RESULT result = GetResult();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		c_apFilenameResultBoard[result],
		&g_pTextureResultBoard);

	g_posResultBoard = D3DXVECTOR3(0.0f, 200.0f, 580.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultBoard,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultBoard->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = -128.0f;
	pVtx[0].pos.y = 72.0f;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = 128.0f;
	pVtx[1].pos.y = 72.0f;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = -128.0f;
	pVtx[2].pos.y = -72.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = 128.0f;
	pVtx[3].pos.y = -72.0f;
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
	g_pVtxBuffResultBoard->Unlock();
}

//===========================
//プレイヤーの終了処理
//===========================
void UninitResultBoard(void)
{
	//テクスチャの破棄
	if (g_pTextureResultBoard != NULL)
	{
		g_pTextureResultBoard->Release();
		g_pTextureResultBoard = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResultBoard != NULL)
	{
		g_pVtxBuffResultBoard->Release();
		g_pVtxBuffResultBoard = NULL;
	}
}

//===========================
//プレイヤーの更新処理
//===========================
void UpdateResultBoard(void)
{
	Camera pCamera = GetCamera();

	g_posResultBoard = pCamera.posR;
}

//===========================
//プレイヤーの描画処理
//===========================
void DrawResultBoard(void)
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

	//αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldResultBoard);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&g_mtxWorldResultBoard, NULL, &mtxView);
	g_mtxWorldResultBoard._41 = 0.0f;
	g_mtxWorldResultBoard._42 = 0.0f;
	g_mtxWorldResultBoard._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_posResultBoard.x, g_posResultBoard.y, g_posResultBoard.z);
	D3DXMatrixMultiply(&g_mtxWorldResultBoard, &g_mtxWorldResultBoard, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldResultBoard);
	
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffResultBoard,
		0,
		sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResultBoard);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//影消し
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}