//==========================================================
//
//プレイヤー処理 [player.cpp]
//Author:佐藤根詩音
//
//==========================================================
#include "player.h"
#include "input.h"
#include "debugproc.h"
#include "sound.h"
#include "score.h"
#include "enemy.h"
#include "game.h"

//マクロ定義
#define POS_X			(1200.0f)	//プレイヤーの位置X
#define POS_Y			(360.0f)	//プレイヤーの位置Y
#define ANGLE_UP		(0.75f)		//上の角度
#define ANGLE_DOWN		(0.25f)		//下の角度
#define MOVE			(0.5f)		//プレイヤーの移動量
#define MOVE_JUMP		(-0.7f)		//ジャンプの高さ
#define COL_UP			(20.0f)					//上の当たり判定
#define COL_DOWN		(SCREEN_HEIGHT - COL_UP)	//下の当たり判定
#define COL_L			(100.0f)					//左の当たり判定
#define COL_R			(SCREEN_WIDTH - COL_L)		//右の当たり判定
#define TEX_DIV			(4)					//テクスチャの分割数
#define TEX_POSX		(1.0f / TEX_DIV)	//テクスチャ座標X
#define TEX_POSY		(1.0f)				//テクスチャ座標Y

//プロトタイプ宣言
void ControlPlayer(void);
void ScreenPlayer(void);
void CollisionHLPlayer(void);
void CollisionLRPlayer(void);

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;			//テクスチャ1へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffplayer = NULL;	//頂点バッファへのポインタ
Player g_aPlayer;									//プレイヤーの情報

//==========================================================
//初期化処理
//==========================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\player000.png",
		&g_pTexturePlayer);

	//変数初期化
	g_aPlayer.pos = D3DXVECTOR3(POS_X, POS_Y, 0.0f);	//位置
	g_aPlayer.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//前回の位置
	g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	g_aPlayer.moveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//前回の移動量
	g_aPlayer.nSwitchLR = 1;		//プレイヤーの左右切り替え
	g_aPlayer.nCntAnim = 0;			//アニメーションカウンター
	g_aPlayer.nPatternAnim = 0;		//アニメーションNo.
	g_aPlayer.nCntGravity = 10;		//重力カウンター
	g_aPlayer.fSpeed = 0.0f;		//移動量
	g_aPlayer.fJump = 0.0f;			//ジャンプ力
	g_aPlayer.fLength = 0.0f;		//対角線の長さ
	g_aPlayer.fAngle = 0.0f;		//対角線の角度
	g_aPlayer.fWidth = WIDTH_PLAYER;		//幅
	g_aPlayer.fHeight = HEIGHT_PLAYER;		//高さ
	g_aPlayer.bJump = false;		//ジャンプしてるか
	g_aPlayer.bUse = true;			//使用してるか
	g_aPlayer.bDisp = true;			//描画するか
	g_aPlayer.state = PLAYER_APPEAR;	//出現状態にする

	//対角線の長さを算出する
	g_aPlayer.fLength = sqrtf(g_aPlayer.fWidth * g_aPlayer.fWidth + g_aPlayer.fHeight * g_aPlayer.fHeight) * 0.5f;

	//対角線の角度を算出する
	g_aPlayer.fAngle = atan2f(g_aPlayer.fWidth, g_aPlayer.fHeight);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffplayer,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffplayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPlayer.pos.x - g_aPlayer.fWidth, g_aPlayer.pos.y - g_aPlayer.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayer.pos.x + g_aPlayer.fWidth, g_aPlayer.pos.y - g_aPlayer.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayer.pos.x - g_aPlayer.fWidth, g_aPlayer.pos.y + g_aPlayer.fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayer.pos.x + g_aPlayer.fWidth, g_aPlayer.pos.y + g_aPlayer.fHeight, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 
	pVtx[1].rhw = 
	pVtx[2].rhw = 
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = 
	pVtx[1].col = 
	pVtx[2].col = 
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffplayer->Unlock();

}

//==========================================================
//終了処理
//==========================================================
void UninitPlayer(void)
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffplayer != NULL)
	{
		g_pVtxBuffplayer->Release();
		g_pVtxBuffplayer = NULL;
	}
}

//==========================================================
//更新処理
//==========================================================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	if (g_aPlayer.bUse == true && g_aPlayer.bDisp == true)
	{//使用してたら

		g_aPlayer.posOld = g_aPlayer.pos;		//現在の位置
		g_aPlayer.nCntAnim++;		//アニメーションカウンター加算

		//プレイヤーの操作
		ControlPlayer();

		//位置を更新
		g_aPlayer.pos.x += g_aPlayer.move.x;
		g_aPlayer.pos.y += g_aPlayer.move.y;

		//移動量を更新
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.1f;

		//向きを更新
		g_aPlayer.rot.z += g_aPlayer.move.z;

		//上下の当たり判定
		CollisionHLPlayer();

		//左右の当たり判定
		CollisionLRPlayer();

		//敵との当たり判定
		if (CollisionEnemy(g_aPlayer.pos, g_aPlayer.posOld) == true)
		{
			g_aPlayer.bUse = false;
			SetGameState(GAMESTATE_END, 0);

			PlaySound(SOUND_LABEL_DAMAGE);
		}

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffplayer->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aPlayer.pos.x - g_aPlayer.fWidth, g_aPlayer.pos.y - g_aPlayer.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPlayer.pos.x + g_aPlayer.fWidth, g_aPlayer.pos.y - g_aPlayer.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPlayer.pos.x - g_aPlayer.fWidth, g_aPlayer.pos.y + g_aPlayer.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPlayer.pos.x + g_aPlayer.fWidth, g_aPlayer.pos.y + g_aPlayer.fHeight, 0.0f);

		if ((g_aPlayer.nCntAnim % 30) == 0)
		{//一定時間経過

			if (g_aPlayer.nSwitchLR == 1)
			{
				//アニメーションNo.変更
				g_aPlayer.nPatternAnim = (g_aPlayer.nPatternAnim + 1) % TEX_DIV;

				if (g_aPlayer.nPatternAnim == 2)
				{
					g_aPlayer.nPatternAnim -= 2;
				}
			}
			else if (g_aPlayer.nSwitchLR == -1)
			{
				//アニメーションNo.変更
				g_aPlayer.nPatternAnim = (g_aPlayer.nPatternAnim + 1) % TEX_DIV;

				if (g_aPlayer.nPatternAnim == 0)
				{
					g_aPlayer.nPatternAnim += 2;
				}
			}

			//テクスチャ座標の再設定
			pVtx[0].tex = D3DXVECTOR2((g_aPlayer.nPatternAnim % TEX_DIV) * TEX_POSX, (g_aPlayer.nPatternAnim / 1) * TEX_POSY);
			pVtx[1].tex = D3DXVECTOR2((g_aPlayer.nPatternAnim % TEX_DIV) * TEX_POSX + TEX_POSX, (g_aPlayer.nPatternAnim / 1) * TEX_POSY);
			pVtx[2].tex = D3DXVECTOR2((g_aPlayer.nPatternAnim % TEX_DIV) * TEX_POSX, (g_aPlayer.nPatternAnim / 1) * TEX_POSY + TEX_POSY);
			pVtx[3].tex = D3DXVECTOR2((g_aPlayer.nPatternAnim % TEX_DIV) * TEX_POSX + TEX_POSX, (g_aPlayer.nPatternAnim / 1) * TEX_POSY + TEX_POSY);
		}

		//頂点バッファをアンロックする
		g_pVtxBuffplayer->Unlock();
	}

	PrintDebugProc("ジャンプ：[SPACE] / [A]\n");
}

//==========================================================
//プレイヤーの操作
//==========================================================
void ControlPlayer(void)
{
	if (GetKeyboardPress(DIK_SPACE) == true || GetGamepadPress(BUTTON_A, 0) == true)
	{//SPACEキーかAボタンが押されたら

		//サウンドの再生
		//PlaySound(SOUND_LABEL_SE_JUMP000);

		g_aPlayer.nCntGravity = 10;

		//上に上げる
		g_aPlayer.move.y += MOVE_JUMP;

		g_aPlayer.bJump = true;		//ジャンプしてる状態にする
	}
	else if (g_aPlayer.nCntGravity > 0 && g_aPlayer.bJump == true)
	{//重力の時間経ってなかったら

		if (g_aPlayer.nCntGravity > 1)
		{
			g_aPlayer.move.y += 0.5f;		//重力加算

		}
		else
		{
			g_aPlayer.move.y += (g_aPlayer.move.y * -1) + 0.3f;		//重力加算

		}

		g_aPlayer.nCntGravity--;		//カウンター減算

		if (g_aPlayer.nCntGravity <= 0)
		{
			g_aPlayer.bJump = false;		//ジャンプしてない状態にする
		}
	}
	else
	{
		//重力処理
		g_aPlayer.move.y += PLAYER_GRAVITY;
	}

	//左右に進む
	g_aPlayer.move.x -= MOVE * g_aPlayer.nSwitchLR * (g_aPlayer.pos.y * 0.002f);
}

//==========================================================
//画面外に出た時の処理
//==========================================================
void ScreenPlayer(void)
{
	//画面外に出た場合
	if (g_aPlayer.pos.x - WIDTH_PLAYER + 5.0f <= 0)
	{//左に出た場合
		g_aPlayer.pos.x = SCREEN_WIDTH - WIDTH_PLAYER;
		g_aPlayer.pos.y = 660.0f;
	}
	else if (g_aPlayer.pos.x + WIDTH_PLAYER - 5.0f >= SCREEN_WIDTH)
	{//右に出た場合
		g_aPlayer.pos.x = WIDTH_PLAYER;
		g_aPlayer.pos.y = 660.0f;
	}
}

//==========================================================
//上下の当たり判定
//==========================================================
void CollisionHLPlayer(void)
{
	if ((g_aPlayer.posOld.y - HEIGHT_PLAYER >= COL_UP && g_aPlayer.pos.y - HEIGHT_PLAYER < COL_UP) ||
		(g_aPlayer.posOld.y + HEIGHT_PLAYER <= COL_DOWN && g_aPlayer.pos.y + HEIGHT_PLAYER > COL_DOWN))
	{//上か下当たってたら
		g_aPlayer.bUse = false;
		SetGameState(GAMESTATE_END, 0);
	}
}

//==========================================================
//左右の当たり判定と往復処理
//==========================================================
void CollisionLRPlayer(void)
{
	if ((g_aPlayer.posOld.x - WIDTH_PLAYER >= COL_L && g_aPlayer.pos.x - WIDTH_PLAYER < COL_L) ||
		(g_aPlayer.posOld.x + WIDTH_PLAYER <= COL_R && g_aPlayer.pos.x + WIDTH_PLAYER > COL_R))
	{//右か左当たってたら

		g_aPlayer.nSwitchLR *= -1;	//左右切り替え
		AddScore(1);		//スコア加算

		if (g_aPlayer.nSwitchLR == 1)
		{
			g_aPlayer.nPatternAnim = 0;
		}
		else if (g_aPlayer.nSwitchLR == -1)
		{
			g_aPlayer.nPatternAnim = 2;
		}
	}
}

//==========================================================
//描画処理
//==========================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffplayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aPlayer.bUse == true && g_aPlayer.bDisp == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
			0,												//プリミティブ（ポリゴンの数）
			2);												//描画するプリミティブ数
	}
}

//==========================================================
//プレイヤーの情報
//==========================================================
Player *GetPlayer(void)
{
	return &g_aPlayer;
}