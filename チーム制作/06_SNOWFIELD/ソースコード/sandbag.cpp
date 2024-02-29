//============================================
//
// 土嚢のメイン処理[sandbag.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "sandbag.h"
#include "useful.h"

#include "sandbag_manager.h"

//=========================================
// コンストラクタ
//=========================================
CSandbag::CSandbag() : CModel(CObject::TYPE_BLOCK, CObject::PRIORITY_BLOCK)
{
	m_pPrev = nullptr;		// 前のへのポインタ
	m_pNext = nullptr;		// 次のへのポインタ

	if (CSandbagManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// マネージャーへの登録処理
		CSandbagManager::Get()->Regist(this);
	}
}

//=========================================
// デストラクタ
//=========================================
CSandbag::~CSandbag()
{

}

//============================
// 前のポインタの設定処理
//============================
void CSandbag::SetPrev(CSandbag* pPrev)
{
	// 前のポインタを設定する
	m_pPrev = pPrev;
}

//============================
// 後のポインタの設定処理
//============================
void CSandbag::SetNext(CSandbag* pNext)
{
	// 次のポインタを設定する
	m_pNext = pNext;
}

//============================
// 前のポインタの設定処理
//============================
CSandbag* CSandbag::GetPrev(void) const
{
	// 前のポインタを返す
	return m_pPrev;
}

//============================
// 次のポインタの設定処理
//============================
CSandbag* CSandbag::GetNext(void) const
{
	// 次のポインタを返す
	return m_pNext;
}


//===========================================
// プレイヤーの初期化処理
//===========================================
HRESULT CSandbag::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//===========================================
// プレイヤーの終了処理
//===========================================
void CSandbag::Uninit(void)
{
	// 終了処理
	CModel::Uninit();

	if (CSandbagManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// リスト構造の引き抜き処理
		CSandbagManager::Get()->Pull(this);
	}

	// リスト構造関係のポインタを NULL にする
	m_pPrev = nullptr;
	m_pNext = nullptr;
}

//===========================================
// 更新処理
//===========================================
void CSandbag::Update(void)
{

}

//===========================================
// プレイヤーの描画処理
//===========================================
void CSandbag::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=======================================
// 情報の設定処理
//=======================================
void CSandbag::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// 情報の設定処理
	SetPos(pos);		// 位置
	SetPosOld(GetPos());											// 前回の位置
	SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));				// 向き
	SetScale(NONE_SCALE);											// 拡大率
	SetFileData(CXFile::TYPE_FRAC_GEAR);
}

//===========================================
// 生成処理
//===========================================
CSandbag* CSandbag::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// 兵士のポインタを宣言
	CSandbag* pSoldier = nullptr;

	if (pSoldier == nullptr)
	{ // オブジェクトが NULL の場合

		// メモリを確保する
		pSoldier = new CSandbag;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return pSoldier;
	}

	if (pSoldier != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pSoldier->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pSoldier->SetData(pos, rot);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pSoldier;
}