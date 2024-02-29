//==========================================
//
//  リザルトメッセージ(result_message.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "result_message.h"
#include "result.h"
#include "particle_2D.h"
#include "sound.h"

//==========================================
//  マクロ定義
//==========================================
#define MESSAGE_POS_LEFT (D3DXVECTOR3(415.0f, 260.0f, 0.0f)) //中心座標
#define MESSAGE_POS_RIGHT (D3DXVECTOR3(865.0f, 260.0f, 0.0f)) //中心座標
#define MESSAGE_WIDTH (225.0f) //横幅
#define MESSAGE_HEIGHT (180.0f) //高さ
#define POLYGON_MOVE (30.2f) //ポリゴンの移動量
#define POLYGON_COLOR (0.03f) //ポリゴンの色

//結果のファイル名を保持
const char *c_apFilenameResultMessege[RESULTTYPE_MAX] =
{
	"data\\TEXTURE\\clear000.png",
	"data\\TEXTURE\\failed000.png"
};

//==========================================
//  リザルトポリゴン構造体
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //中心座標
	D3DXCOLOR col; //色
}RESULT;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureResultMessage[RESULTTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultMessage = NULL;
RESULT g_result[2];
bool g_bSet;

//==========================================
//  初期化処理
//==========================================
void InitResultMessage()
{
	//変数の初期化
	switch (GetResult())
	{
	case RESULTTYPE_WIN:
		//中心座標の設定
		g_result[0].pos = D3DXVECTOR3(-MESSAGE_WIDTH, MESSAGE_POS_LEFT.y, 0.0f);
		g_result[1].pos = D3DXVECTOR3(SCREEN_WIDTH + MESSAGE_WIDTH, MESSAGE_POS_LEFT.y, 0.0f);
		//色の設定
		g_result[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_result[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case RESULTTYPE_LOSE:
		g_result[0].pos = MESSAGE_POS_LEFT;
		g_result[1].pos = MESSAGE_POS_RIGHT;
		//色の設定
		g_result[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		g_result[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		break;
	default:
		break;
	}

	g_bSet = false;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultMessage,
		NULL
	);

	//テクスチャの読み込み
	for (int nCntTex = 0;nCntTex < RESULTTYPE_MAX;nCntTex++)
	{
		D3DXCreateTextureFromFile
		(
			pDevice,
			c_apFilenameResultMessege[nCntTex],
			&g_pTextureResultMessage[nCntTex]
		);
	}

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffResultMessage->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int nCntPolygon = 0; nCntPolygon < 2; nCntPolygon++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_result[nCntPolygon].pos.x - MESSAGE_WIDTH, g_result[nCntPolygon].pos.y - MESSAGE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_result[nCntPolygon].pos.x + MESSAGE_WIDTH, g_result[nCntPolygon].pos.y - MESSAGE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_result[nCntPolygon].pos.x - MESSAGE_WIDTH, g_result[nCntPolygon].pos.y + MESSAGE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_result[nCntPolygon].pos.x + MESSAGE_WIDTH, g_result[nCntPolygon].pos.y + MESSAGE_HEIGHT, 0.0f);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//rhwの設定
			pVtx[nCnt].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[nCnt].col = g_result[nCntPolygon].col;
		}

		//頂点データを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffResultMessage->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitResultMessage()
{
	//頂点バッファの破棄
	if (g_pVtxBuffResultMessage != NULL)
	{
		g_pVtxBuffResultMessage->Release();
		g_pVtxBuffResultMessage = NULL;
	}

	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < RESULTTYPE_MAX; nCntTex++)
	{
		if (g_pTextureResultMessage[nCntTex] != NULL)
		{
			g_pTextureResultMessage[nCntTex]->Release();
			g_pTextureResultMessage[nCntTex] = NULL;
		}
	}
}

//==========================================
//  更新処理
//==========================================
void UpdateResultMessage()
{
	//判定用座標の保存
	float fOldPos[2] = { g_result[0].pos.x , g_result[1].pos.x };

	//座標を補正する
	if (g_result[0].pos.x < MESSAGE_POS_LEFT.x)
	{
		g_result[0].pos.x += POLYGON_MOVE;
	}
	if (g_result[0].pos.x >= MESSAGE_POS_LEFT.x)
	{
		g_result[0].pos.x = MESSAGE_POS_LEFT.x;
	}
	if (g_result[1].pos.x > MESSAGE_POS_RIGHT.x)
	{
		g_result[1].pos.x -= POLYGON_MOVE;
	}
	if (g_result[1].pos.x <= MESSAGE_POS_RIGHT.x)
	{
		g_result[1].pos.x = MESSAGE_POS_RIGHT.x;
	}

	//パーティクルを発生する
	if (fOldPos[0] < MESSAGE_POS_LEFT.x && fOldPos[1] > MESSAGE_POS_RIGHT.x)
	{
		if (g_result[0].pos.x >= MESSAGE_POS_LEFT.x || g_result[1].pos.x <= MESSAGE_POS_RIGHT.x)
		{
			//SE再生
			PlaySound(SOUND_LABEL_SE_RESULT);

			//パーティクル発生
			SetParticle2D(D3DXVECTOR3(MESSAGE_POS_LEFT.x + MESSAGE_WIDTH, MESSAGE_POS_LEFT.y, 0.0f), PARTICLETYPE_2D_RESULT);
		}
	}

	//色を補正する
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (g_result[nCnt].col.a <= 1.0f)
		{
			g_result[nCnt].col.a += POLYGON_COLOR;
		}
	}

	if (g_result[0].pos.x == MESSAGE_POS_LEFT.x && g_result[1].pos.x == MESSAGE_POS_RIGHT.x  && g_result[0].col.a >= 1.0f && g_result[1].col.a >= 1.0f)
	{
		g_bSet = true;
	}

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffResultMessage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < 2; nCntPolygon++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_result[nCntPolygon].pos.x - MESSAGE_WIDTH, g_result[nCntPolygon].pos.y - MESSAGE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_result[nCntPolygon].pos.x + MESSAGE_WIDTH, g_result[nCntPolygon].pos.y - MESSAGE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_result[nCntPolygon].pos.x - MESSAGE_WIDTH, g_result[nCntPolygon].pos.y + MESSAGE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_result[nCntPolygon].pos.x + MESSAGE_WIDTH, g_result[nCntPolygon].pos.y + MESSAGE_HEIGHT, 0.0f);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//頂点カラーの設定
			pVtx[nCnt].col = g_result[nCntPolygon].col;
		}

		//頂点データを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffResultMessage->Unlock();
}

//==========================================
//  描画処理
//==========================================
void DrawResultMessage()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultMessage, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResultMessage[GetResult()]);

	//ロゴの描画
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}

//==========================================
//  メッセージの状態の取得
//==========================================
bool GetResultState()
{
	return g_bSet;
}