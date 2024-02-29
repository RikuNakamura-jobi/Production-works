//=======================================
//
// タイトルロゴのメイン処理[title_logo.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "title_logo.h"
#include "renderer.h"
#include "texture.h"

//---------------------------------------
// マクロ定義
//---------------------------------------

//=========================
// コンストラクタ
//=========================
CTitleLogo::CTitleLogo() : CObject2D(CObject::TYPE_TITLELOGO, CObject::PRIORITY_PLAYER)
{

}

//=========================
// デストラクタ
//=========================
CTitleLogo::~CTitleLogo()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CTitleLogo::Init(void)
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
void CTitleLogo::Uninit(void)
{
	// 終了
	CObject2D::Uninit();
}

//=========================
// 更新処理
//=========================
void CTitleLogo::Update(void)
{

}

//=========================
// 描画処理
//=========================
void CTitleLogo::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}
//=========================
// 情報の設定処理
//=========================
void CTitleLogo::SetData(void)
{
	// スクロールの設定処理
	SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 250.0f, 0.0f));			// 位置設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// 向き設定
	SetSize(D3DXVECTOR3(350.0f, 150.0f, 0.0f));			// サイズ設定
	SetLength();			// 長さ設定
	SetAngle();				// 方向設定

	// 頂点情報の初期化
	SetVertex();

	// テクスチャ情報の設定処理
	SetVtxTexture();

	// テクスチャの読み込み処理
	BindTexture(CManager::Get()->GetTexture()->Regist("data\\TEXTURE\\Title_logo.png"));
}

//=========================
// 生成処理
//=========================
CTitleLogo* CTitleLogo::Create(void)
{
	// ローカルオブジェクトを生成
	CTitleLogo* pLocus2D = nullptr;	// プレイヤーのインスタンスを生成

	if (pLocus2D == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pLocus2D = new CTitleLogo;
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