//===========================
//
//ダイレクトX.playerファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "player.h"
#include "input.h"
#include "camera.h"
#include "object.h"
#include "rock.h"
#include "shock.h"
#include <stdio.h>
#include "meshfield.h"
#include "gauge.h"
#include "score.h"
#include "shadow.h"
#include "enemy.h"
#include "tutorial.h"
#include "orbit.h"
#include "particle.h"
#include "wall.h"
#include "orbit.h"
#include "billboard.h"
#include "sound.h"
#include "game.h"
#include "point.h"
#include "point_log.h"
#include "time.h"
#include "model.h"
#include "fade.h"
#include "book.h"

//================
//マクロ定義
//================
#define LENGTH				(0.3f)							//長さ
#define LIFE				(20)							//体力
#define PLAYER_JUMP_POW		(10.0f)							//初期のジャンプ力
#define PLAYER_GRAVITY		(0.49f)							//重力
#define SHOCK_SCORE			(-100)							//衝撃波に当たった時のスコア
#define GAUGE_COLOR_FRAME	(30)							//HPゲージの色変え速度
#define PLAYER_GROUND_LENG	(1.5f)							//距離で影のサイズ補正

#define CAMERA_FACT			(0.03f)							//カメラの補正係数
#define ROTATE_FACT			(0.1f)							//プレイヤーの回転係数

#define SHADOW_RADIUS		(30.0f)							//影の半径

//================
//プロトタイプ宣言
//================
void SetRot(void);
void SetMotion(void);
void SetMotionRanking(void);
void MovePlayer(void);
void ConPlayer(void);
void ConPlayerPad(void);
void ModelMotionPlayer(void);
void LimitMovePlayer(void);

//================
//グローバル宣言
//================
Player g_Player;
char g_aModelName[32][128];
int g_nCntFramePlayer;
int g_nCntKeyPlayer;
int g_nCheckPlayer;

float g_fRBJustGaugePlayer;	//HPゲージのr,b値調整
int g_fRBJustFramePlayer;	//HPゲージのr,b値調整フレーム数

//===========================
//プレイヤーの初期化処理
//===========================
void InitPlayer(void)
{
	//各種変数の初期化
	ZeroMemory(&g_Player,sizeof(Player));

	//パーツ数取得
	g_Player.nNumModel = GetNumPlayerPart();

	//各種変数の初期化
	SetPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	g_nCheckPlayer = 0;
	g_fRBJustGaugePlayer = 0.0f;
	g_fRBJustFramePlayer = 0;

	//プレイヤーモデル読み込み
	ReadFilePlayer();

	//モデル情報取得
	int nNumVtx = 0;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL *pMat;

	//情報取得
	Model *pPlayerPart = GetModelPlayer();

	for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
	{
		if (pPlayerPart[nCntModel].vtxMax.x >= g_Player.vtxMax.x)
		{
			g_Player.vtxMax.x = pPlayerPart[nCntModel].vtxMax.x;
		}
		if (pPlayerPart[nCntModel].vtxMin.x < g_Player.vtxMin.x)
		{
			g_Player.vtxMin.x = pPlayerPart[nCntModel].vtxMin.x;
		}

		if (pPlayerPart[nCntModel].vtxMax.y >= g_Player.vtxMax.y)
		{
			g_Player.vtxMax.y = pPlayerPart[nCntModel].vtxMax.y;
		}
		if (pPlayerPart[nCntModel].vtxMin.y < g_Player.vtxMin.y)
		{
			g_Player.vtxMin.y = pPlayerPart[nCntModel].vtxMin.y;
		}

		if (pPlayerPart[nCntModel].vtxMax.z >= g_Player.vtxMax.z)
		{
			g_Player.vtxMax.z = pPlayerPart[nCntModel].vtxMax.z;
		}
		if (pPlayerPart[nCntModel].vtxMin.z < g_Player.vtxMin.z)
		{
			g_Player.vtxMin.z = pPlayerPart[nCntModel].vtxMin.z;
		}

		//マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)pPlayerPart[nCntModel].pBuffMat->GetBufferPointer();

		g_Player.pMatDef = *pMat;

		//モーションリセット
		g_Player.aPlayerModel[nCntModel].pos = g_Player.aPlayerModel[nCntModel].posMotion;
		g_Player.aPlayerModel[nCntModel].rot = g_Player.aPlayerModel[nCntModel].rotMotion;
		g_Player.aPlayerModel[nCntModel].rot = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel];
	}

	//ゲージの設定
	switch (GetMode())
	{
	case MODE_RANKING:
		g_Player.nIdxLifeGauge = -1;
		break;
	case MODE_GAME:
		g_Player.nIdxLifeGauge = SetGauge(D3DXVECTOR3(20.0f, 50.0f, 0.0f), (float)g_Player.nLife, 50.0f, 400.0f, GAUGETYPE_HORIZON,PLAYER_LIFE_CR);
		break;
	default:
		break;
	}

	//影の設定
	g_Player.nIdxShadow = SetShadow(40.0f, SHADOWTYPE_CIRCLE, 0.0f, 0.0f,g_Player.rot);
}

//===========================
//プレイヤーの終了処理
//===========================
void UninitPlayer(void)
{
}

//===========================
//プレイヤーの更新処理
//===========================
void UpdatePlayer(void)
{
	//変数宣言
	Camera *pCamera = GetCamera();
	float fHeight = 1.0f - g_Player.pos.y / (PLAYER_JUMP_POW * 10 * PLAYER_GROUND_LENG);
	g_Player.posOld = g_Player.pos;
	g_Player.motionStateOld = g_Player.motionState;
	g_nCheckPlayer = 0;

	if (GetGameState() == GAMESTATE_NORMAL && GetFade() == FADE_NONE && (*GetBookState() == BOOKSTATE_NONE || *GetBookState() == BOOKSTATE_END) && GetPhotoMode() == false)
	{
		if (g_Player.state != PLAYERSTATE_DEATH)
		{
			//操作処理
			ConPlayer();
			ConPlayerPad();
		}
	}

	//モーション設定
	SetMotion();

	//位置更新処理
	MovePlayer();

	//サウンド関係
	SoundPlayer();

	//エリア制限処理
	CollisionWall(&g_Player.pos, &g_Player.posOld, &g_Player.move, 20.0f);
	//LimitMovePlayer();

	//影の位置設定処理
	SetPositionShadow(g_Player.nIdxShadow, D3DXVECTOR3(g_Player.pos.x, 0.1f, g_Player.pos.z),g_Player.rot, fHeight);

	//ビルボードの位置設定処理
	SetPositionBillboard(g_Player.nIdxBillboard, D3DXVECTOR3(g_Player.pos.x + 20.0f, g_Player.pos.y + 58.0f, g_Player.pos.z));

	//ブロックと当たり判定処理
	if (CollisionObject2(&g_Player.pos, &g_Player.posOld, &g_Player.move, g_Player.vtxMin, g_Player.vtxMax, g_Player.bJump) == true)
	{
		if (g_Player.motionState != PLAYERMOTION_JUMP)
		{
			g_Player.bJump = false;
		}
		g_Player.move.y = 0.0f;

		if (g_Player.motionState == PLAYERMOTION_AIR)
		{
			g_Player.motionState = PLAYERMOTION_LAND;
		}
	}
	else
	{
		g_Player.bJump = true;

		//着地した
		if (g_Player.pos.y < 0.0f)
		{
			if (g_Player.motionState != PLAYERMOTION_JUMP)
			{
				g_Player.bJump = false;
			}
			g_Player.move.y = 0.0f;
			g_Player.pos.y = 0.0f;

			if (g_Player.motionState == PLAYERMOTION_AIR)
			{
				g_Player.motionState = PLAYERMOTION_LAND;
			}
		}
	}

	if (g_Player.motionState == PLAYERMOTION_DAMAGE)
	{
		CollisionReflector(&g_Player.pos, &g_Player.posOld, &g_Player.move, 25.0f, 0);
	}
	else
	{
		//反射板との当たり判定
		if (PushReflector(&g_Player.pos, &g_Player.posOld, &g_Player.move, &g_Player.fAngle, g_Player.bJump) == true)
		{
			if (g_Player.bJump == false)
			{
				//モーションを押し込みモーションに変更
				g_Player.motionState = PLAYERMOTION_PUSH;

				pCamera->bBehind = true;
			}
			else
			{
				if (g_Player.motionState == PLAYERMOTION_PUSH)
				{
					//モーションを押し込みモーションに変更
					g_Player.motionState = PLAYERMOTION_NORMAL;
				}
			}
		}
		else
		{
			pCamera->bBehind = false;
		}
	}

	if (g_Player.state != PLAYERSTATE_DAMAGE && g_Player.state != PLAYERSTATE_DEATH)
	{
		//衝撃波との当たり判定
		if (CollisionShock(&g_Player.pos, &g_Player.posOld, &g_Player.rot,g_Player.bJump) == true)
		{
			if (g_Player.motionState != PLAYERMOTION_DAMAGE)
			{
				g_Player.fAngle = g_Player.rot.y;
			}

			//プレイヤーのヒット処理
			HitPlayer(4);
		}

		D3DXVECTOR3 playerModelPos = D3DXVECTOR3(g_Player.aPlayerModel[2].mtxWorld._41, g_Player.aPlayerModel[2].mtxWorld._42, g_Player.aPlayerModel[2].mtxWorld._43);

		//敵とプレイヤーの当たり判定
		if (CollisionEnemy(&g_Player.pos, &playerModelPos) == true)
		{
			if (g_Player.motionState != PLAYERMOTION_DAMAGE)
			{
				g_Player.fAngle = g_Player.rot.y;
			}

			HitPlayer(2);
		}

		//敵の攻撃とプレイヤーの当たり判定
		if (CollisionEnemyAttack(&g_Player.pos, &g_Player.rot) == true)
		{
			if (g_Player.motionState != PLAYERMOTION_DAMAGE)
			{
				g_Player.fAngle = g_Player.rot.y;
			}

			HitPlayer(1);
		}
	}

	//移動方向に向く処理
	SetRot();

	ModelMotionPlayer();

	switch (g_Player.state)
	{
	case PLAYERSTATE_NORMAL:
		break;
	case PLAYERSTATE_DAMAGE:
		g_Player.nTimerState--;

		if (g_Player.nTimerState <= 0)
		{
			if (g_Player.nLife > 0)
			{
				g_Player.state = PLAYERSTATE_NORMAL;
			}
		}
		break;
	case PLAYERSTATE_DEATH:
		g_Player.nTimerState--;

		if (g_Player.nTimerState <= 0)
		{
			if (g_Player.nLife > 0)
			{
				g_Player.state = PLAYERSTATE_DEATH;
			}
		}
		break;
	}

	if ((float)(g_Player.nLife) / (float)(g_Player.nMaxLife) <= 0.4f)
	{//HPゲージの変色
		if ((g_fRBJustFramePlayer % GAUGE_COLOR_FRAME) == 0)
		{
			g_fRBJustGaugePlayer == 0.0f ? g_fRBJustGaugePlayer = 1.0f : g_fRBJustGaugePlayer = 0.0f;
			g_fRBJustFramePlayer = 0;
		}

		g_fRBJustFramePlayer++;
	}

	//体力ゲージの設定
	if (GetMode() == MODE_GAME)
	{
		SetInfoGauge(g_Player.nIdxLifeGauge, D3DXCOLOR(g_fRBJustGaugePlayer, 1.0f, g_fRBJustGaugePlayer, 1.0f), (float)g_Player.nLife);
	}

	if (GetMode() == MODE_TUTORIAL && *GetBookState() == BOOKSTATE_END)
	{
		if (g_nCheckPlayer == 1)
		{
			//チュートリアルを進める
			TUTORIALSTATE *tutorial = GetTutorialState();
			if (*tutorial == TUTORIALSTATE_MOVE && GetMode() == MODE_TUTORIAL)
			{
				*tutorial = TUTORIALSTATE_CAMERA;

				PlaySound(SOUND_LABEL_SE_BUTTON_004);
			}
		}

		//プレイヤーを回復する
		g_Player.nLife = LIFE;
	}

	//フォグを設定
	g_Player.environment = PLAYERENVIRONMENT_CLOUD;

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_7))
	{
		g_Player.environment = PLAYERENVIRONMENT_CLOUD;
	}
	if (GetKeyboardTrigger(DIK_8))
	{
		g_Player.environment = PLAYERENVIRONMENT_NORMAL;
	}

	if (GetKeyboardTrigger(DIK_V))
	{
		HitPlayer(20);
	}
#endif

}

//===========================
//プレイヤーの描画処理
//===========================
void DrawPlayer(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat; 

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//情報取得
	Model *pPlayerPart = GetModelPlayer();

	for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel, mtxPalent;		//計算用マトリックス
		pMat = (D3DXMATERIAL*)pPlayerPart[nCntModel].pBuffMat->GetBufferPointer();

		switch (g_Player.state)
		{
		case PLAYERSTATE_NORMAL:
			//マテリアル情報に対するポインタを取得
			for (int nCntMat = 0; nCntMat < (int)pPlayerPart[nCntModel].dwNumMat; nCntMat++)
			{
				pMat->MatD3D.Diffuse = g_Player.pMatDef.MatD3D.Diffuse;

				pMat++;
			}
			break;
		case PLAYERSTATE_DAMAGE:
			if (g_Player.nTimerState >= 180)
			{
				//マテリアル情報に対するポインタを取得
				for (int nCntMat = 0; nCntMat < (int)pPlayerPart[nCntModel].dwNumMat; nCntMat++)
				{
					pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					pMat++;
				}
			}
			else
			{
				//マテリアル情報に対するポインタを取得
				for (int nCntMat = 0; nCntMat < (int)pPlayerPart[nCntModel].dwNumMat; nCntMat++)
				{
					pMat->MatD3D.Diffuse = g_Player.pMatDef.MatD3D.Diffuse;

					pMat++;
				}
			}
			break;
		case PLAYERSTATE_DEATH:
			if (g_Player.nTimerState >= 1980)
			{
				//マテリアル情報に対するポインタを取得
				for (int nCntMat = 0; nCntMat < (int)pPlayerPart[nCntModel].dwNumMat; nCntMat++)
				{
					pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					pMat++;
				}
			}
			else
			{
				//マテリアル情報に対するポインタを取得
				for (int nCntMat = 0; nCntMat < (int)pPlayerPart[nCntModel].dwNumMat; nCntMat++)
				{
					pMat->MatD3D.Diffuse = g_Player.pMatDef.MatD3D.Diffuse;

					pMat++;
				}
			}
			break;
		}

		//パーツのワールドマトリックス初期化
		D3DXMatrixIdentity(&g_Player.aPlayerModel[nCntModel].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
			g_Player.aPlayerModel[nCntModel].rot.y, g_Player.aPlayerModel[nCntModel].rot.x, g_Player.aPlayerModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_Player.aPlayerModel[nCntModel].mtxWorld, &g_Player.aPlayerModel[nCntModel].mtxWorld, &mtxRotModel);

		//位置を反映
		D3DXMatrixTranslation(&mtxTransModel,
			g_Player.aPlayerModel[nCntModel].pos.x, g_Player.aPlayerModel[nCntModel].pos.y, g_Player.aPlayerModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_Player.aPlayerModel[nCntModel].mtxWorld, &g_Player.aPlayerModel[nCntModel].mtxWorld, &mtxTransModel);

		//パーツの親マトリックスを設定
		if (g_Player.aPlayerModel[nCntModel].nIdxModelParent != -1)
		{
			mtxPalent = g_Player.aPlayerModel[g_Player.aPlayerModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{
			mtxPalent = g_Player.mtxWorld;
		}

		//算出したパーツのワールドマトリックスと親のマトリックスをかけ合わせる
		D3DXMatrixMultiply(&g_Player.aPlayerModel[nCntModel].mtxWorld,
			&g_Player.aPlayerModel[nCntModel].mtxWorld,
			&mtxPalent);

		pDevice->SetTransform(D3DTS_WORLD, &g_Player.aPlayerModel[nCntModel].mtxWorld);

		//マテリアルのデータのポイントを取得
		pMat = (D3DXMATERIAL*)pPlayerPart[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)pPlayerPart[nCntModel].dwNumMat; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, pPlayerPart[nCntModel].apTexture[nCntMat]);

			//モデル(パーツ)の描画
			pPlayerPart[nCntModel].pMesh->DrawSubset(nCntMat);
		}
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================================
//プレイヤーの影の描画処理
//========================================
void DrawPlayerShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxShadow;						//シャドウマトリックス
	D3DLIGHT9 light;							//ライトの情報
	D3DXVECTOR4 posLight;						//ライトの位置
	D3DXVECTOR3 pos, normal;					//平面上の任意の位置、法線ベクトル
	D3DXPLANE plane;							//平面情報
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	//ライトの位置を設定
	pDevice->GetLight(0, &light);
	posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

	//平面情報を生成
	pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	//シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	//シャドウマトリックスの作成
	D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
	D3DXMatrixMultiply(&mtxShadow, &g_Player.mtxWorld, &mtxShadow);

	//シャドウマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//情報取得
	Model *pPlayerPart = GetModelPlayer();

	//全モデル(パーツ)の描画
	for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel;		//計算用マトリックス
		D3DXMATRIX mtxPalent;						//親のマトリックス

		//パーツのワールドマトリックス初期化
		D3DXMatrixIdentity(&g_Player.aPlayerModel[nCntModel].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
			g_Player.aPlayerModel[nCntModel].rot.y, g_Player.aPlayerModel[nCntModel].rot.x, g_Player.aPlayerModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_Player.aPlayerModel[nCntModel].mtxWorld, &g_Player.aPlayerModel[nCntModel].mtxWorld, &mtxRotModel);

		//位置を反映
		D3DXMatrixTranslation(&mtxTransModel,
			g_Player.aPlayerModel[nCntModel].pos.x, g_Player.aPlayerModel[nCntModel].pos.y, g_Player.aPlayerModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_Player.aPlayerModel[nCntModel].mtxWorld, &g_Player.aPlayerModel[nCntModel].mtxWorld, &mtxTransModel);

		//パーツの親マトリックスを設定
		if (g_Player.aPlayerModel[nCntModel].nIdxModelParent != -1)
		{
			mtxPalent = g_Player.aPlayerModel[g_Player.aPlayerModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{
			mtxPalent = g_Player.mtxWorld;
		}

		//算出したパーツのワールドマトリックスと親のマトリックスをかけ合わせる
		D3DXMatrixMultiply(&g_Player.aPlayerModel[nCntModel].mtxWorld,
			&g_Player.aPlayerModel[nCntModel].mtxWorld,
			&mtxPalent);

		pDevice->SetTransform(D3DTS_WORLD, &g_Player.aPlayerModel[nCntModel].mtxWorld);

		//マテリアルのデータのポイントを取得
		pMat = (D3DXMATERIAL*)pPlayerPart[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)pPlayerPart[nCntModel].dwNumMat; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, pPlayerPart[nCntModel].apTexture[nCntMat]);

			//モデル(パーツ)の描画
			pPlayerPart[nCntModel].pMesh->DrawSubset(nCntMat);
		}
	}

	//保存していたマテリアルを戻す 
	pDevice->SetMaterial(&matDef);

	//通常の合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//========================================
//プレイヤーの設置
//========================================
void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_Player.bUse == false)
	{
		g_Player.pos = pos;
		g_Player.posOld = pos;
		g_Player.point = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player.rot = rot;
		g_Player.state = PLAYERSTATE_NORMAL;
		g_Player.motionState = PLAYERMOTION_NORMAL;
		g_Player.motionStateOld = PLAYERMOTION_NORMAL;
		g_Player.environment = PLAYERENVIRONMENT_NORMAL;
		g_Player.nTimerState = 0;
		g_Player.nEasterTimer = 0;
		g_Player.fAngle = 0.0f;
		g_Player.fJumpPow = PLAYER_JUMP_POW;
		g_Player.nLife = LIFE;										//体力
		g_Player.nMaxLife = LIFE;									//最大体力
		g_Player.bJump = false;									//ジャンプを使用してない状態にする
		g_Player.bEaster = false;									//ジャンプを使用してない状態にする
		g_Player.bUse = true;
		g_nCntFramePlayer = 0;
		g_nCntKeyPlayer = 0;
		g_nCheckPlayer = 0;
	}
}

//===========================
//プレイヤーの情報取得処理
//===========================
Player *GetPlayer(void)
{
	return &g_Player;
}

//===========================
//プレイヤーの移動処理
//===========================
void MovePlayer(void)
{
	g_Player.move.y -= PLAYER_GRAVITY;

	g_Player.pos += g_Player.move;

	g_Player.move.x += (0.0f - g_Player.move.x) * 0.1f;
	g_Player.move.z += (0.0f - g_Player.move.z) * 0.1f;
}

//===========================
//プレイヤーの操作(キーボード)処理
//===========================
void ConPlayer(void)
{
	Camera *pCamera = GetCamera();
	float fRotCamera;

	//カメラの角度算出
	fRotCamera = atan2f(pCamera->posV.x - pCamera->posR.x, pCamera->posV.z - pCamera->posR.z);

	if (g_Player.motionState == PLAYERMOTION_PUSH)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{//Aキーが押された時
			if (GetKeyboardPress(DIK_S) == true)
			{//Sキーが押された時
				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//Wキーが押された時
				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.25f);
			}
			else
			{
				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.5f);
			}

			//入力されている
			g_nCheckPlayer = 1;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//Dキーが押された時
			if (GetKeyboardPress(DIK_S) == true)
			{//Sキーが押された時
				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//Wキーが押された時
				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.25f);
			}
			else
			{
				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.5f);
			}

			//入力されている
			g_nCheckPlayer = 1;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//Sキーが押された時
			g_Player.fAngle = fRotCamera + D3DX_PI;

			//入力されている
			g_nCheckPlayer = 1;
		}
		else if (GetKeyboardPress(DIK_W) == true)
		{//Wキーが押された時
			g_Player.fAngle = fRotCamera;

			//入力されている
			g_nCheckPlayer = 1;
		}

		if (g_Player.fAngle > D3DX_PI)
		{
			g_Player.fAngle -= (D3DX_PI * 2);
		}
		else if (g_Player.fAngle <= -D3DX_PI)
		{
			g_Player.fAngle += (D3DX_PI * 2);
		}

		if (g_Player.fAngle >= g_Player.rot.y - D3DX_PI * 0.3f && g_Player.fAngle <= g_Player.rot.y + D3DX_PI * 0.7f)
		{
			if (GetKeyboardPress(DIK_W) == true)
			{//Wキーが押された時
				g_Player.move.x -= cosf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
				g_Player.move.z -= sinf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{//Dキーが押された時
				g_Player.move.x -= cosf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
				g_Player.move.z -= sinf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
			}
			else if (GetKeyboardPress(DIK_A) == true)
			{//Aキーが押された時
				g_Player.move.x -= cosf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
				g_Player.move.z -= sinf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//Sキーが押された時
				g_Player.move.x -= cosf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
				g_Player.move.z -= sinf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
			}
		}
		else
		{
			if (GetKeyboardPress(DIK_A) == true)
			{//Aキーが押された時
				if (GetKeyboardPress(DIK_S) == true)
				{//Sキーが押された時
					g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.25f)) * LENGTH;
					g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.25f)) * LENGTH;
				}
				else if (GetKeyboardPress(DIK_W) == true)
				{//Wキーが押された時
					g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.75f)) * LENGTH;
					g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.75f)) * LENGTH;
				}
				else
				{
					g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.5f)) * LENGTH;
					g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.5f)) * LENGTH;
				}
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{//Dキーが押された時
				if (GetKeyboardPress(DIK_S) == true)
				{//Sキーが押された時
					g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.25f)) * LENGTH;
					g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.25f)) * LENGTH;
				}
				else if (GetKeyboardPress(DIK_W) == true)
				{//Wキーが押された時
					g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.75f)) * LENGTH;
					g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.75f)) * LENGTH;
				}
				else
				{
					g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.5f)) * LENGTH;
					g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.5f)) * LENGTH;
				}
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//Sキーが押された時
				g_Player.move.x += cosf(pCamera->rot.y) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y) * LENGTH;
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//Wキーが押された時
				g_Player.move.x += cosf(pCamera->rot.y + D3DX_PI) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y + D3DX_PI) * LENGTH;
			}

			//入力されている
			g_nCheckPlayer = 0;
		}
	}
	else if (g_Player.state == PLAYERSTATE_DAMAGE)
	{
		
	}
	else if (g_Player.bJump == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{//Aキーが押された時
			if (GetKeyboardPress(DIK_S) == true)
			{//Sキーが押された時
				g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.25f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.25f)) * LENGTH;

				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//Wキーが押された時
				g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.75f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.75f)) * LENGTH;

				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.25f);
			}
			else
			{
				g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.5f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.5f)) * LENGTH;

				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.5f);
			}
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//Dキーが押された時
			if (GetKeyboardPress(DIK_S) == true)
			{//Sキーが押された時
				g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.25f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.25f)) * LENGTH;

				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//Wキーが押された時
				g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.75f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.75f)) * LENGTH;

				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.25f);
			}
			else
			{
				g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.5f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.5f)) * LENGTH;

				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.5f);
			}
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//Sキーが押された時
			g_Player.move.x += cosf(pCamera->rot.y) * LENGTH;
			g_Player.move.z += sinf(pCamera->rot.y) * LENGTH;

			g_Player.fAngle = fRotCamera + D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_W) == true)
		{//Wキーが押された時
			g_Player.move.x += cosf(pCamera->rot.y + D3DX_PI) * LENGTH;
			g_Player.move.z += sinf(pCamera->rot.y + D3DX_PI) * LENGTH;

			g_Player.fAngle = fRotCamera;
		}
	}
	else
	{
		if (GetKeyboardPress(DIK_A) == true)
		{//Aキーが押された時
			if (GetKeyboardPress(DIK_S) == true)
			{//Sキーが押された時
				g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.25f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.25f)) * LENGTH;

				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//Wキーが押された時
				g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.75f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.75f)) * LENGTH;

				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.25f);
			}
			else
			{
				g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.5f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.5f)) * LENGTH;

				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.5f);
			}

			//入力されている
			g_nCheckPlayer = 1;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//Dキーが押された時
			if (GetKeyboardPress(DIK_S) == true)
			{//Sキーが押された時
				g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.25f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.25f)) * LENGTH;

				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//Wキーが押された時
				g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.75f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.75f)) * LENGTH;

				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.25f);
			}
			else
			{
				g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.5f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.5f)) * LENGTH;

				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.5f);
			}

			//入力されている
			g_nCheckPlayer = 1;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//Sキーが押された時
			g_Player.move.x += cosf(pCamera->rot.y) * LENGTH;
			g_Player.move.z += sinf(pCamera->rot.y) * LENGTH;

			g_Player.fAngle = fRotCamera + D3DX_PI;

			//入力されている
			g_nCheckPlayer = 1;
		}
		else if (GetKeyboardPress(DIK_W) == true)
		{//Wキーが押された時
			g_Player.move.x += cosf(pCamera->rot.y + D3DX_PI) * LENGTH;
			g_Player.move.z += sinf(pCamera->rot.y + D3DX_PI) * LENGTH;

			g_Player.fAngle = fRotCamera;

			//入力されている
			g_nCheckPlayer = 1;
		}
	}

	if (GetKeyboardTrigger(DIK_SPACE) == true && g_Player.state == PLAYERSTATE_NORMAL)
	{// ジャンプ
		if (g_Player.bJump == false)
		{
			g_Player.bJump = true;
			g_Player.motionState = PLAYERMOTION_JUMP;
			g_nCntKeyPlayer = 0;
			g_nCntFramePlayer = 0;
		}
	}
}

//===========================
//プレイヤーの操作(PAD)処理
//===========================
void ConPlayerPad(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();
	float fRotCamera;

	//カメラの角度算出
	fRotCamera = atan2f(pCamera->posV.x - pCamera->posR.x, pCamera->posV.z - pCamera->posR.z);

	//移動操作===========================
	//ステックの情報を取得
	D3DXVECTOR3 StickLength = GetStickL(0);	//スティックの情報保存
	float fAngle = atan2f(StickLength.x, StickLength.y);	//スティックの角度
	float fLength = D3DXVec3Length(&StickLength); //ベクトルの長さを取得
	if (fLength > 1.0f)
	{
		fLength = 1.0f;
	}
	D3DXVec3Normalize(&StickLength, &StickLength);	//保存したベクトルの正規化

	if (fLength >= 0.2f)
	{//デッドゾーンの設定

		//入力されている
		g_nCheckPlayer = 1;

		if (g_Player.motionState == PLAYERMOTION_PUSH)
		{
			g_Player.fAngle = -(pCamera->rot.y + fAngle + (D3DX_PI * 0.5f));

			if (g_Player.fAngle > D3DX_PI)
			{
				g_Player.fAngle -= (D3DX_PI * 2);
			}
			else if (g_Player.fAngle <= -D3DX_PI)
			{
				g_Player.fAngle += (D3DX_PI * 2);
			}

			if (g_Player.fAngle >= g_Player.rot.y - D3DX_PI * 0.5f && g_Player.fAngle <= g_Player.rot.y + D3DX_PI * 0.5f)
			{
				g_Player.move.x -= cosf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
				g_Player.move.z -= sinf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
			}
			else
			{
				g_Player.move.x += sinf(fRotCamera + (fAngle - (D3DX_PI * 0.5f))) * LENGTH;
				g_Player.move.z += cosf(fRotCamera + (fAngle - (D3DX_PI * 0.5f))) * LENGTH;

				g_Player.motionState = PLAYERMOTION_NORMAL;

				//入力されている
				g_nCheckPlayer = 0;
			}
			
		}
		else if (g_Player.state == PLAYERSTATE_DAMAGE)
		{

		}
		else if (g_Player.bJump)
		{
			g_Player.move.x += cosf(pCamera->rot.y + fAngle) * fLength * LENGTH;
			g_Player.move.z += sinf(pCamera->rot.y + fAngle) * fLength * LENGTH;

			g_Player.fAngle = -(pCamera->rot.y + fAngle + (D3DX_PI * 0.5f));
		}
		else
		{
			g_Player.move.x += cosf(pCamera->rot.y + fAngle) * fLength * LENGTH;
			g_Player.move.z += sinf(pCamera->rot.y + fAngle) * fLength * LENGTH;

			g_Player.fAngle = -(pCamera->rot.y + fAngle + (D3DX_PI * 0.5f));
		}
	}

	//移動操作===========================

	//視点操作===========================
	StickLength = GetStickR(0);	//スティックの情報保存
	fLength = D3DXVec3Length(D3DXVec3Normalize(&StickLength, &StickLength));	//保存したベクトルの正規化

	if (fLength >= 0.2f)
	{//デッドゾーンの設定
		pCamera->rot.y -= StickLength.x * CAMERA_FACT;
		pCamera->rot.x -= StickLength.y * CAMERA_FACT;
	}
	//視点操作===========================

	//ジャンプ
	if (GetJoyPadButtonTrigger(KEY_A,0) && g_Player.state == PLAYERSTATE_NORMAL)
	{
		if (g_Player.bJump == false)
		{
			g_Player.bJump = true;
			g_Player.motionState = PLAYERMOTION_JUMP;
			g_nCntKeyPlayer = 0;
			g_nCntFramePlayer = 0;
		}
	}
}

//===========================
//プレイヤーのモーションセット処理
//===========================
void SetMotion(void)
{
	//入力されているかの判定
	if (g_nCheckPlayer == 0)
	{//入力されていない
		if (g_Player.motionState == PLAYERMOTION_PUSH)
		{
			g_Player.motionState = PLAYERMOTION_NORMAL;
		}
		else if (g_Player.state == PLAYERSTATE_DAMAGE)
		{

		}
		else if (g_Player.bJump == true)
		{
			if (g_Player.motionState != PLAYERMOTION_JUMP)
			{
				g_Player.motionState = PLAYERMOTION_AIR;
			}
		}
		else
		{
			if (g_Player.motionState == PLAYERMOTION_LAND)
			{

			}
			else
			{
				g_Player.motionState = PLAYERMOTION_NORMAL;
			}
		}
	}
	else
	{//入力されている
		if (g_Player.motionState == PLAYERMOTION_PUSH)
		{
			g_Player.fAngle = g_Player.rot.y;
		}
		else if (g_Player.state == PLAYERSTATE_DAMAGE)
		{

		}
		else if (g_Player.bJump == true)
		{
			if (g_Player.motionState != PLAYERMOTION_JUMP)
			{
				g_Player.motionState = PLAYERMOTION_AIR;
			}
		}
		else
		{
			g_Player.motionState = PLAYERMOTION_MOVE;
		}
	}

	//ジャンプの移動量をたす
	if (g_Player.motionState == PLAYERMOTION_JUMP && g_nCntKeyPlayer == 1 && g_nCntFramePlayer == 0)
	{
		g_Player.move.y = g_Player.fJumpPow;
	}

	if (GetTimeUp() == true)
	{
		g_Player.motionState = PLAYERMOTION_TIMEUP;
	}

	if (g_Player.state == PLAYERSTATE_DEATH)
	{
		g_Player.motionState = PLAYERMOTION_DEATH;
	}
}

//===========================
//プレイヤーのモーション処理
//===========================
void ModelMotionPlayer(void)
{
	D3DXVECTOR3 posDiff, rotDiff;

	//エネミーのモーションとほぼ同じ
	if (g_nCntKeyPlayer < g_Player.aPlayerMotion[g_Player.motionState].nNumKey)
	{
		if (g_nCntFramePlayer < g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame)
		{
			if (g_nCntKeyPlayer == g_Player.aPlayerMotion[g_Player.motionState].nNumKey - 1)
			{
				for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
				{
					if (g_Player.aPlayerMotion[g_Player.motionState].nLoop == 0)
					{
						posDiff.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].x - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].x;
						posDiff.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].y - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].y;
						posDiff.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].z - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].z;

						rotDiff.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].x - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].x;
						rotDiff.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].y - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].y;
						rotDiff.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].z - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].z;

						//posを足す
						g_Player.aPlayerModel[nCntModel].pos.x = g_Player.aPlayerModel[nCntModel].posMotion.x + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].x + (posDiff.x * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
						g_Player.aPlayerModel[nCntModel].pos.y = g_Player.aPlayerModel[nCntModel].posMotion.y + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].y + (posDiff.y * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
						g_Player.aPlayerModel[nCntModel].pos.z = g_Player.aPlayerModel[nCntModel].posMotion.z + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].z + (posDiff.z * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));

						//rotを足す
						g_Player.aPlayerModel[nCntModel].rot.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].x + (rotDiff.x * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
						g_Player.aPlayerModel[nCntModel].rot.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].y + (rotDiff.y * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
						g_Player.aPlayerModel[nCntModel].rot.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].z + (rotDiff.z * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
					}
					else
					{
						posDiff.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[0].pos[nCntModel].x - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].x;
						posDiff.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[0].pos[nCntModel].y - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].y;
						posDiff.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[0].pos[nCntModel].z - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].z;

						rotDiff.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[0].rot[nCntModel].x - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].x;
						rotDiff.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[0].rot[nCntModel].y - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].y;
						rotDiff.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[0].rot[nCntModel].z - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].z;

						//posを足す
						g_Player.aPlayerModel[nCntModel].pos.x = g_Player.aPlayerModel[nCntModel].posMotion.x + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].x + (posDiff.x * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
						g_Player.aPlayerModel[nCntModel].pos.y = g_Player.aPlayerModel[nCntModel].posMotion.y + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].y + (posDiff.y * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
						g_Player.aPlayerModel[nCntModel].pos.z = g_Player.aPlayerModel[nCntModel].posMotion.z + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].z + (posDiff.z * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));

						//rotを足す
						g_Player.aPlayerModel[nCntModel].rot.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].x + (rotDiff.x * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
						g_Player.aPlayerModel[nCntModel].rot.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].y + (rotDiff.y * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
						g_Player.aPlayerModel[nCntModel].rot.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].z + (rotDiff.z * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
					}
				
					if (g_Player.aPlayerModel[nCntModel].rot.x > 3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.x -= 6.28f;
					}
					else if (g_Player.aPlayerModel[nCntModel].rot.x < -3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.x += 6.28f;
					}

					if (g_Player.aPlayerModel[nCntModel].rot.y > 3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.y -= 6.28f;
					}
					else if (g_Player.aPlayerModel[nCntModel].rot.y < -3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.y += 6.28f;
					}

					if (g_Player.aPlayerModel[nCntModel].rot.z > 3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.z -= 6.28f;
					}
					else if (g_Player.aPlayerModel[nCntModel].rot.z < -3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.z += 6.28f;
					}
				}
			}
			else
			{
				for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
				{
					posDiff.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer + 1].pos[nCntModel].x - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].x;
					posDiff.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer + 1].pos[nCntModel].y - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].y;
					posDiff.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer + 1].pos[nCntModel].z - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].z;

					rotDiff.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer + 1].rot[nCntModel].x - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].x;
					rotDiff.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer + 1].rot[nCntModel].y - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].y;
					rotDiff.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer + 1].rot[nCntModel].z - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].z;

					//posを足す
					g_Player.aPlayerModel[nCntModel].pos.x = g_Player.aPlayerModel[nCntModel].posMotion.x + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].x + posDiff.x * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame);
					g_Player.aPlayerModel[nCntModel].pos.y = g_Player.aPlayerModel[nCntModel].posMotion.y + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].y + posDiff.y * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame);
					g_Player.aPlayerModel[nCntModel].pos.z = g_Player.aPlayerModel[nCntModel].posMotion.z + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].z + posDiff.z * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame);

					//rotを足す
					g_Player.aPlayerModel[nCntModel].rot.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].x + (rotDiff.x * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
					g_Player.aPlayerModel[nCntModel].rot.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].y + (rotDiff.y * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
					g_Player.aPlayerModel[nCntModel].rot.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].z + (rotDiff.z * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));

					if (g_Player.aPlayerModel[nCntModel].rot.x > 3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.x -= 6.28f;
					}
					else if (g_Player.aPlayerModel[nCntModel].rot.x < -3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.x += 6.28f;
					}

					if (g_Player.aPlayerModel[nCntModel].rot.y > 3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.y -= 6.28f;
					}
					else if (g_Player.aPlayerModel[nCntModel].rot.y < -3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.y += 6.28f;
					}

					if (g_Player.aPlayerModel[nCntModel].rot.z > 3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.z -= 6.28f;
					}
					else if (g_Player.aPlayerModel[nCntModel].rot.z < -3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.z += 6.28f;
					}
				}
			}

			g_nCntFramePlayer++;
		}
		else
		{
			g_nCntKeyPlayer++;
			g_nCntFramePlayer = 0;

			for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
			{
				g_Player.aPlayerModel[nCntModel].rotMotion = g_Player.aPlayerModel[nCntModel].rot;
			}
		}
	}
	else
	{
		if (g_Player.aPlayerMotion[g_Player.motionState].nLoop == 0)
		{
			if (g_Player.motionState == PLAYERMOTION_JUMP)
			{
				g_Player.motionState = PLAYERMOTION_AIR;
			}
			else if (g_Player.motionState == PLAYERMOTION_DAMAGE)
			{
				g_Player.motionState = PLAYERMOTION_NORMAL;
			}
			else
			{
				g_Player.motionState = PLAYERMOTION_NORMAL;
			}
		}

		//キーとフレームをリセット
		g_nCntKeyPlayer = 0;
		g_nCntFramePlayer = 0;
	}

	if (g_Player.motionState != g_Player.motionStateOld)
	{
		g_nCntKeyPlayer = 0;
		g_nCntFramePlayer = 0;
		for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
		{
			g_Player.aPlayerModel[nCntModel].pos = g_Player.aPlayerModel[nCntModel].posMotion + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel];
			g_Player.aPlayerModel[nCntModel].rot = g_Player.aPlayerModel[nCntModel].rotMotion;
			g_Player.aPlayerModel[nCntModel].rot = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel];
		}
	}
}

//===========================
//プレイヤーの角度調整処理
//===========================
void SetRot(void)
{
	float fRotMove, fRotDest, fRotDiff;

	//現在の角度と目的の角度の差分を計算
	fRotMove = g_Player.rot.y;
	fRotDest = g_Player.fAngle;
	fRotDiff = fRotDest - fRotMove;

	if (fRotDiff > 3.14f)
	{
		fRotDiff -= 6.28f;
	}
	else if (fRotDiff <= -3.14f)
	{
		fRotDiff += 6.28f;
	}

	//徐々に足してく
	fRotMove += fRotDiff * 0.2f;

	if (fRotMove > 3.14f)
	{
		fRotMove -= 6.28f;
	}
	else if (fRotMove <= -3.14f)
	{
		fRotMove += 6.28f;
	}

	g_Player.rot.y = fRotMove;
}

//========================================
//プレイヤーの位置調整処理
//========================================
void SetPosPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	g_Player.pos = pos;
	g_Player.rot = rot;
}

//========================================
//プレイヤーの削除
//========================================
void DeletePlayer(void)
{
	g_Player.bUse = false;
}

//========================================
//プレイヤーのダメージ処理
//========================================
void HitPlayer(int nDamage)
{
	if (g_Player.state != PLAYERSTATE_DAMAGE && g_Player.state != PLAYERMOTION_DEATH && g_Player.nLife > 0)
	{//体力を減らす

		if (GetGameState() == GAMESTATE_NORMAL)
		{//通常状態でのみ体力減少
			g_Player.nLife -= nDamage;

			//ダメージ音再生
			PlaySound(SOUND_LABEL_SE_DAMAGE);

			//スコアの現象
			AddScore(SHOCK_SCORE);

			//取得スコア表示設定
			SetPointScore(SHOCK_SCORE, D3DXVECTOR3(1000.0f, 400.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), POINTTYPE_SLIDE, 0.5f, LOGTYPE_DAMAGE);

			g_Player.state = PLAYERSTATE_DAMAGE;
			g_Player.motionState = PLAYERMOTION_DAMAGE;
			g_Player.nTimerState = 200;
			g_nCntKeyPlayer = 0;
			g_nCntFramePlayer = 0;

			g_Player.move.x = cosf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH * 30.0f;
			g_Player.move.z = sinf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH * 30.0f;

			SetQuake(10, 0.3f);
		}
	}

	//プレイヤーの体力がなくなった
	if (g_Player.nLife <= 0)
	{
		g_Player.nLife = 0;
		g_Player.nTimerState = 2000;
		g_Player.motionState = PLAYERMOTION_DEATH;
		g_Player.state = PLAYERSTATE_DEATH;
		g_Player.move.x = cosf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH * 30.0f;
		g_Player.move.z = sinf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH * 30.0f;
		DeletePlayer();					//使用していない状態にする
	}
}

//===========================
//プレイヤーのモーション処理
//===========================
void ReadFilePlayer(void)
{
	char aText[128];
	FILE *pFile = fopen("data\\MOTION\\motion_TH.txt", "r");

	//ファイル読み込み
	if (pFile == NULL)
	{
		return;
	}

	do
	{
		fgets(&aText[0], 128, pFile);
	} while (strcmp(aText, "CHARACTERSET\n") != 0);

	fscanf(pFile, "%s = %f	%s %s", &aText[0], &g_Player.fMove, &aText[0], &aText[0]);
	fscanf(pFile, "%s = %f	%s %s", &aText[0], &g_Player.fJump, &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %d	%s %s\n", &aText[0], &g_Player.nNumModel, &aText[0], &aText[0]);

	for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
	{
		fgets(&aText[0], 128, pFile);
		fscanf(pFile, "%s = %d", &aText[0], &g_Player.aPlayerModel[nCntModel].nIdxModelParent);
		fscanf(pFile, "%s = %d	%s %s", &aText[0], &g_Player.aPlayerModel[nCntModel].nIdxModelParent, &aText[0], &aText[0]);
		fscanf(pFile, "%s = %f %f %f", &aText[0], &g_Player.aPlayerModel[nCntModel].posMotion.x, &g_Player.aPlayerModel[nCntModel].posMotion.y, &g_Player.aPlayerModel[nCntModel].posMotion.z);
		fscanf(pFile, "%s = %f %f %f\n", &aText[0], &g_Player.aPlayerModel[nCntModel].rotMotion.x, &g_Player.aPlayerModel[nCntModel].rotMotion.y, &g_Player.aPlayerModel[nCntModel].rotMotion.z);
		fgets(&aText[0], 128, pFile);
		fgets(&aText[0], 128, pFile);
	}

	for (int nCnt = 0; nCnt < 7; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	for (int nCntMotion = 0; nCntMotion < PLAYERMOTION_MAX; nCntMotion++)
	{
		fgets(&aText[0], 128, pFile);
		fscanf(pFile, "%s = %d	%s %s %s %s", &aText[0], &g_Player.aPlayerMotion[nCntMotion].nLoop, &aText[0], &aText[0], &aText[0], &aText[0]);
		fscanf(pFile, "%s = %d	%s %s\n", &aText[0], &g_Player.aPlayerMotion[nCntMotion].nNumKey, &aText[0], &aText[0]);

		for (int nCntKey = 0; nCntKey < g_Player.aPlayerMotion[nCntMotion].nNumKey; nCntKey++)
		{
			fgets(&aText[0], 128, pFile);
			fscanf(pFile, "%s = %d\n", &aText[0], &g_Player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].nFrame);

			for (int nCnt = 0; nCnt < g_Player.nNumModel; nCnt++)
			{
				fgets(&aText[0], 128, pFile);
				fscanf(pFile, "%s = %f %f %f", &aText[0], &g_Player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].x, &g_Player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].y, &g_Player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].z);
				fscanf(pFile, "%s = %f %f %f\n", &aText[0], &g_Player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].x, &g_Player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].y, &g_Player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].z);
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
}

//========================================
//プレイヤーのモーション
//========================================
void DebugPlayerMotion(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,580,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "今のモーション; %d\n前のモーション; %d :%d", g_Player.motionState, g_Player.motionStateOld, g_Player.state);

	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//===========================
//プレイヤーの移動制限処理
//===========================
void LimitMovePlayer(void)
{
	if (g_Player.pos.x + PLAYER_RADIUS > (FIELD_RANGE * MESH_U) * 0.5f)
	{//エリア右の制限
		g_Player.pos.x = (FIELD_RANGE * MESH_U) * 0.5f - PLAYER_RADIUS;
	}
	else if (g_Player.pos.x - PLAYER_RADIUS < -(FIELD_RANGE * MESH_U) * 0.5f)
	{//エリア左の制限
		g_Player.pos.x = -(FIELD_RANGE * MESH_U) * 0.5f + PLAYER_RADIUS;
	}

	if (g_Player.pos.z + PLAYER_RADIUS >(FIELD_RANGE * MESH_V) * 0.5f)
	{//エリア奥の制限
		g_Player.pos.z = (FIELD_RANGE * MESH_V) * 0.5f - PLAYER_RADIUS;
	}
	else if (g_Player.pos.z - PLAYER_RADIUS < -(FIELD_RANGE * MESH_V) * 0.5f)
	{//エリア手前の制限
		g_Player.pos.z = -(FIELD_RANGE * MESH_V) * 0.5f + PLAYER_RADIUS;
	}
}

//==========================================
//  環境状態の取得
//==========================================
PLAYERENVIRONMENT GetEnvironment()
{
	return g_Player.environment;
}

//==========================================
//ランキングでのプレイヤーの更新  
//==========================================
void UpdatePlayerRanking(void)
{
	//前回のモーション状態を保存
	g_Player.motionStateOld = g_Player.motionState;

	if (GetKeyboardPress(DIK_T) == true &&
		GetKeyboardPress(DIK_K) == true &&
		GetKeyboardPress(DIK_D) == true)
	{//TKDキーが押された時
		g_Player.bEaster = true;
	}

	SetMotionRanking();

	//モーション管理処理
	ModelMotionPlayer();

	//影の位置設定
		SetPositionShadow(g_Player.nIdxShadow,
							D3DXVECTOR3(g_Player.pos.x, 0.1f, g_Player.pos.z),
							g_Player.rot,
							1.0f);
}

//==========================================
//ランキングでのモーションの更新
//==========================================
void SetMotionRanking(void)
{
	if (g_Player.bEaster == true)
	{
		if (g_Player.nEasterTimer < 30)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG6;

			if (g_Player.nEasterTimer == 0)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 90)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG0;

			if (g_Player.nEasterTimer == 30)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 630)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG1;
			
			if (g_Player.nEasterTimer == 90)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 690)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG2;
			
			if (g_Player.nEasterTimer == 630)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 750)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG1;
			
			if (g_Player.nEasterTimer == 690)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 810)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG2;
			
			if (g_Player.nEasterTimer == 750)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 930)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG3;
			
			if (g_Player.nEasterTimer == 810)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 960)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG4;
			
			if (g_Player.nEasterTimer == 930)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 1050)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG5;
			
			if (g_Player.nEasterTimer == 960)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 1590)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG1;
			
			if (g_Player.nEasterTimer == 1050)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 1650)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG2;
			
			if (g_Player.nEasterTimer == 1590)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 1710)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG1;

			if (g_Player.nEasterTimer == 1650)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 1770)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG2;

			if (g_Player.nEasterTimer == 1710)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 1890)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG3;
			
			if (g_Player.nEasterTimer == 1770)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 1920)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG4;
			
			if (g_Player.nEasterTimer == 1890)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 2010)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG5;
			
			if (g_Player.nEasterTimer == 1920)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 2250)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG1;
			
			if (g_Player.nEasterTimer == 2010)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 2370)
		{
			//モーションを設定
			g_Player.motionState = PLAYERMOTION_EASTEREGG6;

			if (g_Player.nEasterTimer == 2250)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer > 2600)
		{
			g_Player.nEasterTimer = 0;
		}

		g_Player.rot.y = 0.0f;
		g_Player.nEasterTimer++;
	}
	else
	{
		//モーションを設定
		g_Player.motionState = PLAYERMOTION_MOVE;
	}
}

//==========================================
//プレイヤーのサウンド関係処理
//==========================================
void SoundPlayer(void)
{
	if (g_Player.motionState == PLAYERMOTION_MOVE)
	{
		if (g_nCntKeyPlayer == 3 && g_nCntFramePlayer == 0)
		{
			PlaySound(SOUND_LABEL_SE_STEP_000);
		}

		if (g_nCntKeyPlayer == 7 && g_nCntFramePlayer == 0)
		{
			PlaySound(SOUND_LABEL_SE_STEP_000);
		}
	}

	if (g_Player.motionState == PLAYERMOTION_JUMP)
	{
		if (g_nCntKeyPlayer == 1 && g_nCntFramePlayer == 0)
		{
			PlaySound(SOUND_LABEL_SE_JUMP_000);
		}
	}

	if (g_Player.motionState == PLAYERMOTION_LAND)
	{
		if (g_nCntKeyPlayer == 0 && g_nCntFramePlayer == 0)
		{
			PlaySound(SOUND_LABEL_SE_LAND);
			SetParticle(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + 1.0f, g_Player.pos.z), PARTICLETYPE_LANDED);
		}
	}

	if (g_Player.motionState == PLAYERMOTION_DAMAGE)
	{
		if (g_nCntKeyPlayer == 2 && g_nCntFramePlayer == 0)
		{
			PlaySound(SOUND_LABEL_SE_LAND);
		}
	}

	if (g_Player.motionState == PLAYERMOTION_DEATH)
	{
		if (g_nCntKeyPlayer == 2 && g_nCntFramePlayer == 0)
		{
			PlaySound(SOUND_LABEL_SE_LAND);
		}
	}
}
