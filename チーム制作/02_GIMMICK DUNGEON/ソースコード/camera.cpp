//========================================
//
//2Dアクションゲーム(カメラ処理)[item.cpp]
//Author:佐久間優香
//
//========================================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "meshfield.h"
#include "menu.h"
#include "tutorial.h"
#include "enemy.h"
#include "game.h"
#include "result.h"
#include "sound.h"
#include "book.h"
#include "fade.h"

//==========================================
//  マクロ定義
//==========================================
#define GAME_LENGTH	(270.0f)			//ゲーム画面での視点注視点間の距離
#define NOT_GAME_LENGTH	(10.0f)		//ゲーム画面以外での視点注視点間の距離
#define MOVE	(0.05f)					//回転する速度
#define MOVE_SPEED	(2.0f)				//移動するスピード
#define UPROT_MAX	(D3DX_PI * 0.9f)	//下の方向の角度の上限
#define DOWNROT_MIN	(D3DX_PI * 0.1f)	//上の方向の角度の上限
#define POSR_HEGHT	(70.0f)				//注視点の高さ
#define CAMERA_RADIUS	(20.0f)	//カメラの判定の半径
#define CAMERA_SPEED	(0.10f)	//メニュー画面のカメラの速度
#define FLOOR_HEIGHT	(2.2f)	//床の判定の高さ
#define QUAKE_LENGE	(0.04f)	//振動の倍率
#define NORMAL_FACT	(0.1f)	//通常時のカメラの移動慣性
#define END_FACT	(0.05f)	//終了時のカメラの移動慣性

//==========================================
//  グローバル変数宣言
//==========================================
Camera g_camera;					//カメラの情報
float g_fLengthCamera;				//注視点から視点までの距離

//========================================
//カメラの初期化処理
//========================================
void InitCamera(void)
{
	//変数初期化
	g_camera.posRHeight = POSR_HEGHT;

	switch (GetMode())
	{
	case MODE_GAME:
	case MODE_TUTORIAL:
		g_fLengthCamera = GAME_LENGTH;

		g_camera.posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//視点の初期化
		g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//注視点の初期化
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//上方向ベクトルの初期化
		g_camera.rot = D3DXVECTOR3(1.5f, D3DX_PI * 0.5f, atan2f(g_camera.posR.x - g_camera.posV.x, g_camera.posR.z - g_camera.posV.z));			//向きの初期化
		g_camera.posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//目的の視点の初期化
		g_camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//目的の注視点の初期化
		g_camera.bBehind = false;

		g_camera.posV.x = g_camera.posR.x + (sinf(g_camera.rot.x) * cosf(g_camera.rot.y))* g_fLengthCamera;
		g_camera.posV.y = g_camera.posR.y + cosf(g_camera.rot.x) * g_fLengthCamera;
		g_camera.posV.z = g_camera.posR.z + (sinf(g_camera.rot.x) * sinf(g_camera.rot.y))* g_fLengthCamera;
		break;

	case MODE_TITLE:
	case MODE_MENU:

		g_fLengthCamera = NOT_GAME_LENGTH;

		g_camera.posV = D3DXVECTOR3(0.0f, 680.0f, -1650.0f);		//視点の初期化
		g_camera.posR = D3DXVECTOR3(0.0f, 670.0f, -1500.0f);		//注視点の初期化
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//上方向ベクトルの初期化
		g_camera.bBehind = false;
		break;

	case MODE_RANKING:

		g_fLengthCamera = NOT_GAME_LENGTH;

		g_camera.posV = D3DXVECTOR3(0.0f, 50.0f, -30.0f);		//視点の初期化
		g_camera.posR = D3DXVECTOR3(0.0f, 50.0f, 200.0f);		//注視点の初期化
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//上方向ベクトルの初期化
		g_camera.bBehind = false;
		break;
	}
}

//========================================
//カメラの終了処理
//========================================
void UninitCamera(void)
{

}

//========================================
//カメラの更新処理
//========================================
void UpdateCamera(void)
{
	//ローカル変数宣言
	D3DXVECTOR3 mouseMove = GetMouseMove();
	TUTORIALSTATE *tutorial = GetTutorialState();
	bool bTutorial = false;
	float fMeshU = 0.0f;	//大人の壁判定用
	float fMeshV = 0.0f;	//大人の壁判定用

	switch (GetMode())
	{
	case MODE_GAME:
	case MODE_TUTORIAL:

		if (GetPhotoMode())
		{//フォトモードカメラ操作
			ControlCamera();
		}
		else
		{
			//目的の注視点を設定(移動)
			Move();
		}
		
		//画面振動
		if (g_camera.nQuakeTime > 0)
		{
			//時間を減らす
			g_camera.nQuakeTime--;

			//視点注視点を動かしてカメラを揺らす
			g_camera.posV.x += (float)(rand() % 201 - 100) * g_camera.fQuakeSize * sinf(g_camera.rot.y);
			g_camera.posV.y += (float)(rand() % 101 - 50) * g_camera.fQuakeSize;
			g_camera.posV.z += (float)(rand() % 201 - 100) * g_camera.fQuakeSize * cosf(g_camera.rot.y);

			g_camera.posR.x += (float)(rand() % 201 - 100) * g_camera.fQuakeSize * sinf(g_camera.rot.y);
			g_camera.posR.y += (float)(rand() % 101 - 50) * g_camera.fQuakeSize;
			g_camera.posR.z += (float)(rand() % 201 - 100) * g_camera.fQuakeSize * cosf(g_camera.rot.y);

			//徐々に揺れのサイズを小さくする
			g_camera.fQuakeSize += (0.0f - g_camera.fQuakeSize) * 0.03f;
		}

		if (GetGameState() == GAMESTATE_NORMAL && (*GetBookState() == BOOKSTATE_NONE || *GetBookState() == BOOKSTATE_END) && GetFade() == FADE_NONE)
		{
			//視点操作====================================
			if (GetKeyboardPress(DIK_LEFT) == true)
			{//左向く
				g_camera.rot.y += MOVE;
				bTutorial = true;
			}
			else if (GetKeyboardPress(DIK_RIGHT) == true)
			{//右向く
				g_camera.rot.y -= MOVE;
				bTutorial = true;
			}
			else if (GetKeyboardPress(DIK_DOWN) == true)
			{//カメラ上げる
				g_camera.rot.x -= MOVE;
				//g_fPosRHeight -= MOVE * 20;
				bTutorial = true;
			}
			else if (GetKeyboardPress(DIK_UP) == true)
			{//カメラ下げる
				g_camera.rot.x += MOVE;
				//g_fPosRHeight += MOVE * 20;
				bTutorial = true;
			}
			//視点操作====================================

			//マウスの注視点操作
			g_camera.rot.x += mouseMove.y;
			g_camera.rot.y += mouseMove.x;
		}

		//回転の制限
		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y += -D3DX_PI * 2;
		}
		else if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2;
		}

		if (g_camera.rot.x > D3DX_PI)
		{
			g_camera.rot.x += -D3DX_PI * 2;
		}
		else if (g_camera.rot.x < -D3DX_PI)
		{
			g_camera.rot.x += D3DX_PI * 2;
		}

		//カメラの上下回転の上限
		if (g_camera.rot.x > UPROT_MAX)
		{//上
			g_camera.rot.x = UPROT_MAX;
		}
		if (g_camera.rot.x < DOWNROT_MIN)
		{//下
			g_camera.rot.x = DOWNROT_MIN;
		}

		if (GetMode() == MODE_GAME)
		{//大人の壁サイズ設定
			fMeshU = MESH_U;
			fMeshV = MESH_V;
		}
		else
		{
			fMeshU = MESH_TUTORIAL_U;
			fMeshV = MESH_TUTORIAL_V;
		}

		if (g_camera.posV.x >(FIELD_RANGE * fMeshU) * 0.5f - CAMERA_RADIUS)
		{//エリア右の制限
			g_camera.posV.x = (FIELD_RANGE * fMeshU) * 0.5f - CAMERA_RADIUS;
		}
		else if (g_camera.posV.x < -(FIELD_RANGE * fMeshU) * 0.5f + CAMERA_RADIUS)
		{//エリア左の制限
			g_camera.posV.x = -(FIELD_RANGE * fMeshU) * 0.5f + CAMERA_RADIUS;
		}

		if (g_camera.posV.z >(FIELD_RANGE * fMeshV) * 0.5f - CAMERA_RADIUS)
		{//エリア奥の制限
			g_camera.posV.z = (FIELD_RANGE * fMeshV) * 0.5f - CAMERA_RADIUS;
		}
		else if (g_camera.posV.z < -(FIELD_RANGE * fMeshV) * 0.5f + CAMERA_RADIUS)
		{//エリア手前の制限
			g_camera.posV.z = -(FIELD_RANGE * fMeshV) * 0.5f + CAMERA_RADIUS;
		}

		if (g_camera.posR.x >(FIELD_RANGE * fMeshU) * 0.5f)
		{//エリア右の制限
			g_camera.posR.x = (FIELD_RANGE * fMeshU) * 0.5f;
		}
		else if (g_camera.posR.x < -(FIELD_RANGE * fMeshU) * 0.5f)
		{//エリア左の制限
			g_camera.posR.x = -(FIELD_RANGE * fMeshU) * 0.5f;
		}

		if (g_camera.posR.z >(FIELD_RANGE * fMeshV) * 0.5f)
		{//エリア奥の制限
			g_camera.posR.z = (FIELD_RANGE * fMeshV) * 0.5f;
		}
		else if (g_camera.posR.z < -(FIELD_RANGE * fMeshV) * 0.5f)
		{//エリア手前の制限
			g_camera.posR.z = -(FIELD_RANGE * fMeshV) * 0.5f;
		}

		if (g_camera.posV.y < FLOOR_HEIGHT)
		{//床判定
			g_camera.posV.y = FLOOR_HEIGHT;
		}

#ifdef _DEBUG
		//デバッグ操作
		if (GetKeyboardPress(DIK_U))
		{
			g_fLengthCamera -= 5.0f;
		}
		if (GetKeyboardPress(DIK_J))
		{
			g_fLengthCamera += 5.0f;
		}
#endif

		break;
	case MODE_TITLE:
		RotMove();
		break;
	case MODE_RANKING:
		//固定
		break;

	}


}

//========================================
// 操作処理
//========================================
void ControlCamera(void)
{
	//変数宣言
	D3DXVECTOR3 RDiff, VDiff;
	float fSpeed;

	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//加速状態
		fSpeed = MOVE_SPEED * 5;
	}
	else
	{
		fSpeed = MOVE_SPEED;
	}

	//視点移動===============================================
	if (GetKeyboardPress(DIK_A) == true)
	{//左移動
		g_camera.posRDest.x -= sinf(g_camera.rot.y + D3DX_PI * -0.5f) * fSpeed;
		g_camera.posRDest.z -= cosf(g_camera.rot.y + D3DX_PI * -0.5f) * fSpeed;
	}
	if (GetKeyboardPress(DIK_D) == true)
	{//右移動
		g_camera.posRDest.x -= sinf(g_camera.rot.y - D3DX_PI * -0.5f) * fSpeed;
		g_camera.posRDest.z -= cosf(g_camera.rot.y - D3DX_PI * -0.5f) * fSpeed;
	}
	if (GetKeyboardPress(DIK_W) == true)
	{//前移動
		g_camera.posRDest.x -= sinf(g_camera.rot.y) * fSpeed;
		g_camera.posRDest.z -= cosf(g_camera.rot.y) * fSpeed;
	}
	if (GetKeyboardPress(DIK_S) == true)
	{//後移動
		g_camera.posRDest.x -= sinf(g_camera.rot.y + D3DX_PI) * fSpeed;
		g_camera.posRDest.z -= cosf(g_camera.rot.y + D3DX_PI) * fSpeed;
	}
	if (GetKeyboardPress(DIK_T) == true)
	{//上移動
		g_camera.posRDest.y += fSpeed;
	}
	if (GetKeyboardPress(DIK_B) == true)
	{//下移動
		g_camera.posRDest.y -= fSpeed;
	}

	//視点回転===============================================
	if (GetKeyboardPress(DIK_Q) == true)
	{//左を向く
		g_camera.rot.y -= MOVE;
		//注視点を相対位置に設定
	}
	if (GetKeyboardPress(DIK_E) == true)
	{//右を向く
		g_camera.rot.y += MOVE;
		//注視点を相対位置に設定
	}

	//縦回転====================================================================
	if (GetKeyboardPress(DIK_N) == true)
	{//下
		g_camera.rot.x += MOVE;

		//注視点を相対位置に設定

		if (g_camera.rot.x < 0.1f)
		{
			g_camera.rot.x = 0.1f;
		}
	}
	if (GetKeyboardPress(DIK_Y) == true)
	{//上
		g_camera.rot.x -= MOVE;
		//注視点を相対位置に設定

		if (g_camera.rot.x > 3.0f)
		{
			g_camera.rot.x = 3.0f;
		}
	}

	g_camera.posVDest =
	{
		g_camera.posRDest.x + sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_fLengthCamera,
		g_camera.posRDest.y + cosf(g_camera.rot.x) * g_fLengthCamera,
		g_camera.posRDest.z + sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_fLengthCamera
	};

	//注視点の差分
	RDiff = g_camera.posRDest - g_camera.posR;

	//視点の差分
	VDiff = g_camera.posVDest - g_camera.posV;

	//慣性
	g_camera.posR += RDiff * NORMAL_FACT;
	g_camera.posV += VDiff * NORMAL_FACT;
}

//========================================
//カメラの描画処理
//========================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(54.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		4243.0f);

	//プロジェクトマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//========================================
//カメラの取得処理
//========================================
Camera *GetCamera(void)
{
	return &g_camera;
}

//========================================
//目的の注視点を設定(移動) 
//========================================
void Move(void)
{
	D3DXVECTOR3 VDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 RDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fSlow = 0.0f;	//慣性

	Player *pPlayer = GetPlayer();

	if (GetGameState() != GAMESTATE_END)
	{//ゲーム通常時の挙動
		g_camera.posRDest = D3DXVECTOR3
		(
			pPlayer->pos.x + sinf(pPlayer->rot.y) * -20.0f,
			pPlayer->pos.y + g_camera.posRHeight,
			pPlayer->pos.z + cosf(pPlayer->rot.y) * -20.0f
		);

		//目的の視点を設定
		g_camera.posVDest =
		{
			g_camera.posRDest.x + sinf(g_camera.rot.x) * sinf(-g_camera.rot.y + D3DX_PI * 0.5f) * g_fLengthCamera,
			pPlayer->pos.y + cosf(g_camera.rot.x) * g_fLengthCamera,
			g_camera.posRDest.z + sinf(g_camera.rot.x) * cosf(-g_camera.rot.y + D3DX_PI * 0.5f) * g_fLengthCamera
		};

		fSlow = NORMAL_FACT;
	}
	else
	{//ゲーム終了時の挙動
		GameEndCamera(GetResult());

		fSlow = END_FACT;
	}

	//注視点の補正
	RDiff = g_camera.posRDest - g_camera.posR;

	//視点の補正
	VDiff = g_camera.posVDest - g_camera.posV;

	//慣性
	g_camera.posR += RDiff * fSlow;
	g_camera.posV += VDiff * fSlow;
}

//========================================
//目的の注視点を設定(移動) 
//========================================
void RotMove()
{
	D3DXVECTOR3 VDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 RDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 pos;

	switch (GetMenuState())
	{
	case MENUSTATE_START:
		g_camera.posRDest = D3DXVECTOR3(0.0f, 670.0f, -1500.0f);
		g_camera.posVDest = D3DXVECTOR3(0.0f, 680.0f, -1650.0f);
		break;
	case MENUSTATE_TUTORIAL:
		g_camera.posRDest = D3DXVECTOR3(-130.0f, 620.0f, -1650.0f);
		g_camera.posVDest = D3DXVECTOR3(-80.0f, 650.0f, -1650.0f);
		break;
	case MENUSTATE_QUIT:
		g_camera.posRDest = D3DXVECTOR3(0.0f, 660.0f, -1800.0f);
		g_camera.posVDest = D3DXVECTOR3(0.0f, 680.0f, -1650.0f);
		break;
	case MENUSTATE_FADE:
		g_camera.posRDest = D3DXVECTOR3(0.0f, 670.0f, -1350.0f);
		break;
	}

	//注視点の補正
	RDiff = g_camera.posRDest - g_camera.posR;
	VDiff = g_camera.posVDest - g_camera.posV;

	//慣性
	if (GetMenuState() == MENUSTATE_FADE)
	{
		g_camera.posV += RDiff * 0.02f;
		g_camera.posR += RDiff * 0.02f;
	}
	else
	{
		g_camera.posV += VDiff * CAMERA_SPEED;
		g_camera.posR += RDiff * CAMERA_SPEED;
	}

}

//========================================
//目的の注視点を設定(移動) 
//========================================
void GameEndCamera(int nResult)
{
	D3DXVECTOR3 VDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 RDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 pos;

		if (nResult == RESULTTYPE_WIN)
		{
			Enemy *pEnemy = GetEnemy();

			g_camera.posRDest = D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y, pEnemy->pos.z);
			g_camera.posVDest = D3DXVECTOR3(pEnemy->pos.x + sinf(pEnemy->rot.y + (D3DX_PI * 1.0f)) * 800.0f, 
											300.0f, 
											pEnemy->pos.z + cosf(pEnemy->rot.y + (D3DX_PI * 1.0f)) * 800.0f);
		}
		else
		{
			Player *pPlayer = GetPlayer();

			g_camera.posRDest = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + 20.0f, pPlayer->pos.z);
			g_camera.posVDest = D3DXVECTOR3(pPlayer->pos.x + sinf(pPlayer->rot.y + (D3DX_PI * 1.0f)) * 100.0f,
											100.0f,
											pPlayer->pos.z + cosf(pPlayer->rot.y + (D3DX_PI * 1.0f)) * 100.0f);
		}
}

//========================================
//目的の注視点を設定(回転) 
//========================================
void Rot(void)
{
	D3DXVECTOR3 RotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 RotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Player *pPlayer = GetPlayer();

	RotDest.y = -pPlayer->rot.y + D3DX_PI * 0.5f;

	RotDiff.y = RotDest.y - g_camera.rot.y;

	//一周したときに戻す処理
	if (RotDiff.y > D3DX_PI)
	{
		RotDiff.y += -D3DX_PI * 2;
	}
	else if (RotDiff.y < -D3DX_PI)
	{
		RotDiff.y += D3DX_PI * 2;
	}

	g_camera.rot.y += RotDiff.y * 0.05f;

	if (pPlayer->rot.y > D3DX_PI)
	{
		pPlayer->rot.y += -D3DX_PI * 2;
	}
	else if (pPlayer->rot.y < -D3DX_PI)
	{
		pPlayer->rot.y += D3DX_PI * 2;
	}
}

//========================================
//視点をプレイヤーの後ろに動かす
//========================================
void RotPlayer(void)
{
	Player *pPlayer = GetPlayer();
	float fRotMove, fRotDest, fRotDiff;

	if (g_camera.bBehind == true)
	{
		fRotMove = g_camera.rot.y;
		fRotDest = -pPlayer->rot.y + D3DX_PI * 0.5f;
		fRotDiff = fRotDest - fRotMove;

		if (fRotDiff > 3.14f)
		{
			fRotDiff -= 6.28f;
		}
		else if (fRotDiff <= -3.14f)
		{
			fRotDiff += 6.28f;
		}

		fRotMove += fRotDiff * 0.1f;

		if (fRotMove > 3.14f)
		{
			fRotMove -= 6.28f;
		}
		else if (fRotMove <= -3.14f)
		{
			fRotMove += 6.28f;
		}

		g_camera.rot.y = fRotMove;
	}
}

//========================================
//カメラが揺れる処理
//========================================
void SetQuake(int time, float size)
{
	if (size > 0.0f)
	{
		//揺れる時間代入
		g_camera.nQuakeTime = time;

		//揺れの大きさ代入
		g_camera.fQuakeSize = size;
	}
}

//========================================
//カメラのデバッグ表示
//========================================
void DrawDebugCamera(LPD3DXFONT pFont)
{
	RECT rect4 = { 0,290,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "注視点 [ %.2f : %.2f : %.2f ]\n視点 [ %.2f : %.2f : %.2f ]\nカメラの距離　[%.2f]", g_camera.posR.x, g_camera.posR.y, g_camera.posR.z, g_camera.posV.x, g_camera.posV.y, g_camera.posV.z,g_fLengthCamera);

	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}
