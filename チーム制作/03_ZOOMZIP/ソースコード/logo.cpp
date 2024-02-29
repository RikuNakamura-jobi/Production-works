//==========================================================
//
//チームロゴ画面処理 [logo.h]
//Author Ibuki Okusada
//
//==========================================================
#include "main.h"    //作成したmain.hをインクルードする
#include "logo.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

//==========================================================
//マクロ定義
//==========================================================
#define LOGO_X			(SCREEN_WIDTH * 0.5f)	//ロゴX座標
#define LOGO_Y			(SCREEN_HEIGHT * -0.2f)	//ロゴY座標
#define LOGO_MOVEDY		(SCREEN_HEIGHT * 0.5f)	//ロゴ移動後Y座標
#define LOGO_WIDTH		(SCREEN_WIDTH * 0.2f)	//ロゴ幅
#define LOGO_HEIGHT		(SCREEN_HEIGHT * 0.2f)	//ロゴ高さ
#define LOGO_FADECNT	(120)					//遷移カウント
#define LOGO_ROTATECNT	(80)					//回転カウンター
#define LOGO_ROTATESPD	(D3DX_PI * 0.1f)		//回転速度
#define MAX_LENGTH		(500.0f)				//最大長さ
#define LOGO_LENGSPD	(4.0f)					//長さ増加速度
#define LOGO_YMOVESPD	((LOGO_MOVEDY - LOGO_Y) * (float)(1.0f / LOGO_ROTATECNT))	//y座標移動量
#define SUBLOGO_COLSPD	(35)					//色変化タイマー
#define SUBLOGO_X		(SCREEN_WIDTH * 0.2f)	//サブロゴX座標
#define SUBLOGO_Y		(SCREEN_HEIGHT * 0.8f)	//サブロゴY座標
#define SUBLOGO_WIDTH	(SCREEN_WIDTH * 0.2f)	//ロゴ幅
#define SUBLOGO_HEIGHT	(SCREEN_HEIGHT * 0.25f)	//ロゴ高さ
#define SUBLOGO_SPACEX	(SCREEN_WIDTH * 0.2f)	//ロゴ幅スペース

//==========================================================
//構造体の定義
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
	D3DXCOLOR col;		//色
	float fAngle;		//角度
	float fLength;		//対角線の長さ
	float fHeight;		//高さ
	float fWidth;		//幅
	int nMoveCounter;	//移動カウンター
}Logo;

//==========================================================
//グローバル変数
//==========================================================
LPDIRECT3DTEXTURE9 g_apTextureLogo[LOGOTYPE_MAX] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLogo = NULL;				//頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLogoBg = NULL;			//頂点バッファへのポインタ
int g_nLogoFadeCounter = 0;									//ロゴ遷移カウンター
Logo g_aLogo[LOGOTYPE_MAX];	//ロゴの情報配列

//==========================================================
//プロトタイプ宣言
//==========================================================
void SetpVtxLogoBg(void);
void SetpVtxLogo(void);
void DrawLogoBg(void);
Logo *GetLogo(void);
void AppearLogo(void);
void ChangeLogoCola(Logo *pLogo);

//==========================================================
//テクスチャファイル名
//==========================================================
const char* c_apFilenameLogo[LOGOTYPE_MAX] =
{
	"data\\TEXTURE\\team_logo000.png",		//ロゴ元
	"data\\TEXTURE\\team_logo001.png",		//ロゴのコピー
	"data\\TEXTURE\\team_logo002.png",		//ロゴのコピー(2)
	"data\\TEXTURE\\team_logo003.png",		//ロゴのコピー(3)
	"data\\TEXTURE\\team_logo004.png",		//ロゴのコピー(4)
};

//==========================================================
//チームロゴの初期化処理
//==========================================================
void InitLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	Logo *pLogo = GetLogo();	//ロゴのポインタ

	//テクスチャの読み込み
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameLogo[nCnt],
			&g_apTextureLogo[nCnt]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * LOGOTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLogo,
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLogoBg,
		NULL);

	//情報初期化
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX - 1; nCnt++, pLogo++)
	{
		pLogo->pos = D3DXVECTOR3(SUBLOGO_X + nCnt * SUBLOGO_SPACEX, SUBLOGO_Y, 0.0f);
		pLogo->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pLogo->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pLogo->fHeight = SUBLOGO_HEIGHT;
		pLogo->fWidth = SUBLOGO_WIDTH;
		pLogo->fAngle = 0.0f;
		pLogo->fLength = 0.0f;
		pLogo->nMoveCounter;

		//対角線の長さを算出する
		pLogo->fLength = sqrtf(SUBLOGO_WIDTH * SUBLOGO_WIDTH + SUBLOGO_HEIGHT * SUBLOGO_HEIGHT) * 0.5f;

		//対角線の角度を算出する
		pLogo->fAngle = atan2f(SUBLOGO_WIDTH, SUBLOGO_HEIGHT);
	}

	pLogo->pos = D3DXVECTOR3(LOGO_X, LOGO_Y, 0.0f);
	pLogo->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pLogo->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pLogo->fHeight = LOGO_HEIGHT;
	pLogo->fWidth = LOGO_WIDTH;
	pLogo->fAngle = 0.0f;
	pLogo->fLength = 0.0f;
	pLogo->nMoveCounter;

	//対角線の長さを算出する
	pLogo->fLength = sqrtf(LOGO_WIDTH * LOGO_WIDTH + LOGO_HEIGHT * LOGO_HEIGHT) * 0.5f;

	//対角線の角度を算出する
	pLogo->fAngle = atan2f(LOGO_WIDTH, LOGO_HEIGHT);

	//背景設定
	SetpVtxLogoBg();

	//ロゴ設定
	SetpVtxLogo();
}

//==========================================================
//チームロゴの終了処理
//==========================================================
void UninitLogo(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		if (g_apTextureLogo[nCnt] != NULL)
		{
			g_apTextureLogo[nCnt]->Release();
			g_apTextureLogo[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffLogo != NULL)
	{
		g_pVtxBuffLogo->Release();
		g_pVtxBuffLogo = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffLogoBg != NULL)
	{
		g_pVtxBuffLogoBg->Release();
		g_pVtxBuffLogoBg = NULL;
	}
}

//==========================================================
//チームロゴの更新処理
//==========================================================
void UpdateLogo(void)
{
	if (g_nLogoFadeCounter >= LOGO_FADECNT ||
		GetKeyboardTrigger(DIK_RETURN) == true || 
		GetGamepadTrigger(BUTTON_A, 0) == true || 
		GetGamepadTrigger(BUTTON_START,0) == true)
	{//カウンター規定値、またはボタン入力
		g_nLogoFadeCounter = 0;	//カウンターリセット
		SetFade(MODE_TITLE);	//タイトルへ遷移
		PlaySound(SOUND_LABEL_SEBUTTON);
	}

	if (g_aLogo[LOGOTYPE_COPY002].col.a >= 1.0f)
	{//全てのロゴが不透明になった場合
		g_aLogo[LOGOTYPE_COPY003].nMoveCounter++;

		if (g_aLogo[LOGOTYPE_COPY003].nMoveCounter <= LOGO_ROTATECNT)
		{//移動カウンターが回転カウントより少ない場合

			g_aLogo[LOGOTYPE_COPY003].rot.z += LOGO_ROTATESPD;	//回転
			g_aLogo[LOGOTYPE_COPY003].pos.y += LOGO_YMOVESPD;	//Y座標移動
			g_aLogo[LOGOTYPE_COPY003].col.a += (1.0f * (float)(1.0f / LOGO_ROTATECNT));	//不透明に近づける

			if (g_aLogo[LOGOTYPE_COPY003].rot.z >= D3DX_PI)
			{//角度限界超えた場合
				g_aLogo[LOGOTYPE_COPY003].rot.z = -D3DX_PI;	//反転する
			}
		}
		else
		{
			g_aLogo[LOGOTYPE_COPY003].fLength += LOGO_LENGSPD;

			if (g_aLogo[LOGOTYPE_COPY003].fLength > MAX_LENGTH)
			{//長さが超えた場合
				g_aLogo[LOGOTYPE_COPY003].fLength = MAX_LENGTH;
				g_nLogoFadeCounter++;
			}
		}
	}
	else
	{
		//不透明処理
		AppearLogo();
	}

	//ロゴの頂点情報設定
	SetpVtxLogo();
}

//==========================================================
//チームロゴの描画処理
//==========================================================
void DrawLogo(void)
{
	//背景描画
	DrawLogoBg();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLogo, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureLogo[nCnt]);

		//ポリゴンの描画(開始頂点移動)
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}

//==========================================================
//チームロゴ背景の頂点情報設定
//==========================================================
void SetpVtxLogoBg(void)
{
	VERTEX_2D * pVtx;   //頂点情報へのポインタ

	//頂点バッファをロック
	g_pVtxBuffLogoBg->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffLogoBg->Unlock();
}

//==========================================================
//チームロゴの頂点情報設定
//==========================================================
void SetpVtxLogo(void)
{
	Logo *pLogo = GetLogo();	//ロゴのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffLogo->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++, pLogo++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = pLogo->pos.x + sinf(pLogo->rot.z + (-D3DX_PI + pLogo->fAngle)) * pLogo->fLength;
		pVtx[0].pos.y = pLogo->pos.y + cosf(pLogo->rot.z + (-D3DX_PI + pLogo->fAngle)) * pLogo->fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = pLogo->pos.x + sinf(pLogo->rot.z + (D3DX_PI - pLogo->fAngle)) * pLogo->fLength;
		pVtx[1].pos.y = pLogo->pos.y + cosf(pLogo->rot.z + (D3DX_PI - pLogo->fAngle)) * pLogo->fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = pLogo->pos.x + sinf(pLogo->rot.z + -pLogo->fAngle) * pLogo->fLength;
		pVtx[2].pos.y = pLogo->pos.y + cosf(pLogo->rot.z + -pLogo->fAngle) * pLogo->fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = pLogo->pos.x + sinf(pLogo->rot.z + pLogo->fAngle) * pLogo->fLength;
		pVtx[3].pos.y = pLogo->pos.y + cosf(pLogo->rot.z + pLogo->fAngle) * pLogo->fLength;
		pVtx[3].pos.z = 0.0f;

		//頂点カラーの設定
		pVtx[0].col = pLogo->col;
		pVtx[1].col = pLogo->col;
		pVtx[2].col = pLogo->col;
		pVtx[3].col = pLogo->col;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffLogo->Unlock();
}

//==========================================================
//チームロゴ背景の描画処理
//==========================================================
void DrawLogoBg(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLogoBg, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================
//チームロゴの情報取得
//==========================================================
Logo *GetLogo(void)
{
	return &g_aLogo[0];
}

//==========================================================
//チームロゴの出現処理
//==========================================================
void AppearLogo(void)
{
	Logo *pLogo = GetLogo();	//ロゴのポインタ

	for (int nCntLogo = 0; nCntLogo < LOGOTYPE_MAX - 1; nCntLogo++, pLogo++)
	{
		if (nCntLogo == 0 && pLogo->col.a < 1.0f)
		{//カウント0で
			ChangeLogoCola(pLogo);	//色変更
		}
		else if (nCntLogo != 0)
		{
			if ((pLogo - 1)->col.a >= 1.0f && pLogo->col.a < 1.0f)
			{//一つ前が不透明かつ現在のが不透明ではない場合
				ChangeLogoCola(pLogo);	//色変更
			}
		}
	}
}

//==========================================================
//チームロゴの不透明処理
//==========================================================
void ChangeLogoCola(Logo *pLogo)
{
	if (pLogo->col.a < 1.0f)
	{//色が完全に不透明ではない場合
		pLogo->col.a += 1.0f * (float)(1.0f / SUBLOGO_COLSPD);	//不透明に近づける

		if (pLogo->col.a > 1.0f)
		{//色限界を超えた
			pLogo->col.a = 1.0f;	//不透明に
		}
	}
}