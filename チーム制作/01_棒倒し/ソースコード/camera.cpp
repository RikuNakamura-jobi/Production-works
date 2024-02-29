//===========================
//
//ダイレクトX.cameraファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "game.h"

//プロトタイプ宣言

//グローバル変数宣言
Camera g_camera;
int g_nTimerRot;
bool g_follow;

//===========================
//プレイヤーの初期化処理
//===========================
void InitCamera(void)
{
	//3人称
	g_camera.posV = D3DXVECTOR3(0.0f, 100.0f, -400.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 100.0f, 200.0f);
	g_camera.posVDest = D3DXVECTOR3(0.0f, 100.0f, -400.0f);
	g_camera.posRDest = D3DXVECTOR3(0.0f, 100.0f, 200.0f);
	g_camera.rot = D3DXVECTOR3(0.0f, 3.14f, 2.0f);
	g_camera.rotOld = D3DXVECTOR3(0.0f, 3.14f, 2.0f);

	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	g_nTimerRot = 0;

	g_follow = true;

	RotCamera();
}

//===========================
//プレイヤーの初期化処理
//===========================
void InitCameraTitle(void)
{
	//3人称
	g_camera.posV = D3DXVECTOR3(0.0f, 100.0f, -400.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 200.0f, -400.0f);
	g_camera.posVDest = D3DXVECTOR3(0.0f, 100.0f, -400.0f);
	g_camera.posRDest = D3DXVECTOR3(0.0f, 200.0f, -400.0f);
	g_camera.rot = D3DXVECTOR3(0.0f, 3.14f, 3.14f);
	g_camera.rotOld = D3DXVECTOR3(0.0f, 3.14f, 3.14f);

	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	g_nTimerRot = 0;

	g_follow = true;

	RotCamera();
}

//===========================
//プレイヤーの終了処理
//===========================
void UninitCamera(void)
{

}

//===========================
//プレイヤーの更新処理
//===========================
void UpdateCamera(void)
{
	Player *pPlayer = GetPlayer();
	int nRStickLR = GetPadRStickLRPress();
	int nRStickUD = GetPadRStickUDPress();
	g_camera.rotOld = g_camera.rot;

	RotCamera();
}

//===========================
//プレイヤーの更新処理
//===========================
void UpdateCameraTitle(void)
{
	Player *pPlayer = GetPlayer();
	int nRStickLR = GetPadRStickLRPress();
	int nRStickUD = GetPadRStickUDPress();
	g_camera.rotOld = g_camera.rot;

	RotCameraTitle();
}

//===========================
//プレイヤーの描画処理
//===========================
void SetCamera(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f, 16000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION,
		&g_camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW,
		&g_camera.mtxView);
}

//===========================
//プレイヤーの描画処理
//===========================
Camera GetCamera(void)
{
	return g_camera;
}

//===========================
//プレイヤーの描画処理
//===========================
void RotCamera(void)
{
	Player *pPlayer = GetPlayer();
	float fLengthCamera;

	float fRotMove, fRotDest, fRotDiff;

	g_camera.posVOld = g_camera.posV;
	g_camera.posROld = g_camera.posR;

	g_nTimerRot++;

	if (pPlayer->state != PLAYERSTATE_NORMAL)
	{
		g_nTimerRot = 0;
	}

	if (g_camera.rot.y > 3.14f)
	{
		g_camera.rot.y -= 6.28f;
	}
	else if (g_camera.rot.y < -3.14f)
	{
		g_camera.rot.y += 6.28f;
	}

	if (g_camera.rot.z < 0.0f)
	{
		g_camera.rot.z = 0.0f;
	}
	else if (g_camera.rot.z > 3.13f)
	{
		g_camera.rot.z = 3.13f;
	}

	fLengthCamera = 300.0f;

	g_camera.posRDest.x = pPlayer->pos.x - (sinf(pPlayer->rot.y) * 0.0f);
	g_camera.posRDest.y = 70.0f;
	g_camera.posRDest.z = pPlayer->pos.z - (cosf(pPlayer->rot.y) * 0.0f);

	g_camera.posVDest.x = g_camera.posRDest.x - (sinf(g_camera.rot.z) * sinf(g_camera.rot.y)) * fLengthCamera;
	g_camera.posVDest.y = g_camera.posRDest.y + 0.0f - (cosf(g_camera.rot.z) * fLengthCamera);
	g_camera.posVDest.z = g_camera.posRDest.z - (sinf(g_camera.rot.z) * cosf(g_camera.rot.y)) * fLengthCamera;

	g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x) * 0.8f;
	g_camera.posR.y += (g_camera.posRDest.y - g_camera.posR.y) * 0.8f;
	g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z) * 0.8f;

	g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * 0.6f;
	g_camera.posV.y += (g_camera.posVDest.y - g_camera.posV.y) * 0.6f;
	g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * 0.6f;

	if (g_nTimerRot == -100)
	{
		fRotMove = g_camera.rot.y;
		fRotDest = pPlayer->rot.y + 3.14f;
		fRotDiff = fRotDest - fRotMove;

		if (fRotDiff > 3.14f)
		{
			fRotDiff -= 6.28f;
		}
		else if (fRotDiff <= -3.14f)
		{
			fRotDiff += 6.28f;
		}

		fRotMove += fRotDiff * 0.02f;

		if (fRotMove > 3.14f)
		{
			fRotMove -= 6.28f;
		}
		else if (fRotMove <= -3.14f)
		{
			fRotMove += 6.28f;
		}

		g_camera.rot.y = fRotMove;
		g_camera.rot.z += (1.57f - g_camera.rot.z) * 0.02f;
	}
}

//===========================
//プレイヤーの描画処理
//===========================
void RotCameraTitle(void)
{
	Player *pPlayer = GetPlayer();
	float fLengthCamera;

	float fRotMove, fRotDest, fRotDiff;

	g_camera.posVOld = g_camera.posV;
	g_camera.posROld = g_camera.posR;

	g_nTimerRot++;

	if (pPlayer->state != PLAYERSTATE_NORMAL)
	{
		g_nTimerRot = 0;
	}

	if (g_camera.rot.y > 3.14f)
	{
		g_camera.rot.y -= 6.28f;
	}
	else if (g_camera.rot.y < -3.14f)
	{
		g_camera.rot.y += 6.28f;
	}

	if (g_camera.rot.z < 0.0f)
	{
		g_camera.rot.z = 0.0f;
	}
	else if (g_camera.rot.z > 3.13f)
	{
		g_camera.rot.z = 3.13f;
	}

	fLengthCamera = 10.0f;

	g_camera.posRDest.x = pPlayer->pos.x - (sinf(pPlayer->rot.y) * 0.0f);
	g_camera.posRDest.y = 70.0f;
	g_camera.posRDest.z = pPlayer->pos.z - (cosf(pPlayer->rot.y) * 0.0f);

	g_camera.posVDest.x = g_camera.posRDest.x - (sinf(g_camera.rot.z) * sinf(g_camera.rot.y)) * fLengthCamera;
	g_camera.posVDest.y = g_camera.posRDest.y + 0.0f - (cosf(g_camera.rot.z) * fLengthCamera);
	g_camera.posVDest.z = g_camera.posRDest.z - (sinf(g_camera.rot.z) * cosf(g_camera.rot.y)) * fLengthCamera;

	g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x) * 0.8f;
	g_camera.posR.y += (g_camera.posRDest.y - g_camera.posR.y) * 0.8f;
	g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z) * 0.8f;

	g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * 0.6f;
	g_camera.posV.y += (g_camera.posVDest.y - g_camera.posV.y) * 0.6f;
	g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * 0.6f;

	if (g_nTimerRot == -100)
	{
		fRotMove = g_camera.rot.y;
		fRotDest = pPlayer->rot.y + 3.14f;
		fRotDiff = fRotDest - fRotMove;

		if (fRotDiff > 3.14f)
		{
			fRotDiff -= 6.28f;
		}
		else if (fRotDiff <= -3.14f)
		{
			fRotDiff += 6.28f;
		}

		fRotMove += fRotDiff * 0.02f;

		if (fRotMove > 3.14f)
		{
			fRotMove -= 6.28f;
		}
		else if (fRotMove <= -3.14f)
		{
			fRotMove += 6.28f;
		}

		g_camera.rot.y = fRotMove;
		g_camera.rot.z += (1.57f - g_camera.rot.z) * 0.02f;
	}
}