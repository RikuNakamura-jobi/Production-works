//===========================
//
//ダイレクトX.playerファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "player.h"
#include "input.h"
#include "enemy.h"
#include "score.h"
#include "explosion.h"
#include "sound.h"
#include "game.h"
#include "effect.h"
#include "particle.h"
#include "bullet.h"

//プロトタイプ宣言
void BuffPlayer(void);

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;							//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;					//頂点情報を格納
Player g_player;													//プレイヤー構造体
float g_fLengthPlayer;												//対角線の長さ
float g_fAnglePlayer;												//対角線の角度
float g_fLengthPlayerMove = 0.0f;									//対角線の移動量

//===========================
//プレイヤーの初期化処理
//===========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\battinganim1.png",
		&g_pTexturePlayer);

	g_player.nCounterAnim = DEFAULT_NUMBER;							//カウンターを初期化する
	g_player.nPatternAnim = DEFAULT_NUMBER;							//パターンNo.を初期化する
	g_player.pos = D3DXVECTOR3(1159.0f, 480.0f, 0.0f);				//位置を初期化する
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//移動量を初期化する
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//向きを初期化する
	g_player.nLife = 5;
	g_player.nRemaining = 3;
	g_player.state = PLAYERSTATE_APPEAR;
	g_player.nCounterState = 60;
	g_player.bDisp = true;											//使用していない状態にする

	//対角線の長さを算出する
	g_fLengthPlayer = sqrtf((PLAYER_X * PLAYER_X) + (PLAYER_Y * PLAYER_Y)) * 0.5f;

	//対角線の角度を算出する
	g_fAnglePlayer = atan2f(PLAYER_X, PLAYER_Y);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_player.pos.x + sinf(g_player.rot.z - D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_player.pos.y + cosf(g_player.rot.z - D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.pos.x + sinf(g_player.rot.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_player.pos.y + cosf(g_player.rot.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.pos.x + sinf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_player.pos.y + cosf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.pos.x + sinf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_player.pos.y + cosf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	
	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
	pVtx[1].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
	pVtx[2].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
	pVtx[3].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.2f, 0.5f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//===========================
//プレイヤーの終了処理
//===========================
void UninitPlayer(void)
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//===========================
//プレイヤーの更新処理
//===========================
void UpdatePlayer(void)
{
	Enemy aENEMY;
	D3DXVECTOR3 playerpos;
	D3DXVECTOR3 posBullet;
	D3DXVECTOR3 moveBullet;
	Bullet *pBullet = GetBullet();

	int nData = rand() % 21 - 10;

	switch (g_player.state)
	{//状態判定
	case PLAYERSTATE_APPEAR:
		g_player.state = PLAYERSTATE_NORMAL;
		break;
	case PLAYERSTATE_NORMAL:
		break;
	case PLAYERSTATE_DAMAGE:
		g_player.nCounterAnim++;				//カウンターを加算

		if (g_player.nCounterAnim <= 30)
		{
			//パターンN0.を更新する
			g_player.nPatternAnim = ((g_player.nCounterAnim + 1) / PERIOD_PLAYER) % FRAME_TEN;

			BuffPlayer();
		}
		else
		{
			g_player.nCounterAnim = 0;

			g_player.state = PLAYERSTATE_NORMAL;
		}
		break;
	case PLAYERSTATE_WAIT:
		g_player.nCounterState--;
		if (g_player.nCounterState <= 0)
		{
			g_player.nCounterState = 60;
			g_player.pos = D3DXVECTOR3(640.0f, 600.0f, 0.0f);				//位置を初期化する
			g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//移動量を初期化する
			g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//向きを初期化する
			g_player.nLife = 5;
			BuffPlayer();
			g_player.bDisp = true;
			g_player.state = PLAYERSTATE_APPEAR;
		}
		return;
		break;
	case PLAYERSTATE_DEATH:
		return;
		break;
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		g_player.state = PLAYERSTATE_DAMAGE;

		for (int nCntBullet = 0; nCntBullet < 256; nCntBullet++, pBullet++)
		{
			if (pBullet->bUse == true)
			{
				CollisionPlayer(pBullet);
			}
		}
	}
}

//===========================
//プレイヤーの描画処理
//===========================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_pVtxBuffPlayer,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	if (g_player.bDisp == true)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//===========================
//頂点バッファの調整処理
//===========================
void BuffPlayer(void)
{	
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((0.2f * g_player.nPatternAnim), 0.5f * (g_player.nPatternAnim / 5));
	pVtx[1].tex = D3DXVECTOR2((0.2f  * g_player.nPatternAnim) + 0.2f, 0.5f *  (g_player.nPatternAnim / 5));
	pVtx[2].tex = D3DXVECTOR2((0.2f * g_player.nPatternAnim), (0.5f *  (g_player.nPatternAnim / 5)) + 0.5f);
	pVtx[3].tex = D3DXVECTOR2((0.2f * g_player.nPatternAnim) + 0.2f, (0.5f *  (g_player.nPatternAnim / 5)) + 0.5f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//===========================
//プレイヤー状態の取得処理
//===========================
Player *GetPlayer(void)
{
	return &g_player;
}

//===========================
//プレイヤーのヒット処理
//===========================
void HitPlayer(int nDamage)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	Enemy aENEMY;

	g_player.nLife -= nDamage;

	if (g_player.nLife > 5)
	{
		g_player.nLife = 5;
	}

	if (g_player.nLife <= 0)
	{
		//サウンドの再生
		PlaySound(SOUND_LABEL_SE_EXPLOSION);

		SetExplosion(g_player.pos, col, 0);
		SetExplosion(g_player.pos + D3DXVECTOR3(10.0f, 20.0f, 0.0f), col, 0);
		SetExplosion(g_player.pos + D3DXVECTOR3(-20.0f, 20.0f, 0.0f), col, 0);
		SetExplosion(g_player.pos + D3DXVECTOR3(20.0f, -10.0f, 0.0f), col, 0);
		SetExplosion(g_player.pos + D3DXVECTOR3(-30.0f, -20.0f, 0.0f), col, 0);

		if (g_player.nRemaining <= 0)
		{
			g_player.state = PLAYERSTATE_DEATH;
			g_player.bDisp = false;
			SetGameState(GAMESTATE_END, 60);
		}
		else
		{
			g_player.nRemaining--;
			g_player.nCounterState = 60;
			g_player.state = PLAYERSTATE_WAIT;
			g_player.bDisp = false;
		}
	}
	else
	{
		g_player.state = PLAYERSTATE_DAMAGE;
		g_player.nCounterState = 5;

		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		g_pVtxBuffPlayer->Unlock();
	}
}