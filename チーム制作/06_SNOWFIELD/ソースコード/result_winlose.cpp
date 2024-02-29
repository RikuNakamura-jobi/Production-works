//=======================================
//
// リザルト結果のメイン処理[result_winlose.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "result_winlose.h"
#include "renderer.h"
#include "game.h"
#include "texture.h"

//---------------------------------------
// マクロ定義
//---------------------------------------

//=========================
// コンストラクタ
//=========================
CWinLose::CWinLose() : CObject2D(CObject::TYPE_TITLELOGO, CObject::PRIORITY_PLAYER)
{

}

//=========================
// デストラクタ
//=========================
CWinLose::~CWinLose()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CWinLose::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CWinLose::Uninit(void)
{
	// 終了
	CObject2D::Uninit();
}

//=========================
// 更新処理
//=========================
void CWinLose::Update(void)
{

}

//=========================
// 描画処理
//=========================
void CWinLose::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}
//=========================
// 情報の設定処理
//=========================
void CWinLose::SetData(void)
{
	// スクロールの設定処理
	SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 250.0f, 0.0f));			// 位置設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// 向き設定
	SetSize(D3DXVECTOR3(400.0f, 100.0f, 0.0f));			// サイズ設定
	SetLength();			// 長さ設定
	SetAngle();				// 方向設定

	// 頂点情報の初期化
	SetVertex();

	// テクスチャ情報の設定処理
	SetVtxTexture();

	if (CGame::GetState() == CGame::STATE_OFFWIN)
	{ // 勝ちの場合

		// テクスチャの読み込み処理
		BindTexture(CManager::Get()->GetTexture()->Regist("data\\TEXTURE\\SUCCESS.png"));
	}
	else
	{ // 上記以外

		// テクスチャの読み込み処理
		BindTexture(CManager::Get()->GetTexture()->Regist("data\\TEXTURE\\FAILED.png"));
	}
}

//=========================
// 生成処理
//=========================
CWinLose* CWinLose::Create(void)
{
	// ローカルオブジェクトを生成
	CWinLose* pLocus2D = nullptr;	// プレイヤーのインスタンスを生成

	if (pLocus2D == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pLocus2D = new CWinLose;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pLocus2D != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pLocus2D->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pLocus2D->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pLocus2D;
}