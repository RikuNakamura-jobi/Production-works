//=======================================
//
// 攻撃側復活地点のメイン処理[off_respawn.cpp]
// Author 小原立暉
//
//=======================================
#include "main.h"
#include "manager.h"
#include "off_respawn.h"

//==========================================
// コンストラクタ
//==========================================
COffRespawn::COffRespawn() : CObject(TYPE_ACQUIREUI, PRIORITY_UI)
{
	// 全ての値をクリアする
	m_pos = NONE_D3DXVECTOR3;		// 位置
}

//==========================================
// デストラクタ
//==========================================
COffRespawn::~COffRespawn()
{

}

//==========================================
// ポーズの初期化処理
//==========================================
HRESULT COffRespawn::Init(void)
{
	// 全ての値を初期化する
	m_pos = NONE_D3DXVECTOR3;		// 位置

	// 成功を返す
	return S_OK;
}

//========================================
// ポーズの終了処理
//========================================
void COffRespawn::Uninit(void)
{
	// 破棄処理
	Release();
}

//========================================
// ポーズの更新処理
//========================================
void COffRespawn::Update(void)
{
	
}

//=====================================
// ポーズの描画処理
//=====================================
void COffRespawn::Draw(void)
{

}

//=====================================
// 情報の設定処理
//=====================================
void COffRespawn::SetData(const D3DXVECTOR3 pos)
{
	// 全ての値を設定する
	m_pos = pos;		// 位置
}

//=====================================
// 位置の設定処理
//=====================================
void COffRespawn::SetPos(const D3DXVECTOR3& pos)
{
	// 位置を設定する
	m_pos = pos;
}

//=====================================
// 位置の取得処理
//=====================================
D3DXVECTOR3 COffRespawn::GetPos(void) const
{
	// 位置を返す
	return m_pos;
}

//=====================================
// 生成処理
//=====================================
COffRespawn* COffRespawn::Create(const D3DXVECTOR3 pos)
{
	// ローカルオブジェクトを生成
	COffRespawn* pRespawn = nullptr;			// 復活地点のインスタンスを生成

	if (pRespawn == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pRespawn = new COffRespawn;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pRespawn != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pRespawn->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pRespawn->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 復活地点のポインタを返す
	return pRespawn;
}