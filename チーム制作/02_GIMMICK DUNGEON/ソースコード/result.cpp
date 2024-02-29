//==========================================
//
//  リザルト画面(result.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "result_message.h"
#include "effect_2D.h"
#include "particle_2D.h"
#include "sound.h"

//==========================================
//  列挙型定義
//==========================================
typedef enum
{
	RESULT_RETRY = 0, //スタート項目
	RESULT_RANKING, //チュートリアル項目
	RESULT_TITLE, //タイトルに戻る
	RESULT_MAX
}RESULTPATTERN;

//==========================================
//  マクロ定義
//==========================================
#define TOP_MENU (470.0f) //一番上のポリゴンのY座標
#define RESULT_WIDTH (240.0f) //ポリゴンの幅
#define RESULT_HEIGHT (45.0f) //ポリゴンの高さ
#define RESULT_ALPHA (0.01f) //1fで変化するアルファ値
#define RANKING_FROM_RESULT (900) //放置でランキングに遷移する時間
#define RESULT_WIDHT_NONE (RESULT_WIDTH * 0.9) //非選択ポリゴンの幅
#define RESULT_HEIGHT_NONE (RESULT_HEIGHT * 0.9) //非選択ポリゴンの高さ
#define RESULT_WIDHT_SELECT (RESULT_WIDTH * 1.1f) //選択ポリゴンの幅
#define RESULT_HEIGHT_SELECT (RESULT_HEIGHT * 1.1f) //選択ポリゴンの高さ
#define RESULT_BITWEEN (0.07f) //ポリゴンの各縮率
#define RESULT_GRAW (0.05f) //ポリゴンの拡大率

//リザルトのファイル名を保持
const char *c_apFilenameResult[] =
{
	"data\\TEXTURE\\retry001.png",
	"data\\TEXTURE\\ranking000.png",
	"data\\TEXTURE\\title001.png",
};

//==========================================
//  構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //中心座標
	float height; //ポリゴンの高さ
	float width; //ポリゴンの幅
	D3DXCOLOR col; //ポリゴンカラー
	bool bSelect;
}RESULT;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureResult[RESULT_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;
RESULT g_Result[RESULT_MAX];
RESULTTYPE g_resultType;
float g_fResultAlpha;
int g_nResultState;
int g_nResultTimer;

//==========================================
//  初期化処理
//==========================================
void InitResult()
{
	//変数の初期化
	g_nResultState = RESULT_MAX;
	g_resultType = GetResult();
	g_fResultAlpha = RESULT_ALPHA;
	g_nResultTimer = 0;
	for (int nCntBuff = 0; nCntBuff < RESULT_MAX; nCntBuff++)
	{
		g_Result[nCntBuff].pos = D3DXVECTOR3
		(
			SCREEN_WIDTH * 0.75f,
			TOP_MENU + ((RESULT_HEIGHT * 2.0f) * nCntBuff),
			0.0f
		);
		if (nCntBuff == g_nResultState)
		{
			g_Result[nCntBuff].width = RESULT_WIDHT_SELECT;
			g_Result[nCntBuff].height = RESULT_HEIGHT_SELECT;
		}
		else
		{
			g_Result[nCntBuff].width = RESULT_WIDHT_NONE;
			g_Result[nCntBuff].height = RESULT_HEIGHT_NONE;
		}
		g_Result[nCntBuff].bSelect = false;
		g_Result[nCntBuff].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * RESULT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL
	);

	//テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < RESULT_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile
		(
			pDevice,
			c_apFilenameResult[nCntTex],
			&g_pTextureResult[nCntTex]
		);
	}

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	for (int nCntBuff = 0; nCntBuff < RESULT_MAX; nCntBuff++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_Result[nCntBuff].pos.x - g_Result[nCntBuff].width, g_Result[nCntBuff].pos.y - g_Result[nCntBuff].height, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Result[nCntBuff].pos.x + g_Result[nCntBuff].width, g_Result[nCntBuff].pos.y - g_Result[nCntBuff].height, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Result[nCntBuff].pos.x - g_Result[nCntBuff].width, g_Result[nCntBuff].pos.y + g_Result[nCntBuff].height, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Result[nCntBuff].pos.x + g_Result[nCntBuff].width, g_Result[nCntBuff].pos.y + g_Result[nCntBuff].height, 0.0f);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//rhwの設定
			pVtx[nCnt].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[nCnt].col = g_Result[nCntBuff].col;
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点データを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffResult->Unlock();

	//メッセージの初期化
	InitResultMessage();

	//パーティクルの描画
	InitParticle2D();

	//エフェクトの描画
	InitEffect2D();

	//サウンドの停止
	StopSound();

	//サウンド再生 =
	if (g_resultType == RESULTTYPE_WIN)
	{//勝利
		PlaySound(SOUND_LABEL_BGM004);
	}
	else
	{//敗北
		PlaySound(SOUND_LABEL_BGM005);
	}
}

//==========================================
//  終了処理
//==========================================
void UninitResult()
{
	//サウンドの停止
	StopSound();

	//テクスチャの破棄
	for (int nCnt = 0; nCnt < RESULT_MAX; nCnt++)
	{
		if (g_pTextureResult[nCnt] != NULL)
		{
			g_pTextureResult[nCnt]->Release();
			g_pTextureResult[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	//メッセージの終了
	UninitResultMessage();

	//パーティクルの描画
	UninitParticle2D();

	//エフェクトの描画
	UninitEffect2D();
}

//==========================================
//  更新処理
//==========================================
void UpdateResult()
{
	if (GetResultState())
	{
		//状態遷移
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) || GetJoyPadButtonTrigger(KEY_A, 0) || GetJoyPadButtonTrigger(KEY_START, 0) || GetMouseTrigger(MOUSEBUTTON_LEFT))
		{
			PlaySound(SOUND_LABEL_SE_BUTTON_001);

			switch (g_nResultState)
			{
			case RESULT_RETRY:
				SetFade(MODE_GAME);
				break;
			case RESULT_RANKING:
				SetFade(MODE_RANKING);
				break;
			case RESULT_TITLE:
				SetFade(MODE_TITLE);
				break;
			}
		}

		//放置タイマーの更新
		g_nResultTimer++;

		//選択状態の更新
		if (SelectResult())
		{
			if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_S) ||
				GetStickTriggerL(STICK_UP, 0) || GetJoyPadCrossTrigger(CROSS_UP, 0) ||
				GetStickTriggerL(STICK_DOWN, 0) || GetJoyPadCrossTrigger(CROSS_DOWN, 0)
				)
			{
				g_nResultState = RESULT_RETRY;

				PlaySound(SOUND_LABEL_SE_BUTTON_003);
			}
		}
		else
		{
			if (GetFade() == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_W) || GetStickTriggerL(STICK_UP, 0) || GetJoyPadCrossTrigger(CROSS_UP, 0))
				{
					g_nResultState += (RESULT_MAX - 1);

					//選択状態の補正
					g_nResultState %= RESULT_MAX;

					//放置タイマーのリセット
					g_nResultTimer = 0;

					PlaySound(SOUND_LABEL_SE_BUTTON_003);
				}
				if (GetKeyboardTrigger(DIK_S) || GetStickTriggerL(STICK_DOWN, 0) || GetJoyPadCrossTrigger(CROSS_DOWN, 0))
				{
					g_nResultState++;

					//選択状態の補正
					g_nResultState %= RESULT_MAX;

					//放置タイマーのリセット
					g_nResultTimer = 0;

					PlaySound(SOUND_LABEL_SE_BUTTON_003);
				}
			}
		}

		//メニュー項目の点滅
		if (g_Result[g_nResultState].col.a < 0.0f || g_Result[g_nResultState].col.a > 1.0f)
		{
			g_fResultAlpha *= -1.0f;
		}
		g_Result[g_nResultState].col.a += g_fResultAlpha;

		//非選択項目の透明度をリセットする
		for (int nCnt = 0; nCnt < RESULT_MAX; nCnt++)
		{
			if (nCnt == g_nResultState) //選択時
			{
				//対象のポリゴンを拡大する
				if (g_Result[nCnt].width < RESULT_WIDHT_SELECT)
				{
					g_Result[nCnt].width += g_Result[nCnt].width * RESULT_BITWEEN;
				}
				if (g_Result[nCnt].height < RESULT_HEIGHT_SELECT)
				{
					g_Result[nCnt].height += g_Result[nCnt].height * RESULT_BITWEEN;
				}
			}
			else //非選択時
			{
				//対象のポリゴンを縮小する
				if (g_Result[nCnt].width > RESULT_WIDHT_NONE)
				{
					g_Result[nCnt].width -= g_Result[nCnt].width * RESULT_BITWEEN;
				}
				if (g_Result[nCnt].height > RESULT_HEIGHT_NONE)
				{
					g_Result[nCnt].height -= g_Result[nCnt].height * RESULT_BITWEEN;
				}

				g_Result[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		//フェード中の挙動
		if (g_nResultState != RESULT_MAX)
		{
			if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) || GetJoyPadButtonTrigger(KEY_A, 0) || GetJoyPadButtonTrigger(KEY_START, 0) || GetMouseTrigger(MOUSEBUTTON_LEFT))
			{
				g_Result[g_nResultState].bSelect = true;
			}
			if (g_Result[g_nResultState].bSelect)
			{
				g_Result[g_nResultState].width += g_Result[g_nResultState].width * RESULT_GRAW;
				g_Result[g_nResultState].height += g_Result[g_nResultState].height * RESULT_GRAW;
				if (g_Result[g_nResultState].col.a > 0.0f)
				{
					g_Result[g_nResultState].col.a -= g_Result[g_nResultState].col.a * RESULT_GRAW * 3;
				}
				else
				{
					g_Result[g_nResultState].col.a = 0.0f;
				}
			}
		}

		//頂点バッファの呼び出し
		VERTEX_2D *pVtx;

		//頂点バッファをロック
		g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		for (int nCntBuff = 0; nCntBuff < RESULT_MAX; nCntBuff++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_Result[nCntBuff].pos.x - g_Result[nCntBuff].width, g_Result[nCntBuff].pos.y - g_Result[nCntBuff].height, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Result[nCntBuff].pos.x + g_Result[nCntBuff].width, g_Result[nCntBuff].pos.y - g_Result[nCntBuff].height, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Result[nCntBuff].pos.x - g_Result[nCntBuff].width, g_Result[nCntBuff].pos.y + g_Result[nCntBuff].height, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Result[nCntBuff].pos.x + g_Result[nCntBuff].width, g_Result[nCntBuff].pos.y + g_Result[nCntBuff].height, 0.0f);

			//頂点カラーの設定
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				pVtx[nCnt].col = g_Result[nCntBuff].col;
			}

			//頂点データを進める
			pVtx += 4;
		}

		//頂点バッファをアンロック
		g_pVtxBuffResult->Unlock();

		//タイトルに強制送還
		if (g_nResultTimer >= RANKING_FROM_RESULT)
		{
			SetFade(MODE_TITLE);
		}
	}

	//メッセージの更新
	UpdateResultMessage();

	//パーティクルの描画
	UpdateParticle2D();

	//エフェクトの描画
	UpdateEffect2D();
}

//==========================================
//  描画処理
//==========================================
void DrawResult()
{
	if (GetResultState())
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int nCnt = 0; nCnt < RESULT_MAX; nCnt++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureResult[nCnt]);

			//メニュー項目の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	//メッセージの描画
	DrawResultMessage();

	//パーティクルの描画
	DrawParticle2D();

	//エフェクトの描画
	DrawEffect2D();
}

//==========================================
//  非選択の状態
//==========================================
bool SelectResult()
{
	return g_nResultState == RESULT_MAX ? true : false;
}

//==========================================
//  結果の設定
//==========================================
void SetResult(RESULTTYPE type)
{
	g_resultType = type;
}

//==========================================
//  結果の取得
//==========================================
RESULTTYPE GetResult()
{
	return g_resultType;
}
