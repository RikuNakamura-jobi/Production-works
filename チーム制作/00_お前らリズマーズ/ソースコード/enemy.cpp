//===========================
//
//ダイレクトX.enemyファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "fade.h"
#include "particle.h"
#include "effect.h"
#include "input.h"
#include "bullet.h"

//マクロ定義
#define NUM_ENEMY (13)												//敵の種類

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffEnemy = NULL;						//頂点情報を格納
Enemy g_aEnemy[MAX_ENEMY];											//敵の情報
int g_nNumEnemy;													//敵の数
int g_nTimeShot;
int g_nCntShot;
int g_nBulletCount;

//===========================
//敵の初期化処理
//===========================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ballshyuter.png",
		&g_apTextureEnemy[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\hinotamab001.png",
		&g_apTextureEnemy[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\hinotamag001.png",
		&g_apTextureEnemy[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\死神.png",
		&g_apTextureEnemy[3]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\drone_r001.png",
		&g_apTextureEnemy[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\drone_b001.png",
		&g_apTextureEnemy[5]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\drone_g001.png",
		&g_apTextureEnemy[6]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\エア・パニッシャー4.png",
		&g_apTextureEnemy[7]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\inseki002.png",
		&g_apTextureEnemy[8]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\inseki002.png",
		&g_apTextureEnemy[9]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\inseki002.png",
		&g_apTextureEnemy[10]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ドラゴン_神_3.png",
		&g_apTextureEnemy[11]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\EX_BOSS000.png",
		&g_apTextureEnemy[12]);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].fLength = 0.0f;
		g_aEnemy[nCntEnemy].fAngle = 0.0f;
		g_aEnemy[nCntEnemy].fLengthMove = 0.0f;
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].nMoveType = 0;
		g_aEnemy[nCntEnemy].nMoveTime = 0;
		g_aEnemy[nCntEnemy].nLife = 1;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].bUse = false;					//使用していない状態にする

		//対角線の長さを算出する
		g_aEnemy[nCntEnemy].fLength = sqrtf((ENEMY_X * ENEMY_X) + (ENEMY_Y * ENEMY_Y)) * 0.5f;

		//対角線の角度を算出する
		g_aEnemy[nCntEnemy].fAngle = atan2f(ENEMY_X, ENEMY_Y);
	}

	g_nNumEnemy = 0;
	g_nTimeShot = 0;
	g_nCntShot = 0;
	g_nBulletCount = 0;

	srand((unsigned int)time(0));

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffEnemy,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - D3DX_PI + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
		pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - D3DX_PI +g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI -g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
		pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI -g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z -g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
		pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z -g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z +g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
		pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z +g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
		pVtx[3].pos.z = 0.0f;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
		pVtx[1].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
		pVtx[2].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
		pVtx[3].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_VtxBuffEnemy->Unlock();
}

//===========================
//敵の終了処理
//===========================
void UninitEnemy(void)
{
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		//テクスチャの破棄
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_VtxBuffEnemy != NULL)
	{
		g_VtxBuffEnemy->Release();
		g_VtxBuffEnemy = NULL;
	}
}

//===========================
//敵の更新処理
//===========================
void UpdateEnemy(void)
{
	int nCntEnemy;
	VERTEX_2D *pVtx;
	D3DXVECTOR3 posBullet;
	D3DXVECTOR3 moveBullet;
	D3DXVECTOR3 rotBullet;
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Player *pPlayer = GetPlayer();
	GAMESTATE gameState = GetGameState();
	BULLETSTATE Bulletstate = GetBulletState();
	
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			if (g_aEnemy[nCntEnemy].nMoveTime % 240 == 0 && g_nCntShot < 10 && g_aEnemy[nCntEnemy].nMoveTime >= 120)
			{
				if (g_nCntShot < 9)
				{
					if (g_nCntShot % 9 == 0)
					{
						posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.z = g_aEnemy[nCntEnemy].pos.z;

						moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z + 1.57f) * 7.0f;
						moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z + 1.57f) * 7.0f;
						moveBullet.z = 0.0f;
						SetBullet(posBullet, moveBullet, 100, BULLETTYPE_0, col, g_aEnemy[nCntEnemy].nType);
					}
					else if (g_nCntShot % 9 == 1)
					{
						posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.z = g_aEnemy[nCntEnemy].pos.z;

						moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z + 2.7f) * 7.0f;
						moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z + 2.7f) * 7.0f;
						moveBullet.z = 0.0f;
						SetBullet(posBullet, moveBullet, 100, BULLETTYPE_1, col, g_aEnemy[nCntEnemy].nType);
					}
					else if (g_nCntShot % 9 == 2)
					{
						posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.z = g_aEnemy[nCntEnemy].pos.z;

						moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z + 1.57f) * 7.0f;
						moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z + 1.57f) * 7.0f;
						moveBullet.z = 0.0f;
						SetBullet(posBullet, moveBullet, 100, BULLETTYPE_2, col, g_aEnemy[nCntEnemy].nType);
					}
					else if (g_nCntShot % 9 == 3)
					{
						posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.z = g_aEnemy[nCntEnemy].pos.z;

						moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z + 2.355f) * 7.0f;
						moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z + 2.355f) * 7.0f;
						moveBullet.z = 0.0f;
						SetBullet(posBullet, moveBullet, 100, BULLETTYPE_3, col, g_aEnemy[nCntEnemy].nType);
					}
					else if (g_nCntShot % 9 == 4)
					{
						posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.z = g_aEnemy[nCntEnemy].pos.z;

						moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z + 2.355f) * 7.0f;
						moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z + 2.355f) * 7.0f;
						moveBullet.z = 0.0f;
						SetBullet(posBullet, moveBullet, 100, BULLETTYPE_4, col, g_aEnemy[nCntEnemy].nType);
					}
					else if (g_nCntShot % 9 == 5)
					{
						posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.z = g_aEnemy[nCntEnemy].pos.z;

						moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z + 2.355f) * 7.0f;
						moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z + 2.355f) * 7.0f;
						moveBullet.z = 0.0f;
						SetBullet(posBullet, moveBullet, 100, BULLETTYPE_5, col, g_aEnemy[nCntEnemy].nType);
					}
					else if (g_nCntShot % 9 == 6)
					{
						posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.z = g_aEnemy[nCntEnemy].pos.z;

						moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * 7.0f;
						moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * 7.0f;
						moveBullet.z = 0.0f;
						SetBullet(posBullet, moveBullet, 100, BULLETTYPE_6, col, g_aEnemy[nCntEnemy].nType);
					}
					else if (g_nCntShot % 9 == 7)
					{
						posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.z = g_aEnemy[nCntEnemy].pos.z;

						moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z + 3.0f) * 7.0f;
						moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z + 3.0f) * 7.0f;
						moveBullet.z = 0.0f;
						SetBullet(posBullet, moveBullet, 100, BULLETTYPE_7, col, g_aEnemy[nCntEnemy].nType);
					}
					else if (g_nCntShot % 9 == 8)
					{
						posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
						posBullet.z = g_aEnemy[nCntEnemy].pos.z;

						moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z + 1.57f) * 7.0f;
						moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z + 1.57f) * 7.0f;
						moveBullet.z = 0.0f;
						SetBullet(posBullet, moveBullet, 100, BULLETTYPE_8, col, g_aEnemy[nCntEnemy].nType);
					}

					g_nBulletCount += 1;

					SetBulletState(BULLETSTATE_WAIT);
				}

				g_nCntShot++;
			}
		}
	}

	if (g_nCntShot >= 10 && gameState != GAMESTATE_END)
	{
		SetGameState(GAMESTATE_END, 30);
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (Bulletstate == BULLETSTATE_NORMAL)
		{
			g_aEnemy[nCntEnemy].nMoveTime++;
		}

		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//ダメージ状態判定
			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:
				break;
			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCounterState--;

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

					if (g_aEnemy[nCntEnemy].nType >= 8 && g_aEnemy[nCntEnemy].nType <= 10)
					{
						//頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(0.7f, 0.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(0.7f, 0.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(0.7f, 0.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(0.7f, 0.0f, 1.0f, 1.0f);
					}
					else
					{
						//頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}
				}
				break;
			}
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_VtxBuffEnemy->Unlock();
}

//===========================
//敵の描画処理
//===========================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_VtxBuffEnemy,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//===========================
//敵の設定処理
//===========================
void SetEnemy(D3DXVECTOR3 pos, int nType, int nMoveType, int nMoveTime)
{
	int nCntEnemy;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].nType = nType;
			g_aEnemy[nCntEnemy].nMoveTime -= nMoveTime;

			g_aEnemy[nCntEnemy].nLife = 5;
			g_aEnemy[nCntEnemy].nMoveType = nMoveType;
			g_aEnemy[nCntEnemy].nMoveTime -= nMoveTime;

			//対角線の長さを算出する
			g_aEnemy[nCntEnemy].fLength = sqrtf((ENEMY_X * ENEMY_X) + (ENEMY_Y * ENEMY_Y)) * 0.5f;

			//対角線の角度を算出する
			g_aEnemy[nCntEnemy].fAngle = atan2f(ENEMY_X, ENEMY_Y);

			//頂点座標の設定
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - D3DX_PI + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - D3DX_PI + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[3].pos.z = 0.0f;

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

			g_aEnemy[nCntEnemy].bUse = true;						//使用している状態にする

			g_nNumEnemy++;

			break;
		}
		pVtx += 4;
	}

	g_VtxBuffEnemy->Unlock();
}

//===========================
//敵の情報取得処理
//===========================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//===========================
//敵のヒット処理
//===========================
void HitEnemy(int nCntEnemy, int nDamage)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Player *pPlayer = GetPlayer();

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		//サウンドの再生
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		
		g_nNumEnemy--;

		g_aEnemy[nCntEnemy].nMoveTime = 0;
		
		g_aEnemy[nCntEnemy].bUse = false;
	}
	else
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = 5;

		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_VtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (4 * nCntEnemy);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		g_VtxBuffEnemy->Unlock();
	}
}

//===========================
//敵の全滅判定処理
//===========================
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}

//===========================
//プレイヤーとの当たり判定
//===========================
void CollisionPlayerENEMY(int nCntEnemy)
{
	Player *pPlayer = GetPlayer();					//敵の取得
	float fLength1, fLength2, fLengthEnemy1, fLengthEnemy2;

	//距離算出
	if (g_aEnemy[nCntEnemy].nType >= 0 && g_aEnemy[nCntEnemy].nType <= 2)
	{
		fLengthEnemy1 = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.457f;
		fLengthEnemy2 = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI) * g_aEnemy[nCntEnemy].fLength * 0.457f;
		fLength1 = sqrtf(((pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x) * (pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x)) + ((pPlayer->pos.y - fLengthEnemy1) * (pPlayer->pos.y - fLengthEnemy1))) * 0.5f;
		fLength2 = sqrtf(((pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x) * (pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x)) + ((pPlayer->pos.y - fLengthEnemy2) * (pPlayer->pos.y - fLengthEnemy2))) * 0.5f;
	}
	else if (g_aEnemy[nCntEnemy].nType >= 3 && g_aEnemy[nCntEnemy].nType <= 12)
	{
		fLength1 = sqrtf(((pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x) * (pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x)) + ((pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y) * (pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y))) * 0.5f;
	}

	//当たり判定
	if (pPlayer->state == PLAYERSTATE_NORMAL)
	{

	}
}

//===========================
//プレイヤーとの当たり判定
//===========================
int GetBulletCount(void)
{
	return g_nBulletCount;
}