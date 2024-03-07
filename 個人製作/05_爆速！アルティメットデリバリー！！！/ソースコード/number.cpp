//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "number.h"
#include "renderer.h"
#include "manager.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CNumber::CNumber(int nPriority = 6) : CObject2D(nPriority)
{
	m_nNumber = 0;
}

CNumber::~CNumber()
{
}

//=====================================
// 生成処理
//=====================================
CNumber *CNumber::Create(void)
{
	CNumber *pObjectNumber;

	//2Dオブジェクトの生成
	pObjectNumber = new CNumber();

	if (pObjectNumber != NULL)
	{
		//初期化
		if (FAILED(pObjectNumber->Init()))
		{
			pObjectNumber->Release();
		}

		pObjectNumber->BindTexture(m_pTexture);
	}

	return pObjectNumber;
}

//=====================================
// ポリゴンのテクスチャ処理
//=====================================
HRESULT CNumber::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//テクスチャの読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number001.png",
		&m_pTexture)))
	{
		return E_FAIL;
	}

	return S_OK;
}
void CNumber::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CNumber::Init(void)
{
	CObject2D::Init();

	SetType(TYPE_NUMBER);

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CNumber::Uninit(void)
{
	CObject2D::Uninit();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CNumber::Update(void)
{
	D3DXVECTOR2 *texpos = GetTexPos();

	texpos[0] = D3DXVECTOR2(0.1f * m_nNumber, 0.0f);
	texpos[1] = D3DXVECTOR2(0.1f + (0.1f * m_nNumber), 0.0f);
	texpos[2] = D3DXVECTOR2(0.1f * m_nNumber, 1.0f);
	texpos[3] = D3DXVECTOR2(0.1f + (0.1f * m_nNumber), 1.0f);

	CObject2D::Update();
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CNumber::Draw(void)
{
	CObject2D::Draw();
}