//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "bgmulti.h"
#include "renderer.h"
#include "manager.h"
#include "object2D.h"
#include "bg.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
LPDIRECT3DTEXTURE9 CBgMulti::m_pTexture[MAX_BGMULTI] = {};
const char *CBgMulti::m_apFilename[MAX_BGMULTI] = 
{
	"data\\TEXTURE\\sky001.png",
	"data\\TEXTURE\\title001.png",
	"data\\TEXTURE\\title002.png"
};

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CBgMulti::CBgMulti(int nPriority = 0) : CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_colMove = 0.01f;
}

CBgMulti::~CBgMulti()
{
}

//=====================================
// 生成処理
//=====================================
CBgMulti *CBgMulti::Create(void)
{
	CBgMulti *pBgMulti;

	//2Dオブジェクトの生成
	pBgMulti = new CBgMulti();

	if (pBgMulti != NULL)
	{
		//初期化
		if (FAILED(pBgMulti->Init()))
		{
			pBgMulti->Release();
		}
	}

	return NULL;
}

//=====================================
// ポリゴンのテクスチャ処理
//=====================================
HRESULT CBgMulti::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->Get()->GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < MAX_BGMULTI; nCnt++)
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
void CBgMulti::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_BGMULTI; nCnt++)
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
HRESULT CBgMulti::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_BGMULTI; nCnt++)
	{
		m_apObject2D[nCnt] = CBg::Create(CBg::TEXTURE_MAX);

		if (m_apObject2D[nCnt] != NULL)
		{
			m_apObject2D[nCnt]->BindTexture(m_pTexture[nCnt]);
		}
	}

	SetType(TYPE_BGMULTI);

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CBgMulti::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_BGMULTI; nCnt++)
	{
		if (m_apObject2D[nCnt] != NULL)
		{
			m_apObject2D[nCnt]->Uninit();
			m_apObject2D[nCnt] = NULL;
		}
	}

	Release();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CBgMulti::Update(void)
{
	m_posOld = m_pos;

	for (int nCnt = 0; nCnt < MAX_BGMULTI; nCnt++)
	{
		if (m_apObject2D[nCnt] != NULL)
		{
			D3DXVECTOR2 *tex = m_apObject2D[nCnt]->GetTexPos();

			tex[0] = D3DXVECTOR2(0.0f, 0.0f);
			tex[1] = D3DXVECTOR2(1.0f, 0.0f);
			tex[2] = D3DXVECTOR2(0.0f, 1.0f);
			tex[3] = D3DXVECTOR2(1.0f, 1.0f);

			if (nCnt == 2)
			{
				D3DXCOLOR col = m_apObject2D[nCnt]->GetCol();

				col.a += m_colMove;

				if (col.a > 1.0f)
				{
					col.a = 1.0f;
					m_colMove = -0.01f;
				}

				if (col.a < 0.0f)
				{
					col.a = 0.0f;
					m_colMove = 0.01f;
				}

				m_apObject2D[nCnt]->SetCol(col);
			}

			m_apObject2D[nCnt]->Update();
		}
	}
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CBgMulti::Draw(void)
{
	
}

//=====================================
// ポリゴンの設定処理
//=====================================
void CBgMulti::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	m_pos = pos;
	m_rot = rot;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}