//===========================
//
//ダイレクトX.bossファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "boss.h"
#include "player.h"
#include "enemy.h"

//マクロ定義
#define MAX_BOSS (248)			//ボス攻撃範囲の最大数

//ボス攻撃範囲構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXCOLOR col;				//色
	D3DXVECTOR3 rot;			//向き
	float fRadiusX;				//X長さ
	float fRadiusY;				//Y長さ
	float fLength;				//対角線の長さ
	float fAngle;				//対角線の角度
	int nLife;					//寿命
	int nType;					//種類
	bool bUse;					//使用しているかどうか
	bool bCollision;			//使用しているかどうか
}BOSS;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureBoss = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoss = NULL;		//頂点情報を格納
BOSS g_aBoss[MAX_BOSS];								//ボス攻撃範囲の情報
int g_nCntBossSound1;								//効果音の多重判定1
int g_nCntBossSound2;								//効果音の多重判定2
int g_nCntBossSound3;								//効果音の多重判定3

//===========================
//ボス攻撃範囲の初期化処理
//===========================
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBoss;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\JOBICciaoBlack.png",
		&g_pTextureBoss);

	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		g_aBoss[nCntBoss].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].fRadiusX = 10.0f;
		g_aBoss[nCntBoss].fRadiusY = 10.0f;
		g_aBoss[nCntBoss].fAngle = 0.0f;
		g_aBoss[nCntBoss].fLength = 0.0f;
		g_aBoss[nCntBoss].nLife = 10;
		g_aBoss[nCntBoss].bUse = false;									//使用していない状態にする
		g_aBoss[nCntBoss].bCollision = true;							//使用していない状態にする

		//対角線の長さを算出する
		g_aBoss[nCntBoss].fLength = sqrtf((g_aBoss[nCntBoss].fRadiusX * g_aBoss[nCntBoss].fRadiusX) + (g_aBoss[nCntBoss].fRadiusY * g_aBoss[nCntBoss].fRadiusY)) * 0.5f;

		//対角線の角度を算出する
		g_aBoss[nCntBoss].fAngle = atan2f(g_aBoss[nCntBoss].fRadiusX, g_aBoss[nCntBoss].fRadiusY);
	}

	g_nCntBossSound1 = 0;
	g_nCntBossSound2= 0;
	g_nCntBossSound3 = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BOSS,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoss,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z - D3DX_PI + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
		pVtx[0].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z - D3DX_PI + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z + D3DX_PI - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
		pVtx[1].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z + D3DX_PI - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
		pVtx[2].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
		pVtx[3].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
		pVtx[3].pos.z = 0.0f;

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

		pVtx += 4;											//頂点座標のポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBoss->Unlock();
}

//===========================
//ボス攻撃範囲の終了処理
//===========================
void UninitBoss(void)
{
	//テクスチャの破棄
	if (g_pTextureBoss != NULL)
	{
		g_pTextureBoss->Release();
		g_pTextureBoss = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBoss != NULL)
	{
		g_pVtxBuffBoss->Release();
		g_pVtxBuffBoss = NULL;
	}
}

//===========================
//ボス攻撃範囲の更新処理
//===========================
void UpdateBoss(void)
{
	int nCntBoss;
	VERTEX_2D *pVtx;
	Enemy *aEnemy = GetEnemy();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse == true)
		{
			g_aBoss[nCntBoss].nLife -= 1;

			if (g_aBoss[nCntBoss].nLife <= 0)
			{
				g_aBoss[nCntBoss].bUse = false;					//使用してない状態にする
			}
		}
	}

	g_pVtxBuffBoss->Unlock();
}

//===========================
//ボス攻撃範囲の描画処理
//===========================
void DrawBoss(void)
{
	int nCntBoss;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffBoss,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBoss);

	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse == true)
		{//ボス攻撃範囲が使われている
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBoss, 2);
		}
	}
}

//===========================
//ボス攻撃範囲の設定処理
//===========================
void SetBoss(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 rot, float fRadiusX, float fRadiusY, int nLife)
{
	int nCntBoss;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse == false)
		{
			g_aBoss[nCntBoss].pos = pos;
			g_aBoss[nCntBoss].rot = rot;
			g_aBoss[nCntBoss].nLife = nLife;
			g_aBoss[nCntBoss].fRadiusX = fRadiusX;
			g_aBoss[nCntBoss].fRadiusY = fRadiusY;
			g_aBoss[nCntBoss].col = col;

			//対角線の長さを算出する
			g_aBoss[nCntBoss].fLength = sqrtf((g_aBoss[nCntBoss].fRadiusX * g_aBoss[nCntBoss].fRadiusX) + (g_aBoss[nCntBoss].fRadiusY * g_aBoss[nCntBoss].fRadiusY)) * 0.5f;

			//対角線の角度を算出する
			g_aBoss[nCntBoss].fAngle = atan2f(g_aBoss[nCntBoss].fRadiusX, g_aBoss[nCntBoss].fRadiusY);

			//頂点座標の設定
			pVtx[0].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z - D3DX_PI + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[0].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z - D3DX_PI + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z + D3DX_PI - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[1].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z + D3DX_PI - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[2].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[3].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[3].pos.z = 0.0f;

			g_aBoss[nCntBoss].bCollision = true;
			g_aBoss[nCntBoss].bUse = true;						//使用している状態にする

			break;
		}

		pVtx += 4;
	}

	g_pVtxBuffBoss->Unlock();
}

//===========================
//プレイヤーとの当たり判定
//===========================
bool CollisionBoss(D3DXVECTOR3 *pPos)
{
	bool bHit = false;
	float fcos[4];
	D3DXVECTOR3 BossSize[4];
	D3DXVECTOR3 Vec;
	D3DXVECTOR3 VecPB;
	D3DXVECTOR3 ObjPos;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		BossSize[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	Vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	VecPB = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ObjPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse == true && g_aBoss[nCntBoss].bCollision == true)
		{
			//頂点座標の設定
			BossSize[0].x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z - D3DX_PI + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength * 1.6f;
			BossSize[0].y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z - D3DX_PI + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength * 1.6f;
			BossSize[0].z = 0.0f;

			BossSize[1].x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z + D3DX_PI - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength * 1.6f;
			BossSize[1].y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z + D3DX_PI - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength * 1.6f;
			BossSize[1].z = 0.0f;

			BossSize[2].x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength * 1.6f;
			BossSize[2].y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength * 1.6f;
			BossSize[2].z = 0.0f;

			BossSize[3].x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength * 1.6f;
			BossSize[3].y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength * 1.6f;
			BossSize[3].z = 0.0f;

			for (int nCnt1 = 0; nCnt1 < 4; nCnt1++)
			{
				switch (nCnt1)
				{
				case 0:
					Vec.x = BossSize[2].x - BossSize[0].x;
					Vec.y = BossSize[2].y - BossSize[0].y;

					VecPB.x = pPos->x - BossSize[0].x;
					VecPB.y = pPos->y - BossSize[0].y;
					break;
				case 1:
					Vec.x = BossSize[3].x - BossSize[2].x;
					Vec.y = BossSize[3].y - BossSize[2].y;

					VecPB.x = pPos->x - BossSize[2].x;
					VecPB.y = pPos->y - BossSize[2].y;
					break;
				case 2:
					Vec.x = BossSize[1].x - BossSize[3].x;
					Vec.y = BossSize[1].y - BossSize[3].y;

					VecPB.x = pPos->x - BossSize[3].x;
					VecPB.y = pPos->y - BossSize[3].y;
					break;
				case 3:
					Vec.x = BossSize[0].x - BossSize[1].x;
					Vec.y = BossSize[0].y - BossSize[1].y;

					VecPB.x = pPos->x - BossSize[1].x;
					VecPB.y = pPos->y - BossSize[1].y;
					break;
				}

				fcos[nCnt1] = ((Vec.x * VecPB.x) + (Vec.y * VecPB.y)) / (sqrtf((Vec.x * Vec.x) + (Vec.y * Vec.y)) * sqrtf((VecPB.x * VecPB.x) + (VecPB.y * VecPB.y)));
			}

			if (fcos[0] > 0.0f &&
				fcos[1] > 0.0f &&
				fcos[2] > 0.0f &&
				fcos[3] > 0.0f)
			{
				bHit = true;

				g_aBoss[nCntBoss].bCollision = false;
			}
		}
	}

	return bHit;
}