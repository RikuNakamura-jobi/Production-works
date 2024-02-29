//========================================
//
//2Dアクションゲーム(岩処理)[rock.cpp]
//Author:佐久間優香
//
//========================================
//========================
// インクルード
//========================
#include "main.h"
#include "rock.h"
#include "edit.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "object.h"
#include "enemy.h"
#include "player.h"
#include "stdio.h"
#include "score.h"
#include "particle.h"
#include "wall.h"
#include "ceiling.h"
#include "shadow.h"
#include "orbit.h"
#include "sound.h"
#include "collision.h"
#include "point.h"
#include "point_log.h"

//========================
// マクロ定義
//========================
#define SPEED				(4.0f)					//速度
#define LIFE				(600)					//寿命
#define RADIUS_PLAYER		(50.0f)					//半径 プレイヤー
#define RADIUS_ENEMY		(150.0f)				//半径　敵
#define DEBUG_CLO_R			(1.0f)					//デバッグの色R
#define DEBUG_CLO_G			(0.0f)					//デバッグの色G
#define DEBUG_CLO_B			(0.5f)					//デバッグの色B
#define DEBUG_CLO_A			(1.0f)					//デバッグの色A
#define DEBUG_TOP			(140)					//デバッグの位置
#define ROCK_SCORE			(500)					//敵に岩を当てた時のスコア	
#define MAX_TEX				(10)					//テクスチャの数
#define ROTATE_SPEED		(0.2f)					//回転速度
#define SHADOW_RADIUS		(60.0f)					//影の半径
#define FRAG_NUM			(11)					//壊れた際の破片の数
#define HIT_DAMAGE			(10)					//敵に与える事のできるダメージ
#define ROLL_SOUND_TIME		(20)					//転がる音を再生する頻度

//========================
// プロトタイプ宣言
//========================
void CollisionRock(Rock *pRock);				//モデルの当たり判定
void CollisionRock3(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax);	//モデルとオブジェクトの当たり判定
bool CollisionRockWall(int nCntRock);
bool CollisionRockRock(int nCntRock);
void StateChangeRock(void);						//状態の変更
void DebugRock(void);							//モデルのでバック表示
void RotateRock(Rock *pRock);	//岩を転がす処理

//========================
// グローバル変数宣言
//========================
Rock g_Rock[MAX_ROCK];											//構造体の情報
LPD3DXBUFFER g_pBuffMatRock[ROCKTYPE_MAX];						//マテリアルへのバッファ
LPD3DXMESH g_pMeshRock[ROCKTYPE_MAX];							//メッシュへのポインタ
DWORD g_dwNumMatRock[ROCKTYPE_MAX];								//マテリアルの数
LPDIRECT3DTEXTURE9 g_apTextureRock[ROCKTYPE_MAX][MAX_TEX];		//テクスチャへのポインタ

int g_nNumRock;	//岩の総数
int g_RockCounter;	//壊れた岩のカウンター
int g_nCntRollSound;	//転がる音のタイマー

//========================================
//岩の初期化処理
//========================================
void InitRock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得

	int nCntRock;

	//グローバル変数初期化
	g_RockCounter = 0;		//岩のカウンター
	g_nNumRock = 0;

	//各種変数の初期化
	for (nCntRock = 0; nCntRock < MAX_ROCK; nCntRock++)
	{
		ZeroMemory(&g_Rock[nCntRock],sizeof(Rock));
		g_Rock[nCntRock].nIdxShadow = -1;
		g_Rock[nCntRock].nIdxOrbit = -1;
	}

	D3DXMATERIAL *pMat;									//マテリアルデータへのポインタ

	for (nCntRock = 0; nCntRock < ROCKTYPE_MAX; nCntRock++)
	{
		//Xファイル読み込み
		D3DXLoadMeshFromX("data\\MODEL\\OBJECT\\magstone_000.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatRock[nCntRock],
			NULL,
			&g_dwNumMatRock[nCntRock],
			&g_pMeshRock[nCntRock]);

		//マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatRock[nCntRock]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatRock[nCntRock]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_apTextureRock[nCntRock][nCntMat]);
			}
		}
	}
}

//========================================
// 岩の終了処理
//========================================
void UninitRock(void)
{
	for (int nCount = 0;nCount < ROCKTYPE_MAX;nCount++)
	{
		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatRock[nCount]; nCntMat++)
		{// テクスチャの破棄
			if (g_apTextureRock[nCount][nCntMat] != NULL)
			{
				g_apTextureRock[nCount][nCntMat]->Release();
				g_apTextureRock[nCount][nCntMat] = NULL;
			}
		}

		// メッシュの破棄
		if (g_pMeshRock[nCount] != NULL)
		{
			g_pMeshRock[nCount]->Release();
			g_pMeshRock[nCount] = NULL;
		}

		// マテリアルの破棄
		if (g_pBuffMatRock[nCount] != NULL)
		{
			g_pBuffMatRock[nCount]->Release();
			g_pBuffMatRock[nCount] = NULL;
		}
	}
}

//========================================
// 岩の更新処理
//========================================
void UpdateRock(void)
{
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス
	Orbit *pOrbit = GetOrbit();

	bool bSound = false; //転がる音の処理にはいったかどうか

	for (int nCntRock = 0; nCntRock < MAX_ROCK; nCntRock++)
	{
		if (g_Rock[nCntRock].nType == ROCKTYPE_INVISIBLE)
		{
			if (g_Rock[nCntRock].bUse == true)
			{
				if (g_Rock[nCntRock].bWallInvisible == true)
				{//壁に当たったら
					if (g_Rock[nCntRock].nCounter >= 1)
					{//岩を消去
						g_Rock[nCntRock].nCounter = 0;
						DisableRock(&g_Rock[nCntRock]);
					}
					else
					{//カウンター加算
						g_Rock[nCntRock].nCounter++;
					}
				}

				if (g_Rock[nCntRock].bUse == true)
				{
					pOrbit[g_Rock[nCntRock].nIdxOrbit].col = D3DXCOLOR(0.0f, 0.3f, 1.0f, 1.0f);

					while (1)
					{
						// 前回の位置を保存
						g_Rock[nCntRock].posOld = g_Rock[nCntRock].pos;

						// 転がす処理
						RotateRock(&g_Rock[nCntRock]);

						WindMove(nCntRock);

						if (g_Rock[nCntRock].pos.y >= CEILING_POSY)
						{//天井裏だったら

							// 位置を更新
							g_Rock[nCntRock].pos += g_Rock[nCntRock].move * g_Rock[nCntRock].fSpeed;
						}
						else
						{
							// 位置を更新
							g_Rock[nCntRock].pos += g_Rock[nCntRock].move * g_Rock[nCntRock].fSpeed;
						}

						// 状態の変化
						StateChangeRock();

						// 反射板との当たり判定
						if (CollisionReflector(&g_Rock[nCntRock].pos, &g_Rock[nCntRock].posOld, &g_Rock[nCntRock].move, g_Rock[nCntRock].vtxMax.x, g_Rock[nCntRock].nType) == true)
						{
							// 寿命のカウントダウン
							g_Rock[nCntRock].nLife--;
							break;
						}

						if (CollisionRockInvisible(&g_Rock[nCntRock].pos, &g_Rock[nCntRock].vtxMax.x) == true)
						{
							if (g_Rock[nCntRock].pos.y >= CEILING_POSY)
							{//天井裏だったら
								pOrbit[g_Rock[nCntRock].nIdxOrbit].col = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f);
							}
							else
							{
								g_Rock[nCntRock].bWallInvisible = true;
								break;
							}
						}

						//壁との当たり判定
						if (CollisionWall(&g_Rock[nCntRock].pos, &g_Rock[nCntRock].posOld, &g_Rock[nCntRock].move, 0.0f) == true)
						{
							g_Rock[nCntRock].bWallInvisible = true;
							break;
						}

						if (g_Rock[nCntRock].nLife <= 0)
						{
							g_Rock[nCntRock].bWallInvisible = true;
							break;
						}

						if (g_Rock[nCntRock].pos.y >= CEILING_POSY)
						{//天井裏だったら

							g_Rock[nCntRock].nLife --;

							//ワールドマトリックスの初期化
							D3DXMatrixIdentity(&g_Rock[nCntRock].mtxWorld);

							//向きを反映
							D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Rock[nCntRock].rot.y, g_Rock[nCntRock].rot.x, g_Rock[nCntRock].rot.z);
							D3DXMatrixMultiply(&g_Rock[nCntRock].mtxWorld, &g_Rock[nCntRock].mtxWorld, &mtxRot);

							//位置を反映
							D3DXMatrixTranslation(&mtxTrans, g_Rock[nCntRock].pos.x, g_Rock[nCntRock].pos.y, g_Rock[nCntRock].pos.z);
							D3DXMatrixMultiply(&g_Rock[nCntRock].mtxWorld, &g_Rock[nCntRock].mtxWorld, &mtxTrans);

							//軌跡の位置設定
							SetPositionOffset(g_Rock[nCntRock].mtxWorld, g_Rock[nCntRock].nIdxOrbit);
						}
					}
					
					if (g_Rock[nCntRock].pos.y < CEILING_POSY)
					{//地上だったら

						//軌跡の位置設定
						SetPositionOffset(g_Rock[nCntRock].mtxWorld, g_Rock[nCntRock].nIdxOrbit);
					}
				}
			}
		}
		else
		{
			if (g_Rock[nCntRock].bUse == true)
			{
				// 前回の位置を保存
				g_Rock[nCntRock].posOld = g_Rock[nCntRock].pos;

				if (bSound == false)
				{//転がる音を鳴らす処理
					RockRollSound(&g_Rock[nCntRock],&bSound);
				}

				// 転がす処理
				RotateRock(&g_Rock[nCntRock]);

				if (g_Rock[nCntRock].nCounter >= ROCK_TIME)
				{// カウンターの制限
					g_Rock[nCntRock].nCounter = ROCK_TIME;
				}
				else
				{
					// カウンターを加算
					g_Rock[nCntRock].nCounter++;
				}

				// 風の移動量加算処理
				WindMove(nCntRock);

				// 位置を更新
				g_Rock[nCntRock].pos += g_Rock[nCntRock].move * g_Rock[nCntRock].fSpeed;

				if (g_Rock[nCntRock].nType == ROCKTYPE_THROW || g_Rock[nCntRock].nType == ROCKTYPE_DROP)
				{
					g_Rock[nCntRock].move.y -= 0.03f;

					if (g_Rock[nCntRock].pos.y < 50.0f)
					{
						g_Rock[nCntRock].move.y = g_Rock[nCntRock].move.y * -0.7f;
						g_Rock[nCntRock].pos.y = 50.0f;
					}
				}

				// 状態の変化
				StateChangeRock();

				// 寿命のカウントダウン
				g_Rock[nCntRock].nLife--;

				// 反射板との当たり判定
				CollisionReflector(&g_Rock[nCntRock].pos, &g_Rock[nCntRock].posOld, &g_Rock[nCntRock].move, g_Rock[nCntRock].vtxMax.x, g_Rock[nCntRock].nType);

				//壁との当たり判定
				if (CollisionWall(&g_Rock[nCntRock].pos, &g_Rock[nCntRock].posOld, &g_Rock[nCntRock].move, g_Rock[nCntRock].vtxMax.x) == true)
				{
					DisableRock(&g_Rock[nCntRock]);
				}

				if (CollisionRockRock(nCntRock) == true)
				{
					DisableRock(&g_Rock[nCntRock]);
				}

				//プレイヤーとの当たり判定
				CollisionRockPlayer(nCntRock);

				if (g_Rock[nCntRock].pos.y >= CEILING_POSY)
				{//天井裏だったら
					SetParticle(
						D3DXVECTOR3(g_Rock[nCntRock].pos.x,
							g_Rock[nCntRock].pos.y - RADIUS_PLAYER,
							g_Rock[nCntRock].pos.z),
						PARTICLETYPE_SANDDUST
					);
				}

				if (g_Rock[nCntRock].pos.y < CEILING_POSY)
				{//地上だったら
					// 影の位置を更新する
					SetPositionShadow(g_Rock[nCntRock].nIdxShadow,
						D3DXVECTOR3(g_Rock[nCntRock].pos.x, 0.1f, g_Rock[nCntRock].pos.z),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						1.0f);
				}

				// 寿命が尽きた
				if (g_Rock[nCntRock].nLife <= 0)
				{//岩を消す処理
					DisableRock(&g_Rock[nCntRock]);
				}
			}
		}
	}
}

//========================================
// 岩が転がる音の管理
//========================================
void RockRollSound(Rock *pRock,bool *bSound)
{
	//情報取得
	Player *pPlayer = GetPlayer();
	bool bNear;

	// 音が聞こえる位置にいるかの当たり判定
	bNear = SphereCollision(pRock->pos,pPlayer->pos,ROLLSOUND_RANGE,0.0f);

	if (bNear)
	{//音が聞こえる範囲なら
		if (g_nCntRollSound == 0)
		{
			//差分取得
			D3DXVECTOR3 posDiff = pRock->pos - pPlayer->pos;
			float fDiffLength;

			fDiffLength = D3DXVec3Length(&posDiff);

			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_ROLL, (1.0f - fDiffLength / ROLLSOUND_RANGE) * 1.5f);

			//転がる音再生
			PlaySound(SOUND_LABEL_SE_ROLL);

			//カウンターリセット
			g_nCntRollSound = ROLL_SOUND_TIME;
		}

		//カウンター減算
		g_nCntRollSound--;

		// 音が聞こえる範囲に入ったのを伝える
		*bSound = true;
	}
}

//========================================
// 岩を転がす処理
//========================================
void RotateRock(Rock *pRock)
{
	// 岩を移動方向に向ける処理
	pRock->rot.y = atan2f(pRock->move.x, pRock->move.z);

	if (pRock->nType != ROCKTYPE_INVISIBLE)
	{
		// 岩がX軸で転がる処理
		pRock->rot.x += ROTATE_SPEED;
	}
}

//========================================
// 岩を消す処理
//========================================
void DisableRock(Rock *pRock)
{
	//情報取得
	Player *pPlayer = GetPlayer();

	//差分取得
	D3DXVECTOR3 posDiff = pRock->pos - pPlayer->pos;
	float fDiffLength;

	fDiffLength = D3DXVec3Length(&posDiff);

	//使用していない状態にする
	pRock->bUse = false;

	if (SphereCollision(pRock->pos, pPlayer->pos, ROLLSOUND_RANGE, 0) == true && pRock->nType != ROCKTYPE_INVISIBLE)
	{//プレイヤーが音の聞こえる範囲内だったら

		//差分の量によって音量を変える
		SetVolume(SOUND_LABEL_SE_CRASH, 1.0f - fDiffLength / ROLLSOUND_RANGE);

		PlaySound(SOUND_LABEL_SE_CRASH);
	}

	if (pRock->nType == ROCKTYPE_INVISIBLE)
	{
		//軌跡の削除
		EnableOrbit(pRock->nIdxOrbit);
	}
	else
	{
		if (pRock->pos.y < CEILING_POSY)
		{
			//影の削除
			EnableShadow(pRock->nIdxShadow, false);
			//パーティクルの設定0
			SetBreakObj(pRock->pos, FRAG_NUM);
		}

		//岩総数減算
		g_nNumRock--;
	}
}

//========================================
// 岩の描画処理
//========================================
void DrawRock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス
	D3DMATERIAL9 matDef;								//現在のマテリアル保存用
	D3DXMATERIAL *pMat;									//マテリアルデータへのポインタ

	for (int nCntRock = 0; nCntRock < MAX_ROCK; nCntRock++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Rock[nCntRock].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Rock[nCntRock].rot.y, g_Rock[nCntRock].rot.x, g_Rock[nCntRock].rot.z);

		D3DXMatrixMultiply(&g_Rock[nCntRock].mtxWorld, &g_Rock[nCntRock].mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Rock[nCntRock].pos.x, g_Rock[nCntRock].pos.y, g_Rock[nCntRock].pos.z);

		D3DXMatrixMultiply(&g_Rock[nCntRock].mtxWorld, &g_Rock[nCntRock].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Rock[nCntRock].mtxWorld);


		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatRock[g_Rock[nCntRock].nType]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatRock[g_Rock[nCntRock].nType]; nCntMat++)
		{
			//マテリアルの設定
			switch (g_Rock[nCntRock].state)
			{

			case ROCKSTATE_NORMAL:
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				break;

			case ROCKSTATE_DAMEGE:
				pDevice->SetMaterial(&g_Rock[nCntRock].RedMat.MatD3D);
				break;
			}
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureRock[g_Rock[nCntRock].nType][nCntMat]);

			if (g_Rock[nCntRock].bUse && g_Rock[nCntRock].nType != ROCKTYPE_INVISIBLE)
			{
				//モデル(パーツ)の描画
				g_pMeshRock[g_Rock[nCntRock].nType]->DrawSubset(nCntMat);
			}
		}
		//保存していたマテリアルを戻す 
		pDevice->SetMaterial(&matDef);
	}

#ifdef _DEBUG
	//岩のデバッグ表示
	//DebugRock();
#endif
}

//========================================
//岩の設定処理
//========================================
void SetRock(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot,int nLife, int nType)
{
	int nCntRock;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス

	for (nCntRock = 0; nCntRock < MAX_ROCK; nCntRock++)
	{
		if (g_Rock[nCntRock].bUse == false)
		{//使用されてないなら
			//情報設定
			g_Rock[nCntRock].pos = pos;							//位置
			g_Rock[nCntRock].move = move;						//移動量
			g_Rock[nCntRock].rot = rot;							//向き
			g_Rock[nCntRock].nLife = nLife;						//寿命
			g_Rock[nCntRock].nType = nType;						//種類
			g_Rock[nCntRock].bUse = true;						//使用している状態にする
			g_Rock[nCntRock].bWallInvisible = false;
			g_Rock[nCntRock].nCounter = 0;
			g_Rock[nCntRock].fSpeed = SPEED;
			
			//モデルの当たり判定
			CollisionRock(&g_Rock[nCntRock]);

			//影の設定
			if (pos.y < CEILING_POSY && g_Rock[nCntRock].nType != ROCKTYPE_INVISIBLE)
			{
				g_Rock[nCntRock].nIdxShadow = SetShadow(SHADOW_RADIUS, SHADOWTYPE_CIRCLE, 0.0f, 0.0f,g_Rock[nCntRock].rot);
				SetPositionShadow(g_Rock[nCntRock].nIdxShadow, D3DXVECTOR3(g_Rock[nCntRock].pos.x,0.1f, g_Rock[nCntRock].pos.z), g_Rock[nCntRock].rot,1.0f);
			}

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Rock[nCntRock].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Rock[nCntRock].rot.y, g_Rock[nCntRock].rot.x, g_Rock[nCntRock].rot.z);

			D3DXMatrixMultiply(&g_Rock[nCntRock].mtxWorld, &g_Rock[nCntRock].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Rock[nCntRock].pos.x, g_Rock[nCntRock].pos.y, g_Rock[nCntRock].pos.z);

			D3DXMatrixMultiply(&g_Rock[nCntRock].mtxWorld, &g_Rock[nCntRock].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Rock[nCntRock].mtxWorld);

			if (g_Rock[nCntRock].nType == ROCKTYPE_INVISIBLE)
			{
				if (g_Rock[nCntRock].pos.y >= CEILING_POSY/* && (rand() % 2) == 0*/)
				{//天井裏だったら

					 // 軌跡の設定
					g_Rock[nCntRock].nIdxOrbit = SetOrbit(g_Rock[nCntRock].mtxWorld, D3DXVECTOR3(-10.0f, -100.0f, 0.0f), D3DXVECTOR3(10.0f, -100.0f, 0.0f), D3DXCOLOR(0.0f, 0.3f, 1.0f, 1.0f), 180);
				}

				if (g_Rock[nCntRock].pos.y < CEILING_POSY)
				{//地上だったら

					// 軌跡の設定
					g_Rock[nCntRock].nIdxOrbit = SetOrbit(g_Rock[nCntRock].mtxWorld, D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXCOLOR(0.0f, 0.3f, 1.0f, 1.0f), 5);
				}
			}
			else
			{//不可視の岩は加算しない
				//総数加算
				g_nNumRock++;
			}
			
			break;
		}
	}
}

//========================================
//状態の変更
//========================================
void StateChangeRock(void)
{
	for (int nCntModel = 0; nCntModel < MAX_ROCK; nCntModel++)
	{
		if (g_Rock[nCntModel].bUse == true)
		{
			switch (g_Rock[nCntModel].state)
			{
			case ROCKSTATE_NORMAL:
				break;

			case ROCKSTATE_DAMEGE:
				g_Rock[nCntModel].nCounterState--;

				if (g_Rock[nCntModel].nCounterState <= 0)
				{
					g_Rock[nCntModel].state = ROCKSTATE_NORMAL;
				}
				break;
			}
		}
	}
}

//========================================
//岩の最大・最小値取得
//========================================
void CollisionRock(Rock *pRock)
{
	int nNumVtx;				//頂点数
	DWORD dwSizeFVF;			//頂点フォーマットのサイズ
	BYTE *pVtxBuff;				//頂点バッファへのポインタ

	//頂点数を取得
	nNumVtx = g_pMeshRock[pRock->nType]->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshRock[pRock->nType]->GetFVF());

	//頂点バッファのロック
	g_pMeshRock[pRock->nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntModel = 0; nCntModel < nNumVtx; nCntModel++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;			//頂点座標の代入

		if (vtx.x < pRock->vtxMin.x)
		{
			pRock->vtxMin.x = vtx.x;
		}
		if (vtx.x > pRock->vtxMax.x)
		{
			pRock->vtxMax.x = vtx.x;
		}

		if (vtx.y < pRock->vtxMin.y)
		{
			pRock->vtxMin.y = vtx.y;
		}
		if (vtx.y > pRock->vtxMax.y)
		{
			pRock->vtxMax.y = vtx.y;
		}

		if (vtx.z < pRock->vtxMin.z)
		{
			pRock->vtxMin.z = vtx.z;
		}
		if (vtx.z > pRock->vtxMax.z)
		{
			pRock->vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;			//頂点フォーマットのサイズ分ポインタを進める
	}
	pRock->vtxMax;
	pRock->vtxMin;

	//頂点バッファをアンロック
	g_pMeshRock[pRock->nType]->UnlockVertexBuffer();
}

//========================================
//岩とプレイヤーの当たり判定
//========================================
void CollisionRockPlayer(int nCntRock)
{
	//情報取得
	Player *pPlayer = GetPlayer();
	bool bHit = false;

	if (pPlayer->motionState != PLAYERMOTION_DAMAGE)
	{//プレイヤーがダメージモーションでないとき
		bHit = SphereCollision(g_Rock[nCntRock].pos, pPlayer->pos, PLAYER_RADIUS, g_Rock[nCntRock].vtxMax.x);

		if (bHit)
		{//命中したかどうか

			//プレイヤーの向きを岩に向ける
			pPlayer->rot.y = atan2f(pPlayer->pos.x - g_Rock[nCntRock].pos.x, pPlayer->pos.z - g_Rock[nCntRock].pos.z);

			if (pPlayer->motionState != PLAYERMOTION_DAMAGE)
			{//プレイヤーを岩に向ける
				pPlayer->fAngle = pPlayer->rot.y;
			}

			//プレイヤーにダメージを与える
			HitPlayer(2);

			//岩破壊
			DisableRock(&g_Rock[nCntRock]);

			//岩破壊数カウント
			g_RockCounter++;
		}
	}
}

//========================================
//岩とエネミーの当たり判定
//========================================
void CollisionRockEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pPoint, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, bool bJump)
{
	//変数宣言
	int nCntRock;
	float fLengthS;		//2点間の長さ(平面)
	float fLengthH;		//2点間の長さ(平面)
	float fLengthEnemy;

	for (nCntRock = 0; nCntRock < MAX_ROCK; nCntRock++)
	{
		if (g_Rock[nCntRock].bUse == true && g_Rock[nCntRock].nType != ROCKTYPE_THROW && g_Rock[nCntRock].nType != ROCKTYPE_INVISIBLE)
		{
			//円の当たり判定(横)
			fLengthS = sqrtf((pPos->x - g_Rock[nCntRock].pos.x) * (pPos->x - g_Rock[nCntRock].pos.x)
				+ (pPos->z - g_Rock[nCntRock].pos.z) * (pPos->z - g_Rock[nCntRock].pos.z));

			//円の当たり判定(縦)
			fLengthH = sqrtf((pPos->y - g_Rock[nCntRock].pos.y) * (pPos->y - g_Rock[nCntRock].pos.y)
				+ fLengthS * fLengthS);

			//敵の半径を決定
			fLengthEnemy = (RADIUS_ENEMY) * (RADIUS_ENEMY);

			//落下岩用判定拡大
			float posCol;
			if (g_Rock[nCntRock].nType == ROCKTYPE_DROP)
			{
				posCol = 350.0f;
			}
			else
			{
				posCol = 0.0f;
			}

			if (fLengthH * fLengthH < fLengthEnemy && pPos->y < g_Rock[nCntRock].pos.y + g_Rock[nCntRock].vtxMax.y && pPos->y > g_Rock[nCntRock].pos.y + g_Rock[nCntRock].vtxMin.y - posCol)
			{
				//岩を消す処理
				DisableRock(&g_Rock[nCntRock]);

				*pPoint = g_Rock[nCntRock].pos;

				//壊れた岩のカウント
				g_RockCounter++;

				if (g_Rock[nCntRock].nType == ROCKTYPE_DROP)
				{
					//エネミーのヒット処理
					HitEnemy(HIT_DAMAGE * 20, nCntRock);

					//スコアの加算
					AddScore(ROCK_SCORE * 20);

					//取得スコア表示設定
					SetPointScore(ROCK_SCORE * 20, D3DXVECTOR3(1000.0f, 400.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), POINTTYPE_SLIDE, 0.5f, LOGTYPE_HITROCK);
				}
				else
				{
					//エネミーのヒット処理
					HitEnemy(HIT_DAMAGE, nCntRock);

					//スコアの加算
					AddScore(ROCK_SCORE);

					//取得スコア表示設定
					SetPointScore(ROCK_SCORE, D3DXVECTOR3(1000.0f, 400.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), POINTTYPE_SLIDE, 0.5f, LOGTYPE_HITROCK);
				}
			}
		}
	}
}

//========================================
//岩とオブジェクトの当たり判定(未完成)
//========================================
void CollisionRock3(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax)
{
	Object *pObject = ObjectGet();				//オブジェクトの取得
	Player *pPlayer = GetPlayer();
	int nCntRock;								//カウンター

	for (nCntRock = 0; nCntRock < MAX_OBJECT; nCntRock++, pObject++)
	{
		if (pObject->bUse == true && pObject->nType != OBJECTTYPE_REFLECTOR)
		{//ブロックを使っている
			if (pPosOld->x + vtxMax.x <= pObject->pos.x + pObject->vtxMin.x
				&& pPos->x + vtxMax.x > pObject->pos.x + pObject->vtxMin.x
				&& pPos->z + vtxMax.z > pObject->pos.z + pObject->vtxMin.z
				&& pPos->z + vtxMin.z < pObject->pos.z + pObject->vtxMax.z)
			{//右の判定
				pPos->x = pObject->pos.x + pObject->vtxMin.x - vtxMax.x;
			}
			if (pPosOld->x - vtxMax.z >= pObject->pos.x + pObject->vtxMax.x
				&& pPos->x - vtxMax.z < pObject->pos.x + pObject->vtxMax.x
				&& pPos->z - vtxMax.z < pObject->pos.z + pObject->vtxMax.z
				&& pPos->z + vtxMax.z > pObject->pos.z + pObject->vtxMin.z)
			{//左の判定
				pPos->x = pObject->pos.x + pObject->vtxMax.x + vtxMax.z + 0.05f;
			}
			if (pPosOld->z + vtxMax.z <= pObject->pos.z + pObject->vtxMin.z
				&& pPos->z + vtxMax.z > pObject->pos.z + pObject->vtxMin.z
				&& pPos->x + vtxMax.z > pObject->pos.x + pObject->vtxMin.x
				&& pPos->x - vtxMax.z < pObject->pos.x + pObject->vtxMax.x)
			{//前の判定
				pPos->z = pObject->pos.z + pObject->vtxMin.z - vtxMax.z;
			}
			if (pPosOld->z - vtxMax.z >= pObject->pos.z + pObject->vtxMax.z
				&& pPos->z - vtxMax.z < pObject->pos.z + pObject->vtxMax.z
				&& pPos->x - vtxMax.z < pObject->pos.x + pObject->vtxMax.x
				&& pPos->x + vtxMax.z > pObject->pos.x + pObject->vtxMin.x)
			{//後ろの判定
				pPos->z = pObject->pos.z + pObject->vtxMax.z + vtxMax.z + 0.05f;
			}
		}
	}
}

//========================================
//岩と壁の当たり判定
//========================================
bool CollisionRockWall(int nCntRock)
{
	bool bWall;

	bWall = false;

	//壁の情報をもらう
	Wall *pWall = GetWall();

	D3DXVECTOR3 Wallpos0;		//左のpos
	D3DXVECTOR3 Wallpos1;		//右のpos
	D3DXVECTOR3 vecToPos;		//pos0とpos1の差分（位置関係）
	D3DXVECTOR3 vecLine;		//壁の長さ
	D3DXVECTOR3 vecMove;		//弾のベクトル
	float fRate;		//交点の割合

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	{
		if (pWall->bUse == true)
		{
			switch (nCntWall)
			{
			case 0:
				Wallpos0 = D3DXVECTOR3(-1500.0f, 0.0f, 1500.0f);
				Wallpos1 = D3DXVECTOR3(1500.0f, 0.0f, 1500.0f);
				break;

			case 1:
				Wallpos0 = D3DXVECTOR3(1500.0f, 0.0f, -1500.0f);
				Wallpos1 = D3DXVECTOR3(-1500.0f, 0.0f, -1500.0f);
				break;

			case 2:
				Wallpos0 = D3DXVECTOR3(-1500.0f, 0.0f, -1500.0f);
				Wallpos1 = D3DXVECTOR3(-1500.0f, 0.0f, 1500.0f);
				break;

			case 3:
				Wallpos0 = D3DXVECTOR3(1500.0f, 0.0f, 1500.0f);
				Wallpos1 = D3DXVECTOR3(1500.0f, 0.0f, -1500.0f);
				break;
			}

			//Wallpos0 = pWall->aVtxpos[0];
			//Wallpos1 = pWall->aVtxpos[1];

			vecLine = Wallpos1 - Wallpos0;
			vecToPos = g_Rock[nCntRock].pos - Wallpos0;
			vecMove = g_Rock[nCntRock].pos - g_Rock[nCntRock].posOld;

			fRate = (((vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z))
				/ ((vecLine.z * vecMove.x) - (vecLine.x * vecMove.z)));

			if ((vecLine.z * vecToPos.x)
				- (vecLine.x * vecToPos.z) <= 0.0f
				&& fRate >= 0.0f && fRate <= 1.0f)
			{
				D3DXVECTOR3 posCross = vecLine * fRate;	//交点に位置をずらす

				bWall = true;

				if (g_Rock[nCntRock].nType != ROCKTYPE_INVISIBLE)
				{
					DisableRock(&g_Rock[nCntRock]);
				}
				break;
			}
		}
	}

	return bWall;
}

//========================================
//岩と壁の当たり判定
//========================================
bool CollisionRockRock(int nCntThrow)
{
	bool bRock;

	bRock = false;

	for (int nCntRock = 0; nCntRock < MAX_WALL; nCntRock++)
	{
		if (g_Rock[nCntRock].bUse == true && g_Rock[nCntRock].nType != ROCKTYPE_INVISIBLE && nCntRock != nCntThrow)
		{
			if (SphereCollision(g_Rock[nCntRock].pos, g_Rock[nCntThrow].pos, g_Rock[nCntRock].vtxMax.x * 2.0f, g_Rock[nCntThrow].vtxMax.x * 2.0f) == true)
			{
				DisableRock(&g_Rock[nCntRock]);
				bRock = true;
			}
		}
	}

	return bRock;
}

//========================================
//岩のヒット処理
//========================================
void HitRock(int nDamage,int nCntRock)
{
	//体力を減らす
	g_Rock[nCntRock].nLife -= nDamage;

	//プレイヤーの体力がなくなった
	if (g_Rock[nCntRock].nLife == 0)
	{
		g_Rock[nCntRock].bUse = false;					//使用していない状態にする
	}
	else
	{//体力が残っている
		g_Rock[nCntRock].state = ROCKSTATE_DAMEGE;
		g_Rock[nCntRock].nCounterState = 5;
	}
}

//========================================
//岩の取得
//========================================
Rock *GetRock(void)
{
	return &g_Rock[0];
}

//========================================
//岩が当たった時の取得
//========================================
int *GetHitRockNum(void)
{
	return &g_RockCounter;
}

//==================================================
//岩のデバッグ表示
//==================================================
void DebugRock(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//場所
	char aStr[1][256];					//数
	int nNumRock = 0;
	LPD3DXFONT Font = GetFont();		//メインのやつ

	for (int nCnt = 0; nCnt < MAX_ROCK; nCnt++)
	{
		if (g_Rock[nCnt].bUse == true)
		{
			nNumRock++;
		}
	}

	//文字列に代入
	sprintf(&aStr[0][0], "岩の数：%d", nNumRock);

	rect.top += DEBUG_TOP;
	Font->DrawText(NULL, &aStr[0][0], -1, &rect, DT_RIGHT, D3DXCOLOR(DEBUG_CLO_R, DEBUG_CLO_G, DEBUG_CLO_B, DEBUG_CLO_A));
}

//========================================
//音を再生する処理
//========================================
void PlaySoundRockCrash(Rock *pRock)
{
	//情報取得
	Player *pPlayer = GetPlayer();

	//差分取得
	D3DXVECTOR3 posDiff = pRock->pos - pPlayer->pos;
	float fDiffLength;

	fDiffLength = D3DXVec3Length(&posDiff);

	if (SphereCollision(pRock->pos, pPlayer->pos, ROLLSOUND_RANGE, 0) == true && pRock->nType != ROCKTYPE_INVISIBLE)
	{//プレイヤーが音の聞こえる範囲内だったら

		//差分の量によって音量を変える
		SetVolume(SOUND_LABEL_SE_CRASH, 1.0f - fDiffLength / ROLLSOUND_RANGE);
		
		PlaySound(SOUND_LABEL_SE_CRASH);
	}
}