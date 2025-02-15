//===========================
//
//ダイレクトX.particleファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "particle.h"

//マクロ定義
#define MAX_PARTICLE (8192)			//パーティクルの最大数

//パーティクル構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXCOLOR col;					//色
	D3DXVECTOR3 move;				//動き
	D3DXVECTOR3 rot;				//向き
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	float fRadius;					//半径
	float fAngle;					//対角線の角度
	float fRot;						//回転
	int nLife;						//寿命
	int nType;						//種類
	bool bUse;						//使用しているかどうか
}Particle;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureParticle = NULL;						//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;					//頂点情報を格納
Particle g_aParticle[MAX_PARTICLE];									//パーティクルの情報

//===========================
//パーティクルの初期化処理
//===========================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntParticle;
	srand((unsigned int)time(0));

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Effect000.jpg",
		&g_pTextureParticle);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].fRadius = 10.0f;
		g_aParticle[nCntParticle].nLife = 10;
		g_aParticle[nCntParticle].bUse = false;									//使用していない状態にする

		//対角線の長さを算出する
		g_aParticle[nCntParticle].fRadius = sqrtf((10.0f * 10.0f) + (10.0f * 10.0f)) * 0.5f;

		//対角線の角度を算出する
		g_aParticle[nCntParticle].fAngle = atan2f(10.0f, 10.0f);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = sinf(g_aParticle[nCntParticle].rot.z - D3DX_PI + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[0].pos.y = -cosf(g_aParticle[nCntParticle].rot.z - D3DX_PI + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = sinf(g_aParticle[nCntParticle].rot.z + D3DX_PI - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[1].pos.y = -cosf(g_aParticle[nCntParticle].rot.z + D3DX_PI - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = sinf(g_aParticle[nCntParticle].rot.z - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[2].pos.y = -cosf(g_aParticle[nCntParticle].rot.z - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = sinf(g_aParticle[nCntParticle].rot.z + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[3].pos.y = -cosf(g_aParticle[nCntParticle].rot.z + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[3].pos.z = 0.0f;

		//頂点座標の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;											//頂点座標のポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
}

//===========================
//パーティクルの終了処理
//===========================
void UninitParticle(void)
{
	//テクスチャの破棄
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//===========================
//パーティクルの更新処理
//===========================
void UpdateParticle(void)
{
	int nCntParticle;
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			g_aParticle[nCntParticle].nLife -= 1;

			if (g_aParticle[nCntParticle].fRadius > 0)
			{
				g_aParticle[nCntParticle].fRadius -= 0.5f;
			}
			else
			{
				g_aParticle[nCntParticle].fRadius = 0;
			}

			if (g_aParticle[nCntParticle].col.a <= 0)
			{
				g_aParticle[nCntParticle].col.a = 0;
			}
			else
			{
				g_aParticle[nCntParticle].col.a -= g_aParticle[nCntParticle].col.a / g_aParticle[nCntParticle].nLife;
			}

			g_aParticle[nCntParticle].move.y -= 0.8f;

			g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
			g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;
			g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z;
			g_aParticle[nCntParticle].rot.z += g_aParticle[nCntParticle].fRot;

			//頂点座標の設定
			pVtx[0].pos.x = sinf(g_aParticle[nCntParticle].rot.z - D3DX_PI + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[0].pos.y = -cosf(g_aParticle[nCntParticle].rot.z - D3DX_PI + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = sinf(g_aParticle[nCntParticle].rot.z + D3DX_PI - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[1].pos.y = -cosf(g_aParticle[nCntParticle].rot.z + D3DX_PI - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = sinf(g_aParticle[nCntParticle].rot.z - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[2].pos.y = -cosf(g_aParticle[nCntParticle].rot.z - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = sinf(g_aParticle[nCntParticle].rot.z + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[3].pos.y = -cosf(g_aParticle[nCntParticle].rot.z + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[3].pos.z = 0.0f;

			//頂点カラーの設定
			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;

			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;					//使用してない状態にする
			}
		}

		pVtx += 4;
	}

	g_pVtxBuffParticle->Unlock();
}

//===========================
//パーティクルの描画処理
//===========================
void DrawParticle(void)
{
	int nCntParticle;
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxTrans;		//計算用マトリックス
	D3DXMATRIX mtxView;			//ビューマトリックス取得用

	//デバイスの取得
	pDevice = GetDevice();

	//影消し
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aParticle[nCntParticle].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aParticle[nCntParticle].mtxWorld, NULL, &mtxView);
			g_aParticle[nCntParticle].mtxWorld._41 = 0.0f;
			g_aParticle[nCntParticle].mtxWorld._42 = 0.0f;
			g_aParticle[nCntParticle].mtxWorld._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
			D3DXMatrixMultiply(&g_aParticle[nCntParticle].mtxWorld, &g_aParticle[nCntParticle].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aParticle[nCntParticle].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0,
				g_pVtxBuffParticle,
				0,
				sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureParticle);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntParticle, 2);
		}
	}

	//影消し
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//===========================
//パーティクルの設定処理
//===========================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, int nType, int nRot, D3DXVECTOR3 move, int nMax)
{
	int nCntAppear;

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntAppear = 0; nCntAppear < nMax; nCntAppear++)
	{
		if (g_aParticle[nCntAppear].bUse == false)
		{
			g_aParticle[nCntAppear].pos = pos;
			g_aParticle[nCntAppear].nLife = nLife;
			g_aParticle[nCntAppear].col = col;
			g_aParticle[nCntAppear].fRot = (float)(rand() % 629 - nRot) / 100.0f;
			g_aParticle[nCntAppear].fRadius = sqrtf((fRadius * fRadius) + (fRadius * fRadius)) * 0.5f;
			g_aParticle[nCntAppear].fAngle = atan2f(fRadius, fRadius);

			g_aParticle[nCntAppear].fRot = (float)(rand() % 629 - nRot) / 100.0f;

			if (nType == 1)
			{
				g_aParticle[nCntAppear].fRot = (float)(rand() % 41 - nRot) / 100.0f;
			}
			
			if (g_aParticle[nCntAppear].fRot > D3DX_PI)
			{
				g_aParticle[nCntAppear].fRot -= (D3DX_PI * 2);
			}
			else if (g_aParticle[nCntAppear].fRot < -D3DX_PI)
			{
				g_aParticle[nCntAppear].fRot += (D3DX_PI * 2);
			}

			g_aParticle[nCntAppear].move.x = (float)(rand() % 200 - 100) / (10.0f + move.x);
			g_aParticle[nCntAppear].move.y = (float)(rand() % 200 - 10) / (10.0f + move.y);
			g_aParticle[nCntAppear].move.z = (float)(rand() % 200 - 100) / (10.0f + move.z);

			if (nType == 1)
			{
				g_aParticle[nCntAppear].move.x = (float)(rand() % 51 - 25) / (10.0f + move.x);
				g_aParticle[nCntAppear].move.y = (float)(rand() % 51 - 25) / (10.0f + move.y);
				g_aParticle[nCntAppear].move.z = (float)(rand() % 51 - 25) / (10.0f + move.z);
			}

			g_aParticle[nCntAppear].rot.z += g_aParticle[nCntAppear].fRot;

			//頂点座標の設定
			pVtx[0].pos.x = sinf(g_aParticle[nCntAppear].rot.z - D3DX_PI + g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[0].pos.y = -cosf(g_aParticle[nCntAppear].rot.z - D3DX_PI + g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = sinf(g_aParticle[nCntAppear].rot.z + D3DX_PI - g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[1].pos.y = -cosf(g_aParticle[nCntAppear].rot.z + D3DX_PI - g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = sinf(g_aParticle[nCntAppear].rot.z - g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[2].pos.y = -cosf(g_aParticle[nCntAppear].rot.z - g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = sinf(g_aParticle[nCntAppear].rot.z + g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[3].pos.y = -cosf(g_aParticle[nCntAppear].rot.z + g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[3].pos.z = 0.0f;

			g_aParticle[nCntAppear].bUse = true;						//使用している状態にする

			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffParticle->Unlock();
}