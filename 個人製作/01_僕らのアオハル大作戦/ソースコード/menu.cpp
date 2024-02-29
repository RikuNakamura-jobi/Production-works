//===========================
//
//ダイレクトX.menuファイル
//Author:中村　陸
//
//===========================
#include "main.h"
#include "menu.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//マクロ定義
#define NUM_MENU (4)											//タイトル画面の数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureMenu[NUM_MENU] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffMenu = NULL;					//頂点情報を格納
int g_nCounterMenuState;										//状態管理カウンター
int nMenuSelect;
float g_fCntMenu = 0.0f;										//タイトルの点滅用変数
bool g_MenuEnd;												//終了判定

//===========================
//タイトル画面の初期化処理
//===========================
void InitMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntMenu;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\aoharuBG002.png",
		&g_apTextureMenu[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\menu001.png",
		&g_apTextureMenu[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\menu002.png",
		&g_apTextureMenu[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\menu003.png",
		&g_apTextureMenu[3]);

	g_nCounterMenuState = 120;
	g_fCntMenu = 0.0f;
	nMenuSelect = 0;
	g_MenuEnd = false;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_MENU,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffMenu,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMenu = 0; nCntMenu < NUM_MENU; nCntMenu++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_VtxBuffMenu->Unlock();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM007);
}

//===========================
//タイトル画面の終了処理
//===========================
void UninitMenu(void)
{
	int nCntMenu;

	//サウンドの停止
	StopSound();

	for (nCntMenu = 0; nCntMenu < NUM_MENU; nCntMenu++)
	{
		//テクスチャの破棄
		if (g_apTextureMenu[nCntMenu] != NULL)
		{
			g_apTextureMenu[nCntMenu]->Release();
			g_apTextureMenu[nCntMenu] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_VtxBuffMenu != NULL)
	{
		g_VtxBuffMenu->Release();
		g_VtxBuffMenu = NULL;
	}
}

//===========================
//タイトル画面の更新処理
//===========================
void UpdateMenu(void)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	FADE fade = GetFade();
	int nCntMenu;

	if (g_MenuEnd == false)
	{
		col.a = (sinf(g_fCntMenu) + 1.0f) / 2;

		g_fCntMenu += 0.05f;
	}
	else
	{
		col.a = (sinf(g_fCntMenu) + 1.0f) / 2;

		g_fCntMenu += 0.5f;
	}

	//入力カーソル移動
	if (GetKeyboardTrigger(DIK_S) == true && g_MenuEnd == false)
	{
		nMenuSelect++;
	}

	if (GetKeyboardTrigger(DIK_W) == true && g_MenuEnd == false)
	{
		nMenuSelect += 2;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_VtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMenu = 0; nCntMenu < NUM_MENU; nCntMenu++)
	{
		if (nCntMenu == (nMenuSelect % 3) + 1)
		{
			//頂点カラーの設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;
		}
		else if (nCntMenu >= 1)
		{
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		}
		else
		{
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_VtxBuffMenu->Unlock();

	if (GetKeyboardTrigger(DIK_RETURN) == true && g_MenuEnd == false)
	{
		g_MenuEnd = true;

		//サウンドの再生
		PlaySound(SOUND_LABEL_SE_DECISION002);
	}

	if (g_MenuEnd == true)
	{
		g_nCounterMenuState--;

		if (g_nCounterMenuState <= 0 && fade == FADE_NONE)
		{
			if (nMenuSelect % 3 == 0)
			{
				//モード設定
				SetFade(MODE_SELECT);
			}
			else if (nMenuSelect % 3 == 1)
			{
				//モード設定
				SetFade(MODE_TUTORIAL);
			}
			else if(nMenuSelect % 3 == 2)
			{
				//モード設定
				SetFade(MODE_TITLE);
			}
		}
	}
}

//===========================
//タイトル画面の描画処理
//===========================
void DrawMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntMenu;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		g_VtxBuffMenu,
		0,
		sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntMenu = 0; nCntMenu < NUM_MENU; nCntMenu++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureMenu[nCntMenu]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMenu * 4, 2);
	}
}