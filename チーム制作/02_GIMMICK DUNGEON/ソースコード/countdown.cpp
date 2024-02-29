//==========================================
//
//  カウントダウン処理(countdown.cpp)
//  Author : Kai Takada
//
//==========================================
#include "countdown.h"
#include "game.h"
#include "input.h"

//==========================================
//  マクロ定義
//==========================================
#define CD_TEX_MAX (2) //テクスチャ数
#define COUNTDOWN_CNT_MAX (3)	//最大カウント
#define COUNTDOWN_POS (D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f,0.0f)) //メッセージの中心座標
#define COUNTDOWN_ALPHA_SCALE (0.05f) //アルファ値の減少量
#define SIZE_LOOP (0.03f) //ポリゴンの拡大率
#define COUNTDOWN_POLYGON_WIDTH (300.0f) //ポリゴンの幅
#define COUNTDOWN_POLYGON_WIDTH1 (150.0f) //ポリゴンの幅1
#define COUNTDOWN_POLYGON_HEIGHT (150.0f) //ポリゴンの高さ
#define SIZE_RATIO (COUNTDOWN_POLYGON_WIDTH / COUNTDOWN_POLYGON_HEIGHT) //高さと幅の比率
#define FREEZE_TIME (30) //一時停止フレーム

//==========================================
//  構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //中心座標
	D3DXCOLOR col; //ポリゴンカラー
	float fWidth; //ポリゴンの幅
	float fHeight; //ポリゴンの高さ
	float fScale; //ポリゴンの拡大率
	int nNumCnt; //何枚目か
}COUNTDOWN;

//==========================================
//  テクスチャパス配列宣言
//==========================================
const char *c_apFilenameCD[CD_TEX_MAX] =
{
	"data/TEXTURE/countdown_number_000.png",
	"data/TEXTURE/countdown_go_000.png",
};

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureCountdown[CD_TEX_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCountdown = NULL;
COUNTDOWN g_countdown;
int g_nFreezeFrame;
bool g_bSkip;

//==========================================
//  初期化処理
//==========================================
void InitCountdown()
{
	//変数の初期化
	g_countdown.pos = COUNTDOWN_POS;
	g_countdown.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_countdown.fWidth = COUNTDOWN_POLYGON_WIDTH1;
	g_countdown.fHeight = COUNTDOWN_POLYGON_HEIGHT;
	g_countdown.fScale = 0.0f;
	g_countdown.nNumCnt = COUNTDOWN_CNT_MAX;
	
	g_nFreezeFrame = FREEZE_TIME;
	g_bSkip = false;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCountdown,
		NULL
	);

	for (int nCntTex = 0; nCntTex < CD_TEX_MAX; nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile
		(
			pDevice,
			c_apFilenameCD[nCntTex],
			&g_pTextureCountdown[nCntTex]
		);
	}

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffCountdown->Lock(0, 0, (void**)&pVtx, 0);

	
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//rhwの設定
		pVtx[nCnt].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[nCnt].col = g_countdown.col;
	}

	//頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(g_countdown.pos.x - (g_countdown.fWidth * g_countdown.fScale), g_countdown.pos.y - (g_countdown.fHeight * g_countdown.fScale), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_countdown.pos.x + (g_countdown.fWidth * g_countdown.fScale), g_countdown.pos.y - (g_countdown.fHeight * g_countdown.fScale), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_countdown.pos.x - (g_countdown.fWidth * g_countdown.fScale), g_countdown.pos.y + (g_countdown.fHeight * g_countdown.fScale), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_countdown.pos.x + (g_countdown.fWidth * g_countdown.fScale), g_countdown.pos.y + (g_countdown.fHeight * g_countdown.fScale), 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(g_countdown.nNumCnt * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(g_countdown.nNumCnt * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(g_countdown.nNumCnt * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_countdown.nNumCnt * 0.1f + 0.1f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffCountdown->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitCountdown()
{
	//頂点バッファの破棄
	if (g_pVtxBuffCountdown != NULL)
	{
		g_pVtxBuffCountdown->Release();
		g_pVtxBuffCountdown = NULL;
	}

	for (int nCntTex = 0; nCntTex < CD_TEX_MAX; nCntTex++)
	{
		if (g_pTextureCountdown[nCntTex] != NULL)
		{
			g_pTextureCountdown[nCntTex]->Release();
			g_pTextureCountdown[nCntTex] = NULL;
		}
	}
}

//==========================================
//  更新処理
//==========================================
void UpdateCountdown()
{
	if ((GetKeyboardTrigger(DIK_RETURN) ||
		GetKeyboardTrigger(DIK_SPACE) ||
		GetJoyPadButtonTrigger(KEY_A, 0) ||
		GetJoyPadButtonTrigger(KEY_B, 0))
		&& g_bSkip == false)
	{
		g_countdown.nNumCnt = 0;
		g_countdown.fScale = 0.0f;
		g_countdown.col.a = 1.0f;
		g_nFreezeFrame = FREEZE_TIME;
	}

	if (g_nFreezeFrame >= FREEZE_TIME || g_nFreezeFrame < 0)
	{//フリーズ時以外
		g_countdown.fScale += SIZE_LOOP;
	}

	if (g_countdown.fScale >= 1.0f)
	{//規定値以上
		g_nFreezeFrame--;

		if (g_nFreezeFrame <= 0)
		{//フリーズ時以外
			g_countdown.col.a -= COUNTDOWN_ALPHA_SCALE;
			g_nFreezeFrame = -1;
		}
	}

	if (g_countdown.col.a <= 0.0f)
	{//透明になったら元通り
		g_countdown.nNumCnt--;
		g_countdown.fScale = 0.0f;
		g_countdown.col.a = 1.0f;
		g_nFreezeFrame = FREEZE_TIME;
	}

	if (g_countdown.nNumCnt <= 0)
	{
		g_bSkip = true;
	}

	//幅調整
	switch (g_countdown.nNumCnt)
	{
	case 0:
		g_countdown.fWidth = COUNTDOWN_POLYGON_WIDTH;
		break;

	default:
		g_countdown.fWidth = COUNTDOWN_POLYGON_WIDTH1;
		break;
	}
	
	//ゲームスタート
	if (g_countdown.nNumCnt == -1)
	{
		SetGameState(GAMESTATE_NORMAL, 0);
	}

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffCountdown->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(g_countdown.pos.x - (g_countdown.fWidth * g_countdown.fScale), g_countdown.pos.y - (g_countdown.fHeight * g_countdown.fScale), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_countdown.pos.x + (g_countdown.fWidth * g_countdown.fScale), g_countdown.pos.y - (g_countdown.fHeight * g_countdown.fScale), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_countdown.pos.x - (g_countdown.fWidth * g_countdown.fScale), g_countdown.pos.y + (g_countdown.fHeight * g_countdown.fScale), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_countdown.pos.x + (g_countdown.fWidth * g_countdown.fScale), g_countdown.pos.y + (g_countdown.fHeight * g_countdown.fScale), 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//頂点カラーの更新
		pVtx[nCnt].col = g_countdown.col;
	}

	if (g_countdown.nNumCnt == 0)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	else
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(g_countdown.nNumCnt * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_countdown.nNumCnt * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_countdown.nNumCnt * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_countdown.nNumCnt * 0.1f + 0.1f, 1.0f);
	}

	//頂点バッファをアンロック
	g_pVtxBuffCountdown->Unlock();
}

//==========================================
//  描画処理
//==========================================
void DrawCountdown()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//Zテストを無効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCountdown, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	switch (g_countdown.nNumCnt)
	{
	case 0:
		pDevice->SetTexture(0, g_pTextureCountdown[1]);
		break;
	default:
		pDevice->SetTexture(0, g_pTextureCountdown[0]);
		break;
	}

	//ロゴの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//Zテストを有効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}
