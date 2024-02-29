//========================================
//
//3Dモデルの壁ポリゴン処理(wall.cpp)
// Author: Kai Takada
//
//========================================
#include "stdio.h"
#include "input.h"
#include "main.h"
#include "wall.h"
#include "edit.h"
#include "enemy.h"

//=============================
//マクロ定義
//=============================
#define WALL_SIZEX (400.0f)	//サンプルサイズ
#define WALL_SIZEY (100.0f)	//サンプルサイズ
#define WALL_TEX_X (300.0f)	//テクスチャの長さx
#define WALL_TEX_Y (300.0f)	//テクスチャの長さy

//=============================
//プロトタイプ宣言
//=============================
void SetVtxpos(int nNum);

//=============================
//グローバル変数宣言
//=============================
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;		//テクスチャへのポインタ 
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	//頂点バッファへのポインタ
Wall g_aWall[MAX_WALL];

//==============================
//壁の初期化処理
//==============================
void InitWall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\wall000.jpg",				//テクスチャのファイル名
		&g_pTextureWall);

	for (int nCntInit = 0;nCntInit < MAX_WALL;nCntInit++)
	{//初期化する
		g_aWall[nCntInit].pos = D3DXVECTOR3(0.0f, 0.0f, WALL_SIZEX);
		g_aWall[nCntInit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntInit].fHeight = WALL_SIZEY;
		g_aWall[nCntInit].fWidth = WALL_SIZEX;
		g_aWall[nCntInit].fDivX = WALL_SIZEX / WALL_TEX_X;
		g_aWall[nCntInit].fDivY = WALL_SIZEY / WALL_TEX_Y;
		g_aWall[nCntInit].aVtxpos[0] = {};
		g_aWall[nCntInit].aVtxpos[1] = {};
		g_aWall[nCntInit].aVtxpos[2] = {};
		g_aWall[nCntInit].aVtxpos[3] = {};
		g_aWall[nCntInit].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0,0,(void**)&pVtx,0);

	for (int nCntWall = 0;nCntWall < MAX_WALL;nCntWall++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-WALL_SIZEX, WALL_SIZEY * 2, 0.0f);		//(x,y,z)
		pVtx[1].pos = D3DXVECTOR3(WALL_SIZEX, WALL_SIZEY * 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-WALL_SIZEX, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(WALL_SIZEX, 0.0f, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	//(x,y,z)
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				//(x,y)
		pVtx[1].tex = D3DXVECTOR2(g_aWall[nCntWall].fDivX, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aWall[nCntWall].fDivY);
		pVtx[3].tex = D3DXVECTOR2(g_aWall[nCntWall].fDivX, g_aWall[nCntWall].fDivY);

		if (nCntWall >= 4)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.6f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.6f, 1.0f);
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}

//==============================
//壁の終了処理
//==============================
void UninitWall(void)
{
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;					//テクスチャ破棄
	}
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;					//バッファの破棄
	}
}

//==============================
//壁の更新処理
//==============================
void UpdateWall(void)
{

}

//==============================
//壁の描画処理
//==============================
void DrawWall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//ワイヤーフレーム
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	for (int nCntDraw = 0; nCntDraw < MAX_WALL; nCntDraw++)
	{
		if (g_aWall[nCntDraw].bUse == true)
		{
			D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aWall[nCntDraw].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntDraw].rot.y, g_aWall[nCntDraw].rot.x, g_aWall[nCntDraw].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCntDraw].mtxWorld, &g_aWall[nCntDraw].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntDraw].pos.x, g_aWall[nCntDraw].pos.y, g_aWall[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCntDraw].mtxWorld, &g_aWall[nCntDraw].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntDraw].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(nCntDraw, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(nCntDraw, g_pTextureWall);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
				nCntDraw * 4,									//描画する最初のインデックス
				2);												//描画するプリミティブ数
		}
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

//==============================
//壁の設置処理
//==============================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot,float fWidth, float fHeight)
{
	for (int nCntSet = 0; nCntSet < MAX_WALL; nCntSet++)
	{
		if (g_aWall[nCntSet].bUse == false)
		{
			g_aWall[nCntSet].pos = pos;
			g_aWall[nCntSet].rot = rot;
			g_aWall[nCntSet].fHeight = fHeight;
			g_aWall[nCntSet].fWidth = fWidth;
			g_aWall[nCntSet].fDivX = fWidth / WALL_TEX_X;
			g_aWall[nCntSet].fDivY = fHeight / WALL_TEX_Y;
			g_aWall[nCntSet].bUse = true;

			SetEditIndex(nCntSet);
			//SetVtxpos(nCntSet);

			VERTEX_3D *pVtx;		//頂点情報のポインタ

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += nCntSet * 4;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-fWidth, fHeight * 2, 0.0f);		//(x,y,z)
			pVtx[1].pos = D3DXVECTOR3(fWidth, fHeight * 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-fWidth, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fWidth, 0.0f, 0.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				//(x,y)
			pVtx[1].tex = D3DXVECTOR2(g_aWall[nCntSet].fDivX, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aWall[nCntSet].fDivY);
			pVtx[3].tex = D3DXVECTOR2(g_aWall[nCntSet].fDivX, g_aWall[nCntSet].fDivY);

			//頂点バッファをアンロックする
			g_pVtxBuffWall->Unlock();
			break;
		}
	}
}

//==============================
//壁の削除処理
//==============================
void DeleteWall(int nIdx)
{
	g_aWall[nIdx].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aWall[nIdx].bUse = false;
}

//==============================
//壁の情報取得処理
//==============================
Wall *GetWall(void)
{
	return &g_aWall[0];
}

//==============================
//壁の使用情報取得処理
//==============================
int GetUseWall(void)
{
	int nNumUse = 0;

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (g_aWall[nCnt].bUse == true)
		{
			nNumUse++;
		}
	}

	return nNumUse;
}

//==============================
//四つ角の座標取得
//==============================
void SetVtxpos(int nNum)
{
	D3DXVECTOR3 math;

	//頂点を角度に合わせる
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		//math.x = cosf(g_aWall[nNum].rot.y) * g_aWall[nNum].fWidth + g_aWall[nNum].pos.x;
		nCntVtx < 2 ? math.y = +g_aWall[nNum].pos.x + g_aWall[nNum].fHeight : math.y = g_aWall[nNum].pos.y;
		//math.z = sinf(g_aWall[nNum].rot.y) * g_aWall[nNum].fWidth + g_aWall[nNum].pos.z;
	
		if (nCntVtx % 2 == 0)
		{
			math.x = (cosf(g_aWall[nNum].rot.y) * -g_aWall[nNum].fWidth) + g_aWall[nNum].pos.x;
			math.z = (sinf(g_aWall[nNum].rot.y) * -g_aWall[nNum].fWidth) + g_aWall[nNum].pos.z;
		}
		else
		{
			math.x = (cosf(g_aWall[nNum].rot.y) * g_aWall[nNum].fWidth) + g_aWall[nNum].pos.x;
			math.z = (sinf(g_aWall[nNum].rot.y) * g_aWall[nNum].fWidth) + g_aWall[nNum].pos.z;
		}

		//(nCntVtx % 2) == 1 ? math.x * -1 : math.x * 1;
		//(nCntVtx % 2) == 1 ? math.z * -1 : math.z * 1;
		g_aWall[nNum].aVtxpos[nCntVtx] = math;
	}
}

//==============================
//壁の当たり判定
//==============================
bool CollisionWall(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, float fLength)
{
	bool bReflector;
	float fRate, fOutToPos, fOutToPosOld, fOutUnit, fOutRate, fRef, fDot, fRotRef;
	D3DXVECTOR3 vecLine, vecToPos, vecToPosOld, vecMove, vecNor, vecDot, vecMoveRef, posA, posB, posRef, posRefOld;

	bReflector = false;

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (g_aWall[nCnt].bUse == true)
		{
			posA.x = g_aWall[nCnt].pos.x - (cosf(g_aWall[nCnt].rot.y) * g_aWall[nCnt].fWidth);
			posA.y = 0.0f;
			posA.z = g_aWall[nCnt].pos.z + (sinf(g_aWall[nCnt].rot.y) * g_aWall[nCnt].fWidth);

			posB.x = g_aWall[nCnt].pos.x + (cosf(g_aWall[nCnt].rot.y) * g_aWall[nCnt].fWidth);
			posB.y = 0.0f;
			posB.z = g_aWall[nCnt].pos.z - (sinf(g_aWall[nCnt].rot.y) * g_aWall[nCnt].fWidth);

			//プレイヤーの角度
			fRef = atan2f(posA.x - posB.x, posA.z - posB.z) + (D3DX_PI * 0.5f);

			posRef.x = pos->x + (fLength * sinf(fRef));
			posRef.z = pos->z + (fLength * cosf(fRef));

			posRefOld.x = posOld->x + (fLength * sinf(fRef));
			posRefOld.z = posOld->z + (fLength * cosf(fRef));

			//基準点とプレイヤーの位置のベクトル
			vecToPos.x = posRef.x - posB.x;
			vecToPos.z = posRef.z - posB.z;

			//基準点とプレイヤーの前の位置のベクトル
			vecToPosOld.x = posRefOld.x - posB.x;
			vecToPosOld.z = posRefOld.z - posB.z;

			//板側面のベクトル
			vecLine.x = posA.x - posB.x;
			vecLine.y = 0.0f;
			vecLine.z = posA.z - posB.z;

			//移動量ベクトル
			vecMove = posRef - posRefOld;

			//側面とプレイヤーの位置の外積
			fOutToPos = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
			fOutToPosOld = (vecLine.z * vecToPosOld.x) - (vecLine.x * vecToPosOld.z);

			//交点計算
			fOutUnit = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);
			fOutRate = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
			fRate = fOutRate / fOutUnit;

			//側面衝突判定
			if (fOutToPos > 0.0f && fOutToPosOld < 0.0f && fRate < 1.0f && fRate > 0.0f)
			{
				if (pos->y < g_aWall[nCnt].pos.y + 4000.0f && pos->y > g_aWall[nCnt].pos.y - 4000.0f)
				{
					//当たったのでbool型をtrueにする
					bReflector = true;

					//法線ベクトル計算
					vecNor.x = -vecLine.z;
					vecNor.y = 0.0f;
					vecNor.z = vecLine.x;

					//法線ベクトル単位化
					D3DXVec3Normalize(&vecNor, &vecNor);

					float fMoveSize;
					D3DXVECTOR3 point, pointJud, vecRem;

					point.x = posB.x + (vecLine.x * fRate);
					point.z = posB.z + (vecLine.z * fRate);
					pointJud.x = point.x + vecNor.x;
					pointJud.y = 0.0f;
					pointJud.z = point.z + vecNor.z;
					fMoveSize = hypotf(posRef.x - point.x, posRef.z - point.z);

					if (OuterProduct(pointJud, posRefOld, posRef) > 0.0f)
					{
						vecRem.x = vecNor.z;
						vecRem.y = 0.0f;
						vecRem.z = -vecNor.x;
					}
					else
					{
						vecRem.x = -vecNor.z;
						vecRem.y = 0.0f;
						vecRem.z = vecNor.x;
					}

					//法線ベクトル単位化
					D3DXVec3Normalize(&vecRem, &vecRem);

					//水平方向大きさ
					fDot = (-vecMove.x * vecNor.x) + (-vecMove.z * vecNor.z);

					//水平方向ベクトル計算
					vecDot = (vecNor * fDot) * 2;

					//移動量ベクトル計算
					vecMoveRef = vecMove + vecDot;

					fRotRef = atan2f(vecRem.x, vecRem.z) - atan2f(vecMoveRef.x, vecMoveRef.z);

					vecMoveRef.x = (-sinf(fRotRef) * vecMoveRef.z) + (cosf(fRotRef) * vecMoveRef.x);
					vecMoveRef.z = (sinf(fRotRef) * vecMoveRef.x) + (cosf(fRotRef) * vecMoveRef.z);

					//移動量ベクトル単位化(必要か分からない)
					D3DXVec3Normalize(&vecMoveRef, &vecMoveRef);

					//プレイヤーの位置移動
					pos->x = posB.x + (vecLine.x * fRate) + vecNor.x + (fLength * sinf(atan2f(vecNor.x, vecNor.z)));
					pos->z = posB.z + (vecLine.z * fRate) + vecNor.z + (fLength * cosf(atan2f(vecNor.x, vecNor.z)));

					/*move->x = vecMoveRef.x * move->x;
					move->z = vecMoveRef.z * move->z;*/
				}
			}
		}
	}

	return bReflector;
}

//========================================
//ランキングでの壁の更新
//========================================
void UpdateWallRanking(float moveX)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{
			g_aWall[nCntWall].pos.x += moveX;

			if (g_aWall[nCntWall].pos.x <= -1000.0f)
			{
				g_aWall[nCntWall].pos.x = 2000.0f;
			}
		}
	}
}
