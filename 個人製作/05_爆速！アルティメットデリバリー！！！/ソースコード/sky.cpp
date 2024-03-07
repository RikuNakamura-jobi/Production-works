//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "sky.h"
#include "renderer.h"
#include "manager.h"
#include "object3D.h"
#include "bg.h"
#include "skybox.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
LPDIRECT3DTEXTURE9 CSky::m_pTexture[MAX_SKY] = {};
const char *CSky::m_apFilename[MAX_SKY] =
{
	"data\\TEXTURE\\sky001.png",
	"data\\TEXTURE\\sky001.png",
	"data\\TEXTURE\\sky001.png",
	"data\\TEXTURE\\sky001.png",
	"data\\TEXTURE\\sky002.png"
};

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CSky::CSky(int nPriority = 0) : CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_SkyBox = nullptr;
}

CSky::~CSky()
{
}

//=====================================
// 生成処理
//=====================================
CSky *CSky::Create(void)
{
	CSky *pBgMulti;

	//2Dオブジェクトの生成
	pBgMulti = new CSky();

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
HRESULT CSky::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < MAX_SKY; nCnt++)
	{
		//テクスチャの読み込み
		if (FAILED(D3DXCreateTextureFromFile(pDevice,
			m_apFilename[nCnt],
			&m_pTexture[nCnt])))
		{
			return E_FAIL;
		}
	}

	CSkybox::Load();

	return S_OK;
}
void CSky::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_SKY; nCnt++)
	{
		//テクスチャの破棄
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}

	CSkybox::Unload();
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CSky::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_SKY; nCnt++)
	{
		//m_apObject3D[nCnt] = CObject3D::Create();

		if (m_apObject3D[nCnt] != NULL)
		{
			switch (nCnt)
			{
			case 0:

				m_apObject3D[nCnt]->SetPosSizeX(D3DXVECTOR3(SKY_LENGTH * 4.0f - 4000.0f, 1000.0f, 0.0f), D3DXVECTOR3(0.0f, SKY_LENGTH * 4.0f, SKY_LENGTH * 4.0f));

				break;

			case 1:

				m_apObject3D[nCnt]->SetPosSize(D3DXVECTOR3(0.0f, 1000.0f, -4000.0f), D3DXVECTOR3(SKY_LENGTH * 4.0f, SKY_LENGTH * 4.0f, 0.0f));

				break;

			case 2:

				m_apObject3D[nCnt]->SetPosSizeX(D3DXVECTOR3(-4000.0f, 1000.0f, 0.0f), D3DXVECTOR3(0.0f, SKY_LENGTH* 4.0f, -SKY_LENGTH * 4.0f));

				break;

			case 3:

				m_apObject3D[nCnt]->SetPosSize(D3DXVECTOR3(0.0f, 1000.0f, SKY_LENGTH * 4.0f - 4000.0f), D3DXVECTOR3(-SKY_LENGTH * 4.0f, SKY_LENGTH* 4.0f, 0.0f));

				break;

			case 4:

				m_apObject3D[nCnt]->SetPosSize(D3DXVECTOR3(0.0f, SKY_LENGTH * 4.0f + 1000.0f, 0.0f), D3DXVECTOR3(-SKY_LENGTH * 4.1f, 0.0f, SKY_LENGTH * 4.1f));

				break;
			}

			m_apObject3D[nCnt]->BindTexture(m_pTexture[nCnt]);
			m_apObject3D[nCnt]->SetType(CObject::TYPE_SKY);
		}

		m_SkyBox = CSkybox::Create(D3DXVECTOR3(36000.0f, -30000.0f, 36000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
	}

	SetType(TYPE_BGMULTI);

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CSky::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_SKY; nCnt++)
	{
		if (m_apObject3D[nCnt] != NULL)
		{
			m_apObject3D[nCnt]->Uninit();
		}
	}

	Release();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CSky::Update(void)
{
	m_posOld = m_pos;

	/*for (int nCnt = 0; nCnt < MAX_SKY; nCnt++)
	{
		if (m_apObject3D[nCnt] != NULL)
		{
			D3DXVECTOR2 *texPos = NULL;

			texPos = m_apObject3D[nCnt]->GetTexPos();

			texPos[0].x += 0.001f * nCnt;
			texPos[1].x += 0.001f * nCnt;
			texPos[2].x += 0.001f * nCnt;
			texPos[3].x += 0.001f * nCnt;

			m_apObject3D[nCnt]->Update();
		}
	}*/
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CSky::Draw(void)
{
	
}

//=====================================
// ポリゴンの設定処理
//=====================================
void CSky::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	m_pos = pos;
	m_rot = rot;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}