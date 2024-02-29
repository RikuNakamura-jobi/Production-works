//===========================================
//
// 旗のメイン処理[flag.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "flag.h"
#include "soldier.h"
#include "renderer.h"
#include "acquire_UI.h"
#include "objectbillboard.h"
#include "useful.h"

#include "snowball_manager.h"
#include "soldier_manager.h"
#include "game.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const float OFF_WIN = 60.0f;		// 攻撃側が勝利になる値
}

//==============================
// コンストラクタ
//==============================
CFlag::CFlag() : CModel(CObject::TYPE_FLAG, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_acquire = nullptr;		// 旗取得UIの情報
}

//==============================
// デストラクタ
//==============================
CFlag::~CFlag()
{

}

//==============================
// 破片の初期化処理
//==============================
HRESULT CFlag::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値をクリアする
	m_acquire = nullptr;		// 旗取得UIの情報

	// 値を返す
	return S_OK;
}

//========================================
// 破片の終了処理
//========================================
void CFlag::Uninit(void)
{
	if (m_acquire != nullptr)
	{ // 旗取得UIが NULL じゃない場合

		// 旗取得UIの終了処理
		m_acquire->Uninit();
		m_acquire = nullptr;
	}

	// 終了処理
	CModel::Uninit();
}

//=====================================
// 破片の更新処理
//=====================================
void CFlag::Update(void)
{
	// 当たり判定
	if (collision() == true)
	{ // 旗のゲージが溜まった場合

		// 攻撃側の勝ちにする
		CGame::SetState(CGame::STATE_OFFWIN);
	}
}

//=====================================
// 破片の描画処理
//=====================================
void CFlag::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// 旗の当たり判定処理
//=====================================
bool CFlag::collision(void)
{
	CSoldierManager *soldierManager = CSoldierManager::Get();
	int nNumOffe = 0, nNumDefe = 0;

	if (soldierManager != nullptr)
	{ // マネージャーが存在していた場合

		// ポインタを宣言
		CSoldier *pObjectTop = soldierManager->GetTop();	// 先頭オブジェクト

		if (pObjectTop != nullptr)
		{
			// ポインタを宣言
			CSoldier *pObjCheck = pObjectTop;	// オブジェクト確認用

			while (pObjCheck != NULL)
			{ // オブジェクトが使用されている場合繰り返す

				CSoldier *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

				if (D3DXVec3Length(&(pObjCheck->GetPos() - GetPos())) < 500.0f)
				{
					if (pObjCheck->GetBattle() == CSoldier::BATTLE_OFF)
					{
						nNumOffe++;
					}
					else
					{
						nNumDefe++;
					}
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}

			if (nNumOffe == nNumDefe)
			{
				// OBARA::ここはゲージを動かさない
			}
			else if (nNumOffe > nNumDefe)
			{
				// OBARA::ここはゲージを増やす
				if (m_acquire != nullptr)
				{ // 旗取得UIが NULL じゃない場合

					// ビルボードの情報を取得
					D3DXVECTOR3 size = m_acquire->GetPolygon(CAcquireUI::TYPE_METER)->GetSize();

					// サイズを加算する
					size.x += 0.1f;

					// サイズを適用する
					m_acquire->GetPolygon(CAcquireUI::TYPE_METER)->SetSize(size);

					if (size.x >= OFF_WIN)
					{ // サイズが一定数を超えた場合

						// サイズを補正する
						size.x = OFF_WIN;

						// サイズを適用する
						m_acquire->GetPolygon(CAcquireUI::TYPE_METER)->SetSize(size);

						// true を返す
						return true;
					}
				}
			}
			else
			{
				// OBARA::ここはゲージを減らす
				if (m_acquire != nullptr)
				{ // 旗取得UIが NULL じゃない場合

					// ビルボードの情報を取得
					D3DXVECTOR3 size = m_acquire->GetPolygon(CAcquireUI::TYPE_METER)->GetSize();

					// サイズを加算する
					size.x -= 0.1f;

					if (size.x <= 0.0f)
					{ // サイズが0.0f以下になった場合

						// サイズを補正する
						size.x = 0.0f;
					}

					// サイズを適用する
					m_acquire->GetPolygon(CAcquireUI::TYPE_METER)->SetSize(size);
				}
			}
		}
	}

	// false を返す
	return false;
}

//=====================================
// 情報の設定処理
//=====================================
void CFlag::SetData(const D3DXVECTOR3& pos)
{
	// 情報の設定処理
	SetPos(pos);						// 位置
	SetPosOld(pos);						// 前回の位置
	SetRot(NONE_D3DXVECTOR3);			// 向き
	SetScale(NONE_SCALE);				// 拡大率
	SetFileData(CXFile::TYPE_FLAG);		// モデル情報を設定する

	if (m_acquire == nullptr)
	{ // 旗取得UIが NULL の場合

		// 旗取得UIを生成する
		m_acquire = CAcquireUI::Create(D3DXVECTOR3(pos.x, pos.y + 100.0f, pos.z));
	}
}

//=======================================
// 生成処理
//=======================================
CFlag* CFlag::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CFlag* pFlag = nullptr;			// インスタンスを生成

	if (pFlag == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pFlag = new CFlag;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pFlag != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pFlag->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pFlag->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 城のポインタを返す
	return pFlag;
}