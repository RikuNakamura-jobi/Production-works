//===========================
//
//ダイレクトX.enemyファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "life.h"
#include "fade.h"
#include "particle.h"
#include "effect.h"
#include "boss.h"

//マクロ定義
#define NUM_ENEMY (13)												//敵の種類

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffEnemy = NULL;						//頂点情報を格納
Enemy g_aEnemy[MAX_ENEMY];											//敵の情報
int g_nNumEnemy;													//敵の数
int g_nCounterShotTime = 0;											//敵が撃ち始めるまでの時間
int g_nTimeStop;													//敵のストップ用変数
float g_fEnemyMove;													//停止時の移動量格納変数
float g_fEnemyColorTimer;											//敵の色変更タイマー変数
int g_nBossShot = 0;												//ボスの攻撃判別変数

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
		"data\\TEXTURE\\hinotamar001.png",
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
		g_aEnemy[nCntEnemy].nMoveTime = 40;
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
	g_nCounterShotTime = 10;
	g_nTimeStop = 0;
	g_nBossShot = 0;
	g_fEnemyColorTimer = 0.0f;

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
	float fRotMove, fRotDest, fRotDiff;
	VERTEX_2D *pVtx;
	D3DXVECTOR3 posBullet;
	D3DXVECTOR3 moveBullet;
	D3DXVECTOR3 rotBullet;
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Player *pPlayer = GetPlayer();
	WAVESTATE waveState = GetWaveState();

	g_nCounterShotTime--;

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].pos.x > 300.0f && g_aEnemy[nCntEnemy].pos.x < 980.0f && g_aEnemy[nCntEnemy].pos.y > 0.0f && g_aEnemy[nCntEnemy].pos.y < 720.0f)
		{//敵の攻撃処理
			if (g_aEnemy[nCntEnemy].nType == 0 && g_aEnemy[nCntEnemy].nMoveTime % 100 == 0)
			{
				float fRotMove, fRotDest, fRotDiff;
				Player *pPlayer = GetPlayer();
				col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
				fRotDest = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);
				fRotDiff = fRotDest - fRotMove;

				posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
				posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
				posBullet.z = g_aEnemy[nCntEnemy].pos.z;

				moveBullet.x = sinf(fRotDest) * 9.0f;
				moveBullet.y = cosf(fRotDest) * 9.0f;
				moveBullet.z = 0.0f;
				SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, g_aEnemy[nCntEnemy].nType);
			}
			else if (g_aEnemy[nCntEnemy].nType == 4 && g_aEnemy[nCntEnemy].nMoveTime % 100 == 0)
			{
				float fRotMove, fRotDest, fRotDiff;
				Player *pPlayer = GetPlayer();
				col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);


				fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
				fRotDest = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);
				fRotDiff = fRotDest - fRotMove;

				posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
				posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
				posBullet.z = g_aEnemy[nCntEnemy].pos.z;

				moveBullet.x = sinf(fRotDest + 0.2f) * 9.0f;
				moveBullet.y = cosf(fRotDest + 0.2f) * 9.0f;
				moveBullet.z = 0.0f;
				SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, g_aEnemy[nCntEnemy].nType);

				posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
				posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
				posBullet.z = g_aEnemy[nCntEnemy].pos.z;

				moveBullet.x = sinf(fRotDest - 0.2f) * 9.0f;
				moveBullet.y = cosf(fRotDest - 0.2f) * 9.0f;
				moveBullet.z = 0.0f;
				SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, g_aEnemy[nCntEnemy].nType);
			}
			else if ((g_aEnemy[nCntEnemy].nType == 1 || g_aEnemy[nCntEnemy].nType == 5) && g_aEnemy[nCntEnemy].nMoveTime % 70 == 0)
			{
				col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

				posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
				posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
				posBullet.z = g_aEnemy[nCntEnemy].pos.z;

				moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z) * 7.0f;
				moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z) * 7.0f;
				moveBullet.z = 0.0f;
				SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, g_aEnemy[nCntEnemy].nType);

				posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
				posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
				posBullet.z = g_aEnemy[nCntEnemy].pos.z;

				moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z + 1.57f) * 7.0f;
				moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z + 1.57f) * 7.0f;
				moveBullet.z = 0.0f;
				SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, g_aEnemy[nCntEnemy].nType);

				posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
				posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
				posBullet.z = g_aEnemy[nCntEnemy].pos.z;

				moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z - 1.57f) * 7.0f;
				moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z - 1.57f) * 7.0f;
				moveBullet.z = 0.0f;
				SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, g_aEnemy[nCntEnemy].nType);

				posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
				posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
				posBullet.z = g_aEnemy[nCntEnemy].pos.z;

				moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI) * 7.0f;
				moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI) * 7.0f;
				moveBullet.z = 0.0f;
				SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, g_aEnemy[nCntEnemy].nType);
			}
			else if ((g_aEnemy[nCntEnemy].nType == 2 || g_aEnemy[nCntEnemy].nType == 6) && g_aEnemy[nCntEnemy].nMoveTime % 200 == 0)
			{
				col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
				posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
				posBullet.z = g_aEnemy[nCntEnemy].pos.z;

				moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z) * 7.0f;
				moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z) * 7.0f;
				moveBullet.z = 0.0f;
				SetBullet(posBullet, moveBullet, 120, BULLETTYPE_ENEMY_HOMING, col, g_aEnemy[nCntEnemy].nType);
			}
			else if (g_aEnemy[nCntEnemy].nType == 3 && g_aEnemy[nCntEnemy].nMoveTime % 60 == 0)
			{
				g_nBossShot++;

				if (g_nBossShot % 4 == 0 && g_aEnemy[nCntEnemy].nLife <= 300)
				{
					PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

					if (g_nBossShot < 50)
					{
						for (int nCnt = 0; nCnt < 15; nCnt++)
						{
							posBullet.x = ((float)(rand() % 9 + 1) * 68.0f) + 300.0f;
							posBullet.y = ((float)(rand() % 9 + 1) * 72.0f);

							rotBullet.z = (float)(rand() % 629 - 314) / 100.0f;

							SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 3.0f, 2000.0f, 100, 0);
						}
					}
					else if (g_nBossShot < 75)
					{
						for (int nCnt = 0; nCnt < 30; nCnt++)
						{
							posBullet.x = ((float)(rand() % 9 + 1) * 68.0f) + 300.0f;
							posBullet.y = ((float)(rand() % 9 + 1) * 72.0f);

							rotBullet.z = (float)(rand() % 629 - 314) / 100.0f;

							SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 3.0f, 2000.0f, 100, 0);
						}
					}
					else if (g_nBossShot < 100)
					{
						for (int nCnt = 0; nCnt < 60; nCnt++)
						{
							posBullet.x = ((float)(rand() % 9 + 1) * 68.0f) + 300.0f;
							posBullet.y = ((float)(rand() % 9 + 1) * 72.0f);

							rotBullet.z = (float)(rand() % 629 - 314) / 100.0f;

							SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 3.0f, 2000.0f, 100, 0);
						}
					}
					else if (g_nBossShot >= 100)
					{
						for (int nCnt = 0; nCnt < 0; nCnt++)
						{
							posBullet.x = ((float)(rand() % 9 + 1) * 68.0f) + 300.0f;
							posBullet.y = ((float)(rand() % 9 + 1) * 72.0f);

							rotBullet.z = (float)(rand() % 629 - 314) / 100.0f;

							SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 3.0f, 2000.0f, 100, 0);
						}

						for (int nCnt = 0; nCnt < 120; nCnt++)
						{
							posBullet.x = pPlayer->pos.x;
							posBullet.y = pPlayer->pos.y;

							rotBullet.z =  ((5.241f * (float)nCnt) - 314.0f) / 100.0f;

							SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 3.0f, 2000.0f, 100, 0);
						}
					}
				}
				else if (g_nBossShot % 3 == 0 && g_aEnemy[nCntEnemy].nLife > 300)
				{
					float fRotMove, fRotDest, fRotDiff;
					Player *pPlayer = GetPlayer();
					col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
					fRotDest = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);
					fRotDiff = fRotDest - fRotMove;

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(fRotDest) * 9.0f;
					moveBullet.y = cosf(fRotDest) * 9.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, 0);
				}
				else if (g_nBossShot % 3 == 1 && g_aEnemy[nCntEnemy].nLife > 300)
				{
					col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z) * 7.0f;
					moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z) * 7.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, 1);

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z + 1.57f) * 7.0f;
					moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z + 1.57f) * 7.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, 1);

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z - 1.57f) * 7.0f;
					moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z - 1.57f) * 7.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, 1);

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI) * 7.0f;
					moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI) * 7.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, 1);
				}
				else if (g_nBossShot % 3 == 2 && g_aEnemy[nCntEnemy].nLife > 300)
				{
					col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z) * 7.0f;
					moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z) * 7.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 120, BULLETTYPE_ENEMY_HOMING, col, 2);
				}
			}
			else if (g_aEnemy[nCntEnemy].nType == 7 && g_aEnemy[nCntEnemy].nMoveTime % 60 == 0)
			{
				g_nBossShot++;

				if (g_nBossShot % 3 == 1 && g_aEnemy[nCntEnemy].nLife <= 300)
				{
					PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

					if (g_nBossShot < 50)
					{
						for (int nCnt = 0; nCnt < 8; nCnt++)
						{
							posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
							posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * g_aEnemy[nCntEnemy].fLength * 0.4f);

							rotBullet.z = (float)(rand() % 629 - 314) / 100.0f;

							SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 30.0f, 2000.0f, 120, 1);
						}
					}
					else if (g_nBossShot < 75)
					{
						for (int nCnt = 0; nCnt < 16; nCnt++)
						{
							posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
							posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * g_aEnemy[nCntEnemy].fLength * 0.4f);

							rotBullet.z = (float)(rand() % 629 - 314) / 100.0f;

							SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 30.0f, 2000.0f, 120, 1);
						}
					}
					else if (g_nBossShot < 100)
					{
						for (int nCnt = 0; nCnt < 32; nCnt++)
						{
							posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
							posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * g_aEnemy[nCntEnemy].fLength * 0.4f);

							rotBullet.z = (float)(rand() % 629 - 314) / 100.0f;

							SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 30.0f, 2000.0f, 120, 1);
						}
					}
					else if (g_nBossShot >= 100)
					{
						for (int nCnt = 0; nCnt < 64; nCnt++)
						{
							posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
							posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * g_aEnemy[nCntEnemy].fLength * 0.4f);

							rotBullet.z = (float)(rand() % 629 - 314) / 100.0f;

							SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 30.0f, 2000.0f, 120, 1);
						}
					}
				}
				else if (g_nBossShot % 3 == 0)
				{
					float fRotMove, fRotDest, fRotDiff;
					Player *pPlayer = GetPlayer();
					col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
					fRotDest = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);
					fRotDiff = fRotDest - fRotMove;

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(fRotDest + 0.2f) * 9.0f;
					moveBullet.y = cosf(fRotDest + 0.2f) * 9.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, 4);

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(fRotDest - 0.2f) * 9.0f;
					moveBullet.y = cosf(fRotDest - 0.2f) * 9.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, 4);
				}
				else if (g_nBossShot % 3 == 2)
				{
					col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z) * 7.0f;
					moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z) * 7.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 120, BULLETTYPE_ENEMY_HOMING, col, 6);
				}
			}
			else if (g_aEnemy[nCntEnemy].nType == 11 && g_aEnemy[nCntEnemy].nMoveTime % 40 == 0)
			{
				g_nBossShot++;

				if (g_nBossShot < 30)
				{
					if (g_nBossShot % 4 == 0 && g_aEnemy[nCntEnemy].nLife <= 300)
					{
						posBullet.x = pPlayer->pos.x;
						posBullet.y = g_aEnemy[nCntEnemy].pos.y;

						rotBullet.z = 0.0f;

						PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

						SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 0.0f, 2000.0f, 140, 2);
					}

				}
				else if (g_nBossShot < 50)
				{
					if (g_nBossShot % 3 == 0 && g_aEnemy[nCntEnemy].nLife <= 300)
					{
						posBullet.x = pPlayer->pos.x;
						posBullet.y = g_aEnemy[nCntEnemy].pos.y;

						rotBullet.z = 0.0f;

						PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

						SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 0.0f, 2000.0f, 140, 2);
					}
				}
				else if (g_nBossShot < 70)
				{
					if (g_nBossShot % 2 == 0 && g_aEnemy[nCntEnemy].nLife <= 300)
					{
						posBullet.x = pPlayer->pos.x;
						posBullet.y = g_aEnemy[nCntEnemy].pos.y;

						rotBullet.z = 0.0f;

						PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

						SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 0.0f, 2000.0f, 140, 2);
					}
				}
				else if (g_nBossShot >= 70)
				{
					if (g_aEnemy[nCntEnemy].nLife <= 300)
					{
						posBullet.x = pPlayer->pos.x;
						posBullet.y = g_aEnemy[nCntEnemy].pos.y;

						rotBullet.z = 0.0f;

						PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

						SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 0.0f, 2000.0f, 140, 2);
					}
				}

				if (g_nBossShot % 6 == 0)
				{
					float fRotMove, fRotDest, fRotDiff;
					Player *pPlayer = GetPlayer();
					col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
					fRotDest = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);
					fRotDiff = fRotDest - fRotMove;

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(fRotDest) * 9.0f;
					moveBullet.y = cosf(fRotDest) * 9.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, 0);

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(fRotDest + 0.2f) * 9.0f;
					moveBullet.y = cosf(fRotDest + 0.2f) * 9.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, 0);

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(fRotDest - 0.2f) * 9.0f;
					moveBullet.y = cosf(fRotDest - 0.2f) * 9.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, 0);
				}
				else if (g_nBossShot % 6 == 2)
				{
					col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z) * 7.0f;
					moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z) * 7.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, 1);

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z + 1.57f) * 7.0f;
					moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z + 1.57f) * 7.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, 1);

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z - 1.57f) * 7.0f;
					moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z - 1.57f) * 7.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, 1);

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI) * 7.0f;
					moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI) * 7.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 200, BULLETTYPE_ENEMY, col, 1);
				}
				else if (g_nBossShot % 6 == 4)
				{
					col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z - 1.0f) * 7.0f;
					moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z - 1.0f) * 7.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 120, BULLETTYPE_ENEMY_HOMING, col, 2);

					posBullet.x = (g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.y = (g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - 2.9f) * g_aEnemy[nCntEnemy].fLength * 0.4f);
					posBullet.z = g_aEnemy[nCntEnemy].pos.z;

					moveBullet.x = sinf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * 7.0f;
					moveBullet.y = cosf(g_aEnemy[nCntEnemy].rot.z + 1.0f) * 7.0f;
					moveBullet.z = 0.0f;
					SetBullet(posBullet, moveBullet, 120, BULLETTYPE_ENEMY_HOMING, col, 6);
				}
			}
			else if (g_aEnemy[nCntEnemy].nType == 12 && g_aEnemy[nCntEnemy].nMoveTime % 40 == 0)
			{
				g_nBossShot++;

				if (g_nBossShot < 100)
				{
					if (g_nBossShot % 6 == 0)
					{
						posBullet.x = pPlayer->pos.x;
						posBullet.y = g_aEnemy[nCntEnemy].pos.y;

						rotBullet.z = 0.0f;

						PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

						SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 0.0f, 2000.0f, 140, 2);
					}
					else if (g_nBossShot % 6 == 2)
					{
						PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

						for (int nCnt = 0; nCnt < 8; nCnt++)
						{
							posBullet.x = g_aEnemy[nCntEnemy].pos.x;
							posBullet.y = g_aEnemy[nCntEnemy].pos.y;

							rotBullet.z = (float)(rand() % 629 - 314) / 100.0f;

							SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 30.0f, 2000.0f, 120, 1);
						}
					}
					else if (g_nBossShot % 6 == 4)
					{
						PlaySound(SOUND_LABEL_SE_BOSSWARNING001);

						for (int nCnt = 0; nCnt < 15; nCnt++)
						{
							posBullet.x = ((float)(rand() % 9 + 1) * 68.0f) + 300.0f;
							posBullet.y = ((float)(rand() % 9 + 1) * 72.0f);

							rotBullet.z = (float)(rand() % 629 - 314) / 100.0f;

							SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 3.0f, 2000.0f, 100, 0);
						}
					}
				}
				else
				{
					posBullet.x = pPlayer->pos.x;
					posBullet.y = g_aEnemy[nCntEnemy].pos.y;

					rotBullet.z = 0.0f;

					SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 0.0f, 2000.0f, 140, 2);

					if (g_nBossShot % 2 == 0)
					{
						for (int nCnt = 0; nCnt < 64; nCnt++)
						{
							posBullet.x = g_aEnemy[nCntEnemy].pos.x;
							posBullet.y = g_aEnemy[nCntEnemy].pos.y;

							rotBullet.z = (float)(rand() % 629 - 314) / 100.0f;

							SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 30.0f, 2000.0f, 120, 1);
						}
					}
					else if (g_nBossShot % 2 == 1)
					{
						for (int nCnt = 0; nCnt < 120; nCnt++)
						{
							posBullet.x = ((float)(rand() % 9 + 1) * 68.0f) + 300.0f;
							posBullet.y = ((float)(rand() % 9 + 1) * 72.0f);

							rotBullet.z = (float)(rand() % 629 - 314) / 100.0f;

							SetBoss(posBullet, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), rotBullet, 3.0f, 2000.0f, 100, 0);
						}
					}
				}
			}
		}
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
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

			g_aEnemy[nCntEnemy].nMoveTime++;

			//敵の移動
			if (g_aEnemy[nCntEnemy].nMoveType == 0)
			{
				fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
				fRotDest = atan2f(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].pos.x, 820.0f - g_aEnemy[nCntEnemy].pos.y);
				fRotDiff = fRotDest - fRotMove;
			}
			else if (g_aEnemy[nCntEnemy].nMoveType == 1)
			{
				if (g_aEnemy[nCntEnemy].nMoveTime <= 300)
				{
					fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
					fRotDest = atan2f(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].pos.x, 720.0f - g_aEnemy[nCntEnemy].pos.y);
					fRotDiff = fRotDest - fRotMove;
				}
				else if (g_aEnemy[nCntEnemy].nMoveTime <= 440)
				{
					fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
					fRotDest = atan2f(880.0f - g_aEnemy[nCntEnemy].pos.x, 100.0f - g_aEnemy[nCntEnemy].pos.y);
					fRotDiff = fRotDest - fRotMove;

				}
				else
				{
					fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
					fRotDest = atan2f(780.0f - g_aEnemy[nCntEnemy].pos.x, 820.0f - g_aEnemy[nCntEnemy].pos.y);
					fRotDiff = fRotDest - fRotMove;
				}
			}
			else if (g_aEnemy[nCntEnemy].nMoveType == 2)
			{
				if (g_aEnemy[nCntEnemy].nMoveTime <= 300)
				{
					fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
					fRotDest = atan2f(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].pos.x, 720.0f - g_aEnemy[nCntEnemy].pos.y);
					fRotDiff = fRotDest - fRotMove;
				}
				else if (g_aEnemy[nCntEnemy].nMoveTime <= 440)
				{
					fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
					fRotDest = atan2f(400.0f - g_aEnemy[nCntEnemy].pos.x, 100.0f - g_aEnemy[nCntEnemy].pos.y);
					fRotDiff = fRotDest - fRotMove;
				}
				else
				{
					fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
					fRotDest = atan2f(500.0f - g_aEnemy[nCntEnemy].pos.x, 820.0f - g_aEnemy[nCntEnemy].pos.y);
					fRotDiff = fRotDest - fRotMove;
				}
			}
			else if (g_aEnemy[nCntEnemy].nMoveType == 3)
			{
				fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
				fRotDest = atan2f(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].pos.x, 160.0f - g_aEnemy[nCntEnemy].pos.y);
				fRotDiff = fRotDest - fRotMove;

				if (g_aEnemy[nCntEnemy].nType == 3)
				{
					SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(0.3f, 0.0f, 1.0f, 1.0f), 40.0f, 20, 2, 314, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 500);
				}
				else if (g_aEnemy[nCntEnemy].nType == 7)
				{
					SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 40.0f, 20, 2, 314, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 500);
				}
				else if (g_aEnemy[nCntEnemy].nType == 11)
				{
					SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 40.0f, 20, 2, 314, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 500);
				}
			}
			else if (g_aEnemy[nCntEnemy].nMoveType == 4)
			{
				fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
				fRotDest = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, 820.0f - g_aEnemy[nCntEnemy].pos.y);
				fRotDiff = fRotDest - fRotMove;
			}
			else if (g_aEnemy[nCntEnemy].nMoveType == 5)
			{
				if (g_aEnemy[nCntEnemy].nMoveTime <= 280)
				{
					fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
					fRotDest = atan2f(880.0f - g_aEnemy[nCntEnemy].pos.x, 300.0f - g_aEnemy[nCntEnemy].pos.y);
					fRotDiff = fRotDest - fRotMove;
				}
				else
				{
					fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
					fRotDest = atan2f(300.0f - g_aEnemy[nCntEnemy].pos.x, 820.0f - g_aEnemy[nCntEnemy].pos.y);
					fRotDiff = fRotDest - fRotMove;

				}
			}
			else if (g_aEnemy[nCntEnemy].nMoveType == 6)
			{
				if (g_aEnemy[nCntEnemy].nMoveTime <= 280)
				{
					fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
					fRotDest = atan2f(400.0f - g_aEnemy[nCntEnemy].pos.x, 300.0f - g_aEnemy[nCntEnemy].pos.y);
					fRotDiff = fRotDest - fRotMove;
				}
				else
				{
					fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
					fRotDest = atan2f(980.0f - g_aEnemy[nCntEnemy].pos.x, 820.0f - g_aEnemy[nCntEnemy].pos.y);
					fRotDiff = fRotDest - fRotMove;

				}
			}
			else if (g_aEnemy[nCntEnemy].nMoveType == 7)
			{
				fRotMove = atan2f(g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].move.y);
				fRotDest = atan2f(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].pos.x, 160.0f - g_aEnemy[nCntEnemy].pos.y);
				fRotDiff = fRotDest - fRotMove;
			}

			if (fRotDiff > D3DX_PI)
			{
				fRotDiff -= (D3DX_PI * 2);
			}
			else if (fRotDiff < -D3DX_PI)
			{
				fRotDiff += (D3DX_PI * 2);
			}

			if (g_aEnemy[nCntEnemy].nMoveType == 0)
			{
				fRotMove += fRotDiff * 1.0f;
			}
			else if (g_aEnemy[nCntEnemy].nMoveType == 1)
			{
				fRotMove += fRotDiff * 0.3f;
			}
			else if (g_aEnemy[nCntEnemy].nMoveType == 2)
			{
				fRotMove += fRotDiff * 0.3f;
			}
			else if (g_aEnemy[nCntEnemy].nMoveType == 3)
			{
				fRotMove += fRotDiff * 1.0f;
			}
			else if (g_aEnemy[nCntEnemy].nMoveType == 4)
			{
				if (g_aEnemy[nCntEnemy].pos.y >= 0.0f)
				{
					fRotMove += fRotDiff * 0.01f;
				}
				else
				{
					fRotMove += fRotDiff * 0.0f;
				}
			}
			else if (g_aEnemy[nCntEnemy].nMoveType == 5)
			{
				fRotMove += fRotDiff * 1.0f;
			}
			else if (g_aEnemy[nCntEnemy].nMoveType == 6)
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

			if (g_aEnemy[nCntEnemy].nMoveType == 3 && g_aEnemy[nCntEnemy].pos.y >= 150)
			{
				if (g_aEnemy[nCntEnemy].nType == 7 && g_aEnemy[nCntEnemy].nMoveTime % 60 == 0  && g_nBossShot % 3 == 0 && g_aEnemy[nCntEnemy].nLife < 300)
				{
					g_nTimeStop++;
					g_fEnemyMove = g_aEnemy[nCntEnemy].move.x;
				}
				else if (g_aEnemy[nCntEnemy].nType == 7 && g_aEnemy[nCntEnemy].nMoveTime % 180 == 0 && g_aEnemy[nCntEnemy].nLife < 300 && g_nTimeStop > 0)
				{
					g_nTimeStop--;
					g_aEnemy[nCntEnemy].move.x = g_fEnemyMove;
				}

				if (g_aEnemy[nCntEnemy].nMoveTime <= 180)
				{
					g_aEnemy[nCntEnemy].move.x = 0.0f;
					g_aEnemy[nCntEnemy].move.y = 0.0f;
					g_aEnemy[nCntEnemy].rot.z = fRotMove;
				}
				else if (g_nTimeStop > 0)
				{
					g_aEnemy[nCntEnemy].move.x = 0.0f;
					g_aEnemy[nCntEnemy].move.y = 0.0f;
					g_aEnemy[nCntEnemy].rot.z = fRotMove;
				}
				else if (g_aEnemy[nCntEnemy].pos.x < 400.0f)
				{
					g_aEnemy[nCntEnemy].pos.x = 400.0f;
					g_aEnemy[nCntEnemy].move.x = 3.0f;
				}
				else if (g_aEnemy[nCntEnemy].pos.x > 880.0f)
				{
					g_aEnemy[nCntEnemy].pos.x = 880.0f;
					g_aEnemy[nCntEnemy].move.x = -3.0f;
				}
				else if (g_aEnemy[nCntEnemy].move.x != -3.0f)
				{
					g_aEnemy[nCntEnemy].move.x = 3.0f;
					g_aEnemy[nCntEnemy].move.y = 0.0f;
					g_aEnemy[nCntEnemy].rot.z = fRotMove;
				}
				else if (g_aEnemy[nCntEnemy].move.x == -3.0f)
				{
					g_aEnemy[nCntEnemy].move.x = -3.0f;
					g_aEnemy[nCntEnemy].move.y = 0.0f;
					g_aEnemy[nCntEnemy].rot.z = fRotMove;
				}
			}
			else if (g_aEnemy[nCntEnemy].nMoveType == 7 && g_aEnemy[nCntEnemy].pos.y >= 150)
			{
				g_aEnemy[nCntEnemy].move.x = 0.0f;
				g_aEnemy[nCntEnemy].move.y = 0.0f;
				g_aEnemy[nCntEnemy].rot.z = fRotMove;
			}
			else
			{
				g_aEnemy[nCntEnemy].move.x = sinf(fRotMove) * (2.0f);
				g_aEnemy[nCntEnemy].move.y = cosf(fRotMove) * (2.0f);
				g_aEnemy[nCntEnemy].rot.z = fRotMove;
			}

			CollisionPlayerENEMY(nCntEnemy);

			if (g_aEnemy[nCntEnemy].pos.y >= 820.0f)
			{
				g_nNumEnemy--;

				g_aEnemy[nCntEnemy].nMoveTime = 0;

				g_aEnemy[nCntEnemy].bUse = false;
			}

			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

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

			if (g_aEnemy[nCntEnemy].nType == 12)
			{
				col.r = (sinf(g_fEnemyColorTimer) + 1.0f) / 2;
				col.g = (cosf(g_fEnemyColorTimer) + 1.0f) / 2;
				col.b = (sinf(g_fEnemyColorTimer) + cosf(g_fEnemyColorTimer) + 1.0f) / 2;
				g_fEnemyColorTimer += 0.01f;

				//頂点カラーの設定
				pVtx[0].col = col;

				col.r = (sinf(g_fEnemyColorTimer) + 1.0f) / 2;
				col.g = (cosf(g_fEnemyColorTimer) + 1.0f) / 2;
				col.b = (sinf(g_fEnemyColorTimer) + cosf(g_fEnemyColorTimer) + 1.0f) / 2;
				g_fEnemyColorTimer += 0.01f;

				pVtx[1].col = col;

				col.r = (sinf(g_fEnemyColorTimer) + 1.0f) / 2;
				col.g = (cosf(g_fEnemyColorTimer) + 1.0f) / 2;
				col.b = (sinf(g_fEnemyColorTimer) + cosf(g_fEnemyColorTimer) + 1.0f) / 2;
				g_fEnemyColorTimer += 0.01f;

				pVtx[2].col = col;

				col.r = (sinf(g_fEnemyColorTimer) + 1.0f) / 2;
				col.g = (cosf(g_fEnemyColorTimer) + 1.0f) / 2;
				col.b = (sinf(g_fEnemyColorTimer) + cosf(g_fEnemyColorTimer) + 1.0f) / 2;
				g_fEnemyColorTimer += 0.01f;

				pVtx[3].col = col;

				col.r = (sinf(g_fEnemyColorTimer) + 1.0f) / 2;
				col.g = (cosf(g_fEnemyColorTimer) + 1.0f) / 2;
				col.b = (sinf(g_fEnemyColorTimer) + cosf(g_fEnemyColorTimer) + 1.0f) / 2;
				g_fEnemyColorTimer += 0.01f;
			}

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
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

			if (nType == 3)
			{
				g_aEnemy[nCntEnemy].nLife = 600;
				g_aEnemy[nCntEnemy].nMoveType = nMoveType;

				//対角線の長さを算出する
				g_aEnemy[nCntEnemy].fLength = sqrtf((256.0f * 256.0f) + (256.0f * 256.0f)) * 0.5f;

				//対角線の角度を算出する
				g_aEnemy[nCntEnemy].fAngle = atan2f(256.0f, 256.0f);

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
			}
			else if (nType == 7 || nType == 12)
			{
				g_aEnemy[nCntEnemy].nLife = 600;
				g_aEnemy[nCntEnemy].nMoveType = nMoveType;
				g_aEnemy[nCntEnemy].nMoveTime -= nMoveTime;

				//対角線の長さを算出する
				g_aEnemy[nCntEnemy].fLength = sqrtf((256.0f * 256.0f) + (256.0f * 256.0f)) * 0.5f;

				//対角線の角度を算出する
				g_aEnemy[nCntEnemy].fAngle = atan2f(256.0f, 256.0f);

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
			}
			else if (nType == 11)
			{
				g_aEnemy[nCntEnemy].nLife = 600;
				g_aEnemy[nCntEnemy].nMoveType = nMoveType;
				g_aEnemy[nCntEnemy].nMoveTime -= nMoveTime;

				//対角線の長さを算出する
				g_aEnemy[nCntEnemy].fLength = sqrtf((256.0f * 256.0f) + (256.0f * 256.0f)) * 0.5f;

				//対角線の角度を算出する
				g_aEnemy[nCntEnemy].fAngle = atan2f(256.0f, 256.0f);

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
			}
			else if (nType == 4 || nType == 5 || nType == 6)
			{
				g_aEnemy[nCntEnemy].nLife = 5;
				g_aEnemy[nCntEnemy].nMoveType = nMoveType;
				g_aEnemy[nCntEnemy].nMoveTime -= nMoveTime;

				//対角線の長さを算出する
				g_aEnemy[nCntEnemy].fLength = sqrtf((64.0f * 64.0f) + (ENEMY_Y * ENEMY_Y)) * 0.5f;

				//対角線の角度を算出する
				g_aEnemy[nCntEnemy].fAngle = atan2f(64.0f, ENEMY_Y);

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
			}
			else if (nType == 8 || nType == 9 || nType == 10)
			{
				g_aEnemy[nCntEnemy].nLife = 10;
				g_aEnemy[nCntEnemy].nMoveType = nMoveType;
				g_aEnemy[nCntEnemy].nMoveTime -= nMoveTime;

				//対角線の長さを算出する
				g_aEnemy[nCntEnemy].fLength = sqrtf((64.0f * 64.0f) + (ENEMY_Y * ENEMY_Y)) * 0.5f;

				//対角線の角度を算出する
				g_aEnemy[nCntEnemy].fAngle = atan2f(64.0f, ENEMY_Y);

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
				pVtx[0].col = D3DXCOLOR(0.7f, 0.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.7f, 0.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.7f, 0.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.7f, 0.0f, 1.0f, 1.0f);

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			else 
			{
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
			}
			
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
	WAVESTATE waveState = GetWaveState();
	Player *pPlayer = GetPlayer();

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		//サウンドの再生
		PlaySound(SOUND_LABEL_SE_EXPLOSION);

		if (g_aEnemy[nCntEnemy].nType == 0 || g_aEnemy[nCntEnemy].nType == 4 || g_aEnemy[nCntEnemy].nType == 8 || g_aEnemy[nCntEnemy].nType == 9 || g_aEnemy[nCntEnemy].nType == 10)
		{
			col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else if (g_aEnemy[nCntEnemy].nType == 1 || g_aEnemy[nCntEnemy].nType == 5)
		{
			col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		}
		else if (g_aEnemy[nCntEnemy].nType == 2 || g_aEnemy[nCntEnemy].nType == 6)
		{
			col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		}
		else if (g_aEnemy[nCntEnemy].nType == 3)
		{
			col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
		}

		if (g_aEnemy[nCntEnemy].nType >= 0 && g_aEnemy[nCntEnemy].nType <= 2)
		{
			SetEffect(g_aEnemy[nCntEnemy].pos, col, 64.0f, 40);
		}
		else if (g_aEnemy[nCntEnemy].nType == 3)
		{
			SetEffect(g_aEnemy[nCntEnemy].pos, col, 64.0f, 40);

			for (int nCnt = 0; nCnt < 7; nCnt++)
			{
				if (g_aEnemy[nCntEnemy].bUse == true)
				{
					pos.x = g_aEnemy[nCntEnemy].pos.x + ((float)(rand() % 200 - 100) / 10.0f);
					pos.y = g_aEnemy[nCntEnemy].pos.y + ((float)(rand() % 200 - 100) / 10.0f);
				}

				SetParticle(pos, D3DXCOLOR(0.3f, 0.0f, 1.0f, 1.0f), 60.0f, 70, 2, 314, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1000);
			}
		}
		else if (g_aEnemy[nCntEnemy].nType == 7)
		{
			for (int nCnt = 0; nCnt < 7; nCnt++)
			{
				if (g_aEnemy[nCntEnemy].bUse == true)
				{
					pos.x = g_aEnemy[nCntEnemy].pos.x + ((float)(rand() % 200 - 100) / 10.0f);
					pos.y = g_aEnemy[nCntEnemy].pos.y + ((float)(rand() % 200 - 100) / 10.0f);
				}

				col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				SetParticle(pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 60.0f, 70, 2, 314, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1000);
			}
		}
		else if (g_aEnemy[nCntEnemy].nType == 11)
		{
			for (int nCnt = 0; nCnt < 7; nCnt++)
			{
				if (g_aEnemy[nCntEnemy].bUse == true)
				{
					pos.x = g_aEnemy[nCntEnemy].pos.x + ((float)(rand() % 200 - 100) / 10.0f);
					pos.y = g_aEnemy[nCntEnemy].pos.y + ((float)(rand() % 200 - 100) / 10.0f);
				}

				col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

				SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 60.0f, 70, 2, 314, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1000);
			}
		}
		else if (g_aEnemy[nCntEnemy].nType == 12)
		{
			for (int nCnt = 0; nCnt < 7; nCnt++)
			{
				if (g_aEnemy[nCntEnemy].bUse == true)
				{
					pos.x = g_aEnemy[nCntEnemy].pos.x + ((float)(rand() % 200 - 100) / 10.0f);
					pos.y = g_aEnemy[nCntEnemy].pos.y + ((float)(rand() % 200 - 100) / 10.0f);
				}

				col.r = (float)(rand() % 200 - 100) / 100.0f;
				col.g = (float)(rand() % 200 - 100) / 100.0f;
				col.b = (float)(rand() % 200 - 100) / 100.0f;

				SetParticle(g_aEnemy[nCntEnemy].pos, col, 60.0f, 70, 2, 314, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1000);
			}
		}
		else
		{
			SetExplosion(g_aEnemy[nCntEnemy].pos, col, g_aEnemy[nCntEnemy].nType + 1);
		}

		AddScore(1400 * (((g_aEnemy[nCntEnemy].nType + 4) % 4) + 1)* 14 * (pPlayer->nRemaining + 1));

		g_nNumEnemy--;

		if (g_nNumEnemy == 0)
		{
			AddScore(100000);
			AddLife(-1);
			AddPlayer(1, 0);
		}

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
		if (g_aEnemy[nCntEnemy].nType == 3)
		{
			if (fLength1 <= 15.0f + 32.0f)
			{
				//敵のヒット処理
				HitPlayer(1);
				AddLife(1);

				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_HIT);
			}
		}
		else if (g_aEnemy[nCntEnemy].nType == 7)
		{
			if (fLength1 <= 15.0f + 32.0f)
			{
				//敵のヒット処理
				HitPlayer(1);
				AddLife(1);

				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_HIT);
			}
		}
		else if (g_aEnemy[nCntEnemy].nType == 11)
		{
			if (fLength1 <= 15.0f + 32.0f)
			{
				//敵のヒット処理
				HitPlayer(1);
				AddLife(1);

				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_HIT);
			}
		}
		else if (g_aEnemy[nCntEnemy].nType == 12)
		{
			if (fLength1 <= 15.0f + 32.0f)
			{
				//敵のヒット処理
				HitPlayer(1);
				AddLife(1);

				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_HIT);
			}
		}
		else if (g_aEnemy[nCntEnemy].nType >= 0 && g_aEnemy[nCntEnemy].nType <= 2)
		{
			if (fLength1 <= 15.0f + 8.0f || fLength2 <= 15.0f + 8.0f)
			{
				//敵のヒット処理
				HitPlayer(1);
				AddLife(1);

				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_HIT);
			}
		}
		else if (g_aEnemy[nCntEnemy].nType >= 4 && g_aEnemy[nCntEnemy].nType <= 6)
		{
			if (fLength1 <= 15.0f + 16.0f)
			{
				//敵のヒット処理
				HitPlayer(1);
				AddLife(1);

				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_HIT);
			}
		}
		else if (g_aEnemy[nCntEnemy].nType >= 8 && g_aEnemy[nCntEnemy].nType <= 10)
		{
			if (fLength1 <= 15.0f + 16.0f)
			{
				//敵のヒット処理
				HitPlayer(1);
				AddLife(1);

				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_HIT);
			}
		}
	}
}