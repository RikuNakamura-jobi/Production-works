//===========================
//
//ダイレクトX.enemyファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "camera.h"
#include "meshwall.h"
#include "particle.h"
#include "effect.h"
#include "explosion.h"
#include "player.h"
#include "bullet.h"
#include "result.h"
#include "game.h"
#include "model.h"
#include "sound.h"
#include <stdio.h>

//マクロ定義
#define MAX_ENEMY_ENEMY (64)

//プロトタイプ宣言
void MoveEnemy(int nNumEnemy);
void SetRotEnemy(int nNumEnemy);
void SetRotBossEnemy(int nNumEnemy);
void ShotEnemy(int nNumEnemy);

//グローバル変数宣言
Enemy g_aEnemy[MAX_ENEMY_ENEMY];
EnemyX g_aEnemyXmodel[MAX_ENEMY_ENEMY];
int g_nIdxShadowEnemy = -1;
int g_nNumEnemy;
int g_nNumEnemyCount;
int g_nNumEnemyModel;
char g_aEnemyNameEnemy[32][128];

//===========================
//プレイヤーの初期化処理
//===========================
void InitEnemy(void)
{
	int nNumVtx = 0;
	DWORD dwSizeFVF;
	BYTE *pVtxBuff;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL *pMat;

	g_nNumEnemy = 0;
	ReadFileEnemy();

	for (int nCnt = 0; nCnt < g_nNumEnemyModel; nCnt++)
	{
		//xファイルの読み込み
		D3DXLoadMeshFromX(&g_aEnemyNameEnemy[nCnt][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aEnemyXmodel[nCnt].pBuffMatEnemy,
			NULL,
			&g_aEnemyXmodel[nCnt].dwNumMatEnemy,
			&g_aEnemyXmodel[nCnt].pMeshEnemy);


		//頂点数取得
		nNumVtx = g_aEnemyXmodel[nCnt].pMeshEnemy->GetNumVertices();

		//頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(g_aEnemyXmodel[nCnt].pMeshEnemy->GetFVF());

		//頂点バッファをロック
		g_aEnemyXmodel[nCnt].pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (vtx.x >= g_aEnemyXmodel[nCnt].vtxMaxEnemy.x)
			{
				g_aEnemyXmodel[nCnt].vtxMaxEnemy.x = vtx.x;
			}
			if (vtx.x <= g_aEnemyXmodel[nCnt].vtxMinEnemy.x)
			{
				g_aEnemyXmodel[nCnt].vtxMinEnemy.x = vtx.x;
			}

			if (vtx.y >= g_aEnemyXmodel[nCnt].vtxMaxEnemy.y)
			{
				g_aEnemyXmodel[nCnt].vtxMaxEnemy.y = vtx.y;
			}
			if (vtx.y <= g_aEnemyXmodel[nCnt].vtxMinEnemy.y)
			{
				g_aEnemyXmodel[nCnt].vtxMinEnemy.y = vtx.y;
			}

			if (vtx.z >= g_aEnemyXmodel[nCnt].vtxMaxEnemy.z)
			{
				g_aEnemyXmodel[nCnt].vtxMaxEnemy.z = vtx.z;
			}
			if (vtx.z <= g_aEnemyXmodel[nCnt].vtxMinEnemy.z)
			{
				g_aEnemyXmodel[nCnt].vtxMinEnemy.z = vtx.z;
			}

			pVtxBuff += dwSizeFVF;
		}

		//頂点バッファをアンロック
		g_aEnemyXmodel[nCnt].pMeshEnemy->UnlockVertexBuffer();

		//マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_aEnemyXmodel[nCnt].pBuffMatEnemy->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aEnemyXmodel[nCnt].dwNumMatEnemy; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aEnemyXmodel[nCnt].apTextureEnemy[nCntMat]);
			}
		}
	}
}

//===========================
//プレイヤーの初期化処理
//===========================
void InitEnemyGame(void)
{
	g_nNumEnemyCount = 0;

	for (int nCnt = 0; nCnt < MAX_ENEMY_ENEMY; nCnt++)
	{
		g_aEnemy[nCnt].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCnt].nTimer = 0;
		g_aEnemy[nCnt].nMoveTimer = 0;
		g_aEnemy[nCnt].nMoveTime = 100;
		g_aEnemy[nCnt].nLifeCounter = 0;
		g_aEnemy[nCnt].nTimerState = 0;
		g_aEnemy[nCnt].fRot = 0.0f;
		g_aEnemy[nCnt].bUse = false;
	}

	for (int nCnt = 0; nCnt < g_nNumEnemy; nCnt++)
	{
		SetEnemy(g_aEnemy[nCnt].posFile, D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_aEnemy[nCnt].rotFile, g_aEnemy[nCnt].nLifeFile, g_aEnemy[nCnt].nTypeFile);
	}
}

//===========================
//プレイヤーの終了処理
//===========================
void UninitEnemy(void)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY_ENEMY; nCnt++)
	{
		//テクスチャの破棄
		if (g_aEnemy[nCnt].aEnemyX.pMeshEnemy != NULL)
		{
			g_aEnemy[nCnt].aEnemyX.pMeshEnemy->Release();
			g_aEnemy[nCnt].aEnemyX.pMeshEnemy = NULL;
		}

		//頂点バッファの破棄
		if (g_aEnemy[nCnt].aEnemyX.pBuffMatEnemy != NULL)
		{
			g_aEnemy[nCnt].aEnemyX.pBuffMatEnemy->Release();
			g_aEnemy[nCnt].aEnemyX.pBuffMatEnemy = NULL;
		}
	}
}

//===========================
//プレイヤーの更新処理
//===========================
void UpdateEnemy(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_ENEMY_ENEMY; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse == true)
		{
			g_aEnemy[nCnt].posOld = g_aEnemy[nCnt].pos;

			if (g_aEnemy[nCnt].nLife <= 0)
			{
				g_aEnemy[nCnt].nLifeCounter++;
				SetParticle(g_aEnemy[nCnt].pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 50.0f, 30, 0, 314, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 150);

				if (g_aEnemy[nCnt].nType == 4)
				{
					for (int nCntEnemy = 0; nCntEnemy < 200; nCntEnemy++)
					{
						float fRandY = (float)(rand() % 629 - 314) / 100.0f;
						float fRandZ = (float)(rand() % 629 - 314) / 100.0f;

						SetBullet(g_aEnemy[nCnt].pos, g_aEnemy[nCnt].move, g_aEnemy[nCnt].pos, D3DXVECTOR3(0.0f, fRandY, fRandZ), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 300, 2);
					}

					SetEffect(g_aEnemy[nCnt].pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 1000.0f, 30);
				}
				else
				{
					for (int nCntEnemy = 0; nCntEnemy < 10; nCntEnemy++)
					{
						float fRandY = (float)(rand() % 629 - 314) / 100.0f;
						float fRandZ = (float)(rand() % 629 - 314) / 100.0f;

						SetBullet(g_aEnemy[nCnt].pos, g_aEnemy[nCnt].move, g_aEnemy[nCnt].pos, D3DXVECTOR3(0.0f, fRandY, fRandZ), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 300, 2);
					}

					SetEffect(g_aEnemy[nCnt].pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 150.0f, 30);
				}

				if (g_aEnemy[nCnt].nLifeCounter >= 5)
				{
					g_aEnemy[nCnt].bUse = false;
					g_nNumEnemyCount--;
					g_aEnemy[nCnt].nLifeCounter = 0;
				}
			}

			if (g_aEnemy[nCnt].nType >= 0 && g_aEnemy[nCnt].nType <= 2)
			{
				MoveEnemy(nCnt);
			}
			else if (g_aEnemy[nCnt].nType == 3)
			{
				if (g_aEnemy[nCnt].nTimer >= 5000)
				{
					int randEnemy = rand() % 3;

					SetEnemy(g_aEnemy[nCnt].pos, D3DXVECTOR3(0.0f, 0.0f, -0.0f), g_aEnemy[nCnt].rot, 3 - randEnemy, randEnemy);

					g_aEnemy[nCnt].nLife--;

					g_aEnemy[nCnt].nTimer = 0;
				}
				else
				{
					g_aEnemy[nCnt].nTimer += rand() % 100 + 1;
				}
			}
			else if (g_aEnemy[nCnt].nType == 4)
			{
				MoveEnemy(nCnt);
			}

			g_aEnemy[nCnt].vtxMinEnemy = g_aEnemy[nCnt].pos + g_aEnemyXmodel[g_aEnemy[nCnt].nType].vtxMinEnemy;
			g_aEnemy[nCnt].vtxMaxEnemy = g_aEnemy[nCnt].pos + g_aEnemyXmodel[g_aEnemy[nCnt].nType].vtxMaxEnemy;
		}
	}

	if (g_nNumEnemyCount <= 0)
	{
		if (GetGameState() == GAMESTATE_NORMAL)
		{
			//サウンドの停止
			StopSound();

			if (rand() % 65536 == 0)
			{
				//サウンドの再生
				PlaySound(SOUND_LABEL_BGM004);
			}
			else
			{
				//サウンドの再生
				PlaySound(SOUND_LABEL_BGM003);
			}

			SetEnemy(D3DXVECTOR3(0.0f, 2750.0f, 200.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 150, 4);
			SetGameState(GAMESTATE_BOSS, 0);
		}
		else if (GetGameState() == GAMESTATE_BOSSBATTLE)
		{
			SetResult(RESULT_CLEAR);
			SetGameState(GAMESTATE_END, 0);
		}
	}
}

//===========================
//プレイヤーの描画処理
//===========================
void DrawEnemy(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	for (int nCnt = 0; nCnt < MAX_ENEMY_ENEMY; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse == true)
		{
			//ダメージ状態判定
			switch (g_aEnemy[nCnt].state)
			{
			case ENEMYSTATE_NORMAL:
				//マテリアル情報に対するポインタを取得
				pMat = (D3DXMATERIAL*)g_aEnemy[nCnt].aEnemyX.pBuffMatEnemy->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCnt].aEnemyX.dwNumMatEnemy; nCntMat++)
				{
					pMat->MatD3D.Emissive = D3DXCOLOR(0.2f, 0.1f, 0.1f, 1.0f);

					pMat++;
				}

				break;
			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCnt].nTimerState--;

				//マテリアル情報に対するポインタを取得
				pMat = (D3DXMATERIAL*)g_aEnemy[nCnt].aEnemyX.pBuffMatEnemy->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCnt].aEnemyX.dwNumMatEnemy; nCntMat++)
				{
					pMat->MatD3D.Emissive = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					pMat++;
				}

				if (g_aEnemy[nCnt].nTimerState <= 0)
				{
					g_aEnemy[nCnt].state = ENEMYSTATE_NORMAL;
				}
				break;
			}

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCnt].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aEnemy[nCnt].rot.y, g_aEnemy[nCnt].rot.x, g_aEnemy[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCnt].mtxWorld, &g_aEnemy[nCnt].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_aEnemy[nCnt].pos.x, g_aEnemy[nCnt].pos.y, g_aEnemy[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCnt].mtxWorld, &g_aEnemy[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCnt].mtxWorld);


			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルのデータのポイントを取得
			pMat = (D3DXMATERIAL*)g_aEnemy[nCnt].aEnemyX.pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCnt].aEnemyX.dwNumMatEnemy; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_aEnemy[nCnt].aEnemyX.apTextureEnemy[nCntMat]);

				//モデル(パーツ)の描画
				g_aEnemy[nCnt].aEnemyX.pMeshEnemy->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//===========================
//プレイヤーの描画処理
//===========================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nLife, int nType)
{
	for (int nCntSet = 0; nCntSet < MAX_ENEMY_ENEMY; nCntSet++)
	{
		if (g_aEnemy[nCntSet].bUse == false)
		{
			g_aEnemy[nCntSet].pos = pos;
			g_aEnemy[nCntSet].move = move;
			g_aEnemy[nCntSet].rot = rot;
			g_aEnemy[nCntSet].nLife = nLife;
			g_aEnemy[nCntSet].nType = nType;

			g_aEnemy[nCntSet].state = ENEMYSTATE_NORMAL;
			g_aEnemy[nCntSet].nTimer = 0;
			g_aEnemy[nCntSet].nMoveTimer = 0;
			g_aEnemy[nCntSet].nTimerState = 0;

			for (int nCnt1 = 0; nCnt1 < 8; nCnt1++)
			{
				g_aEnemy[nCntSet].aEnemyX.apTextureEnemy[nCnt1] = g_aEnemyXmodel[g_aEnemy[nCntSet].nType].apTextureEnemy[nCnt1];
			}

			g_aEnemy[nCntSet].aEnemyX.dwNumMatEnemy = g_aEnemyXmodel[g_aEnemy[nCntSet].nType].dwNumMatEnemy;
			g_aEnemy[nCntSet].aEnemyX.pBuffMatEnemy = g_aEnemyXmodel[g_aEnemy[nCntSet].nType].pBuffMatEnemy;
			g_aEnemy[nCntSet].aEnemyX.pMeshEnemy = g_aEnemyXmodel[g_aEnemy[nCntSet].nType].pMeshEnemy;
			g_aEnemy[nCntSet].vtxMinEnemy = g_aEnemy[nCntSet].pos + g_aEnemyXmodel[g_aEnemy[nCntSet].nType].vtxMinEnemy;
			g_aEnemy[nCntSet].vtxMaxEnemy = g_aEnemy[nCntSet].pos + g_aEnemyXmodel[g_aEnemy[nCntSet].nType].vtxMaxEnemy;

			g_nNumEnemyCount++;

			g_aEnemy[nCntSet].bUse = true;
			break;
		}
	}
}

//===========================
//プレイヤーの描画処理
//===========================
void MoveEnemy(int nNumEnemy)
{
	Player *pPlayer = GetPlayer();
	float fRot, fLength;

	fLength = ((g_aEnemy[nNumEnemy].pos.x - pPlayer->pos.x) * (g_aEnemy[nNumEnemy].pos.x - pPlayer->pos.x)) + ((g_aEnemy[nNumEnemy].pos.z - pPlayer->pos.z) * (g_aEnemy[nNumEnemy].pos.z - pPlayer->pos.z));

	if (fLength <= 500000.0f && (g_aEnemy[nNumEnemy].nType == 0 || g_aEnemy[nNumEnemy].nType == 2))
	{
		ShotEnemy(nNumEnemy);
		SetRotEnemy(nNumEnemy);

		g_aEnemy[nNumEnemy].move.y -= 0.5f;
	}
	else
	{
		switch (g_aEnemy[nNumEnemy].nType)
		{
		case 0:
			ShotEnemy(nNumEnemy);

			SetRotEnemy(nNumEnemy);
			
			fRot = (float)(rand() % 31 - 15) / 100.0f;
			g_aEnemy[nNumEnemy].rot.y += fRot;

			g_aEnemy[nNumEnemy].move.x += sinf(g_aEnemy[nNumEnemy].rot.y) * 0.5f;
			g_aEnemy[nNumEnemy].move.z += cosf(g_aEnemy[nNumEnemy].rot.y) * 0.5f;

			g_aEnemy[nNumEnemy].move.y -= 0.8f;

			g_aEnemy[nNumEnemy].move.x += (0.0f - g_aEnemy[nNumEnemy].move.x) * 0.15f;
			g_aEnemy[nNumEnemy].move.z += (0.0f - g_aEnemy[nNumEnemy].move.z) * 0.15f;
			break;
		case 1:
			ShotEnemy(nNumEnemy);

			SetRotEnemy(nNumEnemy);
			
			fRot = (float)(rand() % 31 - 15) / 100.0f;
			g_aEnemy[nNumEnemy].rot.y += fRot;

			g_aEnemy[nNumEnemy].move.x += sinf(g_aEnemy[nNumEnemy].rot.y) * 0.6f;
			g_aEnemy[nNumEnemy].move.z += cosf(g_aEnemy[nNumEnemy].rot.y) * 0.6f;

			if (g_aEnemy[nNumEnemy].nMoveTimer == g_aEnemy[nNumEnemy].nMoveTime)
			{
				g_aEnemy[nNumEnemy].move.y += 20.0f;
				g_aEnemy[nNumEnemy].nMoveTimer = 0;
				g_aEnemy[nNumEnemy].nMoveTime = rand() % 60 + 120;
			}
			else
			{
				g_aEnemy[nNumEnemy].nMoveTimer++;
			}

			g_aEnemy[nNumEnemy].move.y -= 0.5f;

			g_aEnemy[nNumEnemy].move.x += (0.0f - g_aEnemy[nNumEnemy].move.x) * 0.15f;
			g_aEnemy[nNumEnemy].move.z += (0.0f - g_aEnemy[nNumEnemy].move.z) * 0.15f;
			break;
		case 2:
			ShotEnemy(nNumEnemy);

			SetRotEnemy(nNumEnemy);
			
			fRot = (float)(rand() % 31 - 15) / 100.0f;
			g_aEnemy[nNumEnemy].rot.y += fRot;

			g_aEnemy[nNumEnemy].move.x += sinf(g_aEnemy[nNumEnemy].rot.y) * 0.4f;
			g_aEnemy[nNumEnemy].move.z += cosf(g_aEnemy[nNumEnemy].rot.y) * 0.4f;

			g_aEnemy[nNumEnemy].move.y -= 0.3f;

			g_aEnemy[nNumEnemy].move.x += (0.0f - g_aEnemy[nNumEnemy].move.x) * 0.03f;
			g_aEnemy[nNumEnemy].move.z += (0.0f - g_aEnemy[nNumEnemy].move.z) * 0.03f;
			break;
		case 4:
			if (GetGameState() == GAMESTATE_BOSSBATTLE)
			{
				ShotEnemy(nNumEnemy);
			}

			if (g_aEnemy[nNumEnemy].nMoveTimer == g_aEnemy[nNumEnemy].nMoveTime * 3 && GetGameState() == GAMESTATE_BOSS)
			{
				SetGameState(GAMESTATE_BOSSBATTLE, 0);
			}
			else
			{
				g_aEnemy[nNumEnemy].nMoveTimer++;
			}

			g_aEnemy[nNumEnemy].move.y -= 0.05f;
			g_aEnemy[nNumEnemy].rot.y -= 0.01f;
			break;
		default:
			break;
		}
	}

	g_aEnemy[nNumEnemy].pos += g_aEnemy[nNumEnemy].move;

	switch (g_aEnemy[nNumEnemy].nType)
	{
	case 0:
		if (g_aEnemy[nNumEnemy].pos.y < 0.0f)
		{
			g_aEnemy[nNumEnemy].move.y = 0.0f;
			g_aEnemy[nNumEnemy].pos.y = 0.0f;
		}

		if (CollisionMeshWall(&g_aEnemy[nNumEnemy].pos, &g_aEnemy[nNumEnemy].posOld, &g_aEnemy[nNumEnemy].move) == true)
		{
			g_aEnemy[nNumEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nNumEnemy].pos = g_aEnemy[nNumEnemy].posOld;
		}

		if (CollisionModel(&g_aEnemy[nNumEnemy].pos, &g_aEnemy[nNumEnemy].posOld, &g_aEnemy[nNumEnemy].move, 0) == true)
		{
			g_aEnemy[nNumEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nNumEnemy].pos = g_aEnemy[nNumEnemy].posOld;
		}
		break;
	case 1:
		if (g_aEnemy[nNumEnemy].pos.y < 0.0f)
		{
			g_aEnemy[nNumEnemy].move.y = 0.0f;
			g_aEnemy[nNumEnemy].pos.y = 0.0f;
		}

		if (CollisionMeshWall(&g_aEnemy[nNumEnemy].pos, &g_aEnemy[nNumEnemy].posOld, &g_aEnemy[nNumEnemy].move) == true)
		{
			g_aEnemy[nNumEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nNumEnemy].pos = g_aEnemy[nNumEnemy].posOld;
		}

		if (CollisionModel(&g_aEnemy[nNumEnemy].pos, &g_aEnemy[nNumEnemy].posOld, &g_aEnemy[nNumEnemy].move, 0) == true)
		{
			g_aEnemy[nNumEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nNumEnemy].pos = g_aEnemy[nNumEnemy].posOld;
		}
		break;
	case 2:
		if (g_aEnemy[nNumEnemy].pos.y < 200.0f)
		{
			g_aEnemy[nNumEnemy].move.y = 0.0f;
			g_aEnemy[nNumEnemy].pos.y = 200.0f;
		}

		if (CollisionMeshWall(&g_aEnemy[nNumEnemy].pos, &g_aEnemy[nNumEnemy].posOld, &g_aEnemy[nNumEnemy].move) == true)
		{
			g_aEnemy[nNumEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nNumEnemy].pos = g_aEnemy[nNumEnemy].posOld;
		}
		break;
	case 4:
		if (g_aEnemy[nNumEnemy].pos.y < 750.0f)
		{
			g_aEnemy[nNumEnemy].move.y = 0.0f;
			g_aEnemy[nNumEnemy].pos.y = 750.0f;
		}

		if (CollisionMeshWall(&g_aEnemy[nNumEnemy].pos, &g_aEnemy[nNumEnemy].posOld, &g_aEnemy[nNumEnemy].move) == true)
		{
			g_aEnemy[nNumEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nNumEnemy].pos = g_aEnemy[nNumEnemy].posOld;
		}

		if (CollisionModel(&g_aEnemy[nNumEnemy].pos, &g_aEnemy[nNumEnemy].posOld, &g_aEnemy[nNumEnemy].move, 0) == true)
		{
			g_aEnemy[nNumEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nNumEnemy].pos = g_aEnemy[nNumEnemy].posOld;
		}
		break;
	default:
		break;
	}
}

//===========================
//プレイヤーの描画処理
//===========================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//===========================
//プレイヤーの描画処理
//===========================
bool CollisionEnemy(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, int nType)
{
	bool bCollision;
	float fLength;

	bCollision = false;

	for (int nCnt = 0; nCnt < MAX_ENEMY_ENEMY; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse == true)
		{
			fLength = ((g_aEnemy[nCnt].pos.x - pos->x) * (g_aEnemy[nCnt].pos.x - pos->x)) + ((g_aEnemy[nCnt].pos.z - pos->z) * (g_aEnemy[nCnt].pos.z - pos->z));

			if (g_aEnemy[nCnt].nType >= 0 && g_aEnemy[nCnt].nType <= 2)
			{
				if (fLength <= 12000.0f && pos->y >= g_aEnemy[nCnt].vtxMinEnemy.y && pos->y <= g_aEnemy[nCnt].vtxMaxEnemy.y)
				{
					if (nType == 1)
					{
						bCollision = true;

						//サウンドの再生
						PlaySound(SOUND_LABEL_SE_HIT);

						g_aEnemy[nCnt].state = ENEMYSTATE_DAMAGE;
						g_aEnemy[nCnt].nTimerState = 10;
						g_aEnemy[nCnt].nLife--;
						break;
					}
				}
			}
			else if (g_aEnemy[nCnt].nType == 3)
			{
				if (fLength <= 4000.0f && pos->y >= g_aEnemy[nCnt].vtxMinEnemy.y && pos->y <= g_aEnemy[nCnt].vtxMaxEnemy.y)
				{
					if (nType == 1)
					{
						bCollision = true;

						//サウンドの再生
						PlaySound(SOUND_LABEL_SE_HIT);

						g_aEnemy[nCnt].state = ENEMYSTATE_DAMAGE;
						g_aEnemy[nCnt].nTimerState = 10;
						g_aEnemy[nCnt].nLife--;
						break;
					}
				}
			}
			else if (g_aEnemy[nCnt].nType == 4 && GetGameState() == GAMESTATE_BOSSBATTLE)
			{
				if (fLength <= 160000.0f && pos->y >= g_aEnemy[nCnt].vtxMinEnemy.y && pos->y <= g_aEnemy[nCnt].vtxMaxEnemy.y)
				{
					if (nType == 1)
					{
						bCollision = true;

						//サウンドの再生
						PlaySound(SOUND_LABEL_SE_HIT);

						g_aEnemy[nCnt].state = ENEMYSTATE_DAMAGE;
						g_aEnemy[nCnt].nTimerState = 10;
						g_aEnemy[nCnt].nLife--;
						break;
					}
				}
			}
		}
	}

	return bCollision;
}

//===========================
//プレイヤーの描画処理
//===========================
void SetRotEnemy(int nNumEnemy)
{
	Player *pPlayer = GetPlayer();
	float fRotMove, fRotDest, fRotDiff;

	fRotMove = g_aEnemy[nNumEnemy].rot.y;
	fRotDest = atan2f(pPlayer->pos.x - g_aEnemy[nNumEnemy].pos.x, pPlayer->pos.z - g_aEnemy[nNumEnemy].pos.z);
	fRotDiff = fRotDest - fRotMove;

	if (fRotDiff > 3.14f)
	{
		fRotDiff -= 6.28f;
	}
	else if (fRotDiff <= -3.14f)
	{
		fRotDiff += 6.28f;
	}

	fRotMove += fRotDiff * 0.5f;

	if (fRotMove > 3.14f)
	{
		fRotMove -= 6.28f;
	}
	else if (fRotMove <= -3.14f)
	{
		fRotMove += 6.28f;
	}

	g_aEnemy[nNumEnemy].rot.y = fRotMove;

	if (g_aEnemy[nNumEnemy].rot.y > 3.14f)
	{
		g_aEnemy[nNumEnemy].rot.y -= 6.28f;
	}
	else if (g_aEnemy[nNumEnemy].rot.y <= -3.14f)
	{
		g_aEnemy[nNumEnemy].rot.y += 6.28f;
	}
}

//===========================
//プレイヤーの描画処理
//===========================
void SetRotBossEnemy(int nNumEnemy)
{
	Player *pPlayer = GetPlayer();
	float fRotMove, fRotDest, fRotDiff;

	fRotMove = g_aEnemy[nNumEnemy].rot.y;
	fRotDest = atan2f(pPlayer->pos.x - g_aEnemy[nNumEnemy].pos.x, pPlayer->pos.z - g_aEnemy[nNumEnemy].pos.z);
	fRotDiff = fRotDest - fRotMove;

	if (fRotDiff > 3.14f)
	{
		fRotDiff -= 6.28f;
	}
	else if (fRotDiff <= -3.14f)
	{
		fRotDiff += 6.28f;
	}

	fRotMove += fRotDiff * 1.0f;

	if (fRotMove > 3.14f)
	{
		fRotMove -= 6.28f;
	}
	else if (fRotMove <= -3.14f)
	{
		fRotMove += 6.28f;
	}

	g_aEnemy[nNumEnemy].rot.y = fRotMove;

	if (g_aEnemy[nNumEnemy].rot.y > 3.14f)
	{
		g_aEnemy[nNumEnemy].rot.y -= 6.28f;
	}
	else if (g_aEnemy[nNumEnemy].rot.y <= -3.14f)
	{
		g_aEnemy[nNumEnemy].rot.y += 6.28f;
	}
}


//===========================
//プレイヤーの描画処理
//===========================
void ShotEnemy(int nNumEnemy)
{
	Player *pPlayer = GetPlayer();
	float fRandY, fRandZ;
	float fShotrot = atan2f(pPlayer->pos.y - g_aEnemy[nNumEnemy].pos.y, pPlayer->pos.x - g_aEnemy[nNumEnemy].pos.x);

	if (g_aEnemy[nNumEnemy].nTimer  >= 50)
	{
		g_aEnemy[nNumEnemy].fRot += 0.5f;

		for (int nCnt = 0; nCnt < 5; nCnt++)
		{
			if (g_aEnemy[nNumEnemy].nType == 0)
			{
				fRandY = (float)(rand() % 7 - 3) / 100.0f;
				fRandZ = (float)(rand() % 7 - 3) / 100.0f;

				if (pPlayer->pos.x >= g_aEnemy[nNumEnemy].pos.x)
				{
					SetBullet(D3DXVECTOR3(g_aEnemy[nNumEnemy].pos.x, g_aEnemy[nNumEnemy].pos.y + 100.0f, g_aEnemy[nNumEnemy].pos.z), g_aEnemy[nNumEnemy].move, pPlayer->pos, D3DXVECTOR3(0.0f, g_aEnemy[nNumEnemy].rot.y + fRandY, (1.57f - fShotrot) + fRandZ + 0.05f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 300, 2);
				}
				else
				{
					SetBullet(D3DXVECTOR3(g_aEnemy[nNumEnemy].pos.x, g_aEnemy[nNumEnemy].pos.y + 100.0f, g_aEnemy[nNumEnemy].pos.z), g_aEnemy[nNumEnemy].move, pPlayer->pos, D3DXVECTOR3(0.0f, g_aEnemy[nNumEnemy].rot.y + fRandY, (fShotrot - 1.57f) + fRandZ + 0.05f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 300, 2);
				}
			}
			else if (g_aEnemy[nNumEnemy].nType == 1)
			{
				fRandY = (float)(rand() % 41 - 20) / 100.0f;
				fRandZ = (float)(rand() % 21 - 10) / 100.0f;

				if (pPlayer->pos.x >= g_aEnemy[nNumEnemy].pos.x)
				{
					SetBullet(D3DXVECTOR3(g_aEnemy[nNumEnemy].pos.x, g_aEnemy[nNumEnemy].pos.y + 100.0f, g_aEnemy[nNumEnemy].pos.z), g_aEnemy[nNumEnemy].move, pPlayer->pos, D3DXVECTOR3(0.0f, g_aEnemy[nNumEnemy].rot.y + fRandY, (1.57f - fShotrot) + fRandZ + 0.05f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 300, 2);
				}
				else
				{
					SetBullet(D3DXVECTOR3(g_aEnemy[nNumEnemy].pos.x, g_aEnemy[nNumEnemy].pos.y + 100.0f, g_aEnemy[nNumEnemy].pos.z), g_aEnemy[nNumEnemy].move, pPlayer->pos, D3DXVECTOR3(0.0f, g_aEnemy[nNumEnemy].rot.y + fRandY, (fShotrot - 1.57f) + fRandZ + 0.05f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 300, 2);
				}

				fRandY = (float)(rand() % 41 - 20) / 100.0f;
				fRandZ = (float)(rand() % 11 - 5) / 100.0f;

				if (pPlayer->pos.x >= g_aEnemy[nNumEnemy].pos.x)
				{
					SetBullet(D3DXVECTOR3(g_aEnemy[nNumEnemy].pos.x, g_aEnemy[nNumEnemy].pos.y + 100.0f, g_aEnemy[nNumEnemy].pos.z), g_aEnemy[nNumEnemy].move, pPlayer->pos, D3DXVECTOR3(0.0f, g_aEnemy[nNumEnemy].rot.y + fRandY, (1.57f - fShotrot) + fRandZ + 0.05f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 300, 2);
				}
				else
				{
					SetBullet(D3DXVECTOR3(g_aEnemy[nNumEnemy].pos.x, g_aEnemy[nNumEnemy].pos.y + 100.0f, g_aEnemy[nNumEnemy].pos.z), g_aEnemy[nNumEnemy].move, pPlayer->pos, D3DXVECTOR3(0.0f, g_aEnemy[nNumEnemy].rot.y + fRandY, (fShotrot - 1.57f) + fRandZ + 0.05f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 300, 2);
				}
			}
			else if (g_aEnemy[nNumEnemy].nType == 2)
			{
				fRandY = (float)(rand() % 11 - 5) / 100.0f;
				fRandZ = 3.14f - ((float)(rand() % 301 - 150) / 100.0f);

				if (pPlayer->pos.x >= g_aEnemy[nNumEnemy].pos.x)
				{
					SetBullet(D3DXVECTOR3(g_aEnemy[nNumEnemy].pos.x, g_aEnemy[nNumEnemy].pos.y + 100.0f, g_aEnemy[nNumEnemy].pos.z), g_aEnemy[nNumEnemy].move, pPlayer->pos, D3DXVECTOR3(0.0f, g_aEnemy[nNumEnemy].rot.y, fRandZ), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 300, 2);
				}
				else
				{
					SetBullet(D3DXVECTOR3(g_aEnemy[nNumEnemy].pos.x, g_aEnemy[nNumEnemy].pos.y + 100.0f, g_aEnemy[nNumEnemy].pos.z), g_aEnemy[nNumEnemy].move, pPlayer->pos, D3DXVECTOR3(0.0f, g_aEnemy[nNumEnemy].rot.y, fRandZ), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 300, 2);
				}
			}
			else if (g_aEnemy[nNumEnemy].nType == 4)
			{
				for (int nCntBullet = 0; nCntBullet < 25; nCntBullet++)
				{
					fRandY = (float)(rand() % 629 - 314) / 100.0f;
					fRandZ = 1.60f + ((float)(rand() % 11) / 100.0f);

					SetBullet(D3DXVECTOR3(g_aEnemy[nNumEnemy].pos.x, g_aEnemy[nNumEnemy].pos.y - 450.0f, g_aEnemy[nNumEnemy].pos.z - 50.0f), g_aEnemy[nNumEnemy].move, pPlayer->pos, D3DXVECTOR3(0.0f, fRandY, fRandZ), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 300, 2);
				}

				for (int nCntBullet = 0; nCntBullet < 30; nCntBullet++)
				{
					SetBullet(D3DXVECTOR3(g_aEnemy[nCnt].pos.x + (cosf(g_aEnemy[nCnt].rot.y) * 150.0f), g_aEnemy[nCnt].pos.y - 720.0f, g_aEnemy[nCnt].pos.z - (sinf(g_aEnemy[nCnt].rot.y) * 150.0f)), g_aEnemy[nNumEnemy].move, pPlayer->pos, D3DXVECTOR3(0.0f, g_aEnemy[nNumEnemy].fRot, 1.575f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 300, 2);

					SetBullet(D3DXVECTOR3(g_aEnemy[nCnt].pos.x - (cosf(g_aEnemy[nCnt].rot.y) * 150.0f), g_aEnemy[nCnt].pos.y - 720.0f, g_aEnemy[nCnt].pos.z + (sinf(g_aEnemy[nCnt].rot.y) * 150.0f)), g_aEnemy[nNumEnemy].move, pPlayer->pos, D3DXVECTOR3(0.0f, -g_aEnemy[nNumEnemy].fRot, 1.575f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 300, 2);

					g_aEnemy[nNumEnemy].fRot += 0.02f;
				}
			}
		}
		g_aEnemy[nNumEnemy].nTimer = 0;
	}
	else
	{
		if (g_aEnemy[nNumEnemy].nType == 4)
		{
			g_aEnemy[nNumEnemy].nTimer++;
			g_aEnemy[nNumEnemy].nTimer++;
		}
		g_aEnemy[nNumEnemy].nTimer++;
	}
}

//===========================
//プレイヤーの描画処理
//===========================
void ReadFileEnemy(void)
{
	char aText[128];
	FILE *pFile = fopen("data\\MODEL\\enemy.txt", "r");

	if (pFile == NULL)
	{
		return;
	}

	for (int nCnt = 0; nCnt < 11; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	fscanf(pFile, "%s = %d\n", &aText[0], &g_nNumEnemyModel);

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	for (int nCnt = 0; nCnt < g_nNumEnemyModel; nCnt++)
	{
		fscanf(pFile, "%s = %s		%s %s\n", &aText[0], &g_aEnemyNameEnemy[nCnt][0], &aText[0], &aText[0]);
	}

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	fscanf(pFile, "%s = %d\n", &aText[0], &g_nNumEnemy);

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	for (int nCnt = 0; nCnt < g_nNumEnemy; nCnt++)
	{
		fscanf(pFile, "%s", &aText[0]);
		fscanf(pFile, "%s = %d", &aText[0], &g_aEnemy[nCnt].nTypeFile);
		fscanf(pFile, "%s = %d", &aText[0], &g_aEnemy[nCnt].nLifeFile);
		fscanf(pFile, "%s = %f %f %f", &aText[0], &g_aEnemy[nCnt].posFile.x, &g_aEnemy[nCnt].posFile.y, &g_aEnemy[nCnt].posFile.z);
		fscanf(pFile, "%s = %f %f %f", &aText[0], &g_aEnemy[nCnt].rotFile.x, &g_aEnemy[nCnt].rotFile.y, &g_aEnemy[nCnt].rotFile.z);
		fscanf(pFile, "%s\n", &aText[0]);

		g_aEnemy[nCnt].bUse = true;
	}

	fclose(pFile);
}