//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "bg.h"
#include "renderer.h"
#include "manager.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
LPDIRECT3DTEXTURE9 CBg::m_pTexture[TEXTURE_MAX] = {};
const char *CBg::m_apFilename[TEXTURE_MAX] =
{
	"data\\TEXTURE\\result001.png",
	"data\\TEXTURE\\effect000.jpg",
	"data\\TEXTURE\\effect000.jpg",
	"data\\TEXTURE\\effect000.jpg",
	"data\\TEXTURE\\effect000.jpg",
	"data\\TEXTURE\\effect000.jpg",
	"data\\TEXTURE\\effect000.jpg",
	"data\\TEXTURE\\game000.png",
	"data\\TEXTURE\\success000.png",
	"data\\TEXTURE\\Failed.png",
	"data\\TEXTURE\\tutorialStart000.png"
};

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CBg::CBg(int nPriority = 6) : CObject2D(nPriority)
{
}

CBg::~CBg()
{
}

//=====================================
// 生成処理
//=====================================
CBg *CBg::Create(TEXTURE texture)
{
	CBg *pObjectBg;

	//2Dオブジェクトの生成
	pObjectBg = new CBg();

	if (pObjectBg != NULL)
	{
		//初期化
		if (FAILED(pObjectBg->Init()))
		{
			pObjectBg->Release();
		}

		if (texture == TEXTURE_MAX)
		{
			pObjectBg->BindTexture(NULL);
		}
		else
		{
			pObjectBg->BindTexture(m_pTexture[texture]);
			pObjectBg->m_textureType = texture;
		}
	}

	return pObjectBg;
}

//=====================================
// ポリゴンのテクスチャ処理
//=====================================
HRESULT CBg::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->Get()->GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		//テクスチャの読み込み
		if (FAILED(D3DXCreateTextureFromFile(pDevice,
			m_apFilename[nCnt],
			&m_pTexture[nCnt])))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}
void CBg::Unload(void)
{
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		//テクスチャの破棄
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CBg::Init(void)
{
	CObject2D::Init();

	SetType(TYPE_BG);

	Set(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1280.0f, 720.0f);

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CBg::Uninit(void)
{
	CObject2D::Uninit();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CBg::Update(void)
{
	CObject2D::Update();
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CBg::Draw(void)
{
	CObject2D::Draw();
}