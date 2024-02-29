//==========================================================
//
//アイテム処理 [item.cpp]
//Author:佐藤根詩音
//
//==========================================================
#include "item.h"
#include "input.h"
#include "debugproc.h"
#include "sound.h"
#include "score.h"
#include "player.h"
#include "enemy.h"
#include "collision.h"
#include "particle.h"

//マクロ定義
#define NUM_ITEM	(3)			//アイテムの数
#define WIDTH_ITEM	(30.0f)		//幅
#define HEIGHT_ITEM	(30.0f)		//高さ
#define MOVE		(0.5f)		//移動量
#define MOVE_JUMP	(-10.0f)	//跳ねる高さ
#define APPEAR_TIME	(60 * 10)	//出現時間

//プロトタイプ宣言
void ControlItem(int nCntItem);
void ScreenItem(int nCntItem);
void CollisionItem(void);

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureItem[NUM_ITEM] = {};			//テクスチャ1へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;			//頂点バッファへのポインタ
Item g_aItem[MAX_ITEM];									//アイテムの情報
int g_nItemAppearCounter;		//出現時間
int g_nItemType;			//アイテムの種類
float g_fItemRandPos;			//ランダムの位置

//==========================================================
//アイテムの初期化処理
//==========================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\item001.png",
		&g_pTextureItem[ITEMSTATE_DEL]);		//全削除

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\item000.png",
		&g_pTextureItem[ITEMSTATE_JET]);		//ジェットパック

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\item002.png",
		&g_pTextureItem[ITEMSTATE_UP]);			//得点アップ

	//変数初期化
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		g_aItem[nCntItem].posOld = g_aItem[nCntItem].pos;			//前回の位置
		g_aItem[nCntItem].posKeep = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//前回の位置
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
		g_aItem[nCntItem].moveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//前回の移動量
		g_aItem[nCntItem].bUse = true;			//使用してるか
		g_aItem[nCntItem].bDisp = true;			//描画するか
		g_aItem[nCntItem].nType = -1;			//種類
		g_aItem[nCntItem].nSwitchLR = 1;		//左右切り替え
		g_aItem[nCntItem].fWidth = WIDTH_ITEM;	//幅
		g_aItem[nCntItem].fHeight = HEIGHT_ITEM;	//高さ
		g_aItem[nCntItem].fMoveRand = (float)(rand() % 5) * 0.1f;	//移動量
	}

	g_nItemAppearCounter = 0;		//出現時間を初期化
	g_fItemRandPos = 0;				//出現場所を設定
	g_nItemType = -1;				//アイテムの種類

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fHeight, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();

}

//==========================================================
//アイテムの終了処理
//==========================================================
void UninitItem(void)
{
	//テクスチャの破棄
	for (int nCntItem = 0; nCntItem < NUM_ITEM; nCntItem++)
	{
		if (g_pTextureItem[nCntItem] != NULL)
		{
			g_pTextureItem[nCntItem]->Release();
			g_pTextureItem[nCntItem] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//==========================================================
//アイテムの更新処理
//==========================================================
void UpdateItem(void)
{
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	g_nItemAppearCounter++;				//出現時間を加算

	if ((g_nItemAppearCounter % APPEAR_TIME) == 0)
	{//一定時間経過

		g_fItemRandPos = (float)(rand() % SCREEN_WIDTH + 0);		//出現場所設定
		g_nItemType = rand() % ITEMSTATE_UP;	//アイテムの種類設定

		//アイテムの設定
		SetItem(D3DXVECTOR3(g_fItemRandPos, SCREEN_HEIGHT, 0.0f), g_nItemType);
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse = true && g_aItem[nCntItem].bDisp == true)
		{//使用してたら

			g_aItem[nCntItem].posOld = g_aItem[nCntItem].pos;		//現在の位置

			//アイテムの操作
			ControlItem(nCntItem);

			//位置を更新
			g_aItem[nCntItem].pos.x += g_aItem[nCntItem].move.x;
			g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;

			//移動量を更新
			g_aItem[nCntItem].move.x += (0.0f - g_aItem[nCntItem].move.x) * 0.1f;

			//向きを更新
			g_aItem[nCntItem].rot.z += g_aItem[nCntItem].move.z;

			//画面外処理
			ScreenItem(nCntItem);

			CollisionItem();

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fHeight, 0.0f);
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//==========================================================
//アイテムの操作
//==========================================================
void ControlItem(int nCntItem)
{
	if (g_aItem[nCntItem].bJump == false)
	{//ジャンプしてないとき

		//上に上げる
		g_aItem[nCntItem].move.y = MOVE_JUMP;

		g_aItem[nCntItem].bJump = true;		//ジャンプしてる状態にする
	}
	else if(g_aItem[nCntItem].bJump == true)
	{//ジャンプしてないとき

		g_aItem[nCntItem].move.y += 0.1f;	//重力加算

	}

	if (g_aItem[nCntItem].posKeep.x <= SCREEN_WIDTH / 2)
	{//中央より左の場合

		//右に移動
		g_aItem[nCntItem].move.x += MOVE + g_aItem[nCntItem].fMoveRand;
	}
	else if (g_aItem[nCntItem].posKeep.x > SCREEN_WIDTH / 2)
	{//中央より右の場合

		//左に移動
		g_aItem[nCntItem].move.x -= MOVE - g_aItem[nCntItem].fMoveRand;
	}
}

//==========================================================
//画面外に出た時の処理
//==========================================================
void ScreenItem(int nCntItem)
{
	//画面外に出た場合
	if ((g_aItem[nCntItem].pos.x - WIDTH_ITEM + 5.0f <= 0) ||
		(g_aItem[nCntItem].pos.x + WIDTH_ITEM - 5.0f >= SCREEN_WIDTH))
	{//左右に出た場合

		//使用してない状態にする
		g_aItem[nCntItem].bDisp = false;
		g_aItem[nCntItem].bUse = false;
	}
}

//==========================================================
//アイテムの当たり判定
//==========================================================
void CollisionItem(void)
{
	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//障害物の情報取得
	Enemy *pEnemy = GetEnemy();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].pos.x >= pPlayer->pos.x - WIDTH_PLAYER &&
			g_aItem[nCntItem].pos.x <= pPlayer->pos.x + WIDTH_PLAYER &&
			g_aItem[nCntItem].pos.y >= pPlayer->pos.y - HEIGHT_PLAYER &&
			g_aItem[nCntItem].pos.y <= pPlayer->pos.y + HEIGHT_PLAYER)
		{
			switch (g_aItem[nCntItem].nType)
			{
			case ITEMSTATE_DEL:		//障害物全削除

				//障害物削除
				for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//使用してたら
						SetParticle(pEnemy->pos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 0, 10, 50.0f);
						pEnemy->bUse = false;		//使用してない状態にする
					}
				}

				break;

			case ITEMSTATE_JET:		//ジェットパック

				AddScore(3);

				break;

			case ITEMSTATE_UP:		//得点アップ

				AddScore(1);

				break;
			}

			//アイテム削除
			g_aItem[nCntItem].bUse = false;
			g_aItem[nCntItem].bDisp = false;

			break;
		}
	}
}

//==========================================================
//アイテムの描画処理
//==========================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true && g_aItem[nCntItem].bDisp == true)
		{
			switch (g_aItem[nCntItem].nType)
			{
			case ITEMSTATE_DEL:		//障害物全削除

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureItem[ITEMSTATE_DEL]);

				break;

			case ITEMSTATE_JET:		//ジェットパック

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureItem[ITEMSTATE_JET]);

				break;

			case ITEMSTATE_UP:		//得点アップ

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureItem[ITEMSTATE_UP]);

				break;
			}

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
				4 * nCntItem,									//プリミティブ（ポリゴンの数）
				2);												//描画するプリミティブ数
		}
	}
}

//==========================================================
//アイテムの設定処理
//==========================================================
void SetItem(D3DXVECTOR3 pos, int nType)
{
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == false)
		{//使用してないとき

			g_aItem[nCntItem].pos = pos;
			g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aItem[nCntItem].posOld = g_aItem[nCntItem].pos;
			g_aItem[nCntItem].posKeep = g_aItem[nCntItem].pos;
			g_aItem[nCntItem].nType = nType;
			g_aItem[nCntItem].bUse = true;
			g_aItem[nCntItem].bDisp = true;
			g_aItem[nCntItem].bJump = false;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fHeight, 0.0f);

			break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//==========================================================
//アイテムの情報
//==========================================================
Item *GetItem(void)
{
	return &g_aItem[0];
}