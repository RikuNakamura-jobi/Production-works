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
#include "life.h"
#include "effect.h"
#include "game.h"

//マクロ定義
#define MAX_BULLET (256)											//弾の最大数

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;												//位置
	D3DXVECTOR3 move;												//移動量
	D3DXVECTOR3 rot;												//向き
	int nLife;														//寿命
	BULLETTYPE type;												//弾のタイプ
	D3DXCOLOR col;													//色
	bool bUse;														//使用しているかどうか
}Bullet;

//プロトタイプ宣言
void CollisionEnemy(Bullet *pBullet);								//敵との当たり判定
void CollisionPlayer(Bullet *pBullet);								//プレイヤーとの当たり判定

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;							//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;					//頂点情報を格納
Bullet g_aBullet[MAX_BULLET];										//弾の情報
float g_fLengthBullet;												//対角線の長さ
float g_fAngleBullet;												//対角線の角度
float g_fLengthHalfBullet;											//半径の長さ
int g_shotEnemy[MAX_ENEMY];											//射撃している敵

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
		"data\\TEXTURE\\Bullet002.png",
		&g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 100;
		g_aBullet[nCntBullet].bUse = false;									//使用していない状態にする
	}

	//対角線の長さを算出する
	g_fLengthBullet = sqrtf((16.0f * 16.0f) + (16.0f * 16.0f)) * 0.5f;

	//対角線の角度を算出する
	g_fAngleBullet = atan2f(16.0f, 16.0f);

	g_fLengthHalfBullet = 8.0f;

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
	D3DXCOLOR col = (255, 255, 255, 255);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{//敵の通常弾
				CollisionPlayer(&g_aBullet[nCntBullet]);
				if (g_shotEnemy[nCntBullet] == 0 || g_shotEnemy[nCntBullet] == 4)
				{
					SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f), 12.0f, 6);
				}
				else if (g_shotEnemy[nCntBullet] == 1)
				{
					SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.6f), 12.0f, 6);
				}
				else if (g_shotEnemy[nCntBullet] == 2)
				{
					SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);
				}
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{//プレイヤーの通常弾
				CollisionEnemy(&g_aBullet[nCntBullet]);
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);
			}

			if (g_aBullet[nCntBullet].type == BULLETTYPE_HOMING)
			{//プレイヤーのホーミング弾
				float fRotMove, fRotDest, fRotDiff;
				Enemy *pEnemy = GetEnemy();					//敵の取得

				for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
				{
					if (pEnemy->bUse != false)
					{
						fRotMove = atan2f(g_aBullet[nCntBullet].move.x, g_aBullet[nCntBullet].move.y);
						fRotDest = atan2f(pEnemy->pos.x - g_aBullet[nCntBullet].pos.x, pEnemy->pos.y - g_aBullet[nCntBullet].pos.y);
						fRotDiff = fRotDest - fRotMove;

						if (fRotDiff > D3DX_PI)
						{
							fRotDiff -= (D3DX_PI * 2);
						}
						else if (fRotDiff < -D3DX_PI)
						{
							fRotDiff += (D3DX_PI * 2);
						}

						if (g_aBullet[nCntBullet].nLife > 80)
						{
							fRotMove += fRotDiff * 0.0f;

							if (fRotMove > D3DX_PI)
							{
								fRotMove -= (D3DX_PI * 2);
							}
							else if (fRotMove < -D3DX_PI)
							{
								fRotMove += (D3DX_PI * 2);
							}

							g_aBullet[nCntBullet].move.x = sinf(fRotMove) * (BULLET_SPEED * 0.3f);
							g_aBullet[nCntBullet].move.y = cosf(fRotMove) * (BULLET_SPEED * 0.3f);
						}
						else if (g_aBullet[nCntBullet].nLife == 80)
						{
							fRotMove += fRotDiff * 1.0f;

							if (fRotMove > D3DX_PI)
							{
								fRotMove -= (D3DX_PI * 2);
							}
							else if (fRotMove < -D3DX_PI)
							{
								fRotMove += (D3DX_PI * 2);
							}

							g_aBullet[nCntBullet].move.x = sinf(fRotMove);
							g_aBullet[nCntBullet].move.y = cosf(fRotMove);
						}
						else if (g_aBullet[nCntBullet].nLife < 80)
						{
							fRotMove += fRotDiff * 0.0f;

							if (fRotMove > D3DX_PI)
							{
								fRotMove -= (D3DX_PI * 2);
							}
							else if (fRotMove < -D3DX_PI)
							{
								fRotMove += (D3DX_PI * 2);
							}

							g_aBullet[nCntBullet].move.x = sinf(fRotMove) * (BULLET_SPEED * 2.0f);
							g_aBullet[nCntBullet].move.y = cosf(fRotMove) * (BULLET_SPEED * 2.0f);
						}

						CollisionEnemy(&g_aBullet[nCntBullet]);
						SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), 12.0f, 20);
					}
				}
			}

			if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY_HOMING)
			{//敵のホーミング弾
				float fRotMove, fRotDest, fRotDiff;
				Player *pPlayer = GetPlayer();

				for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
				{
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

					if (g_shotEnemy[nCntBullet] == 2)
					{
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

						g_aBullet[nCntBullet].move.x = sinf(fRotMove) * (BULLET_SPEED * 0.9f);
						g_aBullet[nCntBullet].move.y = cosf(fRotMove) * (BULLET_SPEED * 0.9f);

						SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);
					}
					else if (g_shotEnemy[nCntBullet] == 6)
					{
						if (g_aBullet[nCntBullet].nLife == 110)
						{
							fRotMove += fRotDiff * 1.0f;
						}
						else if (g_aBullet[nCntBullet].nLife == 60)
						{
							fRotMove += fRotDiff * 1.0f;
						}
						else if (g_aBullet[nCntBullet].nLife == 40)
						{
							fRotMove += fRotDiff * 1.0f;
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

						g_aBullet[nCntBullet].move.x = sinf(fRotMove) * (BULLET_SPEED * 0.9f);
						g_aBullet[nCntBullet].move.y = cosf(fRotMove) * (BULLET_SPEED * 0.9f);
						g_aBullet[nCntBullet].rot.z = fRotMove;

						SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);
					}

					CollisionPlayer(&g_aBullet[nCntBullet]);
				}
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

			if ((pVtx[0].pos.y >= PLAYER_HEIGHT_D || pVtx[1].pos.x <= PLAYER_WIDTH_L || pVtx[2].pos.x >= PLAYER_WIDTH_R || pVtx[3].pos.y <= PLAYER_HEIGHT_U) && g_aBullet[nCntBullet].type != BULLETTYPE_HOMING && g_aBullet[nCntBullet].type != BULLETTYPE_ENEMY_HOMING)
			{
				g_aBullet[nCntBullet].bUse = false;					//使用してない状態にする
			}

			g_aBullet[nCntBullet].nLife -= 1;

			//寿命による弾の消失処理
			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

				if (g_shotEnemy[nCntBullet] == 2 || g_shotEnemy[nCntBullet] == 3 || g_shotEnemy[nCntBullet] == 6 || g_shotEnemy[nCntBullet] == 7 || g_shotEnemy[nCntBullet] == 11)
				{
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.7f), 4);
				}
				else
				{
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), 4);
				}

				g_aBullet[nCntBullet].bUse = false;					//使用してない状態にする
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
			g_aBullet[nCntBullet].pos = pos;

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

			g_shotEnemy[nCntBullet] = nEnemyType;
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
//敵との当たり判定
//===========================
void CollisionEnemy(Bullet *pBullet)
{
	Enemy *pEnemy = GetEnemy();					//敵の取得
	int nCntEnemy;
	float fLength1, fLength2, fLengthEnemy1, fLengthEnemy2;

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			//距離算出
			if (pEnemy->nType >= 0 && pEnemy->nType <= 2)
			{
				fLengthEnemy1 = pEnemy->pos.y + cosf(pEnemy->rot.z) * pEnemy->fLength * 0.457f;
				fLengthEnemy2 = pEnemy->pos.y + cosf(pEnemy->rot.z + D3DX_PI) * pEnemy->fLength * 0.457f;
				fLength1 = sqrtf(((pBullet->pos.x - pEnemy->pos.x) * (pBullet->pos.x - pEnemy->pos.x)) + ((pBullet->pos.y - fLengthEnemy1) * (pBullet->pos.y - fLengthEnemy1))) * 0.5f;
				fLength2 = sqrtf(((pBullet->pos.x - pEnemy->pos.x) * (pBullet->pos.x - pEnemy->pos.x)) + ((pBullet->pos.y - fLengthEnemy2) * (pBullet->pos.y - fLengthEnemy2))) * 0.5f;
			}
			else if (pEnemy->nType >= 3 && pEnemy->nType <= 11)
			{
				fLength1 = sqrtf(((pBullet->pos.x - pEnemy->pos.x) * (pBullet->pos.x - pEnemy->pos.x)) + ((pBullet->pos.y - pEnemy->pos.y) * (pBullet->pos.y - pEnemy->pos.y))) * 0.5f;
			}

			//当たり判定
			if (pEnemy->nType == 3)
			{
				if (fLength1 <= g_fLengthHalfBullet + 40.96f)
				{
					//敵のヒット処理
					HitEnemy(nCntEnemy, 1);

					//サウンドの再生
					PlaySound(SOUND_LABEL_SE_HIT);

					pBullet->bUse = false;
				}
			}
			else if (pEnemy->nType == 7)
			{
				if (fLength1 <= g_fLengthHalfBullet + 40.0f)
				{
					//敵のヒット処理
					HitEnemy(nCntEnemy, 1);

					//サウンドの再生
					PlaySound(SOUND_LABEL_SE_HIT);

					pBullet->bUse = false;
				}
			}
			else if (pEnemy->nType == 11)
			{
				if (fLength1 <= g_fLengthHalfBullet + 48.0f)
				{
					//敵のヒット処理
					HitEnemy(nCntEnemy, 1);

					//サウンドの再生
					PlaySound(SOUND_LABEL_SE_HIT);

					pBullet->bUse = false;
				}
			}
			else if (pEnemy->nType == 12)
			{
				if (pEnemy->pos.x + 128 >= pBullet->pos.x && pEnemy->pos.x - 128 <= pBullet->pos.x && pEnemy->pos.y - 128 <= pBullet->pos.y && pEnemy->pos.y + 128 >= pBullet->pos.y)
				{
					//敵のヒット処理
					HitEnemy(nCntEnemy, 1);

					//サウンドの再生
					PlaySound(SOUND_LABEL_SE_HIT);

					pBullet->bUse = false;
				}
			}
			else if (pEnemy->nType >= 0 && pEnemy->nType <= 2)
			{
				if (fLength1 <= g_fLengthHalfBullet + 8.0f || fLength2 <= g_fLengthHalfBullet + 8.0f)
				{
					//敵のヒット処理
					HitEnemy(nCntEnemy, 1);

					//サウンドの再生
					PlaySound(SOUND_LABEL_SE_HIT);

					pBullet->bUse = false;
				}
			}
			else if (pEnemy->nType >= 4 && pEnemy->nType <= 6)
			{
				if (fLength1 <= g_fLengthHalfBullet + 16.0f)
				{
					//敵のヒット処理
					HitEnemy(nCntEnemy, 1);

					//サウンドの再生
					PlaySound(SOUND_LABEL_SE_HIT);

					pBullet->bUse = false;
				}
			}
			else if (pEnemy->nType >= 8 && pEnemy->nType <= 10)
			{
				if (fLength1 <= g_fLengthHalfBullet + 16.0f)
				{
					//敵のヒット処理
					HitEnemy(nCntEnemy, 1);

					//サウンドの再生
					PlaySound(SOUND_LABEL_SE_HIT);

					pBullet->bUse = false;
				}
			}
		}
	}
}

//===========================
//プレイヤーとの当たり判定
//===========================
void CollisionPlayer(Bullet *pBullet)
{
	Player *pPlayer = GetPlayer();					//敵の取得
	bool bHardMode = GetHardMode();
	float fLength;

	//距離算出
	fLength = sqrtf(((pBullet->pos.x - pPlayer->pos.x) * (pBullet->pos.x - pPlayer->pos.x)) + ((pBullet->pos.y - pPlayer->pos.y) * (pBullet->pos.y - pPlayer->pos.y))) * 0.5f;

	//当たり判定
	if (pPlayer->state == PLAYERSTATE_NORMAL)
	{
		if (fLength <= g_fLengthHalfBullet + 15.0f)
		{
			if (bHardMode == true)
			{
				//敵のヒット処理
				HitPlayer(5);
				AddLife(5);
			}
			else
			{
				//敵のヒット処理
				HitPlayer(1);
				AddLife(1);
			}

			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_HIT);

			pBullet->bUse = false;
		}
	}
}