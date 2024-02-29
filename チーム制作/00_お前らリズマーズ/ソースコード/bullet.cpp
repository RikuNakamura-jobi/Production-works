//===========================
//
//ダイレクトX.bulletファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "sound.h"
#include "effect.h"
#include "game.h"
#include "particle.h"
#include "score.h"

//マクロ定義
#define MAX_BULLET (256)											//弾の最大数

//プロトタイプ宣言

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;							//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;					//頂点情報を格納
Bullet g_aBullet[MAX_BULLET];										//弾の情報
BULLETSTATE g_bulletState;
float g_fLengthBullet;												//対角線の長さ
float g_fAngleBullet;												//対角線の角度
float g_fLengthHalfBullet;											//半径の長さ
float g_fCntBall;
int g_aScoreHit[10];
int g_nScoreHit;

//===========================
//弾の初期化処理
//===========================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\baseball-1.png",
		&g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 100;
		g_aBullet[nCntBullet].bUse = false;									//使用していない状態にする
	}

	//対角線の長さを算出する
	g_fLengthBullet = sqrtf((40.0f * 40.0f) + (40.0f * 40.0f)) * 0.5f;

	//対角線の角度を算出する
	g_fAngleBullet = atan2f(40.0f, 40.0f);

	g_fLengthHalfBullet = 20.0f;
	g_fCntBall = 0.0f;
	g_aScoreHit[9] = {};
	g_nScoreHit = 0;
	g_bulletState = BULLETSTATE_NORMAL;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
		pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
		pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - g_fAngleBullet) * g_fLengthBullet;
		pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - g_fAngleBullet) * g_fLengthBullet;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + g_fAngleBullet) * g_fLengthBullet;
		pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + g_fAngleBullet) * g_fLengthBullet;
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
	g_pVtxBuffBullet->Unlock();
}

//===========================
//弾の終了処理
//===========================
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//===========================
//弾の更新処理
//===========================
void UpdateBullet(void)
{
	int nCntBullet;
	VERTEX_2D *pVtx;
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			if (g_bulletState == BULLETSTATE_WAIT)
			{
				if (g_aBullet[nCntBullet].type == BULLETTYPE_0)
				{//敵のホーミング弾
					float fRotMove, fRotDest, fRotDiff;
					Player *pPlayer = GetPlayer();

					fRotMove = atan2f(g_aBullet[nCntBullet].move.x, g_aBullet[nCntBullet].move.y);
					fRotDest = atan2f(pPlayer->pos.x - g_aBullet[nCntBullet].pos.x, pPlayer->pos.y - g_aBullet[nCntBullet].pos.y);
					fRotDiff = fRotDest - fRotMove;

					if (fRotDiff > D3DX_PI)
					{
						fRotDiff -= (D3DX_PI * 2);
					}
					else if (fRotDiff < -D3DX_PI)
					{
						fRotDiff += (D3DX_PI * 2);
					}


					if (g_aBullet[nCntBullet].nLife >= 50)
					{
						fRotMove += fRotDiff * 0.0003f;
					}
					else
					{
						fRotMove += fRotDiff * 0.0f;
					}

					if (fRotMove > D3DX_PI)
					{
						fRotMove -= (D3DX_PI * 2);
					}
					else if (fRotMove < -D3DX_PI)
					{
						fRotMove += (D3DX_PI * 2);
					}

					g_aBullet[nCntBullet].move.x = sinf(fRotMove) * (10.0f);
					g_aBullet[nCntBullet].move.y = cosf(fRotMove) * (10.0f);
					g_aBullet[nCntBullet].rot.z -= 1.0f;

					SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);
				}
				if (g_aBullet[nCntBullet].type == BULLETTYPE_1)
				{//敵のホーミング弾
					float fRotMove, fRotDest, fRotDiff;
					Player *pPlayer = GetPlayer();

					pPlayer->pos.x -= 56.0f;
					pPlayer->pos.y += 20.0f;

					fRotMove = atan2f(g_aBullet[nCntBullet].move.x, g_aBullet[nCntBullet].move.y);
					fRotDest = atan2f(pPlayer->pos.x - g_aBullet[nCntBullet].pos.x, pPlayer->pos.y - g_aBullet[nCntBullet].pos.y);
					fRotDiff = fRotDest - fRotMove;

					if (fRotDiff > D3DX_PI)
					{
						fRotDiff -= (D3DX_PI * 2);
					}
					else if (fRotDiff < -D3DX_PI)
					{
						fRotDiff += (D3DX_PI * 2);
					}

					if (g_aBullet[nCntBullet].pos.x < 1140)
					{
						fRotMove += fRotDiff * 0.04f;
					}
					else
					{
						fRotMove += fRotDiff * 0.0f;
					}

					if (fRotMove > D3DX_PI)
					{
						fRotMove -= (D3DX_PI * 2);
					}
					else if (fRotMove < -D3DX_PI)
					{
						fRotMove += (D3DX_PI * 2);
					}

					g_aBullet[nCntBullet].move.x = sinf(fRotMove) * (9.0f);
					g_aBullet[nCntBullet].move.y = cosf(fRotMove) * (9.0f);
					g_aBullet[nCntBullet].rot.z -= 0.1f;

					SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);

					pPlayer->pos.x += 56.0f;
					pPlayer->pos.y -= 20.0f;
				}
				if (g_aBullet[nCntBullet].type == BULLETTYPE_2)
				{//敵のホーミング弾
					float fRotMove;
					Player *pPlayer = GetPlayer();

					g_fCntBall += 0.2f;

					fRotMove = sinf(g_fCntBall) * 5.0f;

					g_aBullet[nCntBullet].move.x = 8.0f;
					g_aBullet[nCntBullet].move.y = sinf(g_fCntBall) * (5.0f);
					g_aBullet[nCntBullet].rot.z -= 0.8f;

					SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);
				}
				if (g_aBullet[nCntBullet].type == BULLETTYPE_3)
				{//敵のホーミング弾
					if (g_aBullet[nCntBullet].pos.x <= 250.0f)
					{
						g_aBullet[nCntBullet].move.x = 1.0f;
						g_aBullet[nCntBullet].move.y = 0.0f;
						g_aBullet[nCntBullet].rot.z -= 0.2f;
					}
					else
					{
						g_aBullet[nCntBullet].move.x = 15.0f;
						g_aBullet[nCntBullet].move.y = 0.0f;
						g_aBullet[nCntBullet].rot.z -= 1.5f;
					}
					SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);
				}
				if (g_aBullet[nCntBullet].type == BULLETTYPE_4)
				{//敵のホーミング弾
					if (g_aBullet[nCntBullet].pos.x >= 800.0f)
					{
						g_aBullet[nCntBullet].move.x = 10.0f;
						g_aBullet[nCntBullet].move.y = 0.0f;
						g_aBullet[nCntBullet].rot.z -= 1.0f;
						g_aBullet[nCntBullet].col.a = 0.0f;
					}
					else
					{
						g_aBullet[nCntBullet].move.x = 10.0f;
						g_aBullet[nCntBullet].move.y = 0.0f;
						g_aBullet[nCntBullet].rot.z -= 1.0f;

						SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);
					}
				}
				if (g_aBullet[nCntBullet].type == BULLETTYPE_5)
				{//敵のホーミング弾
					float fRotMove, fRotDest, fRotDiff;
					Player *pPlayer = GetPlayer();

					pPlayer->pos.x -= 56.0f;
					pPlayer->pos.y += 20.0f;

					fRotMove = atan2f(g_aBullet[nCntBullet].move.x, g_aBullet[nCntBullet].move.y);
					fRotDest = atan2f(pPlayer->pos.x - g_aBullet[nCntBullet].pos.x, pPlayer->pos.y - g_aBullet[nCntBullet].pos.y);
					fRotDiff = fRotDest - fRotMove;

					if (fRotDiff > D3DX_PI)
					{
						fRotDiff -= (D3DX_PI * 2);
					}
					else if (fRotDiff < -D3DX_PI)
					{
						fRotDiff += (D3DX_PI * 2);
					}

					if (g_aBullet[nCntBullet].pos.x < 1140)
					{
						fRotMove += fRotDiff * 0.1f;
					}
					else
					{
						fRotMove += fRotDiff * 0.0f;
					}

					if (fRotMove > D3DX_PI)
					{
						fRotMove -= (D3DX_PI * 2);
					}
					else if (fRotMove < -D3DX_PI)
					{
						fRotMove += (D3DX_PI * 2);
					}

					g_aBullet[nCntBullet].move.x = sinf(fRotMove) * (8.0f);
					g_aBullet[nCntBullet].move.y = cosf(fRotMove) * (10.0f);
					g_aBullet[nCntBullet].rot.z -= 0.1f;

					SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);

					pPlayer->pos.x += 56.0f;
					pPlayer->pos.y -= 20.0f;
				}
				if (g_aBullet[nCntBullet].type == BULLETTYPE_6)
				{//敵のホーミング弾
					float fRotMove, fRotDest, fRotDiff;
					Player *pPlayer = GetPlayer();

					pPlayer->pos.x -= 56.0f;
					pPlayer->pos.y += 20.0f;

					fRotMove = atan2f(g_aBullet[nCntBullet].move.x, g_aBullet[nCntBullet].move.y);
					fRotDest = atan2f(pPlayer->pos.x - g_aBullet[nCntBullet].pos.x, pPlayer->pos.y - g_aBullet[nCntBullet].pos.y);
					fRotDiff = fRotDest - fRotMove;

					if (fRotDiff > D3DX_PI)
					{
						fRotDiff -= (D3DX_PI * 2);
					}
					else if (fRotDiff < -D3DX_PI)
					{
						fRotDiff += (D3DX_PI * 2);
					}

					if (g_aBullet[nCntBullet].pos.x < 1140)
					{
						fRotMove += fRotDiff * 0.06f;
					}
					else
					{
						fRotMove += fRotDiff * 0.0f;
					}

					if (fRotMove > D3DX_PI)
					{
						fRotMove -= (D3DX_PI * 2);
					}
					else if (fRotMove < -D3DX_PI)
					{
						fRotMove += (D3DX_PI * 2);
					}

					g_aBullet[nCntBullet].move.x = sinf(fRotMove) * (12.0f);
					g_aBullet[nCntBullet].move.y = cosf(fRotMove) * (12.0f);
					g_aBullet[nCntBullet].rot.z -= 1.2f;

					SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);

					pPlayer->pos.x += 56.0f;
					pPlayer->pos.y -= 20.0f;
				}
				if (g_aBullet[nCntBullet].type == BULLETTYPE_7)
				{//敵のホーミング弾
					float fRotMove, fRotDest, fRotDiff;
					Player *pPlayer = GetPlayer();

					pPlayer->pos.x -= 56.0f;
					pPlayer->pos.y += 20.0f;

					fRotMove = atan2f(g_aBullet[nCntBullet].move.x, g_aBullet[nCntBullet].move.y);
					fRotDest = atan2f(pPlayer->pos.x - g_aBullet[nCntBullet].pos.x, pPlayer->pos.y - g_aBullet[nCntBullet].pos.y);
					fRotDiff = fRotDest - fRotMove;

					if (fRotDiff > D3DX_PI)
					{
						fRotDiff -= (D3DX_PI * 2);
					}
					else if (fRotDiff < -D3DX_PI)
					{
						fRotDiff += (D3DX_PI * 2);
					}

					if (g_aBullet[nCntBullet].pos.x < 225)
					{
						fRotMove += fRotDiff * 0.001f;
					}
					else
					{
						fRotMove += fRotDiff * 1.0f;
					}

					if (fRotMove > D3DX_PI)
					{
						fRotMove -= (D3DX_PI * 2);
					}
					else if (fRotMove < -D3DX_PI)
					{
						fRotMove += (D3DX_PI * 2);
					}

					g_aBullet[nCntBullet].move.x = sinf(fRotMove) * (15.0f);
					g_aBullet[nCntBullet].move.y = cosf(fRotMove) * (15.0f);
					g_aBullet[nCntBullet].rot.z -= 1.5f;

					SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);

					pPlayer->pos.x += 56.0f;
					pPlayer->pos.y -= 20.0f;
				}
				if (g_aBullet[nCntBullet].type == BULLETTYPE_8)
				{//敵のホーミング弾
					g_aBullet[nCntBullet].move.x = 20.0f;
					g_aBullet[nCntBullet].move.y = 0.0f;
					g_aBullet[nCntBullet].rot.z -= 10.0f;

					SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);
				}
			}
			else if (g_bulletState == BULLETSTATE_HIT)
			{
				g_aBullet[nCntBullet].move.x = -25.0f;
				g_aBullet[nCntBullet].move.y = -8.0f;
				g_aBullet[nCntBullet].rot.z -= 1.5f;
				g_aBullet[nCntBullet].col.a = 1.0f;
			}

			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			//頂点座標の設定
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.z = 0.0f;

			//頂点カラーの設定
			pVtx[0].col = g_aBullet[nCntBullet].col;
			pVtx[1].col = g_aBullet[nCntBullet].col;
			pVtx[2].col = g_aBullet[nCntBullet].col;
			pVtx[3].col = g_aBullet[nCntBullet].col;

			if (pVtx[0].pos.y >= 720.0f || pVtx[1].pos.x <= 0.0f || pVtx[2].pos.x >= 1280.0f || pVtx[3].pos.y <= 0.0f)
			{
				if (g_bulletState == BULLETSTATE_HIT)
				{
					g_nScoreHit++;
					g_aScoreHit[g_aBullet[nCntBullet].type] = 1;
					AddScore(1);
				}
				else if (g_bulletState == BULLETSTATE_WAIT)
				{
					g_aScoreHit[g_aBullet[nCntBullet].type] = 0;
				}

				g_bulletState = BULLETSTATE_NORMAL;

				g_aBullet[nCntBullet].bUse = false;
			}
		}

		pVtx += 4;
	}

	g_pVtxBuffBullet->Unlock();
}

//===========================
//弾の描画処理
//===========================
void DrawBullet(void)
{
	int nCntBullet;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffBullet,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使われている
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBullet, 2);
		}
	}
}

//===========================
//弾の設定処理
//===========================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type, D3DXCOLOR col, int nEnemyType)
{
	int nCntBullet;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			g_aBullet[nCntBullet].pos.x = pos.x + 57.0f;
			g_aBullet[nCntBullet].pos.y = pos.y - 84.5f;
			g_aBullet[nCntBullet].pos.z = pos.z;

			//頂点座標の設定
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.z = 0.0f;

			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].nLife = nLife;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].col = col;
			g_aBullet[nCntBullet].bUse = true;						//使用している状態にする

			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffBullet->Unlock();
}

//===========================
//プレイヤーとの当たり判定
//===========================
void CollisionPlayer(Bullet *pBullet)
{
	Player *pPlayer = GetPlayer();					//敵の取得
	float fLength;

	pPlayer->pos.x -= 56.0f;
	pPlayer->pos.y += 20.0f;

	//距離算出
	fLength = sqrtf(((pBullet->pos.x - pPlayer->pos.x) * (pBullet->pos.x - pPlayer->pos.x)) + ((pBullet->pos.y - pPlayer->pos.y) * (pBullet->pos.y - pPlayer->pos.y))) * 0.5f;

	//当たり判定
	if (pPlayer->state == PLAYERSTATE_DAMAGE)
	{
		if (fLength <= g_fLengthHalfBullet + 10.0f)
		{
			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_BAT001);

			SetParticle(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 40.0f, 20, 2, 314, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 500);

			g_bulletState = BULLETSTATE_HIT;
		}
	}

	pPlayer->pos.x += 56.0f;
	pPlayer->pos.y -= 20.0f;
}

//===========================
//弾の取得処理
//===========================
Bullet *GetBullet(void)
{
	return &g_aBullet[0];
}

//===========================
//弾の状態取得処理
//===========================
BULLETSTATE GetBulletState(void)
{
	return g_bulletState;
}

//===========================
//弾の状態設定処理
//===========================
void SetBulletState(BULLETSTATE bulletState)
{
	g_bulletState = bulletState;
}

//===========================
//弾の状態設定処理
//===========================
int *GetBulletHit(void)
{
	return &g_aScoreHit[0];
}

//===========================
//弾の状態設定処理
//===========================
int GetBulletScore(void)
{
	return g_nScoreHit;
}