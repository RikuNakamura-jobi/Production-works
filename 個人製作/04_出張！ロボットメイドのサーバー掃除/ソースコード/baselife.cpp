//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "baselife.h"
#include "renderer.h"
#include "manager.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
LPDIRECT3DTEXTURE9 CBaselife::m_pTexture = NULL;

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CBaselife::CBaselife(int nPriority = 0) : CObject2D(nPriority)
{
}

CBaselife::~CBaselife()
{
}

//=====================================
// 生成処理
//=====================================
CBaselife *CBaselife::Create(void)
{
	CBaselife *pObjectBaselife;

	//2Dオブジェクトの生成
	pObjectBaselife = new CBaselife();

	if (pObjectBaselife != NULL)
	{
		//初期化
		if (FAILED(pObjectBaselife->Init()))
		{
			pObjectBaselife->Release();
		}

		pObjectBaselife->Set(D3DXVECTOR3(640.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f * pObjectBaselife->m_nLife, 30.0f);
		pObjectBaselife->BindTexture(m_pTexture);
	}

	return pObjectBaselife;
}

//=====================================
// ポリゴンのテクスチャ処理
//=====================================
HRESULT CBaselife::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//テクスチャの読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\baselife000.png",
		&m_pTexture)))
	{
		return E_FAIL;
	}

	return S_OK;
}
void CBaselife::Unload(void)
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
HRESULT CBaselife::Init(void)
{
	CObject2D::Init();

	m_nLife = 10;

	SetType(TYPE_BG);

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CBaselife::Uninit(void)
{
	CObject2D::Uninit();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CBaselife::Update(void)
{
	m_nLife = CManager::Get()->GetScene()->GetPlayer()->GetEnegy();

	Set(D3DXVECTOR3(640.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f * m_nLife, 30.0f);
	CObject2D::Update();
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CBaselife::Draw(void)
{
	CObject2D::Draw();
}