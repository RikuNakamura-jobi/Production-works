//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "field.h"
#include "renderer.h"
#include "manager.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
LPDIRECT3DTEXTURE9 CField::m_pTexture = NULL;

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CField::CField(int nPriority = 0) : CObject3D(nPriority)
{
}

CField::~CField()
{
}

//=====================================
// 生成処理
//=====================================
CField *CField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CField *pObjectBg;

	//2Dオブジェクトの生成
	pObjectBg = new CField();

	if (pObjectBg != NULL)
	{
		pObjectBg->SetPos(pos);
		pObjectBg->SetSize(size);

		//初期化
		if (FAILED(pObjectBg->Init()))
		{
			pObjectBg->Release();
		}

		pObjectBg->BindTexture(m_pTexture);
	}

	return pObjectBg;
}

//=====================================
// ポリゴンのテクスチャ処理
//=====================================
HRESULT CField::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//テクスチャの読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\field001.png",
		&m_pTexture)))
	{
		return E_FAIL;
	}

	return S_OK;
}
void CField::Unload(void)
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
HRESULT CField::Init(void)
{
	CObject3D::Init();

	//SetType(TYPE_BG);

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CField::Uninit(void)
{
	CObject3D::Uninit();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CField::Update(void)
{
	//Set(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1280.0f, 720.0f);
	CObject3D::Update();
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CField::Draw(void)
{
	CObject3D::Draw();
}