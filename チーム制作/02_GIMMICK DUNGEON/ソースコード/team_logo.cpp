//==========================================
//
//  チームロゴ画面(team_logo.h)
//  Author : Kai Takada
//
//==========================================
#include "team_logo.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//==========================================
//  マクロ定義
//==========================================
#define IN_TIME (240)	//タイトル状態が切り替わるまでのタイマー
#define RANKING_FROM_TEAM_LOGO (420)	//ランキングに遷移するまでの時間

#define PASS_TITLE_LOGO "data/TEXTURE/Tk_software_01.png"	 //テクスチャパス
#define POS_DEFAULT (D3DXVECTOR3(640.0f, 360.0f ,0.0f))	 //初期位置
#define LOGO_POLYGON_HEIGHT (50.0f)		//ポリゴンの高さ
#define LOGO_POLYGON_WIDTH (LOGO_POLYGON_HEIGHT * 6.86f)	 //ポリゴンの幅
#define LOGO_ALPHA_SCALE (1.0f / IN_TIME)	//アルファ値の上昇量

//==========================================
//  構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //中心座標
	D3DXCOLOR col; //ポリゴンカラー
}LOGO;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureTeamLogo = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTeamLogo = NULL;
LOGO g_Teamlogo; 
int g_Team_logoState; //タイトルの状態
int g_nCntTitle; //ランキングに遷移するカウンタ

//==========================================
//  初期化処理
//==========================================
void InitTeam_logo()
{
	//変数の初期化
	g_Team_logoState = TEAM_LOGOSTATE_IN;
	g_nCntTitle = 0;

	//変数の初期化
	g_Teamlogo.pos = POS_DEFAULT;
	g_Teamlogo.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTeamLogo,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		PASS_TITLE_LOGO,
		&g_pTextureTeamLogo
	);

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffTeamLogo->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Teamlogo.pos.x - LOGO_POLYGON_WIDTH, g_Teamlogo.pos.y - LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Teamlogo.pos.x + LOGO_POLYGON_WIDTH, g_Teamlogo.pos.y - LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Teamlogo.pos.x - LOGO_POLYGON_WIDTH, g_Teamlogo.pos.y + LOGO_POLYGON_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Teamlogo.pos.x + LOGO_POLYGON_WIDTH, g_Teamlogo.pos.y + LOGO_POLYGON_HEIGHT, 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//rhwの設定
		pVtx[nCnt].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[nCnt].col = g_Teamlogo.col;
	}

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffTeamLogo->Unlock();

	//BGM再生
	//PlaySound(SOUND_LABEL_BGM000);
}

//==========================================
//  終了処理
//==========================================
void UninitTeam_logo()
{
	//サウンドの停止
	//StopSound();

	//頂点バッファの破棄
	if (g_pVtxBuffTeamLogo != NULL)
	{
		g_pVtxBuffTeamLogo->Release();
		g_pVtxBuffTeamLogo = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureTeamLogo != NULL)
	{
		g_pTextureTeamLogo->Release();
		g_pTextureTeamLogo = NULL;
	}
}

//==========================================
//  更新処理
//==========================================
void UpdateTeam_logo()
{
	//自動遷移カウンタの更新
	if (g_Team_logoState == TEAM_LOGOSTATE_NORMAL)
	{
		g_nCntTitle++;
		if (g_nCntTitle >= RANKING_FROM_TEAM_LOGO)
		{
			SetFade(MODE_TITLE);
		}
	}

	for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
	{//キーボード AnyKey
		if (GetKeyboardTrigger(nCnt) && nCnt != DIK_ESCAPE)
		{
			if (g_Team_logoState == TEAM_LOGOSTATE_NORMAL)
			{
				g_nCntTitle = RANKING_FROM_TEAM_LOGO;
				break;
			}
			else if (g_Team_logoState == TEAM_LOGOSTATE_IN)
			{
				g_Team_logoState = TEAM_LOGOSTATE_NORMAL;
				break;
			}
		}
	}
	for (int nCnt = 0; nCnt <= KEY_START; nCnt++)
	{//Pad AnyKey
		if (GetJoyPadButtonTrigger((JOYKEY_BUTTON)nCnt, 0))
		{
			if (g_Team_logoState == TEAM_LOGOSTATE_NORMAL)
			{
				g_nCntTitle = RANKING_FROM_TEAM_LOGO;
				break;
			}
			else if (g_Team_logoState == TEAM_LOGOSTATE_IN)
			{
				g_Team_logoState = TEAM_LOGOSTATE_NORMAL;
				break;
			}
		}
	}
	if (GetMouseTrigger(MOUSEBUTTON_LEFT))
	{//左クリック
		if (g_Team_logoState == TEAM_LOGOSTATE_NORMAL)
		{
			g_nCntTitle = RANKING_FROM_TEAM_LOGO;
		}
		else if (g_Team_logoState == TEAM_LOGOSTATE_IN)
		{
			g_Team_logoState = TEAM_LOGOSTATE_NORMAL;
		}
	}


	//状態に対応した処理を実行する
	switch (g_Team_logoState)
	{
	case TEAM_LOGOSTATE_IN:
		//ロゴの情報を更新する
		if (g_Teamlogo.col.a <= 1.0f)
		{
			g_Teamlogo.col.a += LOGO_ALPHA_SCALE;
		}

		//ロゴの情報を補正する
		if (g_Teamlogo.col.a > 1.0f)
		{
			g_Teamlogo.col.a = 1.0f;
			g_Team_logoState = TEAM_LOGOSTATE_NORMAL;
		}
		break;
	case TEAM_LOGOSTATE_NORMAL:
		//色を固定する
		if (g_Teamlogo.col.a != 1.0f)
		{
			g_Teamlogo.col.a = 1.0f;
		}
		break;
	default:
		break;
	}

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffTeamLogo->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//頂点カラーの更新
		pVtx[nCnt].col = g_Teamlogo.col;
	}

	//頂点バッファをアンロック
	g_pVtxBuffTeamLogo->Unlock();
}

//==========================================
//  描画処理
//==========================================
void DrawTeam_logo()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTeamLogo, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTeamLogo);

	//ロゴの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
