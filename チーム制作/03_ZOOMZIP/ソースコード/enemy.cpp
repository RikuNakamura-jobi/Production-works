//==========================================================
//
//障害物処理 [enemy.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "enemy.h"
#include <stdio.h>
#include "input.h"
#include "debugproc.h"
#include "collision.h"
#include "score.h"
#include "effect.h"
#include "polygon.h"
#include "player.h"
#include "particle.h"

//==========================================================
//マクロ定義
//==========================================================
#define MAX_ENEMY	(256)		//エネミーの最大数
#define ENEMY_DELUP	(SCREEN_HEIGHT * -0.3f)		//エネミーの削除上Y座標
#define ENEMY_DELDOWN	(SCREEN_HEIGHT * 1.3f)	//エネミーの削除下Y座標
#define ENEMY_ROTATESPD	(0.01f)
#define ENEMY_METSIZE	(60.0f)					//隕石のサイズ
#define FST_LVUP	(5)							//1レベルアップ規定値
#define SEC_LVUP	(10)						//2レベルアップ規定値
#define THD_LVUP	(15)						//3レベルアップ規定値
#define FOR_LVUP	(20)						//4レベルアップ規定値
#define FST_SETTIMER	(60 * 6)				//1レベルアップ規定値
#define SEC_SETTIMER	(60 * 5)				//2レベルアップ規定値
#define THD_SETTIMER	(60 * 4)				//3レベルアップ規定値
#define FOR_SETTIMER	(60 * 2)				//4レベルアップ規定値
#define LAST_TIMER		(60)					//ラストレベル
#define BIED_WIDTH		(50.0f)					//鳥幅
#define BIED_HEIGHT		(30.0f)					//鳥高さ
#define BIED_MOVESPD	(1.7f)					//鳥移動量
#define JET_MOVESPD		(3.0f)					//飛行機移動量
#define JET_WIDTH		(90.0f)					//飛行機幅
#define JET_HEIGHT		(70.0f)					//飛行機高さ
#define CLOUD_MOVESPD	(1.4f)					//雷雲移動量
#define CLOUD_WIDTH		(70.0f)					//雷雲幅
#define CLOUD_HEIGHT	(70.0f)					//雷雲高さ
#define METEOR_MOVESPD	(1.4f)					//隕石移動量
#define HIT_AREA	(0.9f)						//当たり判定の倍率
#define NEAR_AREA	(1.6f)						//ニアミスの倍率
#define NEAR_COUNT	(300)						//ニアミスのカウント

//==========================================================
//プロトタイプ宣言
//==========================================================
void SetpVtxEnemy(Enemy *pEnemy, int nCntEnemy);
void EnemyGenerator(void);
void SetBird(void);
void SetJet(void);
void SetCloud(void);
void SetMeteor(void);

void InitNearMiss(void);
void UninitNearMiss(void);
void UpdateNearMiss(void);
void DrawNearMiss(void);

//==========================================================
//グローバル変数
//==========================================================
Enemy g_aEnemy[MAX_ENEMY] = {};	//エネミーの配列
LPDIRECT3DTEXTURE9 g_apTextureEnemy[ENEMYTYPE_MAX] = {};	//テクスチャ1へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;			//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureNearMiss = NULL;		//テクスチャ1へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffNearMiss = NULL;			//頂点バッファへのポインタ
NearMiss g_aNearmiss[MAX_ENEMY];
int g_nEnemySetCnter;			//エネミーの設置カウンター

//==========================================================
//テクスチャファイル名
//==========================================================
const char* c_apFilenameEnemy[ENEMYTYPE_MAX] =
{

	"data\\TEXTURE\\enemy000.png",		//鳥
	"data\\TEXTURE\\enemy001.png",		//飛行機
	"data\\TEXTURE\\enemy002.png",		//雷雲
	"data\\TEXTURE\\enemy003.png",		//隕石
};

//==========================================================
//敵の初期化処理
//==========================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	ZeroMemory(g_aEnemy, sizeof(g_aEnemy));	//メモリ初期化

	//テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < ENEMYTYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameEnemy[nCntTex],
			&g_apTextureEnemy[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	InitNearMiss();

	//AddScore(40);
}

//==========================================================
//敵の終了処理
//==========================================================
void UninitEnemy(void)
{
	//テクスチャの終了処理
	for (int nCntTex = 0; nCntTex < ENEMYTYPE_MAX; nCntTex++)
	{
		if (g_apTextureEnemy[nCntTex] != NULL)
		{
			g_apTextureEnemy[nCntTex]->Release();
			g_apTextureEnemy[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

	UninitNearMiss();
}

//==========================================================
//敵の更新処理
//==========================================================
void UpdateEnemy(void)
{
	Enemy *pEnemy = GetEnemy();	//エネミーのポインタ
	int a = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//使用されている場合
			a++;
			pEnemy->pos += pEnemy->move;	//移動量増加

			g_aEnemy[nCntEnemy].nNear--;

			if (g_aEnemy[nCntEnemy].nNear < 0)
			{
				g_aEnemy[nCntEnemy].nNear = 0;
			}

			if (pEnemy->nType != ENEMYTYPE_METEOR)
			{
				if (pEnemy->nType != ENEMYTYPE_CLOUD)
				{
					SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(0.6f, 1.0f, 0.6f, 1.0f), 1, 5, g_aEnemy[nCntEnemy].fWidth);
				}
				else
				{
					SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 1, 5, g_aEnemy[nCntEnemy].fWidth);
				}
			}
			else
			{
				SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(0.7f, 0.4f, 0.4f, 1.0f), 2, 5, g_aEnemy[nCntEnemy].fWidth);
			}

			if (pEnemy->pos.y > ENEMY_DELDOWN ||
				
				pEnemy->pos.y < ENEMY_DELUP ||
				pEnemy->pos.x > SCREEN_WIDTH * 1.1f ||
				pEnemy->pos.x < -SCREEN_WIDTH * 0.1f)
			{//削除座標にいる場合
				pEnemy->bUse = false;	//使用されていない状態にする
			}
			else
			{
				switch (pEnemy->nType)
				{
				case ENEMYTYPE_METEOR:

					//隕石回転
					if (pEnemy->nRot == 0)
					{
						pEnemy->rot.z += ENEMY_ROTATESPD;

						if (pEnemy->rot.z >= D3DX_PI)
						{
							pEnemy->rot.z += -D3DX_PI * 2;
						}
					}
					else
					{
						pEnemy->rot.z -= ENEMY_ROTATESPD;

						if (pEnemy->rot.z <= -D3DX_PI)
						{
							pEnemy->rot.z += D3DX_PI * 2;
						}
					}
					break;
				}

				//頂点情報
				SetpVtxEnemy(pEnemy, nCntEnemy);
			}
		}
	}

	//エネミー出現設定
	EnemyGenerator();

	PrintDebugProc("敵の数%d\n", a);

	UpdateNearMiss();
}

//==========================================================
//敵の描画処理
//==========================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	Enemy *pEnemy = GetEnemy();		//エネミーのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//使用されている場合

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[pEnemy->nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
				nCntEnemy * 4,									//プリミティブ（ポリゴンの数）
				2);												//描画するプリミティブ数
		}
	}

	DrawNearMiss();
}

//==========================================================
//敵の情報を取得
//==========================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//==========================================================
//敵の設定
//==========================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, float fWidth, float fHeight, int nType)
{
	Enemy *pEnemy = GetEnemy();		//エネミーのポインタ

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{//使用されている場合
			pEnemy->pos = pos;
			pEnemy->rot = rot;
			pEnemy->move = move;
			pEnemy->nType = nType;
			pEnemy->nTexCounter = 0;
			pEnemy->nTexAnim = 0;
			pEnemy->fWidth = fWidth;
			pEnemy->fHeight = fHeight;
			pEnemy->nRot = rand() % 2;

			//対角線の長さを算出する
			pEnemy->fLength = sqrtf(pEnemy->fWidth * pEnemy->fWidth + pEnemy->fHeight * pEnemy->fHeight) * 0.5f;

			//対角線の角度を算出する
			pEnemy->fAngle = atan2f(pEnemy->fWidth, pEnemy->fHeight);

			//頂点情報設定
			SetpVtxEnemy(pEnemy, nCntEnemy);

			pEnemy->bUse = true;
			break;
		}
	}
}

//==========================================================
//頂点情報設定
//==========================================================
void SetpVtxEnemy(Enemy *pEnemy, int nCntEnemy)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	float fTexU = 1.0f;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntEnemy * 4;

	//頂点座標の設定
	pVtx[0].pos.x = pEnemy->pos.x + sinf(pEnemy->rot.z + (-D3DX_PI + pEnemy->fAngle)) * pEnemy->fLength;
	pVtx[0].pos.y = pEnemy->pos.y + cosf(pEnemy->rot.z + (-D3DX_PI + pEnemy->fAngle)) * pEnemy->fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = pEnemy->pos.x + sinf(pEnemy->rot.z + (D3DX_PI - pEnemy->fAngle)) * pEnemy->fLength;
	pVtx[1].pos.y = pEnemy->pos.y + cosf(pEnemy->rot.z + (D3DX_PI - pEnemy->fAngle)) * pEnemy->fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = pEnemy->pos.x + sinf(pEnemy->rot.z + -pEnemy->fAngle) * pEnemy->fLength;
	pVtx[2].pos.y = pEnemy->pos.y + cosf(pEnemy->rot.z + -pEnemy->fAngle) * pEnemy->fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = pEnemy->pos.x + sinf(pEnemy->rot.z + pEnemy->fAngle) * pEnemy->fLength;
	pVtx[3].pos.y = pEnemy->pos.y + cosf(pEnemy->rot.z + pEnemy->fAngle) * pEnemy->fLength;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	switch (pEnemy->nType)
	{
	case ENEMYTYPE_BIED:
		if (pEnemy->move.x > 0.0f)
		{
			pEnemy->nTexAnim = 1;
		}
		else
		{
			pEnemy->nTexAnim = 0;
		}

		fTexU = 0.5f;	//サイズ設定
		break;
	case ENEMYTYPE_JET:
		if (pEnemy->move.x > 0.0f)
		{
			pEnemy->nTexAnim = 1;
		}
		else
		{
			pEnemy->nTexAnim = 0;
		}

		fTexU = 0.5f;	//サイズ設定
		break;
	default:
		fTexU = 1.0f;	//サイズ設定
		break;
	}

	pVtx[0].tex = D3DXVECTOR2(fTexU * pEnemy->nTexAnim, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTexU * pEnemy->nTexAnim + fTexU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fTexU * pEnemy->nTexAnim, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fTexU * pEnemy->nTexAnim + fTexU, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//==========================================================
//エネミーの出現設定
//==========================================================
void EnemyGenerator(void)
{
	int nType = ENEMYTYPE_MAX;	//出現する種類

	g_nEnemySetCnter += rand() % 10;	//ランダムでカウントを増加

	if (GetNowScore() < FST_LVUP)
	{//初期レベル
		if (g_nEnemySetCnter >= FST_SETTIMER)
		{//出現タイマーを超えた場合
			nType = rand() % ENEMYTYPE_JET;	//鳥出現
			g_nEnemySetCnter = 0;	//カウンターリセット
		}
	}
	else if (GetNowScore() >= FST_LVUP && 
		GetNowScore() < SEC_LVUP)
	{//1レベル
		if (g_nEnemySetCnter >= FST_SETTIMER)
		{//出現タイマーを超えた場合
			nType = rand() % ENEMYTYPE_CLOUD;	//飛行機まで出現
			g_nEnemySetCnter = 0;	//カウンターリセット
		}
	}
	else if (GetNowScore() >= SEC_LVUP &&
		GetNowScore() < THD_LVUP)
	{//2レベル
		if (g_nEnemySetCnter >= SEC_SETTIMER)
		{//出現タイマーを超えた場合
			nType = rand() % ENEMYTYPE_METEOR;	//雷雲まで出現
			g_nEnemySetCnter = 0;	//カウンターリセット
		}
	}
	else if (GetNowScore() >= THD_LVUP &&
		GetNowScore() < FOR_LVUP)
	{//3レベル
		if (g_nEnemySetCnter >= THD_SETTIMER)
		{//出現タイマーを超えた場合
			nType = rand() % ENEMYTYPE_MAX;	//隕石まで出現
			g_nEnemySetCnter = 0;	//カウンターリセット
		}
	}
	else if (GetNowScore() >= FOR_LVUP)
	{//4レベル以降
		if (g_nEnemySetCnter >= FOR_SETTIMER)
		{//出現タイマーを超えた場合
			nType = rand() % ENEMYTYPE_MAX;	//隕石まで出現
			g_nEnemySetCnter = 0;	//カウンターリセット
		}
	}
	else
	{
		if (g_nEnemySetCnter >= LAST_TIMER)
		{//出現タイマーを超えた場合
			nType = rand() % ENEMYTYPE_MAX;	//隕石まで出現
			g_nEnemySetCnter = 0;	//カウンターリセット
		}
	}

	//規定値であれば設置する
	if (nType != -1)
	{//種類設定済み
		switch (nType)
		{
		case ENEMYTYPE_BIED:
			SetBird();
			break;
		case ENEMYTYPE_JET:
			SetJet();
			break;
		case ENEMYTYPE_CLOUD:
			SetCloud();
			break;
		case ENEMYTYPE_METEOR:
			SetMeteor();
			break;
		default:
			break;
		}
	}
}

//==========================================================
//鳥の出現設定
//==========================================================
void SetBird(void)
{
	int nRand = 0;	//ランダム用

	Enemy enemy;	//エネミーの情報用
	ZeroMemory(&enemy, sizeof(enemy));	//メモリ初期化

	//左右どちらかを設定する
	nRand = rand() % 2;

	if (nRand == 0)
	{
		enemy.pos.x = SCREEN_WIDTH * 1.1f;
		enemy.move.x = -BIED_MOVESPD;
	}
	else
	{
		enemy.pos.x = SCREEN_WIDTH * -0.1f;
		enemy.move.x = BIED_MOVESPD;
	}

	int nY = (int)((float)SCREEN_HEIGHT + (float)SCREEN_HEIGHT * 0.1f);
	enemy.pos.y = (float)(rand() % nY) + SCREEN_HEIGHT * 0.1f;

	SetEnemy(enemy.pos, enemy.rot, enemy.move, BIED_WIDTH, BIED_HEIGHT, ENEMYTYPE_BIED);
}

//==========================================================
//飛行機の出現設定
//==========================================================
void SetJet(void)
{
	int nRand = 0;	//ランダム用

	Enemy enemy;	//エネミーの情報用
	ZeroMemory(&enemy, sizeof(enemy));	//メモリ初期化

	//左右どちらかを設定する
	nRand = rand() % 2;

	if (nRand == 0)
	{
		enemy.pos.x = SCREEN_WIDTH * 1.1f;
		enemy.move.x = -JET_MOVESPD;
	}
	else
	{
		enemy.pos.x = SCREEN_WIDTH * -0.1f;
		enemy.move.x = JET_MOVESPD;
	}

	int nY = (int)((float)SCREEN_HEIGHT + (float)SCREEN_HEIGHT * 0.1f);
	enemy.pos.y = (float)(rand() % nY) + SCREEN_HEIGHT * 0.1f;

	SetEnemy(enemy.pos, enemy.rot, enemy.move, JET_WIDTH, JET_HEIGHT, ENEMYTYPE_JET);
}

//==========================================================
//雲の出現設定
//==========================================================
void SetCloud(void)
{
	int nRand = 0;	//ランダム用

	Enemy enemy;	//エネミーの情報用
	ZeroMemory(&enemy, sizeof(enemy));	//メモリ初期化

	//左右どちらかを設定する
	nRand = rand() % 2;

	if (nRand == 0)
	{
		enemy.pos.x = SCREEN_WIDTH * 1.1f;
		enemy.move.x = -CLOUD_MOVESPD;
	}
	else
	{
		enemy.pos.x = SCREEN_WIDTH * -0.1f;
		enemy.move.x = CLOUD_MOVESPD;
	}

	int nY = (int)((float)SCREEN_HEIGHT + (float)SCREEN_HEIGHT * 0.1f);
	enemy.pos.y = (float)(rand() % nY) + SCREEN_HEIGHT * 0.1f;

	SetEnemy(enemy.pos, enemy.rot, enemy.move, CLOUD_WIDTH, CLOUD_HEIGHT, ENEMYTYPE_CLOUD);
}

//==========================================================
//隕石の出現設定
//==========================================================
void SetMeteor(void)
{
	Enemy enemy;
	ZeroMemory(&enemy, sizeof(enemy));	//メモリ初期化

	enemy.pos.x = (float)(rand() % SCREEN_WIDTH);
	enemy.pos.y = ENEMY_DELUP;
	enemy.move.y = METEOR_MOVESPD;
	enemy.rot.z = 0.0f;

	SetEnemy(enemy.pos, enemy.rot, enemy.move, ENEMY_METSIZE, ENEMY_METSIZE, ENEMYTYPE_METEOR);
}

//==========================================================
//エネミーの出現設定
//==========================================================
bool CollisionEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 posOld)
{
	bool bEnemy;
	bEnemy = false;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			switch (g_aEnemy[nCntEnemy].nType)
			{
			case ENEMYTYPE_BIED:
				if (CollisionEllipse2D(g_aEnemy[nCntEnemy].pos, pos, g_aEnemy[nCntEnemy].rot, g_aEnemy[nCntEnemy].fWidth * HIT_AREA, g_aEnemy[nCntEnemy].fHeight * HIT_AREA) == true)
				{//衝突したとき
					bEnemy = true;
				}
				else if (CollisionEllipse2D(g_aEnemy[nCntEnemy].pos, posOld, g_aEnemy[nCntEnemy].rot, g_aEnemy[nCntEnemy].fWidth * NEAR_AREA, g_aEnemy[nCntEnemy].fHeight * NEAR_AREA) == true &&
					CollisionEllipse2D(g_aEnemy[nCntEnemy].pos, pos, g_aEnemy[nCntEnemy].rot, g_aEnemy[nCntEnemy].fWidth * NEAR_AREA, g_aEnemy[nCntEnemy].fHeight * NEAR_AREA) == false &&
					g_aEnemy[nCntEnemy].nNear == 0)
				{//ニアミスの時
					AddScore(1);		//スコア加算
					g_aEnemy[nCntEnemy].nNear = NEAR_COUNT;
				}
				break;
			case ENEMYTYPE_JET:
				if (CollisionEllipse2D(g_aEnemy[nCntEnemy].pos, pos, g_aEnemy[nCntEnemy].rot, g_aEnemy[nCntEnemy].fWidth * HIT_AREA * 0.8f, g_aEnemy[nCntEnemy].fHeight * HIT_AREA * 0.9f) == true)
				{//衝突したとき
					bEnemy = true;
				}
				else if (CollisionEllipse2D(g_aEnemy[nCntEnemy].pos, posOld, g_aEnemy[nCntEnemy].rot, g_aEnemy[nCntEnemy].fWidth * NEAR_AREA * 0.8f, g_aEnemy[nCntEnemy].fHeight * NEAR_AREA * 0.9f) == true &&
					CollisionEllipse2D(g_aEnemy[nCntEnemy].pos, pos, g_aEnemy[nCntEnemy].rot, g_aEnemy[nCntEnemy].fWidth * NEAR_AREA * 0.8f, g_aEnemy[nCntEnemy].fHeight * NEAR_AREA * 0.9f) == false &&
					g_aEnemy[nCntEnemy].nNear == 0)
				{//ニアミスの時
					AddScore(1);		//スコア加算
					g_aEnemy[nCntEnemy].nNear = NEAR_COUNT;
				}
				break;
			case ENEMYTYPE_CLOUD:
				if (CollisionCircle2D(g_aEnemy[nCntEnemy].pos, pos, g_aEnemy[nCntEnemy].fHeight * HIT_AREA * 0.9f) == true)
				{//衝突したとき
					bEnemy = true;
				}
				else if (CollisionCircle2D(g_aEnemy[nCntEnemy].pos, posOld, g_aEnemy[nCntEnemy].fHeight* NEAR_AREA * 0.9f) == true &&
					CollisionCircle2D(g_aEnemy[nCntEnemy].pos, pos, g_aEnemy[nCntEnemy].fHeight* NEAR_AREA * 0.9f) == false &&
					g_aEnemy[nCntEnemy].nNear == 0)
				{//ニアミスの時
					AddScore(1);		//スコア加算
					g_aEnemy[nCntEnemy].nNear = NEAR_COUNT;
				}
				break;
			case ENEMYTYPE_METEOR:
				if (CollisionCircle2D(g_aEnemy[nCntEnemy].pos, pos, g_aEnemy[nCntEnemy].fHeight * HIT_AREA) == true)
				{//衝突したとき
					bEnemy = true;
				}
				else if (CollisionCircle2D(g_aEnemy[nCntEnemy].pos, posOld, g_aEnemy[nCntEnemy].fHeight* NEAR_AREA) == true &&
					CollisionCircle2D(g_aEnemy[nCntEnemy].pos, pos, g_aEnemy[nCntEnemy].fHeight* NEAR_AREA) == false &&
					g_aEnemy[nCntEnemy].nNear == 0)
				{//ニアミスの時
					AddScore(1);		//スコア加算
					g_aEnemy[nCntEnemy].nNear = NEAR_COUNT;
				}
				break;
			}
		}
	}

	return bEnemy;
}

//==========================================================
//ニアミスポリゴンの初期化
//==========================================================
void InitNearMiss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	ZeroMemory(g_aNearmiss, sizeof(g_aNearmiss));	//メモリ初期化

													//テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < ENEMYTYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\score000.png",
			&g_pTextureNearMiss);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffNearMiss,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffNearMiss->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntNear = 0; nCntNear < MAX_ENEMY; nCntNear++)
	{
		g_aNearmiss[nCntNear].bUse = false;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x - g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y - g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x + g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y - g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x - g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y + g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x + g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y + g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);

		//rhwの設定
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffNearMiss->Unlock();
}

//==========================================================
//ニアミスポリゴンの終了処理
//==========================================================
void UninitNearMiss(void)
{
	if (g_pTextureNearMiss != NULL)
	{
		g_pTextureNearMiss->Release();
		g_pTextureNearMiss = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffNearMiss != NULL)
	{
		g_pVtxBuffNearMiss->Release();
		g_pVtxBuffNearMiss = NULL;
	}
}

//==========================================================
//ニアミスポリゴンの更新処理
//==========================================================
void UpdateNearMiss(void)
{
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffNearMiss->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntNear = 0; nCntNear < MAX_ENEMY; nCntNear++)
	{
		if (g_aNearmiss[nCntNear].bUse == true)
		{//使用されている場合
			g_aNearmiss[nCntNear].Polygon.pos += g_aNearmiss[nCntNear].move;

			g_aNearmiss[nCntNear].nLife--;

			if (g_aNearmiss[nCntNear].nLife <= 0)
			{
				g_aNearmiss[nCntNear].nLife = 0;
				g_aNearmiss[nCntNear].bUse = false;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x - g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y - g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x + g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y - g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x - g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y + g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x + g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y + g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);

			//rhwの設定
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col =
				pVtx[1].col =
				pVtx[2].col =
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffNearMiss->Unlock();
}

//==========================================================
//ニアミスポリゴンの描画処理
//==========================================================
void DrawNearMiss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffNearMiss, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntNear = 0; nCntNear < MAX_ENEMY; nCntNear++)
	{
		if (g_aNearmiss[nCntNear].bUse == true)
		{//使用されている場合

		 //テクスチャの設定
			pDevice->SetTexture(0, g_pTextureNearMiss);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
				nCntNear * 4,									//プリミティブ（ポリゴンの数）
				2);												//描画するプリミティブ数
		}
	}
}

//==========================================================
//ニアミスポリゴンの設定処理
//==========================================================
void SetNearMiss(void)
{
	Player *player = GetPlayer();
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffNearMiss->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntNear = 0; nCntNear < MAX_ENEMY; nCntNear++)
	{
		if (g_aNearmiss[nCntNear].bUse == false)
		{//使用されている場合
			g_aNearmiss[nCntNear].Polygon.pos.x = player->pos.x + 30.0f;
			g_aNearmiss[nCntNear].Polygon.pos.y = player->pos.y - 30.0f;
			g_aNearmiss[nCntNear].Polygon.fWidth = 30.0f;
			g_aNearmiss[nCntNear].Polygon.fHeight = 20.0f;
			g_aNearmiss[nCntNear].move.y = -0.5f;
			g_aNearmiss[nCntNear].bUse = true;
			g_aNearmiss[nCntNear].nLife = 40;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x - g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y - g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x + g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y - g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x - g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y + g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x + g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y + g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);

			//rhwの設定
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col =
				pVtx[1].col =
				pVtx[2].col =
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffNearMiss->Unlock();
}