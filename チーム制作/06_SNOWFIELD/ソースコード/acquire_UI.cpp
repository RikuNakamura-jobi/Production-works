//=======================================
//
// 旗取得UIのメイン処理[acquire_UI.cpp]
// Author 小原立暉
//
//=======================================
#include "main.h"
#include "manager.h"
#include "acquire_UI.h"

#include "objectbillboard.h"

//---------------------------------------
// 無名名前空間
//---------------------------------------
namespace
{
	const D3DXVECTOR3 UI_SIZE[CAcquireUI::TYPE_MAX] = 
	{
		D3DXVECTOR3(60.0f, 20.0f, 0.0f),
		D3DXVECTOR3(0.0f, 20.0f, 0.0f),
	};
	const D3DXCOLOR UI_COLOR[CAcquireUI::TYPE_MAX] =
	{
		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
	};
}

//==========================================
// コンストラクタ
//==========================================
CAcquireUI::CAcquireUI() : CObject(TYPE_ACQUIREUI, PRIORITY_UI)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apUI[nCnt] = nullptr;						// ポリゴンの情報
	}
}

//==========================================
// デストラクタ
//==========================================
CAcquireUI::~CAcquireUI()
{

}

//==========================================
// ポーズの初期化処理
//==========================================
HRESULT CAcquireUI::Init(void)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apUI[nCnt] == nullptr)
		{ // ポリゴンが NULL の場合

			// ポリゴンを生成する
			m_apUI[nCnt] = CBillboard::Create(TYPE_NONE, PRIORITY_UI);
		}
	}

	// 成功を返す
	return S_OK;
}

//========================================
// ポーズの終了処理
//========================================
void CAcquireUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apUI[nCnt] != nullptr)
		{ // ポリゴンが NULL じゃない場合

			// 終了処理
			m_apUI[nCnt]->Uninit();
			m_apUI[nCnt] = nullptr;
		}
	}

	// 破棄処理
	Release();
}

//========================================
// ポーズの更新処理
//========================================
void CAcquireUI::Update(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// 頂点情報の設定処理
		m_apUI[nCnt]->SetVtxLeft(m_apUI[nCnt]->GetSize());
	}
}

//=====================================
// ポーズの描画処理
//=====================================
void CAcquireUI::Draw(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// 描画処理
		m_apUI[nCnt]->Draw();
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CAcquireUI::SetData(const D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apUI[nCnt] != nullptr)
		{ // ポリゴンが NULL じゃない場合

			// 情報の設定処理
			m_apUI[nCnt]->SetPos(pos);					// 位置
			m_apUI[nCnt]->SetPosOld(pos);				// 前回の位置
			m_apUI[nCnt]->SetSize(UI_SIZE[nCnt]);		// サイズ

			switch (nCnt)
			{
			case CAcquireUI::TYPE_GAGE:

				// 頂点座標の設定処理
				m_apUI[nCnt]->SetVertex();

				break;

			case CAcquireUI::TYPE_METER:

				// 頂点座標の設定処理
				m_apUI[nCnt]->SetVtxLeft(UI_SIZE[nCnt]);

				break;

			default:

				// 停止
				assert(false);

				break;
			}

			// 頂点カラーの設定処理
			m_apUI[nCnt]->SetVtxColor(UI_COLOR[nCnt]);
		}
	}
}

//=====================================
// 生成処理
//=====================================
CAcquireUI* CAcquireUI::Create(const D3DXVECTOR3 pos)
{
	// ローカルオブジェクトを生成
	CAcquireUI* pUI = nullptr;			// UIのインスタンスを生成

	if (pUI == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pUI = new CAcquireUI;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pUI != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pUI->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pUI->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// UIのポインタを返す
	return pUI;
}

//=====================================
// ポリゴンの情報の取得処理
//=====================================
CBillboard* CAcquireUI::GetPolygon(const TYPE type)
{
	// 種類を返す
	return m_apUI[type];
}