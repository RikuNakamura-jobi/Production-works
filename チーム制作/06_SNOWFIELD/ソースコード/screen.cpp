//=======================================
//
// 2D画面のメイン処理[screen.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "screen.h"
#include "renderer.h"
#include "texture.h"

//---------------------------------------
// マクロ定義
//---------------------------------------

//=========================
// コンストラクタ
//=========================
CScreen::CScreen() : CObject2D(CObject2D::TYPE_SCREEN, CObject::PRIORITY_PLAYER)
{

}

//=========================
// デストラクタ
//=========================
CScreen::~CScreen()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CScreen::Init(void)
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
void CScreen::Uninit(void)
{
	// 終了
	CObject2D::Uninit();
}

//=========================
// 更新処理
//=========================
void CScreen::Update(void)
{

}

//=========================
// 描画処理
//=========================
void CScreen::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}
//=========================
// 情報の設定処理
//=========================
void CScreen::SetData(void)
{
	// スクロールの設定処理
	SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));			// 位置設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));			// 向き設定
	SetSize(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));			// サイズ設定
	SetLength();			// 長さ設定
	SetAngle();				// 方向設定

	// 頂点情報の初期化
	SetVertex();

	// テクスチャ情報の設定処理
	SetVtxTexture();

	// テクスチャの読み込み処理
	BindTexture(CManager::Get()->GetTexture()->Regist("data\\TEXTURE\\Title_Bg.jpg"));
}

//=========================
// 生成処理
//=========================
CScreen* CScreen::Create(void)
{
	// ローカルオブジェクトを生成
	CScreen* pLocus2D = nullptr;	// プレイヤーのインスタンスを生成

	if (pLocus2D == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pLocus2D = new CScreen;
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