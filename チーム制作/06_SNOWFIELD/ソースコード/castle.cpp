//===========================================
//
// 城のメイン処理[castle.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "castle.h"
#include "renderer.h"
#include "useful.h"

//==============================
// コンストラクタ
//==============================
CCastle::CCastle() : CModel(CObject::TYPE_CASTLE, CObject::PRIORITY_ENTITY)
{

}

//==============================
// デストラクタ
//==============================
CCastle::~CCastle()
{

}

//==============================
// 破片の初期化処理
//==============================
HRESULT CCastle::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//========================================
// 破片の終了処理
//========================================
void CCastle::Uninit(void)
{
	// 終了処理
	CModel::Uninit();
}

//=====================================
// 破片の更新処理
//=====================================
void CCastle::Update(void)
{
	
}

//=====================================
// 破片の描画処理
//=====================================
void CCastle::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CCastle::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// 情報の設定処理
	SetPos(pos);						// 位置
	SetPosOld(pos);						// 前回の位置
	SetRot(rot);						// 向き
	SetScale(NONE_SCALE);				// 拡大率
	SetFileData(CXFile::TYPE_CASTLE);	// モデル情報を設定する
}

//=======================================
// 生成処理
//=======================================
CCastle* CCastle::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ローカルオブジェクトを生成
	CCastle* pCastle = nullptr;			// インスタンスを生成

	if (pCastle == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pCastle = new CCastle;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pCastle != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pCastle->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pCastle->SetData(pos, rot);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 城のポインタを返す
	return pCastle;
}