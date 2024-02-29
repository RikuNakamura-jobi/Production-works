//============================================
//
// ブロックのメイン処理[block.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "block.h"
#include "useful.h"

#include "block_manager.h"

//=========================================
// コンストラクタ
//=========================================
CBlock::CBlock() : CModel(CObject::TYPE_BLOCK, CObject::PRIORITY_BLOCK)
{
	// 全ての値をクリアする
	m_type = TYPE_TREE;		// 種類

	m_pPrev = nullptr;		// 前のへのポインタ
	m_pNext = nullptr;		// 次のへのポインタ

	if (CBlockManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// マネージャーへの登録処理
		CBlockManager::Get()->Regist(this);
	}
}

//=========================================
// デストラクタ
//=========================================
CBlock::~CBlock()
{

}

//============================
// 前のポインタの設定処理
//============================
void CBlock::SetPrev(CBlock* pPrev)
{
	// 前のポインタを設定する
	m_pPrev = pPrev;
}

//============================
// 後のポインタの設定処理
//============================
void CBlock::SetNext(CBlock* pNext)
{
	// 次のポインタを設定する
	m_pNext = pNext;
}

//============================
// 前のポインタの設定処理
//============================
CBlock* CBlock::GetPrev(void) const
{
	// 前のポインタを返す
	return m_pPrev;
}

//============================
// 次のポインタの設定処理
//============================
CBlock* CBlock::GetNext(void) const
{
	// 次のポインタを返す
	return m_pNext;
}


//===========================================
// プレイヤーの初期化処理
//===========================================
HRESULT CBlock::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_type = TYPE_TREE;		// 種類

	// 値を返す
	return S_OK;
}

//===========================================
// プレイヤーの終了処理
//===========================================
void CBlock::Uninit(void)
{
	// 終了処理
	CModel::Uninit();

	if (CBlockManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// リスト構造の引き抜き処理
		CBlockManager::Get()->Pull(this);
	}

	// リスト構造関係のポインタを NULL にする
	m_pPrev = nullptr;
	m_pNext = nullptr;
}

//===========================================
// 更新処理
//===========================================
void CBlock::Update(void)
{

}

//===========================================
// プレイヤーの描画処理
//===========================================
void CBlock::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=======================================
// 情報の設定処理
//=======================================
void CBlock::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// 全ての値を設定する
	m_type = type;

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
CBlock* CBlock::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// 兵士のポインタを宣言
	CBlock* pSoldier = nullptr;

	if (pSoldier == nullptr)
	{ // オブジェクトが NULL の場合

		// メモリを確保する
		pSoldier = new CBlock;
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
		pSoldier->SetData(pos, rot, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pSoldier;
}