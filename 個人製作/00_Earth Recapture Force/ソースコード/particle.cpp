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
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_aParticle[nCntParticle].pos.x + sinf(g_aParticle[nCntParticle].rot.z - D3DX_PI + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[0].pos.y = g_aParticle[nCntParticle].pos.y + cosf(g_aParticle[nCntParticle].rot.z - D3DX_PI + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aParticle[nCntParticle].pos.x + sinf(g_aParticle[nCntParticle].rot.z + D3DX_PI - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[1].pos.y = g_aParticle[nCntParticle].pos.y + cosf(g_aParticle[nCntParticle].rot.z + D3DX_PI - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aParticle[nCntParticle].pos.x + sinf(g_aParticle[nCntParticle].rot.z - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[2].pos.y = g_aParticle[nCntParticle].pos.y + cosf(g_aParticle[nCntParticle].rot.z - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aParticle[nCntParticle].pos.x + sinf(g_aParticle[nCntParticle].rot.z + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[3].pos.y = g_aParticle[nCntParticle].pos.y + cosf(g_aParticle[nCntParticle].rot.z + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
		pVtx[3].pos.z = 0.0f;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

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
	VERTEX_2D *pVtx;

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

			g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
			g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;
			g_aParticle[nCntParticle].rot.z += g_aParticle[nCntParticle].fRot;

			//頂点座標の設定
			pVtx[0].pos.x = g_aParticle[nCntParticle].pos.x + sinf(g_aParticle[nCntParticle].rot.z - D3DX_PI + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[0].pos.y = g_aParticle[nCntParticle].pos.y + cosf(g_aParticle[nCntParticle].rot.z - D3DX_PI + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aParticle[nCntParticle].pos.x + sinf(g_aParticle[nCntParticle].rot.z + D3DX_PI - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[1].pos.y = g_aParticle[nCntParticle].pos.y + cosf(g_aParticle[nCntParticle].rot.z + D3DX_PI - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aParticle[nCntParticle].pos.x + sinf(g_aParticle[nCntParticle].rot.z - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[2].pos.y = g_aParticle[nCntParticle].pos.y + cosf(g_aParticle[nCntParticle].rot.z - g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aParticle[nCntParticle].pos.x + sinf(g_aParticle[nCntParticle].rot.z + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
			pVtx[3].pos.y = g_aParticle[nCntParticle].pos.y + cosf(g_aParticle[nCntParticle].rot.z + g_aParticle[nCntParticle].fAngle) * g_aParticle[nCntParticle].fRadius;
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

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffParticle,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureParticle);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{//パーティクルが使われている
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntParticle, 2);
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===========================
//パーティクルの設定処理
//===========================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, int nType, int nRot, D3DXVECTOR3 move, int nMax)
{
	int nCntAppear;

	VERTEX_2D *pVtx;

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

			if (nType == 0)
			{
				g_aParticle[nCntAppear].move.x = (float)(rand() % 100) / 100.0f + move.x;
				g_aParticle[nCntAppear].move.y = (float)(rand() % 100) / 100.0f + move.y;
			}
			else if (nType == 1)
			{
				g_aParticle[nCntAppear].move.x = (float)(rand() % 100) / 10.0f + move.x;
				g_aParticle[nCntAppear].move.y = (float)(rand() % 100) / 10.0f + move.y;
			}
			else if (nType == 2)
			{
				g_aParticle[nCntAppear].fRot = (float)(rand() % 629 - nRot) / 100.0f;

				if (g_aParticle[nCntAppear].fRot > D3DX_PI)
				{
					g_aParticle[nCntAppear].fRot -= (D3DX_PI * 2);
				}
				else if (g_aParticle[nCntAppear].fRot < -D3DX_PI)
				{
					g_aParticle[nCntAppear].fRot += (D3DX_PI * 2);
				}

				g_aParticle[nCntAppear].move.x = (float)(rand() % 200 - 100) / 10.0f + move.x;
				g_aParticle[nCntAppear].move.y = (float)(rand() % 200 - 100) / 10.0f + move.y;
			}

			g_aParticle[nCntAppear].rot.z += g_aParticle[nCntAppear].fRot;

			//頂点座標の設定
			pVtx[0].pos.x = g_aParticle[nCntAppear].pos.x + sinf(g_aParticle[nCntAppear].rot.z - D3DX_PI + g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[0].pos.y = g_aParticle[nCntAppear].pos.y + cosf(g_aParticle[nCntAppear].rot.z - D3DX_PI + g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aParticle[nCntAppear].pos.x + sinf(g_aParticle[nCntAppear].rot.z + D3DX_PI - g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[1].pos.y = g_aParticle[nCntAppear].pos.y + cosf(g_aParticle[nCntAppear].rot.z + D3DX_PI - g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aParticle[nCntAppear].pos.x + sinf(g_aParticle[nCntAppear].rot.z - g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[2].pos.y = g_aParticle[nCntAppear].pos.y + cosf(g_aParticle[nCntAppear].rot.z - g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aParticle[nCntAppear].pos.x + sinf(g_aParticle[nCntAppear].rot.z + g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[3].pos.y = g_aParticle[nCntAppear].pos.y + cosf(g_aParticle[nCntAppear].rot.z + g_aParticle[nCntAppear].fAngle) * g_aParticle[nCntAppear].fRadius;
			pVtx[3].pos.z = 0.0f;

			g_aParticle[nCntAppear].bUse = true;						//使用している状態にする

			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffParticle->Unlock();
}