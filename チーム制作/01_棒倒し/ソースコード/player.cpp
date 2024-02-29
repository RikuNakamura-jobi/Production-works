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
#include "camera.h"
#include "result.h"
#include "game.h"
#include "ui.h"
#include "model.h"
#include "sound.h"
#include <stdio.h>

//プロトタイプ宣言
void SetRot(void);
void MovePlayer(void);
void ConPlayer(void);
void ConPlayerPad(void);
void ModelMotion(void);

//グローバル変数宣言
Player g_player;
int g_nIdxShadow = -1;
char g_aModelName[32][128];
int g_nCntFrame;
int g_nCntKey;
int g_nSE;

//===========================
//プレイヤーの初期化処理
//===========================
void InitPlayer(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL *pMat;

	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, -1800.0f);
	g_player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.point = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);
	g_player.state = PLAYERSTATE_NORMAL;
	g_player.motionState = PLAYERMOTION_MOVE;
	g_player.motionStateOld = PLAYERMOTION_MOVE;
	g_player.nLife = 50;
	g_player.nTimerState = 0;
	g_player.fAngle = 0.0f;
	g_nCntFrame = 0;
	g_nCntKey = 0;
	g_nSE = 0;

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		//xファイルの読み込み
		D3DXLoadMeshFromX(&g_aModelName[nCntModel][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aPlayerModel[nCntModel].pBuffMatPlayer,
			NULL,
			&g_player.aPlayerModel[nCntModel].dwNumMatPlayer,
			&g_player.aPlayerModel[nCntModel].pMeshPlayer);

		//マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_player.aPlayerModel[nCntModel].pBuffMatPlayer->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aPlayerModel[nCntModel].dwNumMatPlayer; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_player.aPlayerModel[nCntModel].apTexturePlayer[nCntMat]);
			}
		}
	}
}

//===========================
//プレイヤーの終了処理
//===========================
void UninitPlayer(void)
{
	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		//テクスチャの破棄
		if (g_player.aPlayerModel[nCntModel].pMeshPlayer != NULL)
		{
			g_player.aPlayerModel[nCntModel].pMeshPlayer->Release();
			g_player.aPlayerModel[nCntModel].pMeshPlayer = NULL;
		}

		//頂点バッファの破棄
		if (g_player.aPlayerModel[nCntModel].pBuffMatPlayer != NULL)
		{
			g_player.aPlayerModel[nCntModel].pBuffMatPlayer->Release();
			g_player.aPlayerModel[nCntModel].pBuffMatPlayer = NULL;
		}
	}
}

//===========================
//プレイヤーの更新処理
//===========================
void UpdatePlayer(void)
{
	g_player.motionStateOld = g_player.motionState;

	ConPlayer();
	ConPlayerPad();

	MovePlayer();

	SetRot();

	ModelMotion();
}

//===========================
//プレイヤーの更新処理
//===========================
void UpdatePlayerTutorial(void)
{
	g_player.motionStateOld = g_player.motionState;

	ConPlayer();
	ConPlayerPad();

	MovePlayer();

	if (g_player.pos.z <= -3500)
	{
		ResetModelTutorial();
		g_player.pos.z = 0;
	}

	SetRot();

	ModelMotion();
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
	D3DXMatrixIdentity(&g_player.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel, mtxPalent;		//計算用マトリックス

		//パーツのワールドマトリックス初期化
		D3DXMatrixIdentity(&g_player.aPlayerModel[nCntModel].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
			g_player.aPlayerModel[nCntModel].rot.y, g_player.aPlayerModel[nCntModel].rot.x, g_player.aPlayerModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_player.aPlayerModel[nCntModel].mtxWorld, &g_player.aPlayerModel[nCntModel].mtxWorld, &mtxRotModel);

		//位置を反映
		D3DXMatrixTranslation(&mtxTransModel,
			g_player.aPlayerModel[nCntModel].pos.x, g_player.aPlayerModel[nCntModel].pos.y, g_player.aPlayerModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_player.aPlayerModel[nCntModel].mtxWorld, &g_player.aPlayerModel[nCntModel].mtxWorld, &mtxTransModel);

		//パーツの親マトリックスを設定
		if (g_player.aPlayerModel[nCntModel].nIdxModelParent != -1)
		{
			mtxPalent = g_player.aPlayerModel[g_player.aPlayerModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{
			mtxPalent = g_player.mtxWorld;
		}

		//算出したパーツのワールドマトリックスと親のマトリックスをかけ合わせる
		D3DXMatrixMultiply(&g_player.aPlayerModel[nCntModel].mtxWorld,
			&g_player.aPlayerModel[nCntModel].mtxWorld,
			&mtxPalent);

		pDevice->SetTransform(D3DTS_WORLD, &g_player.aPlayerModel[nCntModel].mtxWorld);

		//マテリアルのデータのポイントを取得
		pMat = (D3DXMATERIAL*)g_player.aPlayerModel[nCntModel].pBuffMatPlayer->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aPlayerModel[nCntModel].dwNumMatPlayer; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_player.aPlayerModel[nCntModel].apTexturePlayer[nCntMat]);

			//モデル(パーツ)の描画
			g_player.aPlayerModel[nCntModel].pMeshPlayer->DrawSubset(nCntMat);
		}
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//===========================
//プレイヤーの描画処理
//===========================
void SetPlayer(void)
{
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.point = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);
	g_player.state = PLAYERSTATE_NORMAL;
	g_player.motionState = PLAYERMOTION_MOVE;
	g_player.motionStateOld = PLAYERMOTION_MOVE;
	g_player.nLife = 50;
	g_player.nTimerState = 0;
	g_player.fAngle = 0.0f;
	g_nCntFrame = 0;
	g_nCntKey = 0;
	g_nSE = 0;

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		g_player.aPlayerModel[nCntModel].pos = g_player.aPlayerModel[nCntModel].posMotion;
		g_player.aPlayerModel[nCntModel].rot = g_player.aPlayerModel[nCntModel].rotMotion;
		g_player.aPlayerModel[nCntModel].rot = g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel];
	}
}

//===========================
//プレイヤーの描画処理
//===========================
Player *GetPlayer(void)
{
	return &g_player;
}

//===========================
//プレイヤーの描画処理
//===========================
void MovePlayer(void)
{
	Camera pCamera = GetCamera();
	int nCollisionModel = CollisionModel(&g_player.pos, &g_player.posOld, &g_player.move, 0);

	switch (nCollisionModel)
	{
	case -1:
		g_nSE++;
		if (g_nSE >= 60)
		{
			g_nSE = 0;
			StopSound(SOUND_LABEL_SE_RUN);
			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_RUN);
		}
		break;
	case 0:
		//サウンドの再生
		PlaySound(SOUND_LABEL_SE_BOU);
		//サウンドの再生
		PlaySound(SOUND_LABEL_SE_FANFARE);
		SetResult(RESULT_CLEAR);
		SetGameState(GAMESTATE_END, 0);
		break;
	case 6:
		g_nSE++;
		if (g_nSE >= 30)
		{
			g_nSE = 0;
			StopSound(SOUND_LABEL_SE_DORO);
			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_DORO);
		}
		g_player.move.x -= sinf(pCamera.rot.y) * 0.8f;
		g_player.move.z -= cosf(pCamera.rot.y) * 0.8f;
		break;
	default:
		g_nSE = 0;
		StopSound(SOUND_LABEL_SE_RUN);
		//サウンドの再生
		PlaySound(SOUND_LABEL_SE_SLIP);
		g_player.motionState = PLAYERMOTION_ACTION;
		g_player.move.x -= sinf(pCamera.rot.y) * 30.0f;
		g_player.move.z -= cosf(pCamera.rot.y) * 30.0f;
		break;
	}

	g_player.move.y -= 0.6f;

	g_player.pos += g_player.move;

	g_player.move.x += (0.0f - g_player.move.x) * 0.15f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.15f;

	if (g_player.pos.x > 150.0f)
	{
		g_player.move.x = 0.0f;
		g_player.pos.x = 150.0f;
	}

	if (g_player.pos.x < -150.0f)
	{
		g_player.move.x = 0.0f;
		g_player.pos.x = -150.0f;
	}

	if (g_player.pos.y < 0.0f)
	{
		g_player.move.y = 0.0f;
		g_player.pos.y = 0.0f;

		if (g_player.motionState == PLAYERMOTION_JUMP)
		{
			g_player.motionState = PLAYERMOTION_MOVE;
		}
	}

	if (g_player.pos.z < -12000.0f)
	{
		SetResult(RESULT_OVER);
		SetGameState(GAMESTATE_END, 0);
	}
}

//===========================
//プレイヤーの描画処理
//===========================
void ConPlayer(void)
{
	Camera pCamera = GetCamera();
	g_player.posOld = g_player.pos;

	g_player.fAngle = 0.0f;

	if (g_player.motionState != PLAYERMOTION_ACTION)
	{
		g_player.move.x += sinf(pCamera.rot.y);
		g_player.move.z += cosf(pCamera.rot.y);
		g_player.fAngle = D3DX_PI * 0.0f;

		if (GetKeyboardPress(DIK_A) == true)
		{//Aキーが押された時
			g_player.move.x += sinf(pCamera.rot.y - (D3DX_PI * 0.27f));
			g_player.fAngle = D3DX_PI * -0.5f;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//Dキーが押された時
			g_player.move.x += sinf(pCamera.rot.y + (D3DX_PI * 0.27f));
			g_player.fAngle = D3DX_PI * 0.5f;
		}

		if (GetKeyboardTrigger(DIK_SPACE) == true && g_player.motionState != PLAYERMOTION_JUMP)
		{//Wキーが押された時
			g_nSE = 10;
			StopSound(SOUND_LABEL_SE_RUN);
		 //サウンドの再生
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_player.move.y += 10.0f;
			g_player.motionState = PLAYERMOTION_JUMP;
		}
	}
}

//===========================
//プレイヤーの描画処理
//===========================
void ConPlayerPad(void)
{
	Camera pCamera = GetCamera();
	int nLStickLR = GetPadLStickLRPress();
	int nLStickUD = GetPadLStickUDPress();
	float fLength;
	g_player.posOld = g_player.pos;

	//対角線の長さを算出する
	fLength = sqrtf(((float)nLStickLR * (float)nLStickLR) + ((float)nLStickUD * (float)nLStickUD)) * 0.5f;

	if (g_player.motionState != PLAYERMOTION_ACTION)
	{
		nLStickUD = nLStickUD * -1;
		if (g_player.motionState != PLAYERMOTION_ACTION)
		{
			//対角線の角度を算出する
			g_player.fAngle = atan2f((float)nLStickLR, (float)nLStickUD);
		}

		if (g_player.motionState != PLAYERMOTION_ACTION)
		{
			g_player.move.x += sinf(pCamera.rot.y + g_player.fAngle) * (fLength * 0.0015f);
		}

		if (GetPadButtonTrigger(0) == true && g_player.motionState != PLAYERMOTION_JUMP)
		{//Wキーが押された時
			g_nSE = 10;
			StopSound(SOUND_LABEL_SE_RUN);
		 //サウンドの再生
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_player.move.y += 10.0f;
			g_player.motionState = PLAYERMOTION_JUMP;
		}
	}
}

//===========================
//プレイヤーの描画処理
//===========================
bool CollisionPlayer(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, int nType)
{
	bool bCollision;
	float fLength;

	bCollision = false;

	fLength = ((g_player.pos.x - pos->x) * (g_player.pos.x - pos->x)) + ((g_player.pos.z - pos->z) * (g_player.pos.z - pos->z));

	if (fLength <= 800.0f && pos->y >= g_player.pos.y && pos->y <= g_player.pos.y + 48.0f)
	{
		if (nType == 2 && g_player.state != PLAYERSTATE_DEATH)
		{
			bCollision = true;

			if (g_player.nLife <= 0)
			{
				g_player.state = PLAYERSTATE_DEATH;
				SetResult(RESULT_OVER);
				SetGameState(GAMESTATE_END, 0);
			}
			else
			{
				g_player.state = PLAYERSTATE_DAMAGE;
				g_player.nTimerState = 10;
				g_player.nLife--;
			}
		}
	}

	return bCollision;
}

//===========================
//プレイヤーの描画処理
//===========================
void ModelMotion(void)
{
	if (g_nCntKey < g_player.aPlayerMotion[g_player.motionState].nNumKey)
	{
		if (g_nCntFrame < g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame)
		{
			if (g_nCntKey == g_player.aPlayerMotion[g_player.motionState].nNumKey - 1)
			{
				for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
				{
					g_player.aPlayerModel[nCntModel].pos.x += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[0].pos[nCntModel].x - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].pos[nCntModel].x) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
					g_player.aPlayerModel[nCntModel].pos.y += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[0].pos[nCntModel].y - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].pos[nCntModel].y) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
					g_player.aPlayerModel[nCntModel].pos.z += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[0].pos[nCntModel].z - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].pos[nCntModel].z) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;

					g_player.aPlayerModel[nCntModel].rot.x += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[0].rot[nCntModel].x - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel].x) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
					g_player.aPlayerModel[nCntModel].rot.y += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[0].rot[nCntModel].y - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel].y) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
					g_player.aPlayerModel[nCntModel].rot.z += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[0].rot[nCntModel].z - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel].z) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
				
					if (g_player.aPlayerModel[nCntModel].rot.x > 3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.x -= 6.28f;
					}
					else if (g_player.aPlayerModel[nCntModel].rot.x < -3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.x += 6.28f;
					}

					if (g_player.aPlayerModel[nCntModel].rot.y > 3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.y -= 6.28f;
					}
					else if (g_player.aPlayerModel[nCntModel].rot.y < -3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.y += 6.28f;
					}

					if (g_player.aPlayerModel[nCntModel].rot.z > 3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.z -= 6.28f;
					}
					else if (g_player.aPlayerModel[nCntModel].rot.z < -3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.z += 6.28f;
					}
				}
			}
			else
			{
				for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
				{
					g_player.aPlayerModel[nCntModel].pos.x += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey + 1].pos[nCntModel].x - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].pos[nCntModel].x) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
					g_player.aPlayerModel[nCntModel].pos.y += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey + 1].pos[nCntModel].y - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].pos[nCntModel].y) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
					g_player.aPlayerModel[nCntModel].pos.z += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey + 1].pos[nCntModel].z - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].pos[nCntModel].z) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;

					g_player.aPlayerModel[nCntModel].rot.x += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey + 1].rot[nCntModel].x - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel].x) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
					g_player.aPlayerModel[nCntModel].rot.y += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey + 1].rot[nCntModel].y - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel].y) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
					g_player.aPlayerModel[nCntModel].rot.z += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey + 1].rot[nCntModel].z - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel].z) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
				
					if (g_player.aPlayerModel[nCntModel].rot.x > 3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.x -= 6.28f;
					}
					else if (g_player.aPlayerModel[nCntModel].rot.x < -3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.x += 6.28f;
					}

					if (g_player.aPlayerModel[nCntModel].rot.y > 3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.y -= 6.28f;
					}
					else if (g_player.aPlayerModel[nCntModel].rot.y < -3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.y += 6.28f;
					}

					if (g_player.aPlayerModel[nCntModel].rot.z > 3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.z -= 6.28f;
					}
					else if (g_player.aPlayerModel[nCntModel].rot.z < -3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.z += 6.28f;
					}
				}
			}

			g_nCntFrame++;
		}
		else
		{
			g_nCntKey++;
			g_nCntFrame = 0;

			for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
			{
				g_player.aPlayerModel[nCntModel].rotMotion = g_player.aPlayerModel[nCntModel].rot;
			}
		}
	}
	else
	{
		if (g_player.aPlayerMotion[g_player.motionState].nLoop == 0)
		{
			g_player.motionState = PLAYERMOTION_MOVE;
		}
		else
		{
			g_nCntKey = 0;
			g_nCntFrame = 0;
		}
	}

	if (g_player.motionState != g_player.motionStateOld)
	{
		g_nCntKey = 0;
		g_nCntFrame = 0;
		for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
		{
			g_player.aPlayerModel[nCntModel].pos = g_player.aPlayerModel[nCntModel].posMotion;
			g_player.aPlayerModel[nCntModel].rot = g_player.aPlayerModel[nCntModel].rotMotion;
			g_player.aPlayerModel[nCntModel].rot = g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel];
		}
	}
}

//===========================
//プレイヤーの描画処理
//===========================
void SetRot(void)
{
	float fRotMove, fRotDest, fRotDiff;

	fRotMove = g_player.rot.y;
	fRotDest = g_player.point.y;
	fRotDiff = fRotDest - fRotMove;

	if (fRotDiff > 3.14f)
	{
		fRotDiff -= 6.28f;
	}
	else if (fRotDiff <= -3.14f)
	{
		fRotDiff += 6.28f;
	}

	fRotMove += fRotDiff * 0.2f;

	if (fRotMove > 3.14f)
	{
		fRotMove -= 6.28f;
	}
	else if (fRotMove <= -3.14f)
	{
		fRotMove += 6.28f;
	}

	g_player.rot.y = fRotMove;
}

//===========================
//プレイヤーの描画処理
//===========================
void ReadFilePlayer(void)
{
	char aText[128];
	FILE *pFile = fopen("data\\MOTION\\motion_dessan_HGS.txt", "r");

	if (pFile == NULL)
	{
		return;
	}

	for (int nCnt = 0; nCnt < 11; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	fscanf(pFile, "%s = %d\n", &aText[0], &g_player.nNumModel);

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	for (int nCnt = 0; nCnt < g_player.nNumModel; nCnt++)
	{
		fscanf(pFile, "%s = %s		%s %s\n", &aText[0], &g_aModelName[nCnt][0], &aText[0], &aText[0]);
	}

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	fgets(&aText[0], 128, pFile);
	fscanf(pFile, "%s = %f	%s %s", &aText[0], &g_player.fMove, &aText[0], &aText[0]);
	fscanf(pFile, "%s = %f	%s %s", &aText[0], &g_player.fJump, &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %d	%s %s\n", &aText[0], &g_player.nNumModel, &aText[0], &aText[0]);

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		fgets(&aText[0], 128, pFile);
		fscanf(pFile, "%s = %d", &aText[0], &g_player.aPlayerModel[nCntModel].nIdxModelParent);
		fscanf(pFile, "%s = %d	%s %s", &aText[0], &g_player.aPlayerModel[nCntModel].nIdxModelParent, &aText[0], &aText[0]);
		fscanf(pFile, "%s = %f %f %f", &aText[0], &g_player.aPlayerModel[nCntModel].posMotion.x, &g_player.aPlayerModel[nCntModel].posMotion.y, &g_player.aPlayerModel[nCntModel].posMotion.z);
		fscanf(pFile, "%s = %f %f %f\n", &aText[0], &g_player.aPlayerModel[nCntModel].rotMotion.x, &g_player.aPlayerModel[nCntModel].rotMotion.y, &g_player.aPlayerModel[nCntModel].rotMotion.z);
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
		fscanf(pFile, "%s = %d	%s %s", &aText[0], &g_player.aPlayerMotion[nCntMotion].nLoop, &aText[0], &aText[0]);
		fscanf(pFile, "%s = %d	%s %s\n", &aText[0], &g_player.aPlayerMotion[nCntMotion].nNumKey, &aText[0], &aText[0]);

		for (int nCntKey = 0; nCntKey < g_player.aPlayerMotion[nCntMotion].nNumKey; nCntKey++)
		{
			fgets(&aText[0], 128, pFile);
			fscanf(pFile, "%s = %d\n", &aText[0], &g_player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].nFrame);

			for (int nCnt = 0; nCnt < g_player.nNumModel; nCnt++)
			{
				fgets(&aText[0], 128, pFile);
				fscanf(pFile, "%s = %f %f %f", &aText[0], &g_player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].x, &g_player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].y, &g_player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].z);
				fscanf(pFile, "%s = %f %f %f\n", &aText[0], &g_player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].x, &g_player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].y, &g_player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].z);
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
