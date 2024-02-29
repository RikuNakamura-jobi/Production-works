//===========================
//
//エネミー関連処理
//Author:中村　陸
//
//===========================

//==========================================
//  インクルード
//==========================================
#include "main.h"
#include "enemy.h"
#include "enemyAI.h"
#include "player.h"
#include "object.h"
#include "rock.h"
#include "shock.h"
#include <stdio.h>
#include "input.h"
#include "gauge.h"
#include "shadow.h"
#include "score.h"
#include "wall.h"
#include "orbit.h"
#include "particle.h"
#include "sound.h"
#include "game.h"
#include "tutorial.h"
#include "count.h"
#include "model.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_ENEMY (64)					//エネミーの最大数
#define ENEMY_INERTIA (0.15f)			//移動速度の慣性
#define ENEMY_GRAVITY (0.8f)			//次のポイントに切り替える時間
#define SHADOW_RADIUS	(120.0f)		//影の半径
#define ENEMY_COLLISION (100.0f)		//次のポイントに切り替える時間
#define JUMP_DAMAGE	(50)	//敵がバックジャンプする威力
#define TUTORIAL_HIT_COUNTER	(3)		//チュートリアル遷移するヒット数

//==========================================
//  プロトタイプ宣言
//==========================================
void MoveEnemy(void);
void ModelMotion(void);

//==========================================
//  グローバル変数宣言
//==========================================
Enemy g_enemy;					//エネミー構造体
int g_nTutorialHitCtr;			//チュートリアル時ヒット数

//===========================
//エネミーの初期化処理
//===========================
void InitEnemy(void)
{
	//変数初期化
	ZeroMemory(&g_enemy,sizeof(Enemy));

	//パーツ数取得
	g_enemy.nNumModel = GetNumEnemyPart();

	//ファイル読み込み
	ReadFileEnemy();

	//数値初期化
	g_enemy.nFindCount = 400;											//発見時間
	g_enemy.nLife = ENEMY_LIFE;													//体力
	g_enemy.fLength = 300.0f;
	g_enemy.fSpeed = ENEMY_SPEED;
	g_nTutorialHitCtr = 0;

	for (int nCntModel = 0; nCntModel < g_enemy.nNumModel; nCntModel++)
	{
		g_enemy.aEnemyX[nCntModel].pos = g_enemy.aEnemyX[nCntModel].posMotion;
		g_enemy.aEnemyX[nCntModel].rot = g_enemy.aEnemyX[nCntModel].rotMotion;
		g_enemy.aEnemyX[nCntModel].pos += g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel];
		g_enemy.aEnemyX[nCntModel].rot = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel];
	}

	InitEnemyAi();

	if (GetMode() == MODE_GAME)
	{
		// 体力ゲージの設定
		g_enemy.nIdxGauge = SetGauge(D3DXVECTOR3(80.0f, 680.0f, 0.0f), (float)g_enemy.nLife, 50.0f, 1100.0f, GAUGETYPE_HORIZON,ENEMY_LIFE_CR);
	}
	else
	{
		g_enemy.nIdxGauge = -1;
	}
}

//===========================
//エネミーの終了処理
//===========================
void UninitEnemy(void)
{
	for (int nCnt = 0; nCnt < g_enemy.nNumModel; nCnt++)
	{
		//頂点バッファの破棄
		if (g_enemy.aEnemyX[nCnt].pMeshEnemy != NULL)
		{
			g_enemy.aEnemyX[nCnt].pMeshEnemy->Release();
			g_enemy.aEnemyX[nCnt].pMeshEnemy = NULL;
		}

		//頂点バッファの破棄
		if (g_enemy.aEnemyX[nCnt].pBuffMatEnemy != NULL)
		{
			g_enemy.aEnemyX[nCnt].pBuffMatEnemy->Release();
			g_enemy.aEnemyX[nCnt].pBuffMatEnemy = NULL;
		}

		for (int nCntMat = 0;nCntMat < (int)g_enemy.aEnemyX[nCnt].dwNumMatEnemy;nCntMat++)
		{//テクスチャの破棄
			if (g_enemy.aEnemyX[nCnt].apTextureEnemy[nCntMat] != NULL)
			{
				g_enemy.aEnemyX[nCnt].apTextureEnemy[nCntMat]->Release();
				g_enemy.aEnemyX[nCnt].apTextureEnemy[nCntMat] = NULL;
			}
		}
	}
}

//===========================
//エネミーの更新処理
//===========================
void UpdateEnemy(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (g_enemy.bUse == true && g_enemy.bDeath == false)
	{
		g_enemy.motionStateOld = g_enemy.motionState;
		g_enemy.stateOld = g_enemy.state;
		g_enemy.posOld = g_enemy.pos;

		MoveEnemy();

		CollisionRockEnemy(&g_enemy.pos, &g_enemy.posOld, &g_enemy.pointMove, g_enemy.aEnemyX[0].vtxMinEnemy, g_enemy.aEnemyX[0].vtxMaxEnemy, false);

		CollisionWall(&g_enemy.pos, &g_enemy.posOld, &g_enemy.move, ENEMY_RADIUS);

		ModelMotion();

		if (g_enemy.nLife <= 0)
		{//敵の死亡
			g_enemy.nLife = 0;
		}

		if (GetMode() == MODE_GAME)
		{
			//体力ゲージ更新
			SetInfoGauge(g_enemy.nIdxGauge, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), (float)g_enemy.nLife);
		}

		//影の位置更新
		SetPositionShadow(g_enemy.nIdxShadow, D3DXVECTOR3(g_enemy.aEnemyX[0].mtxWorld._41, 0.2f, g_enemy.aEnemyX[0].mtxWorld._43),g_enemy.rot, 1.0f);

#ifdef _DEBUG
		if (GetKeyboardTrigger(DIK_0))
		{
			g_enemy.bUse = false;
		}

		if (GetKeyboardPress(DIK_B) == true)
		{//Wキーが押された時
			HitEnemy(1, 0);
		}

		if (GetKeyboardPress(DIK_LSHIFT) == true &&
			GetKeyboardTrigger(DIK_B) == true)
		{//Wキーが押された時
			HitEnemy(10000, 0);
		}

		if (GetKeyboardPress(DIK_LCONTROL) == true &&
			GetKeyboardPress(DIK_B) == true)
		{//Wキーが押された時
			HitEnemy(-20, 0);
		}

		if (GetKeyboardPress(DIK_LCONTROL) == true &&
			GetKeyboardPress(DIK_LSHIFT) == true &&
			GetKeyboardTrigger(DIK_N) == true)
		{//Wキーが押された時
			HitEnemy(60, 0);
		}

		if (GetKeyboardPress(DIK_LCONTROL) == true &&
			GetKeyboardPress(DIK_LSHIFT) == true && 
			GetKeyboardPress(DIK_X) == true)
		{//Wキーが押された時
			g_enemy.nAttackCount += 200;
		}
#endif
	}
}

//===========================
//エネミーの描画処理
//===========================
void DrawEnemy(void)
{
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (g_enemy.bUse == true)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_enemy.mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_enemy.rot.y, g_enemy.rot.x, g_enemy.rot.z);
		D3DXMatrixMultiply(&g_enemy.mtxWorld, &g_enemy.mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			g_enemy.pos.x, g_enemy.pos.y, g_enemy.pos.z);
		D3DXMatrixMultiply(&g_enemy.mtxWorld, &g_enemy.mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_enemy.mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//情報取得
		Model *pEnemyPart = GetModelEnemy();

		for (int nCntModel = 0; nCntModel < g_enemy.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransModel, mtxPalent;		//計算用マトリックス

			//パーツのワールドマトリックス初期化
			D3DXMatrixIdentity(&g_enemy.aEnemyX[nCntModel].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
				g_enemy.aEnemyX[nCntModel].rot.y, g_enemy.aEnemyX[nCntModel].rot.x, g_enemy.aEnemyX[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_enemy.aEnemyX[nCntModel].mtxWorld, &g_enemy.aEnemyX[nCntModel].mtxWorld, &mtxRotModel);

			//位置を反映
			D3DXMatrixTranslation(&mtxTransModel,
				g_enemy.aEnemyX[nCntModel].pos.x, g_enemy.aEnemyX[nCntModel].pos.y, g_enemy.aEnemyX[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_enemy.aEnemyX[nCntModel].mtxWorld, &g_enemy.aEnemyX[nCntModel].mtxWorld, &mtxTransModel);

			//パーツの親マトリックスを設定
			if (g_enemy.aEnemyX[nCntModel].nIdxModelParent != -1)
			{
				mtxPalent = g_enemy.aEnemyX[g_enemy.aEnemyX[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{
				mtxPalent = g_enemy.mtxWorld;
			}

			//算出したパーツのワールドマトリックスと親のマトリックスをかけ合わせる
			D3DXMatrixMultiply(&g_enemy.aEnemyX[nCntModel].mtxWorld,
				&g_enemy.aEnemyX[nCntModel].mtxWorld,
				&mtxPalent);

			pDevice->SetTransform(D3DTS_WORLD, &g_enemy.aEnemyX[nCntModel].mtxWorld);

			//マテリアルのデータのポイントを取得
			pMat = (D3DXMATERIAL*)pEnemyPart[nCntModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pEnemyPart[nCntModel].dwNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, pEnemyPart[nCntModel].apTexture[nCntMat]);

				//モデル(パーツ)の描画
				pEnemyPart[nCntModel].pMesh->DrawSubset(nCntMat);
			}
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//===========================
//エネミーのデバッグ表示
//===========================
void DrawEnemyDebug(void)
{
	//敵の位置
	DebugEnemyPos();

	//移動先
	DebugEnemyPoint();

	//外積
	DebugEnemyOuter();

	//発見判定
	DebugEnemyFind();

	//距離
	DebugEnemyLength();

	//角度
	DebugEnemyRot();

	//モーション
	DebugEnemyMotion();

	//状態
	DebugEnemyState();
}

//===========================
//エネミーの配置処理
//===========================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nType)
{
	if (g_enemy.bUse == false)
	{
		g_enemy.pos = pos;						//位置
		g_enemy.move = move;					//移動量
		g_enemy.rot = rot;						//角度
		if (GetMode() == MODE_TUTORIAL)
		{
			g_enemy.state = ENEMYSTATE_NORMAL;
			SetMotionEnemy(ENEMYMOTION_NORMAL);
			g_enemy.motionStateOld = ENEMYMOTION_NORMAL;
		}
		else
		{
			g_enemy.state = ENEMYSTATE_SLEEP;
			SetMotionEnemy(ENEMYMOTION_SLEEP);
			g_enemy.motionStateOld = ENEMYMOTION_SLEEP;
		}
		g_enemy.bDown = false;
		g_enemy.nIdxOrbit = -1;
		g_enemy.nPattern = rand() % g_enemy.nNumPattern;

		for (int nCntModel = 0; nCntModel < g_enemy.nNumModel; nCntModel++)
		{
			g_enemy.aEnemyX[nCntModel].pos = g_enemy.aEnemyX[nCntModel].posMotion + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel];
			g_enemy.aEnemyX[nCntModel].rot = g_enemy.aEnemyX[nCntModel].rotMotion;
			g_enemy.aEnemyX[nCntModel].rot = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel];
		}

		// 影の設定
		g_enemy.nIdxShadow = SetShadow(SHADOW_RADIUS, SHADOWTYPE_CIRCLE, 0.0f, 0.0f, g_enemy.rot);

		//使ってるかどうか
		g_enemy.bUse = true;
	}
}

//===========================
//エネミーの位置・方向調節処理
//===========================
void SetPosEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	g_enemy.pos = pos;
	g_enemy.rot = rot;
}

//===========================
//エネミーの情報取得処理
//===========================
Enemy *GetEnemy(void)
{
	return &g_enemy;
}

//===========================
//エネミーと敵の当たり判定処理
//===========================
bool CollisionEnemy(D3DXVECTOR3 *posMin, D3DXVECTOR3 *posMax)
{
	bool bCollision;

	bCollision = false;

	if (g_enemy.bUse == true)
	{
		if (g_enemy.fLength <= ENEMY_COLLISION && posMax->y >= g_enemy.pos.y && posMin->y <= g_enemy.aEnemyX[2].mtxWorld._42)
		{
			bCollision = true;
		}
	}

	return bCollision;
}

//===========================
//エネミーの攻撃当たり判定処理
//===========================
bool CollisionEnemyAttack(D3DXVECTOR3 *pos, D3DXVECTOR3 *rot)
{
	bool bAttack;
	float fLength1, fLength2;
	D3DXVECTOR3 posFocus1, posFocus2;
	D3DXMATRIX mtxFocus1, mtxFocus2;
	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	bAttack = false;
	
	posFocus1 = D3DXVECTOR3(0.0f, 0.0f, -100.0f);
	posFocus2 = D3DXVECTOR3(0.0f, 0.0f, -300.0f);

	//マトリックスの初期化
	D3DXMatrixIdentity(&mtxFocus1);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, posFocus1.x, posFocus1.y, posFocus1.z);
	D3DXMatrixMultiply(&mtxFocus1, &mtxFocus1, &mtxTrans);

	//マトリックスをかけ合わせる
	D3DXMatrixMultiply(&mtxFocus1, &mtxFocus1, &g_enemy.aEnemyX[13].mtxWorld);

	//マトリックスの初期化
	D3DXMatrixIdentity(&mtxFocus2);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, posFocus2.x, posFocus2.y, posFocus2.z);
	D3DXMatrixMultiply(&mtxFocus2, &mtxFocus2, &mtxTrans);

	//マトリックスをかけ合わせる
	D3DXMatrixMultiply(&mtxFocus2, &mtxFocus2, &g_enemy.aEnemyX[13].mtxWorld);

	posFocus1.x = mtxFocus1._41;
	posFocus1.y = mtxFocus1._42;
	posFocus1.z = mtxFocus1._43;
	posFocus2.x = mtxFocus2._41;
	posFocus2.y = mtxFocus2._42;
	posFocus2.z = mtxFocus2._43;

	fLength1 = hypotf(hypotf(pos->x - posFocus1.x, pos->y - posFocus1.y), pos->z - posFocus1.z);
	fLength2 = hypotf(hypotf(pos->x - posFocus2.x, pos->y - posFocus2.y), pos->z - posFocus2.z);

	if (fLength1 + fLength2 <= 250.0f && g_enemy.motionState == ENEMYMOTION_SHAKE && (g_enemy.nCntKey == 2 || g_enemy.nCntKey == 3))
	{
		rot->y = atan2f(pos->x - ((posFocus1.x + posFocus2.x) * 0.5f), pos->z - ((posFocus1.z + posFocus2.z) * 0.5f));

		bAttack = true;
	}

	return bAttack;
}

//===========================
//エネミーの行動範囲内外判定処理
//===========================
bool CollisionEnemyPoint(D3DXVECTOR3 *pos)
{
	Player *pPlayer = GetPlayer();
	D3DXVECTOR3 vecLine, vecToPos, vecMove;
	float fRate, fOutToPos, fOutUnit, fOutRate;
	bool bPoint;
	int nPoint = 0;

	bPoint = false;

	for (int nCntPoint = 0; nCntPoint < g_enemy.MovePattern[0].nNumPoint; nCntPoint++)
	{
		if (nCntPoint == g_enemy.MovePattern[0].nNumPoint - 1)
		{
			if (OuterProduct(g_enemy.MovePattern[0].point[nCntPoint], *pos, g_enemy.MovePattern[0].point[0]) < 0.0f)
			{
				//基準点とプレイヤーの位置のベクトル
				vecToPos.x = pPlayer->pos.x - g_enemy.MovePattern[0].point[nCntPoint].x;
				vecToPos.z = pPlayer->pos.z - g_enemy.MovePattern[0].point[nCntPoint].z;

				//板側面のベクトル
				vecLine.x = g_enemy.MovePattern[0].point[0].x - g_enemy.MovePattern[0].point[nCntPoint].x;
				vecLine.z = g_enemy.MovePattern[0].point[0].z - g_enemy.MovePattern[0].point[nCntPoint].z;

				//移動量ベクトル
				vecMove = pPlayer->pos - *pos;

				//側面とプレイヤーの位置の外積
				fOutToPos = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);

				//交点計算
				fOutUnit = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);
				fOutRate = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
				fRate = fOutRate / fOutUnit;

				if (fRate > 0.0f && fRate < 1.0f)
				{
					bPoint = true;
					break;
				}
			}
		}
		else
		{
			if (OuterProduct(g_enemy.MovePattern[0].point[nCntPoint], *pos, g_enemy.MovePattern[0].point[nCntPoint + 1]) < 0.0f)
			{
				//基準点とプレイヤーの位置のベクトル
				vecToPos.x = pPlayer->pos.x - g_enemy.MovePattern[0].point[nCntPoint].x;
				vecToPos.z = pPlayer->pos.z - g_enemy.MovePattern[0].point[nCntPoint].z;

				//板側面のベクトル
				vecLine.x = g_enemy.MovePattern[0].point[nCntPoint + 1].x - g_enemy.MovePattern[0].point[nCntPoint].x;
				vecLine.z = g_enemy.MovePattern[0].point[nCntPoint + 1].z - g_enemy.MovePattern[0].point[nCntPoint].z;

				//移動量ベクトル
				vecMove = pPlayer->pos - *pos;

				//側面とプレイヤーの位置の外積
				fOutToPos = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);

				//交点計算
				fOutUnit = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);
				fOutRate = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
				fRate = fOutRate / fOutUnit;

				if (fRate > 0.0f && fRate < 1.0f && fOutToPos > 0.0f)
				{
					bPoint = true;
					break;
				}
			}
		}
	}

	return bPoint;
}

//===========================
//エネミーの移動処理
//===========================
void MoveEnemy(void)
{
	//自分の向いてる向きに進む
	g_enemy.move.x += sinf(g_enemy.rot.y + D3DX_PI) * g_enemy.fSpeed;
	g_enemy.move.z += cosf(g_enemy.rot.y + D3DX_PI) * g_enemy.fSpeed;

	//重力
	g_enemy.move.y -= ENEMY_GRAVITY;

	//慣性
	g_enemy.move.x += (0.0f - g_enemy.move.x) * ENEMY_INERTIA;
	g_enemy.move.z += (0.0f - g_enemy.move.z) * ENEMY_INERTIA;

	if (GetMode() == MODE_TUTORIAL)
	{
		//AI
		AiEnemyTutorial();
	}
	else
	{
		//AI
		AiEnemy();
	}

	//移動量反映
	g_enemy.pos += g_enemy.move;

	if (g_enemy.pos.y < 0.0f)
	{
		g_enemy.move.y = 0.0f;
		g_enemy.pos.y = 0.0f;
	}

	//ブロックと当たり判定処理
	if (CollisionObject2(&g_enemy.pos, &g_enemy.posOld, &g_enemy.move, g_enemy.aEnemyX[0].vtxMinEnemy, g_enemy.aEnemyX[0].vtxMaxEnemy, false) == true)
	{
		
	}
	else
	{

	}

	//CollisionReflector(&g_enemy.pos, &g_enemy.posOld, &g_enemy.move, 100.0f, 0);
}

//===========================
//エネミーのモーション処理
//===========================
void ModelMotion(void)
{
	Player *pPlayer = GetPlayer();
	D3DXVECTOR3 posDiff, rotDiff;

	//キー数が指定の値より少ない
	if (g_enemy.nCntKey < g_enemy.aEnemyMotion[g_enemy.motionState].nNumKey)
	{
		//フレームが指定の値より少ない
		if (g_enemy.nCntFrame < g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame)
		{
			//キー数が最大より1少ない
			if (g_enemy.nCntKey == g_enemy.aEnemyMotion[g_enemy.motionState].nNumKey - 1)
			{
				//モデルごとにposとrotを足す
				for (int nCntModel = 0; nCntModel < g_enemy.nNumModel; nCntModel++)
				{
					if (g_enemy.aEnemyMotion[g_enemy.motionState].nLoop == 0)
					{
						posDiff.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].x - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].x;
						posDiff.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].y - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].y;
						posDiff.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].z - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].z;

						rotDiff.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].x - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].x;
						rotDiff.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].y - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].y;
						rotDiff.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].z - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].z;
						
						//posを足す
						g_enemy.aEnemyX[nCntModel].pos.x = g_enemy.aEnemyX[nCntModel].posMotion.x + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].x + (posDiff.x * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						g_enemy.aEnemyX[nCntModel].pos.y = g_enemy.aEnemyX[nCntModel].posMotion.y + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].y + (posDiff.y * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						g_enemy.aEnemyX[nCntModel].pos.z = g_enemy.aEnemyX[nCntModel].posMotion.z + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].z + (posDiff.z * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						
						//rotを足す
						g_enemy.aEnemyX[nCntModel].rot.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].x + (rotDiff.x * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						g_enemy.aEnemyX[nCntModel].rot.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].y + (rotDiff.y * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						g_enemy.aEnemyX[nCntModel].rot.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].z + (rotDiff.z * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
					}
					else
					{
						posDiff.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[0].pos[nCntModel].x - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].x;
						posDiff.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[0].pos[nCntModel].y - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].y;
						posDiff.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[0].pos[nCntModel].z - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].z;

						rotDiff.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[0].rot[nCntModel].x - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].x;
						rotDiff.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[0].rot[nCntModel].y - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].y;
						rotDiff.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[0].rot[nCntModel].z - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].z;

						//posを足す
						g_enemy.aEnemyX[nCntModel].pos.x = g_enemy.aEnemyX[nCntModel].posMotion.x + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].x + (posDiff.x * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						g_enemy.aEnemyX[nCntModel].pos.y = g_enemy.aEnemyX[nCntModel].posMotion.y + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].y + (posDiff.y * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						g_enemy.aEnemyX[nCntModel].pos.z = g_enemy.aEnemyX[nCntModel].posMotion.z + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].z + (posDiff.z * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));

						//rotを足す
						g_enemy.aEnemyX[nCntModel].rot.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].x + (rotDiff.x * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						g_enemy.aEnemyX[nCntModel].rot.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].y + (rotDiff.y * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						g_enemy.aEnemyX[nCntModel].rot.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].z + (rotDiff.z * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
					}

					//rotの修正
					if (g_enemy.aEnemyX[nCntModel].rot.x > 3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.x -= 6.28f;
					}
					else if (g_enemy.aEnemyX[nCntModel].rot.x < -3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.x += 6.28f;
					}

					if (g_enemy.aEnemyX[nCntModel].rot.y > 3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.y -= 6.28f;
					}
					else if (g_enemy.aEnemyX[nCntModel].rot.y < -3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.y += 6.28f;
					}

					if (g_enemy.aEnemyX[nCntModel].rot.z > 3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.z -= 6.28f;
					}
					else if (g_enemy.aEnemyX[nCntModel].rot.z < -3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.z += 6.28f;
					}
				}
			}
			else
			{
				//モデルごとにposとrotを足す
				for (int nCntModel = 0; nCntModel < g_enemy.nNumModel; nCntModel++)
				{
					posDiff.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey + 1].pos[nCntModel].x - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].x;
					posDiff.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey + 1].pos[nCntModel].y - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].y;
					posDiff.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey + 1].pos[nCntModel].z - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].z;

					rotDiff.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey + 1].rot[nCntModel].x - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].x;
					rotDiff.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey + 1].rot[nCntModel].y - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].y;
					rotDiff.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey + 1].rot[nCntModel].z - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].z;

					//posを足す
					g_enemy.aEnemyX[nCntModel].pos.x = g_enemy.aEnemyX[nCntModel].posMotion.x + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].x + posDiff.x * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame);
					g_enemy.aEnemyX[nCntModel].pos.y = g_enemy.aEnemyX[nCntModel].posMotion.y + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].y + posDiff.y * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame);
					g_enemy.aEnemyX[nCntModel].pos.z = g_enemy.aEnemyX[nCntModel].posMotion.z + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].z + posDiff.z * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame);

					//rotを足す
					g_enemy.aEnemyX[nCntModel].rot.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].x + (rotDiff.x * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
					g_enemy.aEnemyX[nCntModel].rot.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].y + (rotDiff.y * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
					g_enemy.aEnemyX[nCntModel].rot.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].z + (rotDiff.z * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));

					//rotの修正
					if (g_enemy.aEnemyX[nCntModel].rot.x > 3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.x -= 6.28f;
					}
					else if (g_enemy.aEnemyX[nCntModel].rot.x < -3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.x += 6.28f;
					}

					if (g_enemy.aEnemyX[nCntModel].rot.y > 3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.y -= 6.28f;
					}
					else if (g_enemy.aEnemyX[nCntModel].rot.y < -3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.y += 6.28f;
					}

					if (g_enemy.aEnemyX[nCntModel].rot.z > 3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.z -= 6.28f;
					}
					else if (g_enemy.aEnemyX[nCntModel].rot.z < -3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.z += 6.28f;
					}
				}
			}

			//フレームを増やす
			g_enemy.nCntFrame++;
		}
		else
		{
			//キーを増やす
			g_enemy.nCntKey++;

			//フレームをリセットする
			g_enemy.nCntFrame = 0;

			//rotを修正
			for (int nCntModel = 0; nCntModel < g_enemy.nNumModel; nCntModel++)
			{
				g_enemy.aEnemyX[nCntModel].rotMotion = g_enemy.aEnemyX[nCntModel].rot;
			}
		}
	}
	else
	{
		//ループしてるかどうかの判定
		if (g_enemy.aEnemyMotion[g_enemy.motionState].nLoop == 0)
		{
			g_enemy.nAttackCount = 0;

			//次のモーション決め
			if (g_enemy.state == ENEMYSTATE_TRACK)
			{
				if (CollisionEnemyPoint(&g_enemy.pos) == true)
				{
					SetMotionEnemy(ENEMYMOTION_NORMAL);
				}
				else
				{
					SetMotionEnemy(ENEMYMOTION_MOVE);
				}

				if (g_enemy.fLength <= 100.0f)
				{
					SetMotionEnemy(ENEMYMOTION_NORMAL);
				}
			}
			else if (g_enemy.state == ENEMYSTATE_DAMAGE)
			{
				if (g_enemy.motionState == ENEMYMOTION_DAMAGE)
				{
					SetMotionEnemy(ENEMYMOTION_NORMAL);
					g_enemy.state = ENEMYSTATE_COUNTER;
				}
				else if (g_enemy.motionState == ENEMYMOTION_DOWN)
				{
					SetMotionEnemy(ENEMYMOTION_OVERLOOK);
					g_enemy.state = ENEMYSTATE_SEARCH;
				}
			}
			else if (g_enemy.state == ENEMYSTATE_COUNTER)
			{
				if (g_enemy.motionState == ENEMYMOTION_BACKJUMP)
				{
					if (g_enemy.bFind == true)
					{
						g_enemy.state = ENEMYSTATE_TRACK;
						SetMotionEnemy(ENEMYMOTION_NORMAL);
					}
					else
					{
						SetMotionEnemy(ENEMYMOTION_THROW);
					}
				}
				else
				{
					g_enemy.state = ENEMYSTATE_NORMAL;
					SetMotionEnemy(ENEMYMOTION_NORMAL);
				}
			}
			else if (g_enemy.state == ENEMYSTATE_FIND)
			{
				g_enemy.state = ENEMYSTATE_TRACK;
				SetMotionEnemy(ENEMYMOTION_NORMAL);
			}
			else if(g_enemy.state == ENEMYSTATE_DEATH)
			{
				g_enemy.bDeath = true;					//使用していない状態にする
			}
			else if (g_enemy.state == ENEMYSTATE_STANDUP)
			{
				g_enemy.state = ENEMYSTATE_SEARCH;
				SetMotionEnemy(ENEMYMOTION_OVERLOOK);
			}
			else
			{
				SetMotionEnemy(ENEMYMOTION_NORMAL);
			}
		}

		//キーとフレームをリセット
		g_enemy.nCntKey = 0;
		g_enemy.nCntFrame = 0;
	}

	//モーションが切り替わったことの判定
	if (g_enemy.motionState != g_enemy.motionStateOld)
	{
		g_enemy.nCntKey = 0;
		g_enemy.nCntFrame = 0;

		for (int nCntModel = 0; nCntModel < g_enemy.nNumModel; nCntModel++)
		{
			g_enemy.aEnemyX[nCntModel].pos = g_enemy.aEnemyX[nCntModel].posMotion + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel];
			g_enemy.aEnemyX[nCntModel].rot = g_enemy.aEnemyX[nCntModel].rotMotion;
			g_enemy.aEnemyX[nCntModel].rot = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel];
		}
	}
}

//========================================
//エネミーの位置
//========================================
void DebugEnemyPos(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,400,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "エネミーの位置; %0.3f %0.3f %0.3f\nエネミーの移動量; %0.3f %0.3f %0.3f", g_enemy.pos.x, g_enemy.pos.y, g_enemy.pos.z, g_enemy.move.x, g_enemy.move.y, g_enemy.move.z);
	
	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//========================================
//エネミーの移動ポイント
//========================================
void DebugEnemyPoint(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,440,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "移動先; %d\n移動時間; %d", g_enemy.MovePattern[g_enemy.nPattern].posCount, g_enemy.nTimerMove);

	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//========================================
//エネミーの外積
//========================================
void DebugEnemyOuter(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,480,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "外積;L %0.3f | R %0.3f", g_enemy.vecL, g_enemy.vecR);

	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//========================================
//エネミーの発見判定
//========================================
void DebugEnemyFind(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,500,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "発見; %d(0:範囲外 | 1:範囲内)\n発見時間; %d", g_enemy.bFind, g_enemy.nFindCount);

	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//========================================
//エネミーの距離
//========================================
void DebugEnemyLength(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,540,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "距離; %0.3f", g_enemy.fLength);

	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//========================================
//エネミーの角度
//========================================
void DebugEnemyRot(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,560,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "角度; %0.3f %0.3f %0.3f", g_enemy.rot.x, g_enemy.rot.y, g_enemy.rot.z);

	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//========================================
//エネミーのモーション
//========================================
void DebugEnemyMotion(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,620,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "今のモーション(エネミー); %d\n前のモーション(エネミー); %d", g_enemy.motionState, g_enemy.motionStateOld);

	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//========================================
//エネミーの状態
//========================================
void DebugEnemyState(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,660,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "今の状態; %d\nキー数,フレーム数; %d | %d", g_enemy.state, g_enemy.nCntKey, g_enemy.nCntFrame);

	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//========================================
//外積計算用関数(点Aから伸びる2つのベクトル(ABとAC)の外積)
//点Bが直線ACの左にいるときはプラス、点Bが直線ACの右にいるときはマイナスの値を返し直線上にいる場合0を返す
//========================================
float OuterProduct(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 posC)
{
	D3DXVECTOR3 vecLine1, vecLine2;
	float vecAnswer;

	//点ABのベクトル
	vecLine1.x = posB.x - posA.x;
	vecLine1.z = posB.z - posA.z;

	//点ACのベクトル
	vecLine2.x = posC.x - posA.x;
	vecLine2.z = posC.z - posA.z;

	//計算したベクトル
	vecAnswer = (vecLine1.z * vecLine2.x) - (vecLine1.x * vecLine2.z);

	return vecAnswer;
}

//========================================
//エネミーのヒット処理
//========================================
void HitEnemy(int nDamage, int nCntEnemy)
{
	//チュートリアルを進める
	TUTORIALSTATE *tutorial = GetTutorialState();
	if (*tutorial == TUTORIALSTATE_ENEMY && GetMode() == MODE_TUTORIAL)
	{
		AddCount(1);

		if (g_nTutorialHitCtr < TUTORIAL_HIT_COUNTER)
		{
			g_nTutorialHitCtr++;
		}
		else
		{
			*tutorial = TUTORIALSTATE_END;

			PlaySound(SOUND_LABEL_SE_BUTTON_004);
		}
	}

	if (g_enemy.state != ENEMYSTATE_DAMAGE)
	{
		//体力を減らす
		g_enemy.nLife -= nDamage;

		if (g_enemy.state == ENEMYSTATE_SLEEP)
		{
			g_enemy.nHitRock++;
		}
	}

	//敵の体力がなくなった
	if (g_enemy.nLife <= 0 && g_enemy.state != ENEMYSTATE_DEATH)
	{

		if (GetMode() == MODE_TUTORIAL)
		{
			// 体力を1で固定
			g_enemy.nLife = 1;
		}
		else
		{
			g_enemy.state = ENEMYSTATE_DEATH;

			if (g_enemy.nIdxOrbit != -1)
			{
				//軌跡の削除
				EnableOrbit(g_enemy.nIdxOrbit);
				g_enemy.nIdxOrbit = -1;
			}

			//キーとフレームをリセット
			g_enemy.nCntKey = 0;
			g_enemy.nCntFrame = 0;

			// 体力を０で固定
			g_enemy.nLife = 0;
		}
	}
	
	if(g_enemy.state != ENEMYSTATE_DEATH)
	{
		if (nDamage > JUMP_DAMAGE)
		{//怯むダメージより大きかったら
			if (g_enemy.motionState != ENEMYMOTION_BACKJUMP && g_enemy.motionState != ENEMYMOTION_JUMPATTACK && g_enemy.motionState != ENEMYMOTION_JUMPATTACK_LIGHT)
			{
				if (g_enemy.state == ENEMYSTATE_SLEEP)
				{
					g_enemy.nHitRock = 5;
				}
				else
				{
					g_enemy.state = ENEMYSTATE_DAMAGE;

					//ダメージモーションへ移行
					g_enemy.motionState = ENEMYMOTION_DAMAGE;

					if (g_enemy.nLife <= (int)(ENEMY_LIFE * 0.25f) && g_enemy.bDown == false)
					{//ダウンモーションへ移行
						g_enemy.motionState = ENEMYMOTION_DOWN;
						g_enemy.bDown = true;
					}
				}
				
				if (g_enemy.nIdxOrbit != -1)
				{
					//軌跡の削除
					EnableOrbit(g_enemy.nIdxOrbit);
					g_enemy.nIdxOrbit = -1;
				}

				//キーとフレームをリセット
				g_enemy.nCntKey = 0;
				g_enemy.nCntFrame = 0;

				//移動パターンの変更
				g_enemy.nPattern = rand() % g_enemy.nNumPattern;
			}
		}
	}
}

//===========================
//エネミーのファイル読み込み処理
//===========================
void ReadFileEnemy(void)
{
	char aText[128];
	FILE *pFile = fopen("data\\MOTION\\motion_golem.txt", "r");

	//ファイル読み込み
	if (pFile == NULL)
	{
		return;
	}

	do
	{
		fscanf(pFile, "%s", &aText[0]);
	} while (strcmp(aText, "CHARACTERSET") != 0);

	//モデル数
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %d	%s %s\n", &aText[0], &g_enemy.nNumModel, &aText[0], &aText[0]);

	//モデル配置
	for (int nCntModel = 0; nCntModel < g_enemy.nNumModel; nCntModel++)
	{
		fgets(&aText[0], 128, pFile);
		fscanf(pFile, "%s = %d", &aText[0], &g_enemy.aEnemyX[nCntModel].nIdxModelParent);
		fscanf(pFile, "%s = %d	%s %s", &aText[0], &g_enemy.aEnemyX[nCntModel].nIdxModelParent, &aText[0], &aText[0]);
		fscanf(pFile, "%s = %f %f %f", &aText[0], &g_enemy.aEnemyX[nCntModel].posMotion.x, &g_enemy.aEnemyX[nCntModel].posMotion.y, &g_enemy.aEnemyX[nCntModel].posMotion.z);
		fscanf(pFile, "%s = %f %f %f\n", &aText[0], &g_enemy.aEnemyX[nCntModel].rotMotion.x, &g_enemy.aEnemyX[nCntModel].rotMotion.y, &g_enemy.aEnemyX[nCntModel].rotMotion.z);
		fgets(&aText[0], 128, pFile);
		fgets(&aText[0], 128, pFile);
	}

	//いらない部分読み込み
	for (int nCnt = 0; nCnt < 7; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	for (int nCntMotion = 0; nCntMotion < ENEMYMOTION_MAX; nCntMotion++)
	{
		//モーション識別
		fgets(&aText[0], 128, pFile);
		fscanf(pFile, "%s = %d	%s %s %s %s", &aText[0], &g_enemy.aEnemyMotion[nCntMotion].nLoop, &aText[0], &aText[0], &aText[0], &aText[0]);
		fscanf(pFile, "%s = %d	%s %s\n", &aText[0], &g_enemy.aEnemyMotion[nCntMotion].nNumKey, &aText[0], &aText[0]);
		
		//キーモーション
		for (int nCntKey = 0; nCntKey < g_enemy.aEnemyMotion[nCntMotion].nNumKey; nCntKey++)
		{
			fgets(&aText[0], 128, pFile);
			fscanf(pFile, "%s = %d\n", &aText[0], &g_enemy.aEnemyMotion[nCntMotion].aKeyMotion[nCntKey].nFrame);

			for (int nCnt = 0; nCnt < g_enemy.nNumModel; nCnt++)
			{
				fgets(&aText[0], 128, pFile);
				fscanf(pFile, "%s = %f %f %f", &aText[0], &g_enemy.aEnemyMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].x, &g_enemy.aEnemyMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].y, &g_enemy.aEnemyMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].z);
				fscanf(pFile, "%s = %f %f %f\n", &aText[0], &g_enemy.aEnemyMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].x, &g_enemy.aEnemyMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].y, &g_enemy.aEnemyMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].z);
				fgets(&aText[0], 128, pFile);
				fgets(&aText[0], 128, pFile);
			}

			fgets(&aText[0], 128, pFile);
		}

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			fgets(&aText[0], 128, pFile);
		}
	}

	fclose(pFile);

	pFile = fopen("data\\SAVE\\ENEMY\\enemy.txt", "r");		//ファイルオープン

	//ファイル読み込み
	if (pFile == NULL)
	{
		return;
	}

	//いらない部分読み込み
	for (int nCnt = 0; nCnt < 16; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	//移動地点の数
	fscanf(pFile, "%s = %d		%s %s\n", &aText[0], &g_enemy.nNumPattern, &aText[0], &aText[0]);

	//移動地点の座標
	for (int nCntPattern = 0; nCntPattern < g_enemy.nNumPattern; nCntPattern++)
	{
		//いらない部分読み込み
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			fgets(&aText[0], 128, pFile);
		}

		fscanf(pFile, "%s = %d		%s %s\n", &aText[0], &g_enemy.MovePattern[nCntPattern].nNumPoint, &aText[0], &aText[0]);

		for (int nCnt = 0; nCnt < g_enemy.MovePattern[nCntPattern].nNumPoint; nCnt++)
		{
			fscanf(pFile, "%s = %f %f %f		%s %s\n", &aText[0], &g_enemy.MovePattern[nCntPattern].point[nCnt].x, &g_enemy.MovePattern[nCntPattern].point[nCnt].y, &g_enemy.MovePattern[nCntPattern].point[nCnt].z, &aText[0], &aText[0]);
		}
	}

	//ファイルクローズ
	fclose(pFile);
}

//==========================================
//  エネミーライフの取得
//==========================================
bool GetEnemyUse()
{
	return g_enemy.bUse;
}