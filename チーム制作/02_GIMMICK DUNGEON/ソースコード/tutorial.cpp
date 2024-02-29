//==========================================
//
//  チュートリアルの処理(tutorial.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "tutorial.h"
#include "tutorial_message.h"
#include "game.h"
#include "input.h"
#include "fade.h"
#include "polygon.h"
#include "sound.h"
#include "enemy.h"
#include "shadow.h"
#include "gauge.h"
#include "option.h"
#include "count.h"
#include "book.h"

//==========================================
//  マクロ定義
//==========================================
#define TUTORIAL_POS_DEFAULT (D3DXVECTOR3(320.0f, 120.0, 0.0f)) //チュートリアルの位置
#define TUTORIAL_POS_BACK (D3DXVECTOR3(640.0f, 120.0, 0.0f)) //背景ポリゴンの位置

#define SKIP_GAUGE_POS	(D3DXVECTOR3(1050.0f,700.0f,0.0f))	//スキップゲージの位置

#define TUTORIAL_POS_ENTER (D3DXVECTOR3(1000.0f, 270.0, 0.0f)) //背景ポリゴンの位置
#define TUTORIAL_WIDTH (200.0f) //ポリゴンの高さ
#define TUTORIAL_HEIGHT (100.0f) //ポリゴンの幅
#define BACK_WIDTH (550.0f) //背景ポリゴンの幅
#define BACK_HEIGHT (110.0f) //背景ポリゴンの高さ
#define ENTER_WIDTH (160.0f) //決定キー表示ポリゴンの幅
#define ENTER_HEIGHT (40.0f) //決定キー表示ポリゴンの高さ
#define HOLDFRAME_COUNTER (60) //長押し遷移のフレーム数

//==========================================
//  テクスチャパスを設定
//==========================================
const char *c_pFilePassTutorial[TUTORIAL_TEX_MAX] =
{
	"data/TEXTURE/tutorial_move.png", //移動しろ
	"data/TEXTURE/tutorial_move_000.png", //移動の説明
	"data/TEXTURE/tutorial_point.png", //カメラ動かせ
	"data/TEXTURE/tutorial_point_000.png", //カメラの説明
	"data/TEXTURE/tutorial_reflector.png", //反射板を操作しろ
	"data/TEXTURE/tutorial_reflector_000.png", //反射板の説明
	"data/TEXTURE/tutorial_stopper.png", //ボタンを操作しろ
	"data/TEXTURE/tutorial_stopper_000.png", //ボタンの説明
	"data/TEXTURE/tutorial_jump.png", //ジャンプしろ
	"data/TEXTURE/tutorial_jump_000.png", //ジャンプの説明
	"data/TEXTURE/tutorial_enemy.png", //敵を倒せ
	"data/TEXTURE/tutorial_enemy_001.png", //敵の説明
	"data/TEXTURE/tutorial_frame.png", //背景ポリゴン
	"data/TEXTURE/tutorial_decisionr.png", //決定キー表示
};

//==========================================
//  チュートリアル構造体の定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //チュートリアルポリゴンの場所
	D3DXVECTOR3 size; //チュートリアルポリゴンの大きさ
	D3DXCOLOR col; //チュートリアルポリゴンの色
	TUTORIALTYPE type; //設定されるテクスチャの種類
	TUTORIALPOS side; //設定される位置
	bool bUse; //描画の有無
	bool bClear; //進行
}TUTORIAL;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureTutorial[TUTORIAL_TEX_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;
TUTORIALSTATE g_TutorialState; //チュートリアル状態
TUTORIAL g_Tutorial[TUTORIAL_TEX_MAX]; //チュートリアルポリゴンのステータス
bool g_bEnemy; //敵の出現状態
int g_nHoldCtr;	//長押しカウント
int g_nIdxHoldGauge;	//スキップゲージ
bool g_bFade;	//無限フェード防止

//==========================================
//  初期化処理
//==========================================
void InitTutorial()
{
	//変数の初期化
	g_TutorialState = TUTORIALSTATE_MOVE;
	g_bEnemy = false;
	g_bFade = false;
	g_nHoldCtr = 0;

	//変数の設定
	for (int nCnt = 0; nCnt < TUTORIAL_TEX_MAX; nCnt++)
	{
		//不使用で初期化
		g_Tutorial[nCnt].bUse = false;
		g_Tutorial[nCnt].bClear = false;

		//ポリゴンデータを設定
		if (nCnt == TUTORIAL_BG)
		{
			//種類を設定
			g_Tutorial[nCnt].type = TUTORIALTYPE_BG;

			//位置を設定
			g_Tutorial[nCnt].pos = TUTORIAL_POS_BACK;

			//サイズを設定
			g_Tutorial[nCnt].size = D3DXVECTOR3(BACK_WIDTH, BACK_HEIGHT, 0.0f);

			//色を設定
			g_Tutorial[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

			//使用設定
			g_Tutorial[nCnt].bUse = true;
		}
		else if (nCnt == TUTORIAL_ENTER)
		{
			//種類を設定
			g_Tutorial[nCnt].type = TUTORIALTYPE_ENTER;

			//位置を設定
			g_Tutorial[nCnt].pos = TUTORIAL_POS_ENTER;

			//サイズを設定
			g_Tutorial[nCnt].size = D3DXVECTOR3(ENTER_WIDTH, ENTER_HEIGHT, 0.0f);

			//色を設定
			g_Tutorial[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

			//使用設定
			g_Tutorial[nCnt].bUse = false;
		}
		else
		{
			if (nCnt % 2 != 0)
			{
				//種類を設定
				g_Tutorial[nCnt].type = TUTORIALTYPE_EX;

				//位置を設定
				g_Tutorial[nCnt].side = RIGHTSIDE;

				//位置を設定
				g_Tutorial[nCnt].pos = TUTORIAL_POS_DEFAULT;
				g_Tutorial[nCnt].pos.x *= 3.0f;

				//サイズを設定
				g_Tutorial[nCnt].size = D3DXVECTOR3(TUTORIAL_WIDTH, TUTORIAL_HEIGHT, 0.0f);

				//色を設定
				g_Tutorial[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

				//使用設定
				g_Tutorial[nCnt].bUse = false;
			}
			else
			{
				//種類を設定
				g_Tutorial[nCnt].type = TUTORIALTYPE_BASE;

				//位置を設定
				g_Tutorial[nCnt].side = LEFTSIDE;

				//位置を設定
				g_Tutorial[nCnt].pos = TUTORIAL_POS_DEFAULT;

				//サイズを設定
				g_Tutorial[nCnt].size = D3DXVECTOR3(TUTORIAL_WIDTH, TUTORIAL_HEIGHT, 0.0f);

				//色を設定
				g_Tutorial[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

				//使用設定
				g_Tutorial[nCnt].bUse = false;
			}
		}
	}

	//ゲームの初期化
	InitGame();

	//本の初期化
	InitBook();

	//ゲージの設定
	g_nIdxHoldGauge = SetGauge(SKIP_GAUGE_POS, HOLDFRAME_COUNTER, 30.0f, 200.0f, GAUGETYPE_HORIZON,SKIPGAUGE_CR);

	//ポリゴンの初期化
	g_pVtxBuffTutorial = Init_2D_Polygon(TUTORIAL_TEX_MAX);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	for (int nCnt = 0; nCnt < TUTORIAL_TEX_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile
		(
			pDevice,
			c_pFilePassTutorial[nCnt],
			&g_pTextureTutorial[nCnt]
		);
	}

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < TUTORIAL_TEX_MAX; nCntPolygon++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Tutorial[nCntPolygon].pos.x - g_Tutorial[nCntPolygon].size.x, g_Tutorial[nCntPolygon].pos.y - g_Tutorial[nCntPolygon].size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Tutorial[nCntPolygon].pos.x + g_Tutorial[nCntPolygon].size.x, g_Tutorial[nCntPolygon].pos.y - g_Tutorial[nCntPolygon].size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Tutorial[nCntPolygon].pos.x - g_Tutorial[nCntPolygon].size.x, g_Tutorial[nCntPolygon].pos.y + g_Tutorial[nCntPolygon].size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Tutorial[nCntPolygon].pos.x + g_Tutorial[nCntPolygon].size.x, g_Tutorial[nCntPolygon].pos.y + g_Tutorial[nCntPolygon].size.y, 0.0f);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//頂点カラーの設定
			pVtx[nCnt].col = g_Tutorial[nCntPolygon].col;
		}

		if (g_Tutorial[nCntPolygon].type == TUTORIALTYPE_BASE)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
		}

		else
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		//頂点データのポインタを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffTutorial->Unlock();

	//チュートリアルメッセージの初期化
	InitTutorialMessage();

	//カウントの初期化
	InitCount();

	PlaySound(SOUND_LABEL_BGM001);
}

//==========================================
//  終了処理
//==========================================
void UninitTutorial()
{
	//サウンドの停止
	StopSound();

	//ゲームの終了
	UninitGame();

	//本の終了
	UninitBook();

	//チュートリアルメッセージの終了
	UninitTutorialMessage();

	//カウントの終了
	UninitCount();

	//ポリゴンの破棄
	UninitPolygon(&g_pVtxBuffTutorial);

	//テクスチャの破棄
	for (int nCnt = 0; nCnt < TUTORIAL_TEX_MAX; nCnt++)
	{
		if (g_pTextureTutorial[nCnt] != NULL)
		{
			g_pTextureTutorial[nCnt]->Release();
			g_pTextureTutorial[nCnt] = NULL;
		}
	}
}

//==========================================
//  更新処理
//==========================================
void UpdateTutorial()
{
	for (int nCnt = 0; nCnt < TUTORIAL_TEX_MAX; nCnt++)
	{
		//進行状態を保存する
		if (nCnt < g_TutorialState * 2)
		{
			g_Tutorial[nCnt].bClear = true;
		}

		//進行に添ってポリゴンを描画する
		if (nCnt != TUTORIAL_BG)
		{
			if (nCnt == g_TutorialState * 2 || nCnt == g_TutorialState * 2 + 1)
			{
				g_Tutorial[nCnt].bUse = true;
			}
			if (nCnt == TUTORIAL_ENTER)
			{
				if (g_TutorialState == TUTORIALSTATE_CAMERA)
				{
					g_Tutorial[nCnt].bUse = true;
				}
			}

			//ポリゴンを消去する
			if (g_Tutorial[nCnt].col.a < 0.0f)
			{
				g_Tutorial[nCnt].bUse = false;
			}
			if (g_Tutorial[nCnt].bClear && nCnt != TUTORIAL_CAMERA)
			{
				g_Tutorial[nCnt].col.a -= 0.02f;
			}
			if (nCnt == TUTORIAL_CAMERA)
			{
				if (g_Tutorial[nCnt].bClear && g_Tutorial[TUTORIAL_ENTER].bClear)
				{
					g_Tutorial[nCnt].col.a -= 0.02f;
				}
			}

			//ポリゴンを描画する
			if (g_Tutorial[nCnt].bUse)
			{
				if (nCnt < 2)
				{
					if (g_Tutorial[nCnt].col.a < 1.0f)
					{
						g_Tutorial[nCnt].col.a += 0.01f;
					}
				}
				else if(nCnt == TUTORIAL_ENTER)
				{
					if (g_Tutorial[nCnt].col.a < 1.0f)
					{
						g_Tutorial[nCnt].col.a += 0.01f;
					}
				}
				else
				{
					if (g_Tutorial[nCnt - 2].bUse == false)
					{
						if (g_Tutorial[nCnt].col.a < 1.0f)
						{
							g_Tutorial[nCnt].col.a += 0.01f;
						}
					}
				}
			}
		}
	}

	//カメラチュートリアルのみエンターで進める
	if (g_TutorialState == TUTORIALSTATE_CAMERA && (GetKeyboardTrigger(DIK_RETURN) || GetJoyPadButtonTrigger(KEY_START, 0)))
	{
		g_TutorialState = TUTORIALSTATE_REFLECTOR;
		g_Tutorial[TUTORIAL_ENTER].bClear = true;

		PlaySound(SOUND_LABEL_SE_BUTTON_004);
	}

	//カメラチュートリアルのみ感度調整を実装する
	if (g_TutorialState == TUTORIALSTATE_CAMERA)
	{
		UpdateOption();

		if (GetKeyboardPress(DIK_DOWN) || GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT) || GetMouseControll() || GetStickR(0) != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
			g_Tutorial[TUTORIAL_CAMERA].bClear = true;
		}
	}

	//チュートリアルの最後に敵を呼び出す
	if (g_TutorialState == TUTORIALSTATE_JUMP && g_bEnemy == false)
	{
		SetEnemy(D3DXVECTOR3(310.76f, 0.0f, 300.57f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
		SetPositionShadow(GetEnemy()->nIdxGauge, GetEnemy()->pos, GetEnemy()->rot, 1.0f);
		g_bEnemy = true;
	}

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < TUTORIAL_TEX_MAX; nCntPolygon++)
	{
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//頂点カラーの設定
			pVtx[nCnt].col = g_Tutorial[nCntPolygon].col;
		}

		//テクスチャ座標の設定
		if (g_Tutorial[nCntPolygon].bClear && g_Tutorial[nCntPolygon].type == TUTORIALTYPE_BASE)
		{
			pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		//頂点データのポインタを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffTutorial->Unlock();

	//チュートリアルの終了
	if (GetPause() == false)
	{
		if (g_nHoldCtr < HOLDFRAME_COUNTER)
		{
			if ((GetKeyboardPress(DIK_RETURN) || GetJoyPadButtonPress(KEY_START, 0)) && *GetBookState() == BOOKSTATE_END)
			{//カウンター加算
				g_nHoldCtr++;
			}
			else
			{//カウンターリセット
				g_nHoldCtr = 0;
			}
		}

		//画面遷移
		if ((g_nHoldCtr >= HOLDFRAME_COUNTER || g_TutorialState == TUTORIALSTATE_END) &&
			g_bFade == false)
		{
			SetFade(MODE_GAME);

			g_bFade = true;
		}
	}

	//ゲージ情報更新
	SetInfoGauge(g_nIdxHoldGauge, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), (float)g_nHoldCtr);

	//本の更新
	UpdateBook();

	//ゲームの更新
	UpdateGame();

	//チュートリアルメッセージの更新
	UpdateTutorialMessage();

	//カウントの更新
	UpdateCount();
}

//==========================================
//  描画処理
//==========================================
void DrawTutorial()
{
	//ローカル変数宣言
	int nCntPolygon;

	//ゲームの描画
	DrawGame();

	//本の描画
	DrawBook();

	if (*GetBookState() == BOOKSTATE_END && GetFade() == FADE_NONE)
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//チュートリアル内容の描画
		for (nCntPolygon = TUTORIAL_TEX_MAX - 1; nCntPolygon >= 0; nCntPolygon--)
		{
			if (g_Tutorial[nCntPolygon].bUse)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureTutorial[nCntPolygon]);

				//描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPolygon * 4, 2);
			}
		}

		//チュートリアルメッセージの描画
		DrawTutorialMessage();

		//カウントの描画
		if (g_TutorialState == TUTORIALSTATE_JUMP || g_TutorialState == TUTORIALSTATE_ENEMY)
		{
			DrawCount();
		}
	}
}

//==========================================
//  チュートリアル状態のポインタを取得
//==========================================
TUTORIALSTATE *GetTutorialState()
{
	return &g_TutorialState;
}
