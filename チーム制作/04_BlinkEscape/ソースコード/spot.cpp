//==========================================
//
//  地点クラス(spot.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "spot.h"
#include "manager.h"
#include "debugproc.h"
#include "enemygroup.h"

//==========================================
//  コンストラクタ
//==========================================
CSpot::CSpot() : CObject2D(LABEL_NONE)
{
	m_pGroup = NULL;
}

//==========================================
//  デストラクタ
//==========================================
CSpot::~CSpot()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CSpot::Init(void)
{
	return CObject2D::Init();
}

//==========================================
//  終了処理
//==========================================
void CSpot::Uninit(void)
{
	if (m_pGroup != nullptr)
	{
		m_pGroup->Uninit();
		m_pGroup = nullptr;
	}

	CObject2D::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CSpot::Update(void)
{
	if (m_pGroup != nullptr)
	{
		m_pGroup->SetPos(GetPosition());
		m_pGroup->Update();
	}

	CObject2D::Update();
	
	//デバッグ表示
	D3DXVECTOR3 pos = GetPosition();
}

//==========================================
//  描画処理
//==========================================
void CSpot::Draw(void)
{
	CObject2D::Draw();
}

//==========================================
//  生成処理
//==========================================
CSpot* CSpot::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//オブジェクトを生成
	CSpot* pSpot = nullptr;

	//メモリを確保
	if (pSpot == nullptr)
	{
		pSpot = new CSpot;

		if (pSpot != nullptr)
		{
			//初期化処理
			pSpot->Init();

			//各データの設定
			pSpot->SetPosition(pos);
			pSpot->SetScaling(size);
			pSpot->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

			pSpot->m_pGroup = CEnemyGroup::Create(pSpot->GetPosition(), (float)((rand() % 300) + 200));
		}
	}

	return pSpot;
}
