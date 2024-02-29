//===========================
//
//ダイレクトX.shockファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "shock.h"
#include "input.h"
#include "camera.h"
#include "camera.h"
#include "meshwall.h"
#include <stdio.h>
#include "score.h"
#include "game.h"
#include "point.h"
#include "point_log.h"
#include "tutorial.h"
#include "sound.h"
#include "count.h"

//マクロ定義
#define MAX_SHOCK (32)
#define SHOCK_SCORE (300)			//飛び越えた時のスコア

//グローバル変数宣言
Shock g_aShock[MAX_SHOCK];
ShockX g_aShockX;

//===========================
//衝撃波の初期化処理
//===========================
void InitShock(void)
{
	int nNumVtx;
	DWORD dwSizeFVF;
	BYTE *pVtxBuff;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL *pMat;

	//xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\shock_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aShockX.pBuffMatShock,
		NULL,
		&g_aShockX.dwNumMatShock,
		&g_aShockX.pMeshShock);

	for (int nCnt = 0; nCnt < MAX_SHOCK; nCnt++)
	{
		g_aShock[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShock[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShock[nCnt].nLife = 0;
		g_aShock[nCnt].fRadiusMax = 0.0f;
		g_aShock[nCnt].fRadiusMin = 0.0f;
		g_aShock[nCnt].fRadSpeed = 0.0f;
		g_aShock[nCnt].bUse = false;

		//頂点数取得
		nNumVtx = g_aShockX.pMeshShock->GetNumVertices();

		//頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(g_aShockX.pMeshShock->GetFVF());

		//頂点バッファをロック
		g_aShockX.pMeshShock->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (vtx.x >= g_aShockX.vtxMaxShock.x)
			{
				g_aShockX.vtxMaxShock.x = vtx.x;
			}
			if (vtx.x <= g_aShockX.vtxMinShock.x)
			{
				g_aShockX.vtxMinShock.x = vtx.x;
			}

			if (vtx.y >= g_aShockX.vtxMaxShock.y)
			{
				g_aShockX.vtxMaxShock.y = vtx.y;
			}
			if (vtx.y <= g_aShockX.vtxMinShock.y)
			{
				g_aShockX.vtxMinShock.y = vtx.y;
			}

			if (vtx.z >= g_aShockX.vtxMaxShock.z)
			{
				g_aShockX.vtxMaxShock.z = vtx.z;
			}
			if (vtx.z <= g_aShockX.vtxMinShock.z)
			{
				g_aShockX.vtxMinShock.z = vtx.z;
			}

			pVtxBuff += dwSizeFVF;
		}

		//頂点バッファをアンロック
		g_aShockX.pMeshShock->UnlockVertexBuffer();

		//マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_aShockX.pBuffMatShock->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aShockX.dwNumMatShock; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aShockX.apTextureShock[nCntMat]);
			}
		}

		g_aShock[nCnt].mtxScale = g_aShock[nCnt].mtxWorld;
	}
}

//===========================
//衝撃波の終了処理
//===========================
void UninitShock(void)
{
	//メッシュの破棄
	if (g_aShockX.pMeshShock != NULL)
	{
		g_aShockX.pMeshShock->Release();
		g_aShockX.pMeshShock = NULL;
	}

	//頂点バッファの破棄
	if (g_aShockX.pBuffMatShock != NULL)
	{
		g_aShockX.pBuffMatShock->Release();
		g_aShockX.pBuffMatShock = NULL;
	}

	for (int nCntTex = 0; nCntTex < NUM_TEX_SHOCK;nCntTex++)
	{//テクスチャの破棄
		if (g_aShockX.apTextureShock[nCntTex] != NULL)
		{
			g_aShockX.apTextureShock[nCntTex]->Release();
			g_aShockX.apTextureShock[nCntTex] = NULL;
		}
	}
}

//===========================
//衝撃波の更新処理
//===========================
void UpdateShock(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_SHOCK; nCnt++)
	{
		if (g_aShock[nCnt].bUse == true)
		{
			//前回の最大・最小半径を保存
			g_aShock[nCnt].fRadiusMaxOld = g_aShock[nCnt].fRadiusMax;
			g_aShock[nCnt].fRadiusMinOld = g_aShock[nCnt].fRadiusMin;

			g_aShock[nCnt].nLife--;

			g_aShock[nCnt].fRadiusMax += g_aShockX.vtxMaxShock.x * g_aShock[nCnt].fRadSpeed;
			g_aShock[nCnt].fRadiusMin += g_aShockX.vtxMaxShock.x * g_aShock[nCnt].fRadSpeed;

			g_aShock[nCnt].mtxScale._11 += g_aShock[nCnt].fRadSpeed;
			g_aShock[nCnt].mtxScale._33 += g_aShock[nCnt].fRadSpeed;

			if (g_aShock[nCnt].nLife < 0)
			{
				g_aShock[nCnt].bUse = false;
			}
		}
	}
}

//===========================
//衝撃波の描画処理
//===========================
void DrawShock(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	for (int nCnt = 0; nCnt < MAX_SHOCK; nCnt++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aShock[nCnt].mtxWorld);

		g_aShock[nCnt].mtxWorld._11 = g_aShock[nCnt].mtxScale._11;
		g_aShock[nCnt].mtxWorld._33 = g_aShock[nCnt].mtxScale._33;

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_aShock[nCnt].rot.y, g_aShock[nCnt].rot.x, g_aShock[nCnt].rot.z);
		D3DXMatrixMultiply(&g_aShock[nCnt].mtxWorld, &g_aShock[nCnt].mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			g_aShock[nCnt].pos.x, g_aShock[nCnt].pos.y, g_aShock[nCnt].pos.z);
		D3DXMatrixMultiply(&g_aShock[nCnt].mtxWorld, &g_aShock[nCnt].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aShock[nCnt].mtxWorld);

		if (g_aShock[nCnt].bUse == true)
		{
			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルのデータのポイントを取得
			pMat = (D3DXMATERIAL*)g_aShockX.pBuffMatShock->GetBufferPointer();

			pMat->MatD3D.Diffuse = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);
			pMat->MatD3D.Emissive = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);

			for (int nCntMat = 0; nCntMat < (int)g_aShockX.dwNumMatShock; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_aShockX.apTextureShock[nCntMat]);

				//モデル(パーツ)の描画
				g_aShockX.pMeshShock->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//===========================
//衝撃波の設定処理
//===========================
void SetShock(D3DXVECTOR3 pos, int life, float speed)
{
	for (int nCnt = 0; nCnt < MAX_SHOCK; nCnt++)
	{
		if (g_aShock[nCnt].bUse == false)
		{
			g_aShock[nCnt].pos = pos;
			g_aShock[nCnt].nLife = life;
			g_aShock[nCnt].fRadiusMax = g_aShockX.vtxMaxShock.x + 10.0f;
			g_aShock[nCnt].fRadiusMin = g_aShockX.vtxMaxShock.x - 10.0f;
			g_aShock[nCnt].fRadSpeed = speed;
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShock[nCnt].mtxWorld);
			g_aShock[nCnt].mtxScale = g_aShock[nCnt].mtxWorld;
			g_aShock[nCnt].bUse = true;

			break;
		}
	}
}

//===========================
//衝撃波の取得処理
//===========================
Shock *GetShock(void)
{
	return &g_aShock[0];
}

//===========================
//衝撃波の当たり判定処理
//===========================
bool CollisionShock(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot,bool bJump)
{
	bool bMeshWall;
	float fLength;
	float fLengthOld;

	bMeshWall = false;

	for (int nCnt = 0; nCnt < MAX_SHOCK; nCnt++)
	{
		if (g_aShock[nCnt].bUse == true)
		{
			//プレイヤーとエネミーの距離計算
			fLength = hypotf(pos->x - g_aShock[nCnt].pos.x,pos->z - g_aShock[nCnt].pos.z);

			fLengthOld = hypotf(posOld->x - g_aShock[nCnt].pos.x, posOld->z - g_aShock[nCnt].pos.z);

			if (fLength < g_aShock[nCnt].fRadiusMax && fLength > g_aShock[nCnt].fRadiusMin && pos->y <= g_aShock[nCnt].pos.y + g_aShockX.vtxMaxShock.y)
			{
				if (fabsf(fLength - g_aShock[nCnt].fRadiusMax) <= fabsf(fLength - g_aShock[nCnt].fRadiusMin))
				{
					rot->y = atan2f(pos->x - g_aShock[nCnt].pos.x, pos->z - g_aShock[nCnt].pos.z);
				}
				else if (fabsf(fLength - g_aShock[nCnt].fRadiusMax) > fabsf(fLength - g_aShock[nCnt].fRadiusMin))
				{
					rot->y = atan2f(g_aShock[nCnt].pos.x - pos->x, g_aShock[nCnt].pos.z - pos->z);
				}

				bMeshWall = true;
			}
			if (pos->y >= g_aShock[nCnt].pos.y + g_aShockX.vtxMaxShock.y
				&& fLength < g_aShock[nCnt].fRadiusMax
				&& fLengthOld > g_aShock[nCnt].fRadiusMaxOld)
			{
				if (bJump == true)
				{
					//スコアの増加
					AddScore(SHOCK_SCORE);

					//取得スコア表示設定
					SetPointScore(SHOCK_SCORE, D3DXVECTOR3(1000.0f, 400.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), POINTTYPE_SLIDE, 0.5f, LOGTYPE_DODGE_SHOCK);
				}

				if (GetMode() == MODE_TUTORIAL)
				{
					AddCount(1);
				}
			}
			if (pos->y >= g_aShock[nCnt].pos.y + g_aShockX.vtxMaxShock.y
				&& fLength > g_aShock[nCnt].fRadiusMin
				&& fLengthOld < g_aShock[nCnt].fRadiusMinOld)
			{//内側から外側
				if (bJump == true)
				{
					//スコアの増加
					AddScore(SHOCK_SCORE);

					//取得スコア表示設定
					SetPointScore(SHOCK_SCORE, D3DXVECTOR3(1000.0f, 400.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), POINTTYPE_SLIDE, 0.5f, LOGTYPE_DODGE_SHOCK);
				}
			}
		}
	}

	return bMeshWall;
}